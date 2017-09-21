using System;
using System.IO;
using System.Runtime.InteropServices;

namespace sample
{
    class Program
    {
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetUrl(string url);
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetLed(int x, int y, int z, int color);
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Clear();
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Show();
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Wait(int millisecond);
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ShowMotioningText1(string text);
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetChar(int x, int y, int z, char c, int color);
        [DllImport("ledLib32.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ShowFirework(int x, int y, int z);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);

        static void Main(string[] args)
        {
            String test = AppDomain.CurrentDomain.BaseDirectory;
            DirectoryInfo libDir = Directory.GetParent(test).Parent.Parent.Parent.Parent;
            SetDllDirectory(Path.Combine(libDir.FullName,"00_lib"));
            while (true)
            {
                ShowFirework(5, 5, 5);
                Wait(20);
            }
        }
    }
}
