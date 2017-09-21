using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LEDLIB
{
    public class LED3DSheet : LED3DObject
    {


        public LED3DSheet(RGB color)
            :base(color, TimeSpan.Zero)
        {
        }


        public override void Draw(ILED3DCanvas canvas)
        {
            for(int x = 0; x<LED.WIDTH; x++)
            {
                for(int z=0; z<LED.DEPTH; z++)
                {
                    canvas.SetLed(x, LED.HEIGHT -1, z, this.Color - (Math.Sin(GetElapsedAt().TotalMilliseconds / 400) * 150));
                }
            }
        }
    }
}

