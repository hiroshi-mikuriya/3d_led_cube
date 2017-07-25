using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample
{
    public static class Extensions
    {
        public static Rectangle ToRectangle(this RectangleF rect)
        {
            return new Rectangle(Convert.ToInt32(rect.X),
                Convert.ToInt32(rect.Y),
                Convert.ToInt32(rect.Width),
                Convert.ToInt32(rect.Height));
        }
    }
}
