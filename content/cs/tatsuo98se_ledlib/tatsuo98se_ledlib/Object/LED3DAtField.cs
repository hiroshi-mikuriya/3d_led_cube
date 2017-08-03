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
        private double z;
        private double width;
        private List<float> rs = new List<float>();
        private TimeSpan lastUpdateAt;
        private int size;
        private bool longTimeNoSee = true;
        private Random rand = new Random();

        float FIRST_R = 2.0f;
        float DISTANCE = 3;

        float NEAR_BORDER = 0.35f;

        DrawUtility drawUtility = new DrawUtility(2f);
        /*
         *  z :  0(near) ～ 1(far)
         *  */
        public LED3DAtField(double x, double y, double z, int size, double width)
            :base(null)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.width = width;

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
        public void SetPos(double x, double y, double z, double width)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.width = width;

            var updateSpan = this.GetElapsedAt().Subtract(this.lastUpdateAt).TotalMilliseconds;
            if (updateSpan > 1000)
            {
                this.longTimeNoSee = true;
            }

            this.lastUpdateAt = this.GetElapsedAt();
        }

        [MethodImpl(MethodImplOptions.Synchronized)]
        public override void Draw(ILED3DCanvas canvas)
        {
            bool isNeedUpdate = false;
            var updateSpan = this.GetElapsedAt().Subtract(this.lastUpdateAt).TotalMilliseconds;
            if (updateSpan < 100 )
            {
                isNeedUpdate = true;
            }

            if (this.longTimeNoSee == true && this.z < NEAR_BORDER)
            {
                this.SetFlashEffect(canvas);
                this.longTimeNoSee = false;
            }

            if (isNeedUpdate)
            {
                if(this.z < NEAR_BORDER)
                {
                    this.NearEffect(canvas);
                }
                else
                {
                    this.FarEffect(canvas);
                }

            }
            else
            {
                foreach (var r in this.rs)
                {
                    var dcline = this.GetElapsedAt().Subtract(this.lastUpdateAt).TotalMilliseconds;
                    DrawOctagon(canvas, (float)this.x, (float)this.y, r + this.GetVibe(), new RGB(0xff, 0x65, 0x00, (int)(0xff - dcline * 0.8) ) );
                }
            }

        }

        private void FarEffect(ILED3DCanvas canvas)
        {

            RGB color = this.GetAtFieldColor(3);

            int count = 1;
            foreach (var r in this.rs)
            {
                DrawOctagon(canvas, (float)this.x, (float)this.y, r, new RGB(color.R, color.G, color.B, (int)(0xff - this.z * 300f - count * 20f)));
                count++;
            }
        }

        private void NearEffect(ILED3DCanvas canvas)
        {
            RGB color = this.GetAtFieldColor();
            if (color == null)
            {
                this.SetFlashEffect(canvas);
            }
            else
            {
                int count = 1;
                foreach (var r in this.rs)
                {
                    DrawOctagon(canvas, (float)this.x, (float)this.y, r + GetVibe(), new RGB(color.R, color.G, color.B, (int)(0xff - count * 20)));
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
            return GetAtFieldColor(colorType);
        }

        private RGB GetAtFieldColor(int type)
        {
            switch (type)
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
            var dx = (float)Math.Max(0, width * 0.5);

            pts.Add(new PointF(r + x + dx, d + y)); // 右上
            pts.Add(new PointF(r + x + dx, -d + y));// 右下

            pts.Add(new PointF(d + x + dx, -r + y)); // 下右
            pts.Add(new PointF(-d + x - dx, -r + y));// 下左
            pts.Add(new PointF(-r + x - dx, -d + y));// 左下
            pts.Add(new PointF(-r + x - dx, d + y));// 左上
            pts.Add(new PointF(-d + x - dx, r + y));// 上左
            pts.Add(new PointF(d + x + dx, r + y)); // 上右
            pts.Add(new PointF(r + x + dx, d + y)); // 1番目と同じ


            for (int i = 0; i < pts.Count - 1; i++)
            {
                var pt1 = pts[i];
                var pt2 = pts[i + 1];
                Dot[] dots = drawUtility.Line(pt1, pt2, color, 2f);
                foreach (var dot in dots)
                {
                    canvas.SetLed(dot.X, dot.Y, 0, dot.RGB);

                }

            }
        }
    }
}
