import sys
from socket import *

ECHO_PORT = 55555
BUFSIZE = 1024

s = socket(AF_INET, SOCK_DGRAM)
s.bind(('', ECHO_PORT))

print('UDP Echo Server ready')

while True:
    data, addr = s.recvfrom(BUFSIZE)
    print('Server received %r from %r' % (data, addr))
    s.sendto(data, addr)
