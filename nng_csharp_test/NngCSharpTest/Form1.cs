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
            IntPtr api = NngServer.ConnectRequestSocket(url);

            if (api == IntPtr.Zero)
            {
                MessageBox.Show("Connect Failed");
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
                return;
            }

            MessageBox.Show(NngServer.GetMessage(result));
        }
    }
}
