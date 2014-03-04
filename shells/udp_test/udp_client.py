#!/usr/bin/python

from socket import socket, AF_INET, SOCK_DGRAM
s = socket(AF_INET, SOCK_DGRAM)
s.sendto(b'Hello', ('localhost', 20000))
print s.recvfrom(8192)
