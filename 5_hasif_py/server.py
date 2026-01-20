import socket
import mysql.connector
import time
import threading


PORT = 8085

def auto_update():
    while True:
        try:
            db = mysql.connector.connect(
                host="my_db",
                user="root",
                password="pass123",
                database="project_db"
            )
            cursor = db.cursor()
            
            cursor.execute("UPDATE points_table SET points = points + 1, datetime_stamp = NOW() WHERE user = 'hasif'")
            db.commit()
            cursor.close()
            db.close()
        except Exception as e:
            print(f"DB Update Error: {e}")
        time.sleep(30)

def start_server():
    
    threading.Thread(target=auto_update, daemon=True).start()

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', PORT))
    server.listen(5)
    print(f"Server Hasif (Python) sedia di port {PORT}...")

    while True:
        client_sock, addr = server.accept()
        try:
            db = mysql.connector.connect(
                host="my_db",
                user="root",
                password="pass123",
                database="project_db"
            )
            cursor = db.cursor()
            
            cursor.execute("SELECT points, datetime_stamp FROM points_table WHERE user = 'hasif'")
            row = cursor.fetchone()
            
            if row:
                
                response = f"User: hasif | Points: {row[0]} | Last Update: {row[1]}"
            else:
                response = "User hasif tak jumpa!"
            
            client_sock.send(response.encode())
            cursor.close()
            db.close()
        except Exception as e:
            print(f"Error: {e}")
        finally:
            client_sock.close()

if __name__ == "__main__":
    start_server()