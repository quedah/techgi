#!/bin/bash

for (( i = 0; i < 1000; i++ )); do
  ./tcpClient 127.0.0.1 2001 $i 349
done
