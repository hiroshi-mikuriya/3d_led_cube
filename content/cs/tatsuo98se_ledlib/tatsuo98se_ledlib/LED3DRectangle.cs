using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static LEDLIB.DrawUtility;

namespace LEDLIB
{
    public class LED3DRectangle : LED3DObject
    {
        private Rectangle rect;
        public LED3DRectangle(Rectangle rect, RGB color) : this(rect, color, TimeSpan.Zero)
        {
        }
        public LED3DRectangle(Rectangle rect, RGB color, TimeSpan lifeTime) : base(color, lifeTime)
        {
            this.rect = rect;
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var dots = DrawUtility.Rectangle(rect, this.Color);
            foreach(var dot in dots)
            {
                canvas.SetLed(dot.X, dot.Y, 0, dot.RGB);
            }
        }
    }
}
