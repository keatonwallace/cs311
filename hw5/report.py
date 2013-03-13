'''
file created 3/12/13
authored by Keaton Wallace
Any code not wirtten by me is created to the creator and is used under fair use laws for educational purpose.
'''
#!/usr/bin/env python 
import socket 
from xml.dom import minidom
import os
import sys
import getopt

HOST = 'localhost'
PORT = 6550
BUFFSIZE = 4096
sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockfd.connect((HOST,PORT))

