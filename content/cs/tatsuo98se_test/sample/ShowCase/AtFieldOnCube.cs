using LEDLIB;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample.ShowCase
{
    class AtFieldOnCube : IShowCase
    {
        public void SetUp(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {
        }

        public void Run(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {
            var rand = new Random();
            while (true)
            {
                if (canvas.GetObjectCount() < 1)
                {
                    var x = rand.Next() % LED.WIDTH;
                    var y = rand.Next() % LED.HEIGHT;
                    canvas.AddObject(new LED3DAtField(x, y, 0, 0, 4)
                        , filter
                        );
                }
                canvas.Show();
                LED.Wait(20);
            }
        }
    }
}
