
import socket
import random
import time

SERVER_IP = '192.168.1.72'
SERVER_PORT = 9080

emociones = ['feliz', 'triste', 'enojado', 'curioso', 'confundido', 'dormido', 'parpadeo']

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((SERVER_IP, SERVER_PORT))
server_socket.listen(1)

print(f"Servidor escuchando en {SERVER_IP}:{SERVER_PORT}")

conn, addr = server_socket.accept()
print(f"Conexión establecida con {addr}")

try:
    while True:
        # Recibir datos del ESP32 (opcional si estás enviando sensores)
        conn.settimeout(0.1)  # Pequeño timeout para no bloquear el envío
        try:
            data = conn.recv(1024)
            if data:
                sensor_value = data.decode('utf-8').strip()
                print(f"Valor recibido del ESP32: {sensor_value}")
        except socket.timeout:
            pass

        # Elegir emoción aleatoria y enviarla
        emocion = random.choice(emociones)
        print(f"Enviando emoción: {emocion}")
        conn.sendall((emocion + '\n').encode('utf-8'))

        time.sleep(5)

except ConnectionResetError:
    print("El ESP32 cerró la conexión.")
finally:
    conn.close()
    server_socket.close()
    print("Servidor cerrado.")
