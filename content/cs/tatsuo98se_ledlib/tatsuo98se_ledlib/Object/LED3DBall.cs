using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static LEDLIB.DrawUtility;

namespace LEDLIB
{
    public class LED3DBall : LED3DObject
    {
        private double x;
        private double y;
        private double z;
        private double w;
        private double h;

        DrawUtility drawUtility = new DrawUtility(1.5f);

        public LED3DBall(double x, double y, double z, double w, double h, RGB color) : this(x, y, z, w, h, color, TimeSpan.Zero)
        {
        }
        public LED3DBall(double x, double y, double z, double w, double h, RGB color, TimeSpan lifeTime) : base(color, lifeTime)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
            this.h = h;
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var src = drawUtility.CircleCenterAt(0, 0, this.w, this.h, this.Color);

            var dots = new List<Dot>();
            var offset = Util.GetOffsetMatrix(new Dot(x, y, z));
            for(double i=0; i<2*Math.PI; i=i+(2*Math.PI / (w * 3)))
            {

                dots.AddRange(Util.appryMatrix(src, Util.GetYAxisRotateMatrix(i, new Dot(x, y, z))));

            }

            foreach (var dot in dots)
            {
                canvas.SetLed(dot.X, dot.Y, dot.Z, dot.RGB);

            }
        }
    }
}
