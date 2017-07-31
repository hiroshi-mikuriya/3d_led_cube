using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class LED3DSurfaceCanvasFilter : LED3DCanvasFilter
    {

        public LED3DSurfaceCanvasFilter(ILED3DCanvas canvas)
            :base(canvas)
        {
        }

        override public void SetLed(double x, double y, double ignore, RGB color)
        {
            double dotx, doty, dotz;

            dotz = 0;
            dotx = x;


            if((x > LED.WIDTH - 1 || x < 0) && (y > LED.HEIGHT -1 || y < 0))
            {
                return;
            }

            if(x < 0)
            {
                dotz = Math.Abs(x);
                dotx = 0;
            }
            if (x > LED.WIDTH - 1)
            {
                dotz = x - LED.WIDTH;
                dotx = LED.WIDTH - 1;
            }
            if (x < -LED.DEPTH - 1)
            {
                dotz = LED.DEPTH - 1;
                dotx = Math.Abs(x) - dotz;
            }
            if(x > LED.WIDTH - 1 + LED.DEPTH - 1)
            {
                dotz = LED.DEPTH - 1;
                dotx = LED.WIDTH - (x - (LED.WIDTH - 1 + LED.DEPTH - 1));
            }

            doty = y;
            if(y < 0)
            {
                dotz = Math.Abs(y);
                doty = 0;
            }
            if(y > LED.HEIGHT - 1)
            {
                dotz = y - LED.HEIGHT;
                doty = LED.HEIGHT - 1;
            }
            if(y < -LED.DEPTH - 1)
            {
                dotz = LED.DEPTH - 1;
                doty = Math.Abs(y) - dotz;
            }
            if (y > LED.HEIGHT - 1 + LED.DEPTH - 1)
            {
                dotz = LED.DEPTH - 1;
                doty = LED.HEIGHT - (y - (LED.HEIGHT - 1 + LED.DEPTH - 1));
            }
            base.SetLed(dotx, doty, dotz, color);

        }
    }
}
