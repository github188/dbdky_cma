import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_address = ('localhost', 20000)
message = 'This is the message'

try:
	print >> sys.stderr, 'sending "%s"' % message
	sent = sock.sendto(message, server_address)
        
        # Wait for the response
        print >> sys.stderr, 'Waiting for response'
        data, server = sock.recvfrom(4096)
        print >> sys.stderr, 'received "%s"' % data

finally:
	print >> sys.stderr, 'closing socket'
	sock.close()
