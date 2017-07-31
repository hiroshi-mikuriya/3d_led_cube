using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class RGB
    {
        private int r;
        private int g;
        private int b;
        private int a = 0xff;

        private static Random rand = null;

        private static Random getRandom()
        {
            if(rand == null)
            {
                rand = new Random(); 
            }
            return rand;
        }

        public static RGB randamBG()
        {
            int r = RGB.getRandom().Next(0x22, 0x77);
            int g = RGB.getRandom().Next(0x88, 0xff);
            int b = RGB.getRandom().Next(0x88, 0xff);
            return new RGB(r, g, b);
        }

        public static RGB fromColor(Color color)
        {
            return new RGB(
                color.R,
                color.G,
                color.B,
                color.A);
        }

        public RGB(int r, int g, int b, int a)
            :this(r, g, b)
        {
            this.a = a;
        }

        public RGB(float r, float g, float b)
            :this(Util.Round(0xff*r), Util.Round(0xff*g), Util.Round(0xff*b))
        {

        }

        public RGB(int r, int g, int b)
        {
            this.r = r;
            this.g = g;
            this.b = b;

            if (this.r < 0)
            {
                this.r = 0;
            }
            if (this.g < 0)
            {
                this.g = 0;
            }
            if (this.b < 0)
            {
                this.b = 0;
            }
            if (this.r > 0xff)
            {
                this.r = 0xff;
            }
            if (this.g > 0xff)
            {
                this.g = 0xff;
            }
            if (this.b > 0xff)
            {
                this.b = 0xff;
            }
        }

        public float Rf
        {
            get
            {
                return this.r / 255f;
            }
        }
        public float Gf
        {
            get
            {
                return this.g / 255f;
            }
        }
        public float Bf
        {
            get
            {
                return this.b / 255f;
            }
        }
        public float Af
        {
            get
            {
                return this.a / 255f;
            }
        }
        public int R
        {
            get
            {
                return this.r;
            }
        }
        public int G
        {
            get
            {
                return this.g;
            }
        }
        public int B
        {
            get
            {
                return this.b;
            }
        }
        public int A
        {
            get
            {
                return this.a;
            }
        }

        public static RGB operator -(RGB lhs, int rhs)
        {
            return new RGB(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs, lhs.a);
        }

        public static RGB operator -(RGB lhs, double rhs)
        {
            return new RGB(Util.Round(lhs.r - rhs),
                Util.Round(lhs.g - rhs),
                Util.Round(lhs.b - rhs),
                lhs.a);
        }

        public static RGB operator -(RGB lhs, RGB rhs)
        {
            return new RGB(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);
        }

        public static RGB operator *(RGB lhs, int rhs)
        {
            return new RGB(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
        }

        public static RGB operator *(RGB lhs, double rhs)
        {
            return new RGB(Convert.ToInt32(lhs.r * rhs),
                Convert.ToInt32(lhs.g * rhs),
                Convert.ToInt32(lhs.b * rhs));
        }

        public int ToInt()
        {
            return (this.r << 16) + (this.g << 8) + this.b;
        }

        public Color ToColor()
        {
            return Color.FromArgb(this.a, this.r, this.g, this.b);
        }

        public bool isBlack()
        {
            return ToInt() == 0;
        }
    }
}
