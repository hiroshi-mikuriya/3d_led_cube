using LEDLIB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample.ShowCase
{
    class BallOnCube : IShowCase
    {

        public void SetUp(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {
            canvas.AddObject(new LED3DBall(4, 4, 4, 8, 8, new RGB(0xff, 0xff, 0xff)), filter);
        }

        public void Run(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {

        }
    }
}
