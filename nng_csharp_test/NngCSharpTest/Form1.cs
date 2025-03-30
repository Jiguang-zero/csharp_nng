using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NngCSharpTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            AddCustomEvents();
        }

        private void AddCustomEvents()
        {
            this.button1.Click += Button1_Click;
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            string url = "tcp://127.0.0.1:3001";
            byte[] urlBytes = Encoding.UTF8.GetBytes(url);

            // 分配非托管内存以null结尾的字符串
            IntPtr urlPtr = Marshal.AllocHGlobal(urlBytes.Length + 1);
            Marshal.Copy(urlBytes, 0, urlPtr, urlBytes.Length);

            // 设置null终止符
            Marshal.WriteByte(urlPtr, urlBytes.Length, 0);

            IntPtr api = NngServer.ConnectUrl(urlPtr);
            if (api == IntPtr.Zero)
            {
                MessageBox.Show("Connect Failed");
                NngServer.Release(api);
                return;
            }

            if (NngServer.SetSocketSendTimeOut(api, 1000) != 0)
            {
                MessageBox.Show("Set SendTimeOut Failed");
                NngServer.Release(api);
                return;
            }

            if (NngServer.SetSocketReceiveTimeOut(api, 1000) != 0)
            {
                MessageBox.Show("Set ReceiveTimeOut Failed");
                NngServer.Release(api);
                return;
            }

            string message = "test";

            // 将字符串转换为字节数组
            byte[] msgBytes = Encoding.UTF8.GetBytes(message);

            // 调用Send函数发送消息
            IntPtr result = NngServer.Send(api, msgBytes, msgBytes.Length);
            if (result != IntPtr.Zero)
            {
                MessageBox.Show("收不到消息");
                NngServer.Release(api);
                return;
            }

            NngServer.Release(api);
        }
    }
}
