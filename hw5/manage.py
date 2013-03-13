'''
file created 3/12/13
authored by Keaton Wallace
Any code not written by me is creditted to the creator and is used under fair use laws for educational purpose.
'''

import socket
import os
import time
import select
import sys

HOST = "localhost"
PORT = 6548
BUFFSIZE = 4096

srvsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
srvsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
srvsock.bind((HOST, PORT))
srvsock.listen(5) #5 is the maximum number of queued connections
descriptors = [srvsock] #array of sockets

