using LEDLIB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample.ShowCase
{
    interface IShowCase
    {
        void SetUp(ILED3DCanvas canvas, LED3DCanvasFilter filter);
        void Run(ILED3DCanvas canvas, LED3DCanvasFilter filter);
    }
}
