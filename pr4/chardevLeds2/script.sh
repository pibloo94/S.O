#!/bin/bash
sudo su
make clean
make
sudo rmmod chardevLeds.
sudo insmod chardevLeds.ko
sudo cat /dev/chardevLeds
sudo mknod /dev/chardevLeds -m 666 c 250 0
sudo echo 1 > /dev/chardevLeds

