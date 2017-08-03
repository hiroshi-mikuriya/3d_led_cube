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
        private int produced = 0;

        float FIRST_R = 2.0f;

        DrawUtility drawUtility = new DrawUtility(1.5f);

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
            return (this.produced != 0 && this.rs.Count == 0);
        }

        public override void Draw(ILED3DCanvas canvas)
        {
            var isNeedUpdate = false;
            if(this.GetElapsedAt().Subtract(this.lastUpdateAt).Milliseconds > 100)
            {
                this.lastUpdateAt = this.GetElapsedAt();
                isNeedUpdate = true;
            }
            
            if( this.rs.Min() - 4 >= FIRST_R && this.produced < this.size)
            {
                this.rs.Add(FIRST_R);
                produced++;
            }

            var newRs = new List<float>();


            foreach(var r in this.rs)
            {
                if (r * 20 >= 0xff)
                {
                    continue;
                }
                else
                {
                    var dots = drawUtility.CircleCenterAt(x, y, r * 2, r * 2, new RGB(this.Color.R, this.Color.G, this.Color.B, (int)(this.Color.A - r * 20)));

                    foreach (var dot in dots)
                    {
                        canvas.SetLed(dot.X, dot.Y, 0, dot.RGB);
                    }

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
