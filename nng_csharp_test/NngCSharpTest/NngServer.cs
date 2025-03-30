using System;
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
        public extern static IntPtr ConnectUrl(IntPtr url);

        [DllImport("packaged_nng.dll", EntryPoint = "SetSocketSendTimeOut",
            CallingConvention = CallingConvention.StdCall)]
        public extern static int SetSocketSendTimeOut(IntPtr socket, int timeout);

        [DllImport("packaged_nng.dll", EntryPoint = "SetSocketReceiveTimeOut",
            CallingConvention = CallingConvention.StdCall)]
        public extern static int SetSocketReceiveTimeOut(IntPtr socket, int timeout);

        [DllImport("packaged_nng.dll", EntryPoint = "Release",
            CallingConvention = CallingConvention.StdCall)]
        public extern static void Release(IntPtr socket);

        [DllImport("packaged_nng.dll", EntryPoint = "GetReceiveMessage",
            CallingConvention = CallingConvention.StdCall)]
        public extern static IntPtr Send(IntPtr socket, byte[] msg, int msg_size);
    }
}
