#!coding: utf-8

import sys
import os
import Queue
import threading
import time
from mosquitto import Mosquitto
from xml.dom.minidom import parse, parseString, getDOMImplementation
from client import Client


RED = '\033[31m'
GREEN = '\033[32m'
MAGENTA = '\033[35m'
DEFAULT = '\033[37m'
SUBS = "Gossip"
BLOCK = list()


def main():
    name = "Fast Client"
    HOST = "localhost"
    PORT = 4711

    client_A = Client(HOST, PORT, name + " A")
    client_B = Client(HOST, PORT, name + " B")

    client_A.publish("Gossip", client_A.pack("A"),1)
    client_B.publish("Gossip", client_B.pack("B"),1)
    client_A.publish("Gossip", client_A.pack("C"),1)
    client_B.publish("Gossip", client_B.pack("D"),1)
    client_A.publish("Gossip", client_A.pack("E"),1)
    client_B.publish("Gossip", client_B.pack("F"),1)
    client_A.publish("Gossip", client_A.pack("G"),1)
    client_B.publish("Gossip", client_B.pack("I"),1)
    client_A.publish("Gossip", client_B.pack("H"),1)
    client_B.publish("Gossip", client_A.pack("J"),1)
    client_A.publish("Gossip", client_B.pack("K"),1)

if __name__ == "__main__":
    main()
