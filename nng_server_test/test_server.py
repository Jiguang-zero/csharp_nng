import pynng
import threading

# Server function
def nng_server():
    with pynng.Rep0(listen='tcp://127.0.0.1:3001') as server:
        print("Server is listening on tcp://127.0.0.1:3001")
        while True:
            message = server.recv().decode()
            print(f"Server received: {message}")
            response = f"Echo: {message}"
            server.send(response.encode())

# Client function
def nng_client():
    with pynng.Req0(dial='tcp://127.0.0.1:3001') as client:
        test_message = "test"
        print(f"Client sending: {test_message}")
        client.send(test_message.encode())
        response = client.recv().decode()
        print(f"Client received: {response}")

# Run server in a separate thread
server_thread = threading.Thread(target=nng_server, daemon=True)
server_thread.start()

# Run client
nng_client()