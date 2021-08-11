## Giant Guitar Tuner ##

### Setup and Installation ###

* Using Raspberry Pi Imager, choose the `Raspberry Pi OS Lite` image and select the SD card, click Write.
* Mount the boot drive and make the following changes to config.txt:
  * Comment out dtparam=audio=on
  * Add
```
  disable_splash=1
  dtoverlay=pi3-disable-bt
  dtoverlay=sdtweak,overclock_50=100
  boot_delay=0
  dtoverlay=gpio-shutdown
```
* Unmount and remove the SD card.
* Boot rapsi, press enter if it seems to hang at "Update UTMP about System Runlevel Changes."
* Default username/password: pi/raspberry

### Configure the Pi. ###

`sudo raspi-config`

Localization, WLAN Country - US
Localization, Timezone - US, Eastern
System, Wireless LAN (configure to allow installing packages and transferring application code). Use Atreyu-24
System, Network at Boot (disable to speed bootup).
Interface, SSH (enables SFTP and SSH console).
Interface, Serial Port (enable to allow console access over pins 6 GND, 8 TX, 10 RX).
Finish

Ensure apt-get has the latest versions and install them.

`sudo apt-get update`
`sudo apt-get upgrade`

Install Git, cmake, pigpio.

`sudo apt-get install git cmake pigpio`

Create a directory to hold dependencies, making it easier to clean up afterwards.

`mkdir app`
`mkdir deps`
`cd deps`

Download rpi_ws28x library.  This requires Git.

```
git clone https://github.com/jgarff/rpi_ws281x.git
cd rpi_ws281x
mkdir build
cd build
cmake ..
cmake --build .
sudo make install
cd ../..
```

Download LibFT4222.

```
mkdir libft4222
cd libft4222
curl https://ftdichip.com/wp-content/uploads/2021/01/libft4222-linux-1.4.4.44.zip -o libft4222-linux-1.4.4.44.zip
unzip libft4222-linux-1.4.4.44.zip
tar -xvzf libft4222-linux-1.4.4.44.tgz
sudo ./install4222.sh
cd examples
cc get-version.c -lft4222 -Wl,-rpath,/usr/local/lib -o get-version
sudo ./get-version
```

It should say something like "Device 0: 'FT4222 A'"

Create application.

Using WinSCP, copy the source code to the raspi in ~/app directory.

```
cd ~/app
chmod 744 install.sh
chmod 744 compile.sh
sudo ./compile.sh
```

It should say something like "SPI: FT4222 SPI Slave OK, Running..."
Press Ctrl+C to close the program.

Install the service.
`sudo ./install.sh`

Notes:
 To resize the terminal, `nano .profile` and add:
```
if [[ "$(tty)" == "/dev/tty1" ]] ; then
        # commands in here only execute on local console.
        echo -e "\033[1,33mWelcome.\033[0m"
else
        stty rows 60 cols 160
fi
```

The gpio-shutdown dtoverlay (config.txt) allows connecting GPIO 3 to GND to safely shut down the computer.  The rpi comes pre-configured to allow GPIO 3 to GND to power up the computer.