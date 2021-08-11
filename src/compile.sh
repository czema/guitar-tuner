#!/bin/bash
if [[ "$EUID" -ne 0 ]]; then
	echo -e "\033[1;31mRoot access required.\033[0m" 2>&1
	exit 1
fi
echo -e "\033[1;33mCompiling...\033[0m"
cc leds.c -c -o leds.o
cc display.c -c -o display.o
cc spi.c -c -o spi.o
cc main.c -c -o main.o
cc *.o -lft4222 -lws2811 -lm -Wl,-rpath,/usr/local/lib -o ../bin/guitar-tuner
rm *.o
echo -e "\033[0;32mDone.\033[0m"
chown 0:0 ../bin/guitar-tuner
chmod u+s ../bin/guitar-tuner