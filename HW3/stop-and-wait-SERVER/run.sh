#!/bin/bash

cd /home/wilson/Documents/courses/1111/data communication/Experiment-7/stop-and-wait-SERVER/
c++ ./server.cpp -o server

konsole --noclose -e ./server
