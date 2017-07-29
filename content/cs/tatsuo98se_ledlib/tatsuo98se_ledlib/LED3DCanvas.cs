using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace LEDLIB
{
    public class LED3DCanvas : ILED3DCanvas
    {
        public class LED3DObjectSet
        {
            public LED3DObject obj;
            public ILED3DCanvas filter;

            public LED3DObjectSet(LED3DObject obj, ILED3DCanvas filter)
            {
                this.obj = obj;
                this.filter = filter;
            }

            public LED3DObjectSet(LED3DObject obj)
                : this(obj, null)
            {

            }
        }


        List<LED3DObjectSet> objects = new List<LED3DObjectSet>();


        public LED3DCanvas()
        {

        }

        public void SetLed(double x, double y, double z, RGB color)
        {
            LED.SetLed(Convert.ToInt32(x),
                Convert.ToInt32(y),
                Convert.ToInt32(z),
                color.ToInt());
        }


        [MethodImpl(MethodImplOptions.Synchronized)]
        public void Show()
        {
            LED.Clear();
            var deleteList = new List<LED3DObjectSet>();
            foreach (var obj in this.objects)
            {
                if (obj.obj.IsExpired())
                {
                    deleteList.Add(obj);
                }
                else
                {
                    if (obj.filter == null)
                    {
                        obj.obj.Draw(this);
                    }
                    else
                    {
                        obj.obj.Draw(obj.filter);
                    }
                }
            }
            foreach (var obj in deleteList)
            {
                this.objects.Remove(obj);
            }
            LED.Show();

        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public int GetObjectCount()
        {
            return this.objects.Count;
        }


        [MethodImpl(MethodImplOptions.Synchronized)]
        public void AddObject(LED3DObject obj)
        {
            this.objects.Add(new LED3DObjectSet(obj));
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void AddObject(LED3DObject obj, ILED3DCanvas filter)
        {
            this.objects.Add(new LED3DObjectSet(obj, filter));
        }
    }

}
