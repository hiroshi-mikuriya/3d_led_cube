using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using static LEDLIB.DrawUtility;
using MathNet.Numerics.LinearAlgebra.Double;


namespace LEDLIB
{
    public class LED3D6thAngel : LED3DObject
    {
        public LED3D6thAngel()
            : base(null)
        {

        }

        public DenseMatrix GetOffsetMatrix(Dot offset)
        {
            return DenseMatrix.OfArray(new double[,] {
                { 1, 0, 0, offset.X },
                { 0, 1, 0, offset.Y },
                { 0, 0, 1, offset.Z },
                { 0, 0, 0, 1 } });
        }

        public DenseMatrix GetXAxisRotateMatrix(double rad)
        {
            return GetXAxisRotateMatrix(rad, new Dot(0, 0, 0));
        }
        public DenseMatrix GetXAxisRotateMatrix(double rad, Dot offset)
        {
            return DenseMatrix.OfArray(new double[,] { 
                { 1, 0, 0, offset.X }, 
                { 0, Math.Cos(rad), -Math.Sin(rad), offset.Y },
                { 0, Math.Sin(rad), Math.Cos(rad), offset.Z },
                { 0, 0, 0, 1 } });
        }
        public DenseMatrix GetYAxisRotateMatrix(double rad)
        {
            return GetYAxisRotateMatrix(rad, new Dot(0, 0, 0));
        }
        public DenseMatrix GetYAxisRotateMatrix(double rad, Dot offset)
        {
            return DenseMatrix.OfArray(new double[,] {
                { Math.Cos(rad), 0, Math.Sin(rad), offset.X },
                { 0, 1, 0, offset.Y },
                { -Math.Sin(rad),0, Math.Cos(rad), offset.Z },
                { 0, 0, 0, 1 } });
        }
        public DenseMatrix GetZAxisRotateMatrix(double rad)
        {
            return GetZAxisRotateMatrix(0, new Dot(0, 0, 0));
        }
        public DenseMatrix GetZAxisRotateMatrix(double rad, Dot offset)
        {
            return DenseMatrix.OfArray(new double[,]{
                { Math.Cos(rad), -Math.Sin(rad), 0, offset.X },
                { Math.Sin(rad), Math.Cos(rad), 0, offset.Y },
                { 0, 0, 1, offset.Z },
                { 0, 0, 0, 1 } });
        }
        public DenseMatrix fromXYZ(double x, double y, double z)
        {
            return DenseMatrix.OfArray(new double[,] {
                { x },
                { y },
                { z },
                { 1 } });
        }

        public Dot[] appryMatrix(Dot[] src, DenseMatrix matrix)
        {

            var newdot = new List<Dot>();
            foreach(var pt in src)
            {
                var s = fromXYZ(pt.X, pt.Y, pt.Z);
                var result = matrix.Multiply(s);
                newdot.Add(new Dot(
                    result[0,0],
                    result[1,0],
                    result[2,0],
                    pt.RGB));
            }
            return newdot.ToArray();
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var yangle = (GetElapsedAt().TotalMilliseconds / 15) * (Math.PI / 180);

            var newDots = Get6thAngel(yangle, new Dot(8, 13, 7));

            foreach (var dot in newDots)
            {
                canvas.SetLed(dot.X, dot.Y, dot.Z, dot.RGB);
            }
        }

        public Dot[] Get6thAngel(double yangle, Dot offset)
        {
            var newDots = new List<Dot>();

            var under = Get6thAngelHelf(Math.PI, yangle);
            var forUnder = GetOffsetMatrix(new Dot(0, 4, 0));
            newDots.AddRange(appryMatrix(under, forUnder));

            var upper = Get6thAngelHelf(0, yangle);
            var forUpper = GetOffsetMatrix(new Dot(0, -4, 0));
            newDots.AddRange(appryMatrix(upper, forUpper));

            return appryMatrix(newDots.ToArray(), GetYAxisRotateMatrix(yangle, offset)).ToArray();

        }

        public double Normlize(double angle)
        {
            double normalize = 0;
            if(angle < 0)
            {
                normalize = (2 * Math.PI) + angle % (2 * Math.PI);
            }
            else
            {
                normalize = angle % (2 * Math.PI);
            }
            return normalize;
        }

        public RGB GetTriangleColor( double xangle, double yangle)
        {
            var shift = Math.PI / 2;
            var xanglen = Normlize(xangle);
            var yanglen = Normlize(yangle + shift);

            var brightColor = new RGB(0xcc, 0xcc, 0xff);
            var darkColor = new RGB(0x33, 0x33, 0xff);

            var distance = Math.Abs(Math.PI - yanglen);

            if (xanglen != 0)
            {
                var offsetColor = (int)Math.Round(68f * (1 - distance / Math.PI));
                return darkColor - offsetColor;
            }
            else
            {
                var offsetColor = (int)Math.Round(204f * (1 - distance / Math.PI));
                return new RGB(
                    0xcc - offsetColor,
                    0xcc - offsetColor,
                    0xff);

            }
        }

        public Dot[] Get6thAngelBseTriangle(double xangle, double yangle)
        {
            var M = GetXAxisRotateMatrix(-Math.PI / 4);
            return appryMatrix(GetTriangle(GetTriangleColor(xangle, yangle)), M);

        }

        public Dot[] Get6thAngelHelf(double xangle, double yangle)
        {
            var newDots = new List<Dot>();

            Dot[] t1 = Get6thAngelBseTriangle(xangle, yangle);

            var for2 = GetYAxisRotateMatrix(Math.PI / 2);
            Dot[] t2 = appryMatrix(Get6thAngelBseTriangle(xangle, yangle + (Math.PI / 2)), for2);

            var for3 = GetYAxisRotateMatrix(Math.PI);
            Dot[] t3 = appryMatrix(Get6thAngelBseTriangle(xangle, yangle + Math.PI), for3);

            var for4 = GetYAxisRotateMatrix(- Math.PI / 2);
            Dot[] t4 = appryMatrix(Get6thAngelBseTriangle(xangle, yangle - (Math.PI / 2)), for4);


            newDots.AddRange(t1);
            newDots.AddRange(t2);
            newDots.AddRange(t3);
            newDots.AddRange(t4);

            var xaxis = GetXAxisRotateMatrix(xangle);
            return appryMatrix(newDots.ToArray(), xaxis).ToArray();
        }

        public Dot[] GetTriangle(RGB color)
        {
            PointF[] pts = new PointF[] {
                new PointF(0, 0),
                new PointF(-6, 6),
                new PointF(6, 6)
            };

            return DrawUtility.FillPolygon(pts, color);


        }

    }
}
