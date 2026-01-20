import socket
import time

def start_client():
    
    host = 'server_azim'
    port = 8081

    print("Client Azim bermula...")
    while True:
        try:
            client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client.connect((host, port))
            response = client.recv(1024).decode()
            print(f"Respon Server -> {response}", flush=True)
            client.close()
        except:
            print("Mencari server_azim...")
        
        time.sleep(5)

if __name__ == "__main__":
    start_client()