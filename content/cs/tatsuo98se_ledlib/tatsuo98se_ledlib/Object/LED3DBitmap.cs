using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static LEDLIB.DrawUtility;

namespace LEDLIB
{
    public class LED3DBitmap : LED3DObject
    {
        private double x;
        private double y;
        private double w;
        private double h;

        private List<Dot> dots = new List<Dot>();


        DrawUtility drawUtility = new DrawUtility(1.5f);

        public LED3DBitmap(RGB color)
            : base(color, TimeSpan.Zero)
        {
        }

        public void SetBitmap(Bitmap bitmap)
        {
            dots = new List<Dot>(drawUtility.Bitmap(bitmap));
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            foreach(var dot in this.dots)
            {
                canvas.SetLed(dot.X, dot.Y, 7, dot.RGB);

            }
        }
    }
}
