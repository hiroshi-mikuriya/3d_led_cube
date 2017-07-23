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

        public static RGB fromColor(Color color)
        {
            return new RGB(color.R, color.G, color.B);
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

        public static RGB operator -(RGB lhs, int rhs)
        {
            return new RGB(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs);
        }

        public static RGB operator -(RGB lhs, double rhs)
        {
            return new RGB(Convert.ToInt32(lhs.r - rhs), 
                Convert.ToInt32(lhs.g - rhs),
                Convert.ToInt32(lhs.b - rhs));
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
            return Color.FromArgb(this.r, this.g, this.b);
        }
    }
}
