using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class LED3DHsvColorFilter : LED3DCanvasFilter
    {
        public LED3DHsvColorFilter(ILED3DCanvas canvas)
            :base(canvas)
        {
        }

        override public void SetLed(double x, double y, double z, RGB color)
        {

            var hsv = new HSV((int)(360 * (0.8 - z / LED.DEPTH)), 360, 360);

            base.SetLed(x, y, z, Util.HSV2RGB(hsv.H, hsv.S, hsv.V, color.A));
        }
    }
}
