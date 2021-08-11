cd deps/libft4222
./install.sh
cd examples
cc get-version.c -lft4222 -Wl,-rpath,/usr/local/lib -o get-version
./get-version
echo -e "\033[1;31mShould have output something like \"Device 0: 'FT4222 A'\"\033[0m"
cd ../..
git clone https://github.com/jgarff/rpi_ws281x.git
cd rpi_ws281x
mkdir build
cd build
cmake ..
cmake --build .
sudo make install
cd ../..
