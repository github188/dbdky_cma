#!/usr/bin/python

from socket import socket, AF_INET, SOCK_DGRAM
s = socket(AF_INET, SOCK_DGRAM)
s.sendto(b'A5 5A 0E 14 01 21 12 00 01 00 72 4F 4C 52 41 00 3A 00 78 00 46 00 23 00 00 00 A9 DD ', ('localhost', 20000))
print s.recvfrom(8192)
