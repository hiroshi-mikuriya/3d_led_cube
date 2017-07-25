using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LEDLIB
{
    class LED3DDot : LED3DObject
    {
        float x;
        float y;
        float z;

        public LED3DDot(float x, float y, float z, RGB color)
            : this(x, y, z, color, TimeSpan.Zero)
        {

        }

        public LED3DDot(float x, float y, float z, RGB color, TimeSpan lifeTime)
            :base(color, lifeTime)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }


        public override void Draw(LED3DCanvas canvas)
        {
            canvas.SetLed(this.x, this.y, this.z, this.Color);
        }
    }
}

