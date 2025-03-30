import pynng
import threading


def handle_request(sock):
    while True:
        try:
            # 接收请求
            msg = sock.recv()
            request = msg.decode('utf-8')
            
            # 检查请求内容
            if request == "test":
                # 发送响应
                response = "Here is the test server"
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
    # 定义服务器地址和端口
    server_address = 'tcp://127.0.0.1:3001'
    
    # 启动服务器
    server_thread = threading.Thread(target=start_server, args=(server_address,))
    server_thread.start()
    
    # 等待服务器线程结束
    server_thread.join()