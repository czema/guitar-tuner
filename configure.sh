#!/bin/bash
if [[ "$EUID" -ne 0 ]]; then
	echo -e "\033[1;31mRoot access required.\033[0m" 2>&1
	exit 1
fi
[ -d "bin" ] && rm -rf bin
mkdir bin
echo -e "\033[1;33mPreparing LibFT4222.\033[0m"
cd deps/libft4222/
./install4222.sh
cd examples
cc get-version.c -lft4222 -Wl,-rpath,/usr/local/lib -o get-version
./get-version
echo -e "\033[1;36mShould have output something like \"Device 0: 'FT4222 A'\"\033[0m"
cd ../..
echo -e "\033[1;33mPreparing rpi_ws281x.\033[0m"
[ -d "rpi_ws281x" ] || git clone https://github.com/jgarff/rpi_ws281x.git
cd rpi_ws281x
git pull
[ -d "build" ] && rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
sudo make install
cd ../..
echo -e "\033[1;32mConfiguration complete.\033[0m"