## Giant Guitar Tuner ##

### Setup and Installation ###

* Using Raspberry Pi Imager, choose the `Raspberry Pi OS Lite` image and select the SD card, click Write.
* Mount the boot drive and make the following changes to config.txt:
  * Comment out `dtparam=audio=on`
  * Add
  ```
  disable_splash=1
  dtoverlay=disable-bt
  dtoverlay=sd_overclock=100
  boot_delay=0
  dtoverlay=gpio-shutdown
  ```
* Unmount and remove the SD card.
* Boot the raspi (press enter if it seems to hang at "Update UTMP about System Runlevel Changes.")
* Default username/password: pi/raspberry

### Configure the Pi. ###

`sudo raspi-config`

* Localization, WLAN Country - US
* Localization, Timezone - US, Eastern
* Locatization, Keyboard - English (US), Other.
* System, Wireless LAN (configure to allow installing packages and transferring application code). Use Atreyu-24
* System, Network at Boot (disable to speed bootup).
* Interface, SSH (enables SFTP and SSH console).
* Interface, Serial Port (enable to allow console access over pins 6 GND, 8 TX, 10 RX).
* Finish

Ensure apt-get has the latest versions and install them.

```bash
sudo apt-get update
sudo apt-get upgrade -y
```

Install Git, cmake, pigpio.

`sudo apt-get install git cmake pigpio -y`

### Install Application ###

```bash
git clone https://github.com/czema/guitar-tuner
cd guitar-tuner
sudo ./configure.sh
cd src
sudo ./compile.sh
cd ..
./guitar-tuner
```

It should say something like "SPI: FT4222 SPI Slave OK, Running..."

Press Ctrl+C to close the program.

Install the service.
`sudo ./src/service.sh`

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