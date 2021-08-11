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
sudo ./compile.sh
cd bin
./guitar-tuner
```

It should say something like "SPI: FT4222 SPI Slave OK, Running."

Press Ctrl+C to close the program.

To start the application as a service, run `sudo ./install.sh`

### Hardware Notes ###
https://pinout.xyz/

```
Pin #02 - 5V
Pin #05 - GPIO3 (active low, power on/off)
Pin #06 - GND
Pin #08 - TX (serial console)
Pin #09 - GND (power on/off)
Pin #10 - RX (serial console)
Pin #12 - GPIO18 (PWM LED data)
Pin #14 - GND
Pin #16 - GPIO23 (Motion Sensor)
Pin #18 - GPIO24 (Relay, power supply)
```

### Generic Raspberry Pi Notes ###

* To resize the serial console terminal, `nano .profile` and add:
```bash
if [[ "$(tty)" != "/dev/tty1" ]] ; then
        stty rows 60 cols 160
fi
```
* The gpio-shutdown dtoverlay (config.txt) allows connecting GPIO 3 to GND to safely shut down the computer.  The rpi comes pre-configured to allow GPIO 3 to GND to power up the computer.