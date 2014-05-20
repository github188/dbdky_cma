#!/usr/bin/python

from socket import socket, AF_INET, SOCK_DGRAM
s = socket(AF_INET, SOCK_DGRAM)
s.sendto(b'A5 5A 0E 14 01 01 1E 00 05 00 72 4F 4C 52 00 29 00 EA 00 42 00 2B 03 98 00 00 01 B5 00 00 78 00 46 00 00 00 00 00 58 F7', ('localhost', 20000))
print s.recvfrom(8192)
