#!/bin/bash

# should edit /etc/ldconfig.
sudo /sbin/ldconfig
#sudo nice -n -18 ionice -c 1 -n 0 ./main
./main