using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static LEDLIB.DrawUtility;

namespace LEDLIB
{
    public class LED3DCircle : LED3DObject
    {
        private double x;
        private double y;
        private double w;
        private double h;

        public LED3DCircle(double x, double y, double w, double h, RGB color) : this(x, y, w, h, color, TimeSpan.Zero)
        {
        }
        public LED3DCircle(double x, double y, double w, double h, RGB color, TimeSpan lifeTime) : base(color, lifeTime)
        {
            this.x = x + w / 2;
            this.y = y + h / 2;
            this.w = w;
            this.h = h;
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var dots = DrawUtility.Circle(this.x, this.y, this.w, this.h, this.Color);
            foreach(var dot in dots)
            {
                canvas.SetLed(dot.X, dot.Y, 0, dot.RGB);

            }
        }
    }
}
