using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace LEDLIB
{
    public partial class LED3DCanvas : ILED3DCanvas
    {

        object[,,] canvas = new object[LED.WIDTH, LED.HEIGHT, LED.DEPTH];

        List<LED3DObjectSet> objects = new List<LED3DObjectSet>();


        public LED3DCanvas()
        {

        }

        public void SetLed(double x, double y, double z, RGB color)
        {
            int ix, iy, iz;
            ix = Util.Round(x);
            iy = Util.Round(y);
            iz = Util.Round(z);

            if (ix < 0 || ix >= LED.WIDTH)
            {
                return;
            }
            if (iy < 0 || iy >= LED.HEIGHT)
            {
                return;
            }
            if (iz < 0 || iz >= LED.DEPTH)
            {
                return;
            }

            if (color.A == 0x00)
            {
                return;
            }

            RGB setcolor = color;

            if(color.A != 0xff) {
                RGB dest;

                if (this.canvas[ix, iy, iz] != null)
                {
                    dest = (RGB)this.canvas[ix, iy, iz];
                }
                else
                {
                    dest = new RGB(0, 0, 0);
                }
                // alpha blend
                // Cr = Cd * (1 - As) + Cs * As
                float newr = dest.Rf * (1 - color.Af) + color.Rf * color.Af;
                float newg = dest.Gf * (1 - color.Af) + color.Gf * color.Af;
                float newb = dest.Bf * (1 - color.Af) + color.Bf * color.Af;

                setcolor = new RGB(newr, newg, newb);
            }


            this.canvas[ix, iy, iz] = (object)setcolor;
            LED.SetLed(ix, iy, iz,
               setcolor.ToInt());
        }


        [MethodImpl(MethodImplOptions.Synchronized)]
        public void Show()
        {
            this.Clear();
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
        public List<LED3DObjectSet> GetObjects()
        {
            return this.objects;
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void RemoveObject(LED3DObjectSet obj)
        {
            this.objects.Remove(obj);
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

        private void Clear()
        {
            for(int x = 0; x<LED.WIDTH; x++)
            {
                for(int y = 0; y<LED.HEIGHT; y++)
                {
                    for(int z = 0; z<LED.DEPTH; z++)
                    {
                        canvas[x, y, z] = null;
                    }
                }
            }
            LED.Clear();
        }
    }

}
