#!/bin/bash
fbi -T 2 -d /dev/fb0 -noverbose -a $1
fbi -T 2 -d /dev/fb1 -noverbose -a $1
