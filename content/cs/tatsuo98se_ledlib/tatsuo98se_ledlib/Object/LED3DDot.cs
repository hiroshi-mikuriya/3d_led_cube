using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LEDLIB
{
    public class LED3DDot : LED3DObject
    {
        double x;
        double y;
        double z;

        public LED3DDot(double x, double y, double z, RGB color)
            : this(x, y, z, color, TimeSpan.Zero)
        {

        }

        public LED3DDot(double x, double y, double z, RGB color, TimeSpan lifeTime)
            :base(color, lifeTime)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }


        public override void Draw(ILED3DCanvas canvas)
        {
            canvas.SetLed(this.x, this.y, this.z, this.Color);
        }
    }
}

