using LEDLIB;
using System;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;

namespace sample
{
    class Program
    {

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);

        static void Main(string[] args)
        {
            var rand = new Random();
            var test = AppDomain.CurrentDomain.BaseDirectory;
            var libDir = Directory.GetParent(test).Parent.Parent.Parent.Parent;
            SetDllDirectory(Path.Combine(libDir.FullName,"00_lib"));

            var canvas = new LED3DCanvas();

            while (true)
            {
                if(canvas.GetObjectCount() < 2)
                {
                    var x = rand.Next() % LED.WIDTH;
                    var y = rand.Next() % LED.HEIGHT;
                    canvas.AddObject(new LED3DRipple(x, y, new RGB(0xff, 0x00, 0xff), LED.Direction.Front, 6));
                }
                canvas.Show();
                LED.Wait(20);
            }
        }
    }
}
