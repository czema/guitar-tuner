/*

# Guitar Tuner
Copyright 2021 Chet Zema

This component interprets the Polytune LEDs state and converts it to a format suitable for an LED string.

*/

#include <stdint.h>

#include "leds.h"

#define NUM_LEDS 64
#define RED   0x00F00000
#define GREEN 0x0000F000
#define BLUE  0x000000F0

void Display_Row(uint8_t row, uint8_t col01, uint8_t col02, uint8_t col03, uint8_t col04, uint8_t col05, uint8_t col06, uint8_t col07, uint8_t col08, uint8_t col09, uint8_t col10, uint8_t col11, uint8_t col12, uint8_t col13, uint8_t col14, uint8_t col15, uint8_t col16, uint8_t col17) {
	
	return;
}

uint32_t buffer[NUM_LEDS]; // Built during the current update.
uint32_t active[NUM_LEDS]; // Contains the maximum value for every cell in a given frame.  A frame lasts 16ms and is cleared once rendered.

void display_update(uint8_t q1, uint8_t q2, uint8_t q3, uint8_t q4) {
	if (q4 != 0) return; // If q4 doesn't hold all zeros then we are out of sync (the device transmits 32 bits but only has 3 HC595's).

	uint8_t row5 = (q1 & 0b00000001) != 1; //   q1->a
	uint8_t row1 = (q1 & 0b00000010) != 2; //   q1->b
	uint8_t row2 = (q1 & 0b00000100) != 4; //   q1->c
	uint8_t row3 = (q1 & 0b00001000) != 8; //   q1->d
	uint8_t row4 = (q1 & 0b00010000) != 16; //  q1->e
	uint8_t row6 = (q1 & 0b00100000) != 32; //  q1->f
	uint8_t row7 = (q1 & 0b01000000) != 64; //  q1->g
	uint8_t col09 = (q1 & 0b10000000) != 128; // q1->h

	uint8_t col01 = (q2 & 0b00000001) != 1; //   q2->a
	uint8_t col02 = (q2 & 0b00000010) != 2; //   q2->b
	uint8_t col03 = (q2 & 0b00000100) != 4; //   q2->c
	uint8_t col04 = (q2 & 0b00001000) != 8; //   q2->d
	uint8_t col05 = (q2 & 0b00010000) != 16; //  q2->e
	uint8_t col06 = (q2 & 0b00100000) != 32; //  q2->f
	uint8_t col07 = (q2 & 0b01000000) != 64; //  q2->g
	uint8_t col08 = (q2 & 0b10000000) != 128; // q2->h

	uint8_t col10 = (q3 & 0b00000001) != 1; //   q3->a
	uint8_t col11 = (q3 & 0b00000010) != 2; //   q3->b
	uint8_t col12 = (q3 & 0b00000100) != 4; //   q3->c
	uint8_t col13 = (q3 & 0b00001000) != 8; //   q3->d
	uint8_t col14 = (q3 & 0b00010000) != 16; //  q3->e
	uint8_t col15 = (q3 & 0b00100000) != 32; //  q3->f
	uint8_t col16 = (q3 & 0b01000000) != 64; //  q3->g
	uint8_t col17 = (q3 & 0b10000000) != 128; // q3->h

	Display_Row(row1, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	Display_Row(row2, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	Display_Row(row3, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	Display_Row(row4, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	Display_Row(row5, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);

	Display_Row(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, row6 & col12, row6 & col13, 0, 0, 0, 0, 0, 0);
	Display_Row(1, 0, 0, 0, 0, 0, row7 & col06, row6 & col06, row6 & col08, row6 & col11, row6 & col14, row6 & col15, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row7 & col05, row7 & col07, row6 & col07, row6 & col16, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row6 & col05, row6 & col09, row6 & col10, row6 & col17, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row6 & col01, row6 & col02, row6 & col03, row6 & col04, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row7 & col01, row7 & col02, row7 & col03, row7 & col04, 0, 0, 0, 0, 0, 0, 0, 0);

	int k = 0;

	// row 11 (bottom)
	// k = 0
	buffer[k++] = (row7 & col04) * RED;
	buffer[k++] = (row7 & col04) * RED;
	buffer[k++] = (row7 & col03) * RED;
	buffer[k++] = (row7 & col03) * RED;
	buffer[k++] = (row7 & col02) * RED;
	buffer[k++] = (row7 & col02) * RED;
	buffer[k++] = (row7 & col01) * RED;
	buffer[k++] = (row7 & col01) * RED;

	buffer[k++] = (row7 & col01) * RED;
	buffer[k++] = (row7 & col01) * RED;
	buffer[k++] = (row7 & col02) * RED;
	buffer[k++] = (row7 & col02) * RED;
	buffer[k++] = (row7 & col03) * RED;
	buffer[k++] = (row7 & col03) * RED;
	buffer[k++] = (row7 & col04) * RED;
	buffer[k++] = (row7 & col04) * RED;

	// row 10
	// k = 16
	buffer[k++] = (row6 & col04) * RED;
	buffer[k++] = (row6 & col04) * RED;
	buffer[k++] = (row6 & col03) * RED;
	buffer[k++] = (row6 & col03) * RED;
	buffer[k++] = (row6 & col02) * RED;
	buffer[k++] = (row6 & col02) * RED;
	buffer[k++] = (row6 & col01) * RED;
	buffer[k++] = (row6 & col01) * RED;

	buffer[k++] = (row6 & col01) * RED;
	buffer[k++] = (row6 & col01) * RED;
	buffer[k++] = (row6 & col02) * RED;
	buffer[k++] = (row6 & col02) * RED;
	buffer[k++] = (row6 & col03) * RED;
	buffer[k++] = (row6 & col03) * RED;
	buffer[k++] = (row6 & col04) * RED;
	buffer[k++] = (row6 & col04) * RED;

	// row 9
	// k = 32
	buffer[k++] = (row6 & col17) * RED;
	buffer[k++] = (row6 & col17) * RED;
	buffer[k++] = (row6 & col10) * RED;
	buffer[k++] = (row6 & col10) * RED;
	buffer[k++] = (row6 & col09) * RED;
	buffer[k++] = (row6 & col09) * RED;
	buffer[k++] = (row6 & col05) * RED;
	buffer[k++] = (row6 & col05) * RED;

	buffer[k++] = (row6 & col05) * RED;
	buffer[k++] = (row6 & col05) * RED;
	buffer[k++] = (row6 & col09) * RED;
	buffer[k++] = (row6 & col09) * RED;
	buffer[k++] = (row6 & col10) * RED;
	buffer[k++] = (row6 & col10) * RED;
	buffer[k++] = (row6 & col17) * RED;
	buffer[k++] = (row6 & col17) * RED;

	// row 8
	// k = 48
	buffer[k++] = (row6 & col16) * RED;
	buffer[k++] = (row6 & col16) * RED;
	buffer[k++] = (row6 & col07) * RED;
	buffer[k++] = (row6 & col07) * RED;
	buffer[k++] = (row7 & col07) * RED;
	buffer[k++] = (row7 & col07) * RED;
	buffer[k++] = (row7 & col05) * RED;
	buffer[k++] = (row7 & col05) * RED;

	buffer[k++] = (row7 & col05) * RED;
	buffer[k++] = (row7 & col05) * RED;
	buffer[k++] = (row7 & col07) * RED;
	buffer[k++] = (row7 & col07) * RED;
	buffer[k++] = (row6 & col07) * RED;
	buffer[k++] = (row6 & col07) * RED;
	buffer[k++] = (row6 & col16) * RED;
	buffer[k++] = (row6 & col16) * RED;

	// row 7
	// k = 64

	// If an LED goes high during this cycle then it is latched until cleared.
	int i;
	for (i = 0;i < NUM_LEDS;i++) {
		if (buffer[i] > 0) active[i] = buffer[i];
	}
}

void display_clear(void) {
	int i;
	for (i = 0; i < NUM_LEDS;i++) {
		active[i] = 0;
	}
}

void display_render(void) {
	leds_render(active, NUM_LEDS);
}
