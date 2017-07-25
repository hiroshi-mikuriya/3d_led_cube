using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static LEDLIB.DrawUtility;

namespace LEDLIB
{
    class LED3DCircle : LED3DObject
    {
        private float x;
        private float y;
        private float w;
        private float h;

        public LED3DCircle(float x, float y, float w, float h, RGB color) : this(x, y, w, h, color, TimeSpan.Zero)
        {
        }
        public LED3DCircle(float x, float y, float w, float h, RGB color, TimeSpan lifeTime) : base(color, lifeTime)
        {
            this.x = x + w / 2;
            this.y = y + h / 2;
            this.w = w;
            this.h = h;
        }

        public override void Draw(LED3DCanvas canvas)
        {
            var dots = DrawUtility.Circle(this.x, this.y, this.w, this.h, this.Color);
            foreach(var dot in dots)
            {
                canvas.SetLed(dot.X, dot.Y, 0, dot.RGB);

            }
        }
    }
}
