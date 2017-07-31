using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LEDLIB
{
    public class Util
    {
        public static int Round(double val)
        {
            return (int)Math.Round(val, MidpointRounding.AwayFromZero);
        }
    }
}
