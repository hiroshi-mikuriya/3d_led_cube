using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class LED3DWaveCanvasFilter2 : LED3DCanvasFilter
    {
        DateTime bornAt;
        double MAX_WAVE_DEPTH = 1.5;
        DateTime lastUpdate;

        public LED3DWaveCanvasFilter2(ILED3DCanvas canvas)
            :base(canvas)
        {
            bornAt = DateTime.Now;
            lastUpdate = DateTime.Now;
        }

        public void SetMaxWaveDepth()
        {
            lastUpdate = DateTime.Now;


        }

        override public void SetLed(double x, double y, double z, RGB color)
        {
            TimeSpan offset = DateTime.Now.Subtract(bornAt);
            TimeSpan elasped = DateTime.Now.Subtract(lastUpdate);
            var waveDepth = Math.Max(0, MAX_WAVE_DEPTH - elasped.TotalMilliseconds / 1000f);
            double ywavelength = 3 * Math.PI;
            double ywavedepth = waveDepth;
            double ydot = ywavelength / LED.HEIGHT;
            double ystart = (offset.TotalMilliseconds / 200) * ydot;

            double xwavelength = 3 * Math.PI;
            double xwavedepth = waveDepth;
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
