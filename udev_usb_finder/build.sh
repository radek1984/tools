#!/usr/bin/bash
g++ -std=c++11 -o udev_usb_finder main.cpp $(pkg-config --cflags --libs libudev)
