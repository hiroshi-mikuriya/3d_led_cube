using LEDLIB;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample.ShowCase
{
    class AngleOnCube : IShowCase
    {
        public void SetUp(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {
            canvas.AddObject(new LED3D6thAngel(), filter);
        }

        public void Run(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {

        }
    }
}
