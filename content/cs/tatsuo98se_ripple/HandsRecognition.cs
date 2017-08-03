using System;
using System.Collections.Generic;
using System.ComponentModel;
//using System.Drawing;
//using System.Drawing.Imaging;
using System.Linq;
using Intel.RealSense;
using Intel.RealSense.Hand;
using System.Timers;
using LEDLIB;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace hands_viewer.cs
{
    class HandsRecognition
    {
        // ImageInfo _info;  // never used
        readonly byte[] _lut;
        private readonly MainForm _form;
        private bool _disconnected = false;
        //Queue containing depth image - for synchronization purposes
        private readonly Queue<Image> _mImages;

        private readonly Queue<Point3DF32>[] _mCursorPoints;
        private readonly Queue<Point3DF32>[] _mAdaptivePoints;
        private const int NumberOfFramesToDelay = 3;
        
        private float _maxRange;

        private const int CURSOR_FACTOR_X = 60;
        private const int CURSOR_FACTOR_Y_UP = 120;
        private const int CURSOR_FACTOR_Y_DOWN = 40;
        private static LED3DCanvas canvas = new LED3DCanvas();
                private static LED3DWaveCanvasFilter waveFilter = new LED3DWaveCanvasFilter(new LED3DHsvColorFilter(canvas));
//        private static LED3DWaveCanvasFilter waveFilter = new LED3DWaveCanvasFilter(canvas);

        public static void addObject(LED3DObject obj)
        {
            canvas.AddObject(obj);
        }
        public static void addObjectOnWave(LED3DObject obj)
        {
            canvas.AddObject(obj, waveFilter);
        }

        public static void Show()
        {
            canvas.Show();
        }

        public HandsRecognition(MainForm form)
        {
            _mImages = new Queue<Image>();
            _mCursorPoints = new Queue<Point3DF32>[2];
            _mCursorPoints[0] = new Queue<Point3DF32>();
            _mCursorPoints[1] = new Queue<Point3DF32>();

            _mAdaptivePoints = new Queue<Point3DF32>[2];
            _mAdaptivePoints[0] = new Queue<Point3DF32>();
            _mAdaptivePoints[1] = new Queue<Point3DF32>();

            this._form = form;
            _lut = Enumerable.Repeat((byte)0, 256).ToArray();
            _lut[255] = 1;

        }


        /* Checking if sensor device connect or not */
        private bool DisplayDeviceConnection(bool state)
        {
            if (state)
            {
                if (!_disconnected) _form.UpdateStatus("Device Disconnected");
                _disconnected = true;
            }
            else
            {
                if (_disconnected) _form.UpdateStatus("Device Reconnected");
                _disconnected = false;
            }
            return _disconnected;
        }

        /* Displaying current frame gestures */
        private void DisplayGesture(HandData handAnalysis,int frameNumber)
        {
            if (handAnalysis.FiredGestureData != null)
            {
                int firedGesturesNumber = handAnalysis.FiredGestureData.Length;
                string gestureStatusLeft = string.Empty;
                string gestureStatusRight = string.Empty;

                if (firedGesturesNumber == 0)
                {
                    return;
                }

                foreach (var gestureData in handAnalysis.FiredGestureData)
                {
                    IHand handData;
                    if (handAnalysis.QueryHandDataById(gestureData.handId, out handData) != Status.STATUS_NO_ERROR)
                        return;

                    BodySideType bodySideType = handData.BodySide;
                    if (bodySideType == BodySideType.BODY_SIDE_LEFT)
                    {
                        gestureStatusLeft += "Left Hand Gesture: " + gestureData.name;
                    }
                    else if (bodySideType == BodySideType.BODY_SIDE_RIGHT)
                    {
                        gestureStatusRight += "Right Hand Gesture: " + gestureData.name;
                    }

                }

                if (gestureStatusLeft == String.Empty)
                    _form.UpdateInfo("Frame " + frameNumber + ") " + gestureStatusRight + "\n", System.Drawing.Color.SeaGreen);
                else
                    _form.UpdateInfo("Frame " + frameNumber + ") " + gestureStatusLeft + ", " + gestureStatusRight + "\n", System.Drawing.Color.SeaGreen);
            }
        }


        /* Displaying Depth/Mask Images - for depth image only we use a delay of NumberOfFramesToDelay to sync image with tracking */
        private unsafe void DisplayPicture(Image depth, HandData handAnalysis)
        {
            if (depth == null)
                return;

            Image image = depth;

           
                //Mask Image
                if (_form.GetLabelmapState())
                {
                System.Drawing.Bitmap labeledBitmap = null;
                    try
                    {
                        int numOfHands = handAnalysis.NumberOfHands;

                        PointI32[][] pointOuter = new PointI32[numOfHands][];
                        PointI32[][] pointInner = new PointI32[numOfHands][];

                        labeledBitmap = new System.Drawing.Bitmap(image.Info.width, image.Info.height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                        for (int j = 0; j < numOfHands; j++)
                        {
                            int id;
                            ImageData data;

                            handAnalysis.QueryHandId(AccessOrderType.ACCESS_ORDER_BY_TIME, j, out id);
                            //Get hand by time of appearance
                            IHand handData;
                            handAnalysis.QueryHandData(AccessOrderType.ACCESS_ORDER_BY_TIME, j, out handData);
                            if (handData != null)
                            {
                                image = handData.SegmentationImage;
                                if(image != null)
                                {
                                    if (image.AcquireAccess(ImageAccess.ACCESS_READ, PixelFormat.PIXEL_FORMAT_Y8,
                                        out data) >= Status.STATUS_NO_ERROR)
                                    {
                                        var rect = new System.Drawing.Rectangle(0, 0, image.Info.width, image.Info.height);

                                        var bitmapdata = labeledBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, labeledBitmap.PixelFormat);
                                        byte* numPtr = (byte*)bitmapdata.Scan0; //dst
                                        byte* numPtr2 = (byte*)data.planes[0]; //row
                                        int imagesize = image.Info.width * image.Info.height;
                                        byte num2 = (_form.GetFullHandModeState()) ? (byte)handData.BodySide : (byte)1;

                                        byte tmp = 0;
                                        for (int i = 0; i < imagesize; i++, numPtr += 4, numPtr2++)
                                        {
                                            tmp = (byte)(_lut[numPtr2[0]] * num2 * 100);
                                            numPtr[0] = (Byte)(tmp | numPtr[0]);
                                            numPtr[1] = (Byte)(tmp | numPtr[1]);
                                            numPtr[2] = (Byte)(tmp | numPtr[2]);
                                            numPtr[3] = 0xff;
                                        }

                                        labeledBitmap.UnlockBits(bitmapdata);
                                        image.ReleaseAccess(data);

                                    }

                                    if ((_form.GetContourState()))
                                    {
                                        int contourNumber = handData.NumberOfContours;
                                        if (contourNumber > 0)
                                        {
                                            for (int k = 0; k < contourNumber; ++k)
                                            {
                                                IContour contour = handData.Contours[k];
                                                if (contour != null)
                                                {
                                                    if (contour.Outer)
                                                    {
                                                        pointOuter[j] = contour.Points;
                                                    }
                                                    else
                                                    {
                                                        pointInner[j] = contour.Points;
                                                    }
                                                }
                                            }

                                        }
                                    }

                                }
                            }
                        }
                        if (labeledBitmap != null)
                        {

                            _form.DisplayBitmap(labeledBitmap);
                            labeledBitmap.Dispose();
                        }
                        if(image !=null)
                            image.Dispose();

                        for (int i = 0; i < numOfHands; i++)
                        {
                            if (_form.GetContourState())
                            {
                                if (pointOuter[i] != null && pointOuter[i].Length > 0)
                                    _form.DisplayContour(pointOuter[i], i);
                                if (pointInner[i] != null && pointInner[i].Length > 0)
                                    _form.DisplayContour(pointInner[i], i);
                            }

                        }

                    }
                    catch (Exception)
                    {
                        if (labeledBitmap != null)
                        {
                            labeledBitmap.Dispose();
                        }
                        if (image != null)
                        {
                            image.Dispose();
                        }
                    }

                }//end label image

                //Depth Image
                else
                {
                    //collecting 3 images inside a queue and displaying the oldest image
                    ImageInfo info;
                    Image image2;
                ImageData imageData = new ImageData();
                    info = image.Info;
                image2 = Image.CreateInstance(_form.session, info, imageData);
                    //image2 = _form.g_session. CreateImage(info);
                    if (image2 == null) { return; }
                    image2.CopyImage(image);
                    _mImages.Enqueue(image2);
                    if (_mImages.Count == NumberOfFramesToDelay)
                    {
                        System.Drawing.Bitmap depthBitmap;
                        try
                        {
                            depthBitmap = new System.Drawing.Bitmap(image.Info.width, image.Info.height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                        }
                        catch (Exception)
                        {
                            image.Dispose();
                            Image queImage = _mImages.Dequeue();
                            queImage.Dispose();
                            return;
                        }

                        ImageData data3;
                        Image image3 = _mImages.Dequeue();
                        if (image3.AcquireAccess(ImageAccess.ACCESS_READ, PixelFormat.PIXEL_FORMAT_DEPTH, out data3) >= Status.STATUS_NO_ERROR)
                        {
                            float fMaxValue = _maxRange;
                            byte cVal;

                            var rect = new System.Drawing.Rectangle(0, 0, image.Info.width, image.Info.height);
                            var bitmapdata = depthBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, depthBitmap.PixelFormat);

                            byte* pDst = (byte*)bitmapdata.Scan0;
                            short* pSrc = (short*)data3.planes[0];
                            int size = image.Info.width * image.Info.height;

                            for (int i = 0; i < size; i++, pSrc++, pDst += 4)
                            {
                                cVal = (byte)((*pSrc) / fMaxValue * 255);
                                if (cVal != 0)
                                    cVal = (byte)(255 - cVal);

                                pDst[0] = cVal;
                                pDst[1] = cVal;
                                pDst[2] = cVal;
                                pDst[3] = 255;
                            }
                            try
                            {
                                depthBitmap.UnlockBits(bitmapdata);
                            }
                            catch (Exception)
                            {
                                image3.ReleaseAccess(data3);
                                depthBitmap.Dispose();
                                image3.Dispose();
                                return;
                            }

                            _form.DisplayBitmap(depthBitmap);
                            image3.ReleaseAccess(data3);
                        }
                        depthBitmap.Dispose();
                        image3.Dispose();
                    }
                }
            
        }


        /* Displaying current frames hand joints */
        private void DisplayJoints(HandData handOutput, long timeStamp = 0)
        {
          
           
            if (_form.GetJointsState() || _form.GetSkeletonState()  || _form.GetExtremitiesState())
            {
                //Iterate hands
                var nodes = new JointData[][] { new JointData[0x20], new JointData[0x20] };
                var extremityNodes = new ExtremityData[][] { new ExtremityData[0x6], new ExtremityData[0x6] };
               
                int numOfHands = handOutput.NumberOfHands;

                if (numOfHands == 1) _mCursorPoints[1].Clear();

                for (int i = 0; i < 2; i++)
                {
                    //Get hand by time of appearance
                    IHand handData;
                    if (handOutput.QueryHandData(AccessOrderType.ACCESS_ORDER_BY_TIME, i, out handData) == Status.STATUS_NO_ERROR)
                    {
                        if (handData != null)
                        {
                            var nodeIndex = handData.BodySide == BodySideType.BODY_SIDE_LEFT ? 0 : 1;
                            //Iterate Joints
                            foreach (var tJ in handData.TrackedJoints)
                            {
                                nodes[nodeIndex][(int)tJ.Key] = tJ.Value;
                            }   // end iterating over joints

                            // iterate over extremity points
                            if (_form.GetExtremitiesState())
                            {
				                for(int j = 0; j < HandData.NUMBER_OF_EXTREMITIES ; j++)
				                {
                                    foreach (var eP in handData.ExtremityPoints)
                                    {
                                        extremityNodes[nodeIndex][(int)eP.Key] = eP.Value;
                                    }
                                }
			                }

                        }
                    }
                } // end iterating over hands

         
                _form.DisplayJoints(nodes, 2);
                if (numOfHands > 0)
                {

                    if (_form.GetExtremitiesState())
                        _form.DisplayExtremities(numOfHands, extremityNodes);
                }
               
            }
        }

        ///* Displaying current frames hand joints */
        //private void DisplayCursorJoints(CursorData cursorOutput, long timeStamp = 0)
        //{
        //    _mCursorClick[0] = Math.Max(0, _mCursorClick[0] - 1);
        //    _mCursorClick[1] = Math.Max(0, _mCursorClick[1] - 1);

            
        //    int numOfHands = cursorOutput.QueryNumberOfCursors();
        //    if (numOfHands == 1)
        //    {
        //        _mCursorPoints[1].Clear();
        //        _mAdaptivePoints[1].Clear();
        //    }

        //    for (int i = 0; i < numOfHands; i++)
        //    {
        //        //Get hand by time of appearance
        //        CursorData.ICursor cursorData;
        //        if (cursorOutput.QueryCursorData(CursorData.AccessOrderType.ACCESS_ORDER_BY_TIME, i, out cursorData) == Status.STATUS_NO_ERROR)
        //        {
        //            if (cursorData != null)
        //            {
        //                // collect cursor points
        //                if (_form.GetCursorState())
        //                {
        //                    Point3DF32 imagePoint = cursorData.QueryCursorPointImage();

        //                    _mCursorPoints[i].Enqueue(imagePoint);
        //                    if (_mCursorPoints[i].Count > 50)
        //                        _mCursorPoints[i].Dequeue();
        //                }

        //                // collect adaptive points
        //                if (_form.GetAdaptiveState())
        //                {
        //                    Point3DF32 adaptivePoint = cursorData.QueryAdaptivePoint();
        //                    adaptivePoint.x *= 640;
        //                    adaptivePoint.y = adaptivePoint.y * 480;
  
        //                    _mAdaptivePoints[i].Enqueue(adaptivePoint);
        //                    if (_mAdaptivePoints[i].Count > 50)
        //                        _mAdaptivePoints[i].Dequeue();
        //                }
        //                _mCursorHandSide[i] = cursorData.QueryBodySide();
        //                CursorData.GestureData gestureData;
        //                if (cursorOutput.IsGestureFiredByHand(CursorData.GestureType.CURSOR_CLICK, cursorData.QueryUniqueId(), out gestureData))
        //                {
        //                    _mCursorClick[i] = 7;
        //                }
        //            }    
        //        }
        //    } // end iterating over hands


        //    if (numOfHands > 0)
        //    {

        //        if (_form.GetCursorModeState())
        //            _form.DisplayCursor(numOfHands, _mCursorPoints, _mAdaptivePoints, _mCursorClick,_mCursorHandSide);
        //    }
        //    else
        //    {
        //        _mCursorPoints[0].Clear();
        //        _mCursorPoints[1].Clear();
        //        _mAdaptivePoints[0].Clear();
        //        _mAdaptivePoints[1].Clear();
        //        _mCursorHandSide[0] = CursorData.BodySideType.BODY_SIDE_UNKNOWN;
        //        _mCursorHandSide[1] = CursorData.BodySideType.BODY_SIDE_UNKNOWN;
        //    }
        //}
        
        /* Displaying current frame alerts */
        private void DisplayAlerts(HandData handAnalysis, int frameNumber)
        {
            bool isChanged = false;
            string sAlert = "Alert: ";
            if (handAnalysis.FiredAlertData != null)
            for (int i = 0; i < handAnalysis.FiredAlertData.Length; i++)
            {
               
                //See HandAnalysis.AlertData.AlertType for all available alerts
                switch (handAnalysis.FiredAlertData[i].label)
                {
                    case AlertType.ALERT_HAND_DETECTED:
                        {

                            sAlert += "Hand Detected, ";
                            isChanged = true;
                            break;
                        }
                    case AlertType.ALERT_HAND_NOT_DETECTED:
                        {

                            sAlert += "Hand Not Detected, ";
                            isChanged = true;
                            break;
                        }
                    case AlertType.ALERT_HAND_CALIBRATED:
                        {

                            sAlert += "Hand Calibrated, ";
                            isChanged = true;
                            break;
                        }
                    case AlertType.ALERT_HAND_NOT_CALIBRATED:
                        {

                            sAlert += "Hand Not Calibrated, ";
                            isChanged = true;
                            break;
                        }
                    case AlertType.ALERT_HAND_INSIDE_BORDERS:
                        {

                            sAlert += "Hand Inside Border, ";
                            isChanged = true;
                            break;
                        }
                    case AlertType.ALERT_HAND_OUT_OF_BORDERS:
                        {

                            sAlert += "Hand Out Of Borders, ";
                            isChanged = true;
                            break;
                        }
               
                }
            }
            if (isChanged)
            {
                _form.UpdateInfo("Frame " + frameNumber + ") " + sAlert + "\n", System.Drawing.Color.RoyalBlue);
            }
        }


        public static Status OnNewFrame(Int32 mid, Base module, Sample sample)
        {
            return Status.STATUS_NO_ERROR;
        }


        /* Using SenseManager to handle data */
        public void SimplePipeline()
        {   
            _form.UpdateInfo(String.Empty, System.Drawing.Color.Black);
            bool liveCamera = false;
       
            bool flag = true;
            SenseManager instance = null;
            _disconnected = false;
            instance = _form.session.CreateSenseManager();
            if (instance == null)
            {
                _form.UpdateStatus("Failed creating SenseManager");
                _form.EnableTrackingMode(true);
                return;
            }
            CaptureManager captureManager =  instance.CaptureManager;
            DeviceInfo info = null;
            if (captureManager != null)
            {
                if (_form.GetPlaybackState())
                {
                    captureManager.SetFileName(_form.GetFileName(), false);
                    info = _form.GetDeviceFromFileMenu(_form.GetFileName());
                }
                else
                {
                    if (_form.Devices.Count == 0)
                    {
                        _form.UpdateStatus("No device were found");
                        return;
                    }

                    _form.Devices.TryGetValue(_form.GetCheckedDevice(), out info);


                    if (_form.GetRecordState())
                    {
                        captureManager.SetFileName(_form.GetFileName(), true);
                        if (_form.Devices.TryGetValue(_form.GetCheckedDevice(), out info))
                        {
                            captureManager.FilterByDeviceInfo(_form.GetCheckedDeviceInfo());
                        }

                    }
                    else
                    {
                        captureManager.FilterByDeviceInfo(_form.GetCheckedDeviceInfo());
                        liveCamera = true;
                    }
                    if (info == null)
                    {
                        _form.UpdateStatus("Device Failure");
                        return;
                    }
                }
            }
            /* Set Module */

            HandModule handAnalysis;

            // SenseManager.Handler handler = new SenseManager Handler();
            // handler.onModuleProcessedFrame = new SenseManager.Handler.OnModuleProcessedFrameDelegate(OnNewFrame);
           

            HandConfiguration handConfiguration = null;
            HandData handData = null;



            handAnalysis = HandModule.Activate(instance);

            if ( handAnalysis == null )
            {
                _form.UpdateStatus("Failed Loading Module");
                _form.EnableTrackingMode(true);

                return;
            }

            handConfiguration = handAnalysis.CreateActiveConfiguration();
            if (handConfiguration == null)
            {
                _form.UpdateStatus("Failed Create Configuration");
                _form.EnableTrackingMode(true);
                instance.Close();
                instance.Dispose();
                return;
            }
            handData = handAnalysis.CreateOutput();
            if (handData == null)
            {
                _form.UpdateStatus("Failed Create Output");
                _form.EnableTrackingMode(true);
                handConfiguration.Dispose();
                instance.Close();
                instance.Dispose();
                return;
            }
               
            
            FPSTimer timer = new FPSTimer(_form);
            _form.UpdateStatus("Init Started");
            
            if (instance.Init() == Status.STATUS_NO_ERROR)
            {

                DeviceInfo dinfo;
                DeviceModel dModel = DeviceModel.DEVICE_MODEL_F200;
                Device device = instance.CaptureManager.Device;
                if (device != null)
                {
                    device.QueryDeviceInfo(out dinfo);
                    dModel = dinfo.model;
                    _maxRange = device.DepthSensorRange.max;

                }

               
                if (handConfiguration != null)
                {
                    TrackingModeType trackingMode = TrackingModeType.TRACKING_MODE_FULL_HAND;

                    if (_form.GetFullHandModeState())
                        trackingMode = TrackingModeType.TRACKING_MODE_FULL_HAND;

                    handConfiguration.TrackingMode = trackingMode;

                    handConfiguration.EnableAllAlerts();
                    handConfiguration.SegmentationImageEnabled = true;
                    bool isEnabled = handConfiguration.SegmentationImageEnabled;

                    handConfiguration.ApplyChanges();
                       

                    _form.resetGesturesList();
                    int totalNumOfGestures = handConfiguration.NumberOfGestures;

                    if (totalNumOfGestures > 0)
                    {
                        this._form.UpdateGesturesToList("", 0);
                        for (int i = 0; i < totalNumOfGestures; i++)
                        {
                            string gestureName = string.Empty;
                            if (handConfiguration.QueryGestureNameByIndex(i, out gestureName) ==
                                Status.STATUS_NO_ERROR)
                            {
                                this._form.UpdateGesturesToList(gestureName, i + 1);
                            }
                        }

                        _form.UpdateGesturesListSize();
                    }


                }
                
                

                _form.UpdateStatus("Streaming");
                int frameCounter = 0;
                int frameNumber = 0;
          
                while (!_form.stop)
                {
                    
                        string gestureName = _form.GetGestureName();
                    if (handConfiguration != null)
                    {
                        if (string.IsNullOrEmpty(gestureName) == false)
                        {
                            if (handConfiguration.IsGestureEnabled(gestureName) == false)
                            {
                                handConfiguration.DisableAllGestures();
                                handConfiguration.EnableGesture(gestureName, true);
                                handConfiguration.ApplyChanges();
                            }
                        }
                        else
                        {
                            handConfiguration.DisableAllGestures();
                            handConfiguration.ApplyChanges();
                        }
                    }
                
                    

                    if (instance.AcquireFrame(true) < Status.STATUS_NO_ERROR)
                    {
                        break;
                    }

                    frameCounter++;

                    if (!DisplayDeviceConnection(!instance.IsConnected()))
                    {
                        Sample sample= instance.Sample;

                        if (sample != null && sample.Depth != null)
                        {
                            frameNumber = liveCamera ? frameCounter : instance.CaptureManager.FrameIndex; 

                            if (handData != null)
                            {
                                handData.Update();

                                DisplayPicture(sample.Depth, handData);
                                DisplayGesture(handData, frameNumber);
                                DisplayJoints(handData);
                                DisplayAlerts(handData, frameNumber);
                            }
                            

                            _form.UpdatePanel();
                        }
                        timer.Tick();
                    }
                    instance.ReleaseFrame();
                }
            }
            else
            {
                _form.UpdateStatus("Init Failed");
                flag = false;
            }
            foreach (Image Image in _mImages)
            {
                Image.Dispose();
            }

            // Clean Up
            if (handData != null) handData.Dispose();
            if (handConfiguration != null) handConfiguration.Dispose();
            
            instance.Close();
            instance.Dispose();

            if (flag)
            {
                _form.UpdateStatus("Stopped");
            }
        }
    }
}


