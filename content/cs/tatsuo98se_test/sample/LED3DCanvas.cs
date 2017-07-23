using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    class LED3DCanvas
    {
        List<LED3DObject> objects = new List<LED3DObject>();

        public LED3DCanvas()
        {

        }

        public void SetLed(float x, float y, float z, RGB color)
        {
            LED.SetLed(Convert.ToInt32(x), 
                Convert.ToInt32(y), 
                Convert.ToInt32(z),
                color.ToInt());
        }

        public void Show()
        {
            LED.Clear();
            var deleteList = new List<LED3DObject>();
            foreach (var obj in this.objects)
            {
                if (obj.IsExpired())
                {
                    deleteList.Add(obj);
                }
                else
                {
                    obj.Draw(this);
                }
            }
            foreach(var obj in deleteList)
            {
                this.objects.Remove(obj);
            }
            LED.Show();

        }

        public int GetObjectCount()
        {
            return this.objects.Count;
        }

        public void AddObject(LED3DObject obj)
        {
            this.objects.Add(obj);
        }
    }
}
