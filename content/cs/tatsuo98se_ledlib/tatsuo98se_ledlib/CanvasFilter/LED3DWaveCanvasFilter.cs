using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class LED3DWaveCanvasFilter : LED3DCanvasFilter
    {
        DateTime bornAt;

        public LED3DWaveCanvasFilter(ILED3DCanvas canvas)
            :base(canvas)
        {
            bornAt = DateTime.Now;
        }

        override public void SetLed(double x, double y, double z, RGB color)
        {
            TimeSpan offset = DateTime.Now.Subtract(bornAt);
            double ywavelength = 3 * Math.PI;
            double ywavedepth = 1.5;
            double ydot = ywavelength / LED.HEIGHT;
            double ystart = (offset.TotalMilliseconds / 200) * ydot;

            double xwavelength = 3 * Math.PI;
            double xwavedepth = 1.5;
            double xdot = xwavelength / LED.WIDTH;
            double xstart = (offset.TotalMilliseconds / 130) * xdot;

            z = (float)(
                (xwavedepth + Math.Sin(xdot * x + xstart) * xwavedepth)
                +
                (ywavedepth + Math.Sin(ydot * y + ystart) * ywavedepth));


            base.SetLed(x, y, z, color);
        }
    }
}
