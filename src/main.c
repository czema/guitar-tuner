/*

# Guitar Tuner
Copyright 2021 Chet Zema

This application reads the Polytune 3 display and renders it on an LED string.

## Polytune 3 Hardware

The Polytune performs the waveform analysis and determines how out of pitch the instument is.
To indicate this, it displays information on a grid of 109 LEDs.
The LEDs are controlled by a triplet of HC595 shift registers.  The protocol is very similar to SPI.

## FT4222 Hardware

The FT4222 can operate as an SPI slave and transfer the data over USB to a host.

## Raspberry Pi Hardware

Using a Raspberry Pi 3, read the SPI data from the USB port.  Also write to an WS2812 LED string
using PWM on GPIO18 (physical pin #12).

## WS2812 LED Harware

The WS2812 is a shift register which allows you to write data to the string and then latch the data
to produce an output.  When a byte is written the WS2812 passes its current byte to the next chip
down the string.  It requires specific timing which is possible on the Raspberry Pi using the PWM output.

## Software

This application consists of three portions:
 - SPI Read
 - Virtual Display
 - WS2812 Write

The SPI reading component requests data from the FT4222 connected via USB.  As the bytes are read,
they are synced into a byte stream which can be analyzed.

The Virtual Display analyzes the SPI byte stream and attempts to reconstruct the LED state on the Polytune.
It then outputs that reconstruction into a format suitable for display on the WS2812 string.  It also
accomplishes the 4x multiplication of the original display.

The WS2812 writing component sends the output display to the WS2818 string using the jgarff/rpi_ws281x library.

*/

#include <stdio.h>
#include <unistd.h>

#include "spi.h"
#include "leds.h"

int main(int argc, char* argv[]) {
	int serr = setuid(0);
	if (serr != 0) {
		printf("MAIN: setuid(0) failed: %d\n", serr);
		return -1;
	}
	else {
		puts("MAIN: Superuser granted.");
	}

	if (leds_init() != 0) {
		puts("LED Initialization failed.");
		return -2;
	}

	const char* description = spi_init();

	// This will start the SPI reader.  It in turn will call display_update which calls leds_render.
	// Note that the LEDs will only update in response to SPI data.
	// Press Ctrl+C to cleanup and shutdown.
	spi_reader(description);

	puts("\nMAIN: Shutdown.");
}
