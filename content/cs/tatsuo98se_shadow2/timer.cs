using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;

namespace hands_viewer.cs
{
    class FPSTimer
    {
        [DllImport("Kernel32.dll")]
        private static extern bool QueryPerformanceCounter(out long data);
        [DllImport("Kernel32.dll")]
        private static extern bool QueryPerformanceFrequency(out long data);

        private MainForm form;
        private long freq, last, cfreq, clast;
        private int fps;

        public FPSTimer(MainForm mf)
        {
            form = mf;
            QueryPerformanceFrequency(out freq);
            cfreq = freq / 50;
            fps = 0;
            QueryPerformanceCounter(out last);
            QueryPerformanceCounter(out clast);
        }

        public void Tick()
        {
            long now;
            QueryPerformanceCounter(out now);
            fps++;
            if (now - last > freq) // update every second
            {
                last = now;
                form.UpdateFPSStatus("FPS=" + fps);
                fps = 0;
            }
            if (now - clast > cfreq)
            {
                clast = now;
                HandsRecognition.Show();
                fps = 0;
            }
        }
    }
}
