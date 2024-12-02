#Python Chat app

import rsa  
import socket
import threading


choice = input("Do you want to host(1) or connect(2):")

if choice == "1":
    # Create a server socket
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    server.bind(("192.168.1.33", 9999))
    server.listen()

    print("Waiting for connection...")
    client, _ = server.accept()
    print("Connection established!")
elif choice == "2":
    # Create a client socket
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(("192.168.1.33", 9999))  
    print("Connected to the server!")
else:
    exit()


def send_mess(c):
    while True:
        message = input("")
        c.send(message.encode())
        print("You: " + message)


def receive_mess(c):
    while True:
        print("Partner: " + c.recv(1024).decode())  


# Start send and receive threads
threading.Thread(target=send_mess, args=(client,)).start()
threading.Thread(target=receive_mess, args=(client,)).start()