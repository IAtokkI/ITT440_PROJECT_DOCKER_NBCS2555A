import socket
import time

def start_client():
    
    host = 'server_hasif'
    port = 8085

    print("Client Hasif bermula...")
    while True:
        try:
            client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client.connect((host, port))
            response = client.recv(1024).decode()
            print(f"Respon Server -> {response}", flush=True)
            client.close()
        except:
            print("Mencari server_hasif...")
        
        time.sleep(5)

if __name__ == "__main__":
    start_client()