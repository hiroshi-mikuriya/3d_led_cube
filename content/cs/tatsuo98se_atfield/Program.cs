/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2013 Intel Corporation. All Rights Reserved.

*******************************************************************************/
using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Intel.RealSense;
using System.IO;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace hands_viewer.cs
{
    static class Program
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool SetDllDirectory(string lpPathName);

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            var test = AppDomain.CurrentDomain.BaseDirectory;
            var libDir = Directory.GetParent(test).Parent.Parent.Parent.Parent;
            SetDllDirectory(Path.Combine(libDir.FullName, "00_lib"));

            var cmds = Environment.GetCommandLineArgs();
            if(cmds.Length > 1)
            {
                Debug.WriteLine("Target IP: " + cmds[1]);
                LEDLIB.LED.SetUrl(cmds[1]);
            }

            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                Session session = null;
                session = Session.CreateInstance();
                if (session != null)
                {
                    Application.Run(new MainForm(session));
                    session.Dispose();
                }
            } catch (Exception e)
            {
                MessageBox.Show(null, e.ToString(), "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
