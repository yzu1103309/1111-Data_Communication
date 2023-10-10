#!/bin/bash

cd /home/wilson/Documents/courses/1111/data communication/Experiment-7/stop-and-wait-CLIENT/
c++ ./client.cpp -o client

konsole --noclose -e ./client
