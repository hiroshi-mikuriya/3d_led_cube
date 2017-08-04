using LEDLIB;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample.ShowCase
{
    class AngelOnCube : IShowCase
    {
        public void SetUp(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {
            canvas.AddObject(new LED3D5thAngel(), filter);
        }

        public void Run(ILED3DCanvas canvas, LED3DCanvasFilter filter)
        {

        }
    }
}
