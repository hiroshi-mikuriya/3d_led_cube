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
        public struct Dot
        {
            public double X;
            public double Y;
            public RGB RGB;
            public Dot(double x, double y, RGB rgb)
            {
                this.X = x;
                this.Y = y;
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

        static public XYZDot[] Line(XYZDot pt1, XYZDot pt2)
        {
            return null;
        }

        static public Dot[] Rectangle(RectangleF rect, RGB rgb)
        {
            var bmp = new Bitmap(LED.WIDTH, LED.HEIGHT, PixelFormat.Format32bppArgb);

            using (bmp)
            {
                bmp.MakeTransparent();

                var gc = Graphics.FromImage(bmp);
                using (gc)
                {
                    var pen = new Pen(rgb.ToColor());
                    gc.DrawRectangle(pen, rect.ToRectangle());

                }

                return GetPixel(bmp);
            }
        }

        static public Dot[] Circle(double x, double y, double w, double h, RGB rgb)
        {
            var bmp = new Bitmap(LED.WIDTH, LED.HEIGHT, PixelFormat.Format32bppArgb);

            using (bmp)
            {
                bmp.MakeTransparent();

                var gc = Graphics.FromImage(bmp);
                gc.SmoothingMode = SmoothingMode.HighSpeed;
                using (gc)
                {
                    var pen = new Pen(rgb.ToColor(), 1f);
                    gc.DrawEllipse(pen, (float)x, (float)y, (float)w, (float)h);

                }

                return GetPixel(bmp);
            }
        }
        static public Dot[] CircleCenterAt(double x, double y, double w, double h, RGB rgb)
        {
            var bmp = new Bitmap(LED.WIDTH, LED.HEIGHT, PixelFormat.Format32bppArgb);

            using (bmp)
            {
                bmp.MakeTransparent();

                var gc = Graphics.FromImage(bmp);
                gc.SmoothingMode = SmoothingMode.HighSpeed;
                using (gc)
                {
                    var pen = new Pen(rgb.ToColor(), 1f);
                    gc.DrawEllipse(pen, (float)(x - w/2), (float)(y - h/2), (float)w, (float)h);

                }

                return GetPixel(bmp);
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
        static private Dot[] GetPixel(Bitmap bmp)
        {
            List<Dot> points = new List<Dot>();
            for (int x = 0; x < LED.WIDTH; x++)
            {
                for (int y = 0; y < LED.HEIGHT; y++)
                {
                    if (bmp.GetPixel(x, y).A != 0)
                    {
                        points.Add(new Dot(x, y, RGB.fromColor(bmp.GetPixel(x, y))));
                    }
                }
            }
            return points.ToArray();
        }
 
    }
}
