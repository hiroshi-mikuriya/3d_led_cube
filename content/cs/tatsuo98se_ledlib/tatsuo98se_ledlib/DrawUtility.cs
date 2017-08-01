using System.Drawing.Imaging;
using System.Drawing;
using System;
using System.Drawing.Drawing2D;
using System.Collections.Generic;

namespace LEDLIB
{
    public delegate void SetPixel(double x, double y, RGB color);
    public class DrawUtility
    {
        private static float DENCITY = 2f;

        public struct Dot
        {
            public double X;
            public double Y;
            public double Z;
            public RGB RGB;

            public Dot(double x, double y, double z)
                :this(x, y, z, new RGB(0xff, 0xff, 0xff))
            {

            }

            public Dot(double x, double y, RGB rgb)
                :this(x, y, 0, rgb)
            {

            }
            public Dot(double x, double y, double z, RGB rgb)
            {
                this.X = x;
                this.Y = y;
                this.Z = z;
                this.RGB = rgb;
            }
        }
        public struct XYZDot
        {
            public double X;
            public double Y;
            public double Z;
            public RGB RGB;
            public XYZDot(double x, double y, double z, RGB rgb)
            {
                this.X = x;
                this.Y = y;
                this.Z = z;
                this.RGB = rgb;
            }
        }

        static Bitmap createBitmap(double w, double h)
        {
            Bitmap bitmap = new Bitmap(1 + Util.Round(w * 1.5f * DENCITY),
                                           1 + Util.Round(h * 1.5f * DENCITY),
                                           PixelFormat.Format32bppArgb);
            bitmap.MakeTransparent();
            return bitmap;
        }

        static public XYZDot[] Line(XYZDot pt1, XYZDot pt2)
        {
            return null;
        }

        static public PointF getMin(PointF[] pts)
        {
            var minPt = new PointF(float.MaxValue, float.MaxValue);
            foreach(var pt in pts)
            {
                if (pt.X < minPt.X)
                {
                    minPt.X = pt.X;
                }
                if (pt.Y < minPt.Y)
                {
                    minPt.Y = pt.Y;
                }
            }
            return minPt;
        }
        static public PointF getMax(PointF[] pts)
        {
            var maxPt = new PointF(float.MinValue, float.MinValue);
            foreach (var pt in pts)
            {
                if (pt.X > maxPt.X)
                {
                    maxPt.X = pt.X;
                }
                if (pt.Y > maxPt.Y)
                {
                    maxPt.Y = pt.Y;
                }
            }
            return maxPt;
        }

        static Graphics craeteGraphics(Bitmap bmp, SmoothingMode mode)
        {
            var gc = Graphics.FromImage(bmp);
            gc.SmoothingMode = mode;
            gc.ScaleTransform(DENCITY, DENCITY);
            return gc;
        }

        static public Dot[] FillPolygon(PointF[] pts, RGB rgb)
        {
            PointF minPt, maxPt;
            minPt = getMin(pts);
            maxPt = getMax(pts);

            float width = maxPt.X - minPt.X;
            float height = maxPt.Y - minPt.Y;

            using (var bmp = createBitmap(width, height))
            {
                using (var gc = craeteGraphics(bmp, SmoothingMode.AntiAlias))
                {
                    var brush = new SolidBrush(rgb.ToColor());

                    var newPts = new List<PointF>();
                    foreach (var pt in pts)
                    {
                        newPts.Add(new PointF(pt.X - minPt.X, pt.Y - minPt.Y));
                    }
                    gc.FillPolygon(brush, newPts.ToArray());

                }
                return GetPixel(bmp, minPt);
            }
        }

        static public Dot[] Line(PointF pt1, PointF pt2, RGB rgb, float penwidth)
        {
            float x, y, width, height;
            x = Math.Min(pt1.X, pt2.X);
            y = Math.Min(pt1.Y, pt2.Y);
            width = Math.Max(pt1.X, pt2.X) - x;
            height = Math.Max(pt1.Y, pt2.Y) - y;


            using (var bmp = createBitmap(width + penwidth, height + penwidth))
            {
                using (var gc = craeteGraphics(bmp, SmoothingMode.AntiAlias))
                {
                    var pen = new Pen(rgb.ToColor(), penwidth);
                    gc.DrawLine(pen, pt1.X - x + penwidth /2, 
                        pt1.Y - y + penwidth / 2,
                        pt2.X - x + penwidth / 2,
                        pt2.Y - y + penwidth /2);

                }

                return GetPixel(bmp, new PointF(x + penwidth / 2, y + penwidth / 2));

            }
        }

        static public Dot[] Rectangle(RectangleF rect, RGB rgb)
        {
            using (var bmp = createBitmap(rect.Width, rect.Height))
            {
                using (var gc = craeteGraphics(bmp, SmoothingMode.None))
                {
                    var pen = new Pen(rgb.ToColor());
                    gc.DrawRectangle(pen, 0f, 0f, rect.Width, rect.Height);

                }

                return GetPixel(bmp, new PointF(rect.X, rect.Y));
            }
        }

        static public Dot[] Circle(double x, double y, double w, double h, RGB rgb)
        {
            using (var bmp = createBitmap(w, h))
            {

                using (var gc = craeteGraphics(bmp, SmoothingMode.HighSpeed))
                {
                    var pen = new Pen(rgb.ToColor(), 1f);
                    gc.DrawEllipse(pen, 0f, 0f, (float)w, (float)h);

                }

                return GetPixel(bmp, new PointF((float)x, (float)y));
            }
        }
        static public Dot[] CircleCenterAt(double x, double y, double w, double h, RGB rgb)
        {
            using (var bmp = createBitmap(w, h))
            {
                using (var gc = craeteGraphics(bmp, SmoothingMode.AntiAlias))
                {
                    var pen = new Pen(rgb.ToColor(), 1f);
                    gc.DrawEllipse(pen, 0f, 0f, (float)w, (float)h);
                    //                    gc.DrawEllipse(pen, (float)(x - w/2), (float)(y - h/2), (float)w, (float)h);

                }

                return GetPixel(bmp, new PointF((float)(x - w / 2), (float)(y - h / 2)));
            }
        }
        static private void SetPixel(Bitmap bmp, SetPixel setPixel)
        {
            for (int x = 0; x < LED.WIDTH; x++)
            {
                for (int y = 0; y < LED.HEIGHT; y++)
                {
                    if (bmp.GetPixel(x, y).A != 0)
                    {
                        Console.WriteLine("x:" + x + " y:" + y);
                        setPixel(x, y, RGB.fromColor(bmp.GetPixel(x, y)));
                    }
                }
            }
        }
        static private Dot[] GetPixel(Bitmap bmp, PointF offset)
        {
            List<Dot> points = new List<Dot>();
            for (int x = 0; x < bmp.Width; x++)
            {
                for (int y = 0; y < bmp.Height; y++)
                {
                    if (bmp.GetPixel(x, y).A != 0)
                    {
                        points.Add(new Dot(
                            (x / DENCITY + offset.X), 
                            (y / DENCITY + offset.Y),
                            RGB.fromColor(bmp.GetPixel(x, y))));
                    }
                }
            }
            return points.ToArray();
        }
 
    }
}
