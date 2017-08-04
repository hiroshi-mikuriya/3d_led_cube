using LEDLIB;
using sample.ShowCase;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;

namespace sample
{
    class Program
    {
        public interface TestCase
        {
            void SetUp();
            void Run();
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);

        static void Main(string[] args)
        {
            var test = AppDomain.CurrentDomain.BaseDirectory;
            var libDir = Directory.GetParent(test).Parent.Parent.Parent.Parent;
            SetDllDirectory(Path.Combine(libDir.FullName,"00_lib"));

            var canvas = new LED3DCanvas();
            //            var filter = new LED3DWaveCanvasFilter(canvas);
//            var filter = new LED3DSurfaceCanvasFilter(canvas);
            var filter = new LED3DWaveCanvasFilter( new LED3DHsvColorFilter(canvas));

//            LED3DCanvasFilter filter = null;

            int testcase = 3;
            var showCases = new List<IShowCase>();

            showCases.Add(new CircleOnCube());
            showCases.Add(new RectangleOnCube());
            showCases.Add(new RippleOnCube());
            showCases.Add(new AtFieldOnCube());
            showCases.Add(new AngelOnCube());
            showCases.Add(new RippleOnWaveCube());
            showCases.Add(new BallOnCube());

            showCases[testcase].SetUp(canvas, filter);
            showCases[testcase].Run(canvas, filter);

            while (true)
            {
                canvas.Show();
                LED.Wait(10);
            }

        }
    }
}
