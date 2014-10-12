#!/usr/bin/env python
import socket
import sys

import numpy as np

HOST, PORT = "localhost", 9999
#data = " ".join(sys.argv[1:])
x = np.arange(0,2*np.pi,0.01)
data = np.sin(x)

# SOCK_DGRAM is the socket type to use for UDP sockets
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# As you can see, there is no connect() call; UDP has no connections.
# Instead, data is directly sent to the recipient via sendto().
sock.sendto(data, (HOST, PORT))
#received = sock.recv(1024)

print "Sent:     %s" % data
#print "Received: %s" % received
