#!/bin/bash

gcc tcp_client_ed.c -o tcpClient
gcc udp_client_ed.c -o udpClient
gcc server.c -o server
./server 127.0.0.1 2001 2002
