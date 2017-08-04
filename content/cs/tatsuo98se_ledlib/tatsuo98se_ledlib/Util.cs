using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class Util
    {
        public static int Round(double val)
        {
            return (int)Math.Round(val, MidpointRounding.AwayFromZero);
        }

        // RGBをHSVに変換
        public static HSV RGB2HSV(RGB RGB)
        {
            HSV hsv = new HSV();

            double max = Math.Max(Math.Max(RGB.R, RGB.G), RGB.B);
            double min = Math.Min(Math.Min(RGB.R, RGB.G), RGB.B);

            hsv.V = (int)max;

            if (max == min)
            {
                hsv.H = 0;
                hsv.S = 0;
            }
            else
            {
                hsv.S = (int)(((max - min) * 255) / max);

                hsv.H = 0;

                if (max == RGB.R)
                {
                    hsv.H = (int)(60 * (RGB.G - RGB.B) / (max - min));
                }
                else if (max == RGB.G)
                {
                    hsv.H = (int)(60 * (2 + (RGB.B - RGB.R) / (max - min)));
                }
                else if (max == RGB.B)
                {
                    hsv.H = (int)(60 * (4 + (RGB.R - RGB.G) / (max - min)));
                }

                if (hsv.H < 0) hsv.H += 360;
            }
            return hsv;
        }

        // HSVをRGBに変換
        public static RGB HSV2RGB(int H, int S, int V, int A)
        {
            if (H > 360) H = 360;
            if (S > 360) S = 360;
            if (V > 360) V = 360;
            if (H == 360) H = 0;

            int Hi = (int)Math.Floor((double)H / 60) % 6;

            float f = ((float)H / 60) - Hi;
            float p = ((float)V / 255) * (1 - ((float)S / 255));
            float q = ((float)V / 255) * (1 - f * ((float)S / 255));
            float t = ((float)V / 255) * (1 - (1 - f) * ((float)S / 255));

            p *= 255;
            q *= 255;
            t *= 255;

            RGB rgb = null;

            switch (Hi)
            {
                case 0:
                    rgb = new RGB(V, (int)t, (int)p, A);
                    break;
                case 1:
                    rgb = new RGB((int)q, V, (int)p, A);
                    break;
                case 2:
                    rgb = new RGB((int)p, V, (int)t, A);
                    break;
                case 3:
                    rgb = new RGB((int)p, (int)q, V, A);
                    break;
                case 4:
                    rgb = new RGB((int)t, (int)p, V, A);
                    break;
                case 5:
                    rgb = new RGB(V, (int)p, (int)q, A);
                    break;
            }

            return rgb;
        }
    }
}
