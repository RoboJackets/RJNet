import socket
import signal


def signal_handler(signal, frame):
    print('You pressed Ctrl+C!')
    done = True
    s.close()
    sys.exit(0)

s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = 12345                # Reserve a port for your service.
s.bind((host, port))        # Bind to the port

s.listen(5)                 # Now wait for client connection.
c, addr = s.accept()     # Establish connection with client.
print('Got connection from', addr)
while True:
	receivedData = c.recv(1024).decode()
	print(receivedData)
	c.send(bytes('Thank you for connecting','utf-8'))    