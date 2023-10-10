#!/bin/bash

cd /home/wilson/Documents/courses/1111/data\ communication/Experiment-6/tcp_server/
c++ ./server.cpp -o server

konsole --noclose -e ./server
