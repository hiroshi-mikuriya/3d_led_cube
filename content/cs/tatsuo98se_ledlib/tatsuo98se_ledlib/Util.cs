using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using static LEDLIB.DrawUtility;
using MathNet.Numerics.LinearAlgebra.Double;


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

        public static DenseMatrix GetOffsetMatrix(Dot offset)
        {
            return DenseMatrix.OfArray(new double[,] {
                { 1, 0, 0, offset.X },
                { 0, 1, 0, offset.Y },
                { 0, 0, 1, offset.Z },
                { 0, 0, 0, 1 } });
        }

        public static DenseMatrix GetXAxisRotateMatrix(double rad)
        {
            return GetXAxisRotateMatrix(rad, new Dot(0, 0, 0));
        }

        public static DenseMatrix GetXAxisRotateMatrix(double rad, Dot offset)
        {
            return DenseMatrix.OfArray(new double[,] {
                { 1, 0, 0, offset.X },
                { 0, Math.Cos(rad), -Math.Sin(rad), offset.Y },
                { 0, Math.Sin(rad), Math.Cos(rad), offset.Z },
                { 0, 0, 0, 1 } });
        }

        public static DenseMatrix GetYAxisRotateMatrix(double rad)
        {
            return GetYAxisRotateMatrix(rad, new Dot(0, 0, 0));
        }

        public static DenseMatrix GetYAxisRotateMatrix(double rad, Dot offset)
        {
            return DenseMatrix.OfArray(new double[,] {
                { Math.Cos(rad), 0, Math.Sin(rad), offset.X },
                { 0, 1, 0, offset.Y },
                { -Math.Sin(rad),0, Math.Cos(rad), offset.Z },
                { 0, 0, 0, 1 } });
        }
        public static DenseMatrix GetZAxisRotateMatrix(double rad)
        {
            return GetZAxisRotateMatrix(0, new Dot(0, 0, 0));
        }

        public static DenseMatrix GetZAxisRotateMatrix(double rad, Dot offset)
        {
            return DenseMatrix.OfArray(new double[,]{
                { Math.Cos(rad), -Math.Sin(rad), 0, offset.X },
                { Math.Sin(rad), Math.Cos(rad), 0, offset.Y },
                { 0, 0, 1, offset.Z },
                { 0, 0, 0, 1 } });
        }

        public static DenseMatrix fromXYZ(double x, double y, double z)
        {
            return DenseMatrix.OfArray(new double[,] {
                { x },
                { y },
                { z },
                { 1 } });
        }

        public static Dot[] appryMatrix(Dot[] src, DenseMatrix matrix)
        {

            var newdot = new List<Dot>();
            foreach (var pt in src)
            {
                var s = fromXYZ(pt.X, pt.Y, pt.Z);
                var result = matrix.Multiply(s);
                newdot.Add(new Dot(
                    result[0, 0],
                    result[1, 0],
                    result[2, 0],
                    pt.RGB));
            }
            return newdot.ToArray();
        }

    }
}
