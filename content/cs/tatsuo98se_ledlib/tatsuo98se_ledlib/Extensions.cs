using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LEDLIB
{
    public static class Extensions
    {
        public static Rectangle ToRectangle(this RectangleF rect)
        {
            return new Rectangle(Util.Round(rect.X),
                Util.Round(rect.Y),
                Util.Round(rect.Width),
                Util.Round(rect.Height));
        }
    }
}
