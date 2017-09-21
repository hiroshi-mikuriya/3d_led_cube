using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using static LEDLIB.LED3DCanvas;

namespace LEDLIB
{
    public interface ILED3DCanvas
    {
        void SetLed(double x, double y, double z, RGB color);

        [MethodImpl(MethodImplOptions.Synchronized)]
        void Show();

        [MethodImpl(MethodImplOptions.Synchronized)]
        int GetObjectCount();

        [MethodImpl(MethodImplOptions.Synchronized)]
        List<LED3DObjectSet> GetObjects();

        [MethodImpl(MethodImplOptions.Synchronized)]
        void RemoveObject(LED3DObjectSet obj);

        [MethodImpl(MethodImplOptions.Synchronized)]
        void AddObject(LED3DObject obj);

        [MethodImpl(MethodImplOptions.Synchronized)]
        void AddObject(LED3DObject obj, ILED3DCanvas filter);
    }
}
