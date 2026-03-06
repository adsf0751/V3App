import socket
import csv
from datetime import datetime
HOST = "10.105.108.35"
PORT = 5000

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)
server_socket.settimeout(1)  # 每秒檢查一次 Ctrl+C

print(f"Server started on port {PORT}...")

try:
    while True:
        try:
            client_socket, client_addr = server_socket.accept()
        except socket.timeout:
            continue  # 沒連線就重新檢查 Ctrl+C
        print(f"Client connected from {client_addr}")
        filename = f"output_{datetime.now().strftime('%Y%m%d_%H%M%S_%f')}.csv"
        client_socket.settimeout(1)  # recv 阻塞時間 1 秒
        while True:
            try:
                data = client_socket.recv(4096)
            except socket.timeout:
                continue
            if not data:
                print("Client disconnected")
                break
            message = data.decode("utf-8")
            print(f"Received: {message}")
            # 轉成 CSV
            row = message.split(',')
            with open(filename, "a", newline='') as f:
                writer = csv.writer(f)
                writer.writerow(row)
            #client_socket.send(f"Server received: {message}".encode("utf-8"))
        client_socket.close()
except KeyboardInterrupt:
    print("\nServer stopped by user")
finally:
    server_socket.close()
