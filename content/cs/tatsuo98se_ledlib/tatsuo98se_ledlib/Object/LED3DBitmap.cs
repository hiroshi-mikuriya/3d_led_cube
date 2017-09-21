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
        DateTime lastUpdate;

        private List<List<Dot>> dots = new List<List<Dot>>();


        DrawUtility drawUtility = new DrawUtility(1.5f);

        public LED3DBitmap(RGB color)
            : base(color, TimeSpan.Zero)
        {
        }

        public void SetBitmap(Bitmap bitmap)
        {
            dots.Insert(0, new List<Dot>(drawUtility.Bitmap(bitmap)));
            if(dots.Count > LED.DEPTH)
            {
                dots.RemoveAt(dots.Count - 1);
            }
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var count = 0;
            foreach (var layer in this.dots)
            {
                foreach (var dot in layer)
                {
                    var hsv = Util.RGB2HSV(dot.RGB);
                    hsv.S = hsv.V = 360;
                    hsv.H = 360 * count / LED.DEPTH;
                    canvas.SetLed(dot.X, dot.Y, count, Util.HSV2RGB(hsv.H, hsv.S, hsv.V, dot.RGB.A));

                }
                count++;
            }
        }
    }
}
