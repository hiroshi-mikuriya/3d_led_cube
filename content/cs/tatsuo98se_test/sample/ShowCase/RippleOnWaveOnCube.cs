using LEDLIB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample.ShowCase
{
    class RippleOnWaveCube : IShowCase
    {

        public void SetUp(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {
        }

        public void Run(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {
            var rand = new Random();
            while (true)
            {
                if (canvas.GetObjectCount() < 2)
                {
                    var x = rand.Next() % LED.WIDTH;
                    var y = rand.Next() % LED.HEIGHT;
                    canvas.AddObject(new LED3DRipple(x, y, new RGB(0xff, 0x00, 0xff), LED.Direction.Front, 6)
                        , filter
                        );
                }
                canvas.Show();
                LED.Wait(20);
            }
        }
    }
}
