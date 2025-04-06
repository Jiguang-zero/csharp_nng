import pynng
import threading
import sys


i = 0

def handle_request(sock):
    global i
    while True:
        try:
            # 接收请求
            msg = sock.recv()
            request = msg.decode('utf-8')
            
            # 检查请求内容
            if request == "test":
                # 发送响应
                response = "test: " + str(i)
                i += 1
                sock.send(response.encode('utf-8'))
        except pynng.Timeout:
            continue
        except KeyboardInterrupt:
            break

def start_server(address):
    # 创建一个REP套接字
    with pynng.Rep0(listen=address) as sock:
        # 设置超时时间，避免recv无限阻塞
        sock.recv_timeout = 1000
        print(f"Server listening on {address}")
        
        # 处理请求
        handle_request(sock)


if __name__ == "__main__":
    len_argv = len(sys.argv)
    if len_argv != 2:
        print("The length of argv is not 2 but " + str(len_argv))
        sys.exit(-1)

    # 定义服务器地址和端口
    server_address = 'tcp://127.0.0.1:' + sys.argv[1]
    
    # 启动服务器
    server_thread = threading.Thread(target=start_server, args=(server_address,))
    server_thread.start()
    
    # 等待服务器线程结束
    server_thread.join()