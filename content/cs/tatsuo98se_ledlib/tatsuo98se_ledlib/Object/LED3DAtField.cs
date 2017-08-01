using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using static LEDLIB.DrawUtility;

namespace LEDLIB
{
    public class LED3DAtField : LED3DObject
    {
        private double x;
        private double y;
        private List<float> rs = new List<float>();
        private TimeSpan lastUpdateAt;
        private int size;
        private int callCount = 0;
        private Random rand = new Random();

        float FIRST_R = 2.0f;
        float DISTANCE = 3;

        public LED3DAtField(double x, double y, int size)
            :base(null)
        {
            this.x = x;
            this.y = y;

            float r = FIRST_R;
            for(int i=0; i<size; i++)
            {
                this.rs.Add(r);
                r = r + DISTANCE + (1 + i/10f);
            }

            this.lastUpdateAt = this.GetElapsedAt();
            this.size = size;
        }

        public override bool IsExpired()
        {
            return this.GetElapsedAt().Subtract(this.lastUpdateAt).TotalMilliseconds > 2000;
            
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public void SetPos(double x, double y)
        {
            this.x = x;
            this.y = y;
            this.lastUpdateAt = this.GetElapsedAt();
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public override void Draw(ILED3DCanvas canvas)
        {
            bool isNeedUpdate = false;
            if(this.GetElapsedAt().Subtract(this.lastUpdateAt).TotalMilliseconds < 100)
            {
                isNeedUpdate = true;
            }

            callCount++;

            if(callCount == 1)
            {
                this.SetFlashEffect(canvas);
            }

            float vibe = this.GetVibe();
            RGB color = this.GetAtFieldColor();

            if (isNeedUpdate)
            {
                if (color == null)
                {
                    this.SetFlashEffect(canvas);
                }
                else
                {
                    foreach (var r in this.rs)
                    {
                        DrawOctagon(canvas, (float)this.x, (float)this.y, r + vibe, color);
                    }
                }
            }
            else
            {
                foreach (var r in this.rs)
                {
                    var dcline = this.GetElapsedAt().Subtract(this.lastUpdateAt).TotalMilliseconds;
                    DrawOctagon(canvas, (float)this.x, (float)this.y, r + vibe, new RGB(0xff, 0x65, 0x00) - dcline * 0.8);
                }
            }
        }

        private int GetVibe()
        {
            return rand.Next(5) == 5 ? 1 : 0;
        }

        private RGB GetAtFieldColor()
        {
            int colorType = this.rand.Next(0, 4);
            switch (colorType)
            {
                case 0:
                    return new RGB(0xff, 0xff, 0xff);
                case 1:
                    return new RGB(0xff, 0xc0, 0x00);
                case 2:
                case 3:
                    return new RGB(0xff, 0x65, 0x00);
                default:
                    return null; // flush
            }

        }

        private void SetFlashEffect(ILED3DCanvas canvas)
        {

            for (int x = 0; x < LED.WIDTH; x++)
            {
                for (int y = 0; y < LED.HEIGHT; y++)
                {
                    canvas.SetLed(x, y, 0, new RGB(0xff, 0xff, 0xff));
                }
            }
            return;
        }

        private void DrawOctagon(ILED3DCanvas canvas, float x, float y, float r, RGB color)
        {
            float d = r * (float)Math.Tan(Math.PI / 8f);

            var pts = new List<PointF>();

            pts.Add(new PointF(r + x, d + y));
            pts.Add(new PointF(r + x, -d + y));

            pts.Add(new PointF(d + x, -r + y));
            pts.Add(new PointF(-d + x, -r + y));
            pts.Add(new PointF(-r + x, -d + y));
            pts.Add(new PointF(-r + x, d + y));
            pts.Add(new PointF(-d + x, r + y));
            pts.Add(new PointF(d + x, r + y));
            pts.Add(new PointF(r + x, d + y)); // 1番目と同じ
            

            for (int i = 0; i < pts.Count - 1; i++)
            {
                var pt1 = pts[i];
                var pt2 = pts[i + 1];
                Dot[] dots = DrawUtility.Line(pt1, pt2, color, 2f);
                foreach (var dot in dots)
                {
                    canvas.SetLed(dot.X, dot.Y, 0, dot.RGB);

                }

            }
        }
    }
}
