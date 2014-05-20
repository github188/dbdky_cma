import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_address = ('localhost', 20000)
message = 'This is the message'

try:
	print >> sys.stderr, 'sending "%s"' % message
	sent = sock.sendto(message, server_address)

finally:
	print >> sys.stderr, 'closing socket'
	sock.close()