using System.Runtime.InteropServices;

namespace LEDLIB
{
    class LED
    {
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetUrl(string url);
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetLed(int x, int y, int z, int color);
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Clear();
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Show();
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Wait(int millisecond);
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ShowMotioningText1(string text);
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetChar(int x, int y, int z, char c, int color);
        [DllImport("ledLib64.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ShowFirework(int x, int y, int z);

        public static int HEIGHT = 32;
        public static int WIDTH = 16;
        public static int DEPTH = 8;

        public enum Direction
        {
            Front,
            Side,
            Horizontal,
        }
    }
}
