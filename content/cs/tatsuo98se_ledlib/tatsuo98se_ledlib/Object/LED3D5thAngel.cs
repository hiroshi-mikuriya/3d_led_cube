using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using static LEDLIB.DrawUtility;

namespace LEDLIB
{
    public class LED3D5thAngel : LED3DObject
    {
        DrawUtility drawUtility = new DrawUtility(2f);

        public LED3D5thAngel()
            : base(null)
        {

        }

        const int triangle_height = 8;
        const int triangle_helf_width = 6;
        public double getXAngle()
        {
            return Math.Atan2(triangle_helf_width, triangle_height);
        }

        public double getUnderOffset()
        {
            return Math.Cos(getXAngle()) * triangle_height;
        }

        public Dot[] GetTriangle(RGB color)
        {
            PointF[] pts = new PointF[] {
                new PointF(0, 0),
                new PointF(-triangle_helf_width, triangle_height),
                new PointF(triangle_helf_width, triangle_height)
            };

            return drawUtility.FillPolygon(pts, color);


        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var yangle = (GetElapsedAt().TotalMilliseconds / 15) * (Math.PI / 180);

            var newDots = Get6thAngel(yangle, new Dot(8, 4, 7));

            foreach (var dot in newDots)
            {
                canvas.SetLed(dot.X, dot.Y, dot.Z, dot.RGB);
            }
        }

        public Dot[] Get6thAngel(double yangle, Dot offset)
        {
            var newDots = new List<Dot>();

            var under = Get6thAngelHelf(Math.PI, yangle);
            var forUnder = Util.GetOffsetMatrix(new Dot(0, getUnderOffset()*2, 0));
            newDots.AddRange(Util.appryMatrix(under, forUnder));

            var upper = Get6thAngelHelf(0, yangle);
            var forUpper = Util.GetOffsetMatrix(new Dot(0, 0, 0));
            newDots.AddRange(Util.appryMatrix(upper, forUpper));

            return Util.appryMatrix(newDots.ToArray(),
                Util.GetYAxisRotateMatrix(yangle, offset)).ToArray();

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
            var shift = Math.PI / 3;
            var xanglen = Normlize(xangle);
            var yanglen = Normlize(yangle + shift);

            var brightColor = new RGB(0xff, 0xff, 0xff);
            var darkColor = new RGB(0x22, 0x22, 0xbb);

            var distance = Math.Abs(Math.PI - yanglen);

            if (xanglen != 0)
            {
                var offsetColor = (int)Math.Round(68f * (1 - distance / Math.PI));
                return darkColor - offsetColor;
            }
            else
            {
                var offsetColor = (int)Math.Round(222f * (1 - distance / Math.PI));
                return new RGB(
                    0xcc - offsetColor,
                    0xcc - offsetColor,
                    0xff);

            }
        }

        public Dot[] Get6thAngelBaseTriangle(double xangle, double yangle)
        {
            var M = Util.GetXAxisRotateMatrix(getXAngle());
            return Util.appryMatrix(GetTriangle(GetTriangleColor(xangle, yangle)), M);

        }

        public Dot[] Get6thAngelHelf(double xangle, double yangle)
        {
            var newDots = new List<Dot>();

            Dot[] t1 = Get6thAngelBaseTriangle(xangle, yangle);

            var for2 = Util.GetYAxisRotateMatrix(Math.PI / 2);
            Dot[] t2 = Util.appryMatrix(Get6thAngelBaseTriangle(xangle, yangle + (Math.PI / 2)), for2);

            var for3 = Util.GetYAxisRotateMatrix(Math.PI);
            Dot[] t3 = Util.appryMatrix(Get6thAngelBaseTriangle(xangle, yangle + Math.PI), for3);

            var for4 = Util.GetYAxisRotateMatrix(- Math.PI / 2);
            Dot[] t4 = Util.appryMatrix(Get6thAngelBaseTriangle(xangle, yangle - (Math.PI / 2)), for4);


            newDots.AddRange(t1);
            newDots.AddRange(t2);
            newDots.AddRange(t3);
            newDots.AddRange(t4);

            var xaxis = Util.GetXAxisRotateMatrix(xangle);
            return Util.appryMatrix(newDots.ToArray(), xaxis).ToArray();
        }


    }
}
