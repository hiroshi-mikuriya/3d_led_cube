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

        private static int GetNormalized(int value)
        {
            var normalized = value;
            if (value > 0xff)
            {
                normalized = 0xff;
            }
            if (value < 0)
            {
                normalized = 0;
            }
            return normalized;

        }

        public RGB(int r, int g, int b, int a)
        {
            this.r = GetNormalized(r);
            this.g = GetNormalized(g);
            this.b = GetNormalized(b);
            this.a = GetNormalized(a);

        }

        public RGB(float r, float g, float b)
            :this(Util.Round(0xff*r), Util.Round(0xff*g), Util.Round(0xff*b))
        {

        }

        public RGB(int r, int g, int b)
            :this(r,g,b,0xff)
        {
 
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
            set
            {
                this.r = GetNormalized(value);
            }
        }
        public int G
        {
            get
            {
                return this.g;
            }
            set
            {
                this.g = GetNormalized(value);
            }
        }
        public int B
        {
            get
            {
                return this.b;
            }
            set
            {
                this.b = GetNormalized(value);
            }
        }
        public int A
        {
            get
            {
                return this.a;
            }
            set
            {
                this.a = GetNormalized(value);
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

        public static RGB operator +(RGB lhs, int rhs)
        {
            return new RGB(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a);
        }

        public static RGB operator +(RGB lhs, double rhs)
        {
            return new RGB(Util.Round(lhs.r + rhs),
                Util.Round(lhs.g + rhs),
                Util.Round(lhs.b + rhs),
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
