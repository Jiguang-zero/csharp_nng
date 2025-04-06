﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace NngCSharpTest
{
    public class NngServer
    {
        [DllImport("packaged_nng.dll", EntryPoint = "ConnectRequestSocket",
            CallingConvention = CallingConvention.StdCall)]
        public extern static IntPtr ConnectRequestSocket(string url);

        [DllImport("packaged_nng.dll", EntryPoint = "SetSocketSendTimeOut",
            CallingConvention = CallingConvention.StdCall)]
        public extern static int SetSocketSendTimeOut(IntPtr socket, int timeout);

        [DllImport("packaged_nng.dll", EntryPoint = "SetSocketReceiveTimeOut",
            CallingConvention = CallingConvention.StdCall)]
        public extern static int SetSocketReceiveTimeOut(IntPtr socket, int timeout);

        [DllImport("packaged_nng.dll", EntryPoint = "Release",
            CallingConvention = CallingConvention.StdCall)]
        public extern static void Release(IntPtr socket);

        [DllImport("packaged_nng.dll", EntryPoint = "GetResponseWithRequest",
            CallingConvention = CallingConvention.StdCall)]
        public extern static IntPtr Send(IntPtr socket, byte[] msg, int msg_size);
        
        [DllImport("packaged_nng.dll", EntryPoint = "FreeMessage",
            CallingConvention = CallingConvention.StdCall)]
        public extern static void FreeMessage(IntPtr str);

        public static string GetMessage(IntPtr response)
        {
            if (response == IntPtr.Zero)
            {
                return String.Empty;
            }

            string str = Marshal.PtrToStringAnsi(response);
            FreeMessage(response);
            //Marshal.FreeHGlobal(response);
            return str;
        }
    }
}
