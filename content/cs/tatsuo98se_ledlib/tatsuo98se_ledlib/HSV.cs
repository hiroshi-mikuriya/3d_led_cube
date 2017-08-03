using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class HSV
    {
        private int h;
        private int s;
        private int v;

        private static int GetNormalized(int value)
        {
            var normalized = value;
            if (value > 360)
            {
                normalized = 360;
            }
            if (value < 0)
            {
                normalized = 0;
            }
            return normalized;

        }

        public HSV()
            :this(0,0,0)
        {

        }

        public HSV(int h, int s, int v)
        {
            this.h = h;
            this.s = s;
            this.v = v;
        }

        public int H
        {

            get
            {
                return this.h;
            }
            set
            {
                this.h = GetNormalized(value);
            }

        }
        public int S
        {
            get
            {
                return this.s;
            }
            set
            {
                this.s = GetNormalized(value);
            }

        }
        public int V
        {
            get
            {
                return this.v;
            }
            set
            {
                this.v = GetNormalized(value);
            }

        }
    }
}
