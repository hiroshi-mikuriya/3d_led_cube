using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace LEDLIB
{
    public class LED3DCanvasFilter : ILED3DCanvas
    {
        ILED3DCanvas canvas;
        public LED3DCanvasFilter(ILED3DCanvas canvas)
        {
            this.canvas = canvas;
        }


        virtual public void SetLed(double x, double y, double z, RGB color)
        {
            this.canvas.SetLed(x, y, z, color);
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        virtual public void Show()
        {
            this.canvas.Show();
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        virtual public int GetObjectCount()
        {
            return this.canvas.GetObjectCount();
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        virtual public void AddObject(LED3DObject obj)
        {
            this.canvas.AddObject(obj);
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        virtual public void AddObject(LED3DObject obj, ILED3DCanvas filter)
        {
            this.canvas.AddObject(obj, filter);
        }
    }
}
