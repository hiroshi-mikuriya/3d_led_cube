using System.Drawing.Imaging;
using System.Drawing;
using sample;
using System;
using System.Drawing.Drawing2D;
using System.Collections.Generic;

namespace LEDLIB
{
    delegate void SetPixel(float x, float y, RGB color);
    class DrawUtility
    {
        public struct Dot
        {
            public float X;
            public float Y;
            public RGB RGB;
            public Dot(float x, float y, RGB rgb)
            {
                this.X = x;
                this.Y = y;
                this.RGB = rgb;
            }
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

        static public Dot[] Circle(float x, float y, float w, float h, RGB rgb)
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
                    gc.DrawEllipse(pen, x, y, w, h);

                }

                return GetPixel(bmp);
            }
        }
        static public Dot[] CircleCenterAt(float x, float y, float w, float h, RGB rgb)
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
                    gc.DrawEllipse(pen, x - w/2, y - h/2, w, h);

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
