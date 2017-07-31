using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LEDLIB
{
    public class LED3DRipple : LED3DObject
    {

        private double x;
        private double y;
        private LED.Direction direction;
        private List<float> rs = new List<float>();
        private TimeSpan lastUpdateAt;
        private int size;

        float FIRST_R = 2.0f;

        public LED3DRipple(double x, double y, RGB color, LED.Direction direction, int size)
            :base(color, TimeSpan.Zero)
        {
            this.x = x;
            this.y = y;
            this.direction = direction;
            this.rs.Add(FIRST_R);
            this.lastUpdateAt = this.GetElapsedAt();
            this.size = size;
        }

        public override bool IsExpired()
        {
            return this.rs.Min() > 32;
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var isNeedUpdate = false;
            if(this.GetElapsedAt().Subtract(this.lastUpdateAt).Milliseconds > 100)
            {
                this.lastUpdateAt = this.GetElapsedAt();
                isNeedUpdate = true;
            }
            
            if( this.rs.Min() - 2 >= FIRST_R && this.rs.Count < this.size)
            {
                this.rs.Add(FIRST_R);
            }

            var newRs = new List<float>();

            foreach(var r in this.rs)
            {
                var dots = DrawUtility.CircleCenterAt(x, y, r * 2, r * 2, this.Color - r * 15);

                foreach(var dot in dots)
                {
                    canvas.SetLed(dot.X, dot.Y, 0, dot.RGB);
                }    
    

                if (isNeedUpdate)
                {
                    newRs.Add(r + 1);
                }
                else
                {
                    newRs.Add(r);
                }
            }
            this.rs = newRs;
        }
    }
}
