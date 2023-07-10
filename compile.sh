#!/bin/bash
if [[ "$EUID" -ne 0 ]]; then
	echo -e "\033[1;31mRoot access required.\033[0m" 2>&1
	exit 1
fi
pushd src/
echo -e "\033[1;33mCompiling...\033[0m"
cc leds.c -c -Ofast -o ../obj/leds.o
cc display.c -c -Ofast -Wa,-ahl=../obj/display.s -o ../obj/display.o
cc spi.c -c -Ofast -o ../obj/spi.o
cc main.c -c -Ofast -o ../obj/main.o
cd ../obj
cc *.o -lft4222 -lws2811 -lm -Wl,-rpath,/usr/local/lib -Ofast -o ../bin/guitar-tuner
cd ../bin
chown 0:0 guitar-tuner
chmod u+s guitar-tuner
popd
echo -e "\033[0;32mDone.\033[0m"
 