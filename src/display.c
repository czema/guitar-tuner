/*

# Guitar Tuner
Copyright 2021 Chet Zema

This component interprets the Polytune LEDs state and converts it to a format suitable for an LED string.

*/

#include <stdint.h>

#include "leds.h"

#define RED   0x00F00000
#define GREEN 0x0000F000
#define BLUE  0x000000F0

// If the current LED is already on, leave it on.  Otherwise switch it based on row/col/color.
#define SET(K, ROW, COL, COLOR) (LEDS[K] = LEDS[K] == 0 ? (ROW & COL) * COLOR : LEDS[K]); K++

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

	//Display_Row(row1, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row2, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row3, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row4, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row5, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);

	//Display_Row(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, row6 & col12, row6 & col13, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row7 & col06, row6 & col06, row6 & col08, row6 & col11, row6 & col14, row6 & col15, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row7 & col05, row7 & col07, row6 & col07, row6 & col16, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row6 & col05, row6 & col09, row6 & col10, row6 & col17, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row6 & col01, row6 & col02, row6 & col03, row6 & col04, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row7 & col01, row7 & col02, row7 & col03, row7 & col04, 0, 0, 0, 0, 0, 0, 0, 0);

	int k = 0;

	// unused
	// k = 0
	SET(k, 1, 1, RED);
	SET(k, 1, 1, GREEN);
	SET(k, 1, 1, BLUE);
	SET(k, 1, 1, RED);
	SET(k, 1, 1, GREEN);
	SET(k, 1, 1, BLUE);

	// row 11 (bottom)
	// k = 0 (acutally 6)
	SET(k, row7, col04, RED);
	SET(k, row7, col04, RED);
	SET(k, row7, col03, RED);
	SET(k, row7, col03, RED);
	SET(k, row7, col02, RED);
	SET(k, row7, col02, RED);
	SET(k, row7, col01, RED);
	SET(k, row7, col01, RED);

	SET(k, row7, col01, RED);
	SET(k, row7, col01, RED);
	SET(k, row7, col02, RED);
	SET(k, row7, col02, RED);
	SET(k, row7, col03, RED);
	SET(k, row7, col03, RED);
	SET(k, row7, col04, RED);
	SET(k, row7, col04, RED);

	// row 10
	// k = 16
	SET(k, row6, col04, RED);
	SET(k, row6, col04, RED);
	SET(k, row6, col03, RED);
	SET(k, row6, col03, RED);
	SET(k, row6, col02, RED);
	SET(k, row6, col02, RED);
	SET(k, row6, col01, RED);
	SET(k, row6, col01, RED);

	SET(k, row6, col01, RED);
	SET(k, row6, col01, RED);
	SET(k, row6, col02, RED);
	SET(k, row6, col02, RED);
	SET(k, row6, col03, RED);
	SET(k, row6, col03, RED);
	SET(k, row6, col04, RED);
	SET(k, row6, col04, RED);

	// row 9
	// k = 32
	SET(k, row6, col17, RED);
	SET(k, row6, col17, RED);
	SET(k, row6, col10, RED);
	SET(k, row6, col10, RED);
	SET(k, row6, col09, RED);
	SET(k, row6, col09, RED);
	SET(k, row6, col05, RED);
	SET(k, row6, col05, RED);

	SET(k, row6, col05, RED);
	SET(k, row6, col05, RED);
	SET(k, row6, col09, RED);
	SET(k, row6, col09, RED);
	SET(k, row6, col10, RED);
	SET(k, row6, col10, RED);
	SET(k, row6, col17, RED);
	SET(k, row6, col17, RED);

	// row 8
	// k = 48
	SET(k, row6, col16, RED);
	SET(k, row6, col16, RED);
	SET(k, row6, col07, RED);
	SET(k, row6, col07, RED);
	SET(k, row7, col07, RED);
	SET(k, row7, col07, RED);
	SET(k, row7, col05, RED);
	SET(k, row7, col05, RED);

	SET(k, row7, col05, RED);
	SET(k, row7, col05, RED);
	SET(k, row7, col07, RED);
	SET(k, row7, col07, RED);
	SET(k, row6, col07, RED);
	SET(k, row6, col07, RED);
	SET(k, row6, col16, RED);
	SET(k, row6, col16, RED);

	// row 7
	// k = 64
	SET(k, row6, col11, RED);
	SET(k, row6, col11, RED);
	SET(k, row6, col08, RED);
	SET(k, row6, col08, RED);
	SET(k, row6, col06, RED);
	SET(k, row6, col06, RED);
	SET(k, row7, col06, RED);
	SET(k, row7, col06, RED);

	SET(k, row7, col06, RED);
	SET(k, row7, col06, RED);
	SET(k, row6, col06, RED);
	SET(k, row6, col06, RED);
	SET(k, row6, col08, RED);
	SET(k, row6, col08, RED);
	SET(k, row6, col11, RED);
	SET(k, row6, col11, RED);

	// row 7 (sharp, bottom)
	SET(k, row6, col14, RED);
	SET(k, row6, col14, RED);
	SET(k, row6, col15, RED);
	SET(k, row6, col15, RED);
	SET(k, row6, col15, RED);
	SET(k, row6, col15, RED);
	SET(k, row6, col14, RED);
	SET(k, row6, col14, RED);

	// row 6 (sharp, top)
	SET(k, row6, col12, RED);
	SET(k, row6, col12, RED);
	SET(k, row6, col13, RED);
	SET(k, row6, col13, RED);
	SET(k, row6, col13, RED);
	SET(k, row6, col13, RED);
	SET(k, row6, col12, RED);
	SET(k, row6, col12, RED);

	k += 8; // Skip 8 leds.

	// row 5
	SET(k, row5, col17, RED);
	SET(k, row5, col17, RED);
	SET(k, row5, col16, RED);
	SET(k, row5, col16, RED);
	SET(k, row5, col15, RED);
	SET(k, row5, col15, RED);
	SET(k, row5, col14, RED);
	SET(k, row5, col14, RED);
	SET(k, row5, col13, RED);
	SET(k, row5, col13, RED);
	SET(k, row5, col12, RED);
	SET(k, row5, col12, RED);
	SET(k, row5, col11, RED);
	SET(k, row5, col11, RED);
	SET(k, row5, col10, RED);
	SET(k, row5, col10, RED);
	SET(k, row5, col09, RED);
	SET(k, row5, col09, RED);
	SET(k, row5, col08, RED);
	SET(k, row5, col08, RED);
	SET(k, row5, col07, RED);
	SET(k, row5, col07, RED);
	SET(k, row5, col06, RED);
	SET(k, row5, col06, RED);
	SET(k, row5, col05, RED);
	SET(k, row5, col05, RED);
	SET(k, row5, col04, RED);
	SET(k, row5, col04, RED);
	SET(k, row5, col03, RED);
	SET(k, row5, col03, RED);
	SET(k, row5, col02, RED);
	SET(k, row5, col02, RED);
	SET(k, row5, col01, RED);
	SET(k, row5, col01, RED);

	SET(k, row5, col01, RED);
	SET(k, row5, col01, RED);
	SET(k, row5, col02, RED);
	SET(k, row5, col02, RED);
	SET(k, row5, col03, RED);
	SET(k, row5, col03, RED);
	SET(k, row5, col04, RED);
	SET(k, row5, col04, RED);
	SET(k, row5, col05, RED);
	SET(k, row5, col05, RED);
	SET(k, row5, col06, RED);
	SET(k, row5, col06, RED);
	SET(k, row5, col07, RED);
	SET(k, row5, col07, RED);
	SET(k, row5, col08, RED);
	SET(k, row5, col08, RED);
	SET(k, row5, col09, RED);
	SET(k, row5, col09, RED);
	SET(k, row5, col10, RED);
	SET(k, row5, col10, RED);
	SET(k, row5, col11, RED);
	SET(k, row5, col11, RED);
	SET(k, row5, col12, RED);
	SET(k, row5, col12, RED);
	SET(k, row5, col13, RED);
	SET(k, row5, col13, RED);
	SET(k, row5, col14, RED);
	SET(k, row5, col14, RED);
	SET(k, row5, col15, RED);
	SET(k, row5, col15, RED);
	SET(k, row5, col16, RED);
	SET(k, row5, col16, RED);
	SET(k, row5, col17, RED);
	SET(k, row5, col17, RED);

	// row 4
	SET(k, row4, col17, RED);
	SET(k, row4, col17, RED);
	SET(k, row4, col16, RED);
	SET(k, row4, col16, RED);
	SET(k, row4, col15, RED);
	SET(k, row4, col15, RED);
	SET(k, row4, col14, RED);
	SET(k, row4, col14, RED);
	SET(k, row4, col13, RED);
	SET(k, row4, col13, RED);
	SET(k, row4, col12, RED);
	SET(k, row4, col12, RED);
	SET(k, row4, col11, RED);
	SET(k, row4, col11, RED);
	SET(k, row4, col10, RED);
	SET(k, row4, col10, RED);
	SET(k, row4, col09, RED);
	SET(k, row4, col09, RED);
	SET(k, row4, col08, RED);
	SET(k, row4, col08, RED);
	SET(k, row4, col07, RED);
	SET(k, row4, col07, RED);
	SET(k, row4, col06, RED);
	SET(k, row4, col06, RED);
	SET(k, row4, col05, RED);
	SET(k, row4, col05, RED);
	SET(k, row4, col04, RED);
	SET(k, row4, col04, RED);
	SET(k, row4, col03, RED);
	SET(k, row4, col03, RED);
	SET(k, row4, col02, RED);
	SET(k, row4, col02, RED);
	SET(k, row4, col01, RED);
	SET(k, row4, col01, RED);

	SET(k, row4, col01, RED);
	SET(k, row4, col01, RED);
	SET(k, row4, col02, RED);
	SET(k, row4, col02, RED);
	SET(k, row4, col03, RED);
	SET(k, row4, col03, RED);
	SET(k, row4, col04, RED);
	SET(k, row4, col04, RED);
	SET(k, row4, col05, RED);
	SET(k, row4, col05, RED);
	SET(k, row4, col06, RED);
	SET(k, row4, col06, RED);
	SET(k, row4, col07, RED);
	SET(k, row4, col07, RED);
	SET(k, row4, col08, RED);
	SET(k, row4, col08, RED);
	SET(k, row4, col09, RED);
	SET(k, row4, col09, RED);
	SET(k, row4, col10, RED);
	SET(k, row4, col10, RED);
	SET(k, row4, col11, RED);
	SET(k, row4, col11, RED);
	SET(k, row4, col12, RED);
	SET(k, row4, col12, RED);
	SET(k, row4, col13, RED);
	SET(k, row4, col13, RED);
	SET(k, row4, col14, RED);
	SET(k, row4, col14, RED);
	SET(k, row4, col15, RED);
	SET(k, row4, col15, RED);
	SET(k, row4, col16, RED);
	SET(k, row4, col16, RED);
	SET(k, row4, col17, RED);
	SET(k, row4, col17, RED);

	// row 3
	SET(k, row3, col17, GREEN);
	SET(k, row3, col17, GREEN);
	SET(k, row3, col16, GREEN);
	SET(k, row3, col16, GREEN);
	SET(k, row3, col15, GREEN);
	SET(k, row3, col15, GREEN);
	SET(k, row3, col14, GREEN);
	SET(k, row3, col14, GREEN);
	SET(k, row3, col13, GREEN);
	SET(k, row3, col13, GREEN);
	SET(k, row3, col12, GREEN);
	SET(k, row3, col12, GREEN);
	SET(k, row3, col11, GREEN);
	SET(k, row3, col11, GREEN);
	SET(k, row3, col10, GREEN);
	SET(k, row3, col10, GREEN);
	SET(k, row3, col09, GREEN);
	SET(k, row3, col09, GREEN);
	SET(k, row3, col08, GREEN);
	SET(k, row3, col08, GREEN);
	SET(k, row3, col07, GREEN);
	SET(k, row3, col07, GREEN);
	SET(k, row3, col06, GREEN);
	SET(k, row3, col06, GREEN);
	SET(k, row3, col05, GREEN);
	SET(k, row3, col05, GREEN);
	SET(k, row3, col04, GREEN);
	SET(k, row3, col04, GREEN);
	SET(k, row3, col03, GREEN);
	SET(k, row3, col03, GREEN);
	SET(k, row3, col02, GREEN);
	SET(k, row3, col02, GREEN);
	SET(k, row3, col01, GREEN);
	SET(k, row3, col01, GREEN);

	SET(k, row3, col01, GREEN);
	SET(k, row3, col01, GREEN);
	SET(k, row3, col02, GREEN);
	SET(k, row3, col02, GREEN);
	SET(k, row3, col03, GREEN);
	SET(k, row3, col03, GREEN);
	SET(k, row3, col04, GREEN);
	SET(k, row3, col04, GREEN);
	SET(k, row3, col05, GREEN);
	SET(k, row3, col05, GREEN);
	SET(k, row3, col06, GREEN);
	SET(k, row3, col06, GREEN);
	SET(k, row3, col07, GREEN);
	SET(k, row3, col07, GREEN);
	SET(k, row3, col08, GREEN);
	SET(k, row3, col08, GREEN);
	SET(k, row3, col09, GREEN);
	SET(k, row3, col09, GREEN);
	SET(k, row3, col10, GREEN);
	SET(k, row3, col10, GREEN);
	SET(k, row3, col11, GREEN);
	SET(k, row3, col11, GREEN);
	SET(k, row3, col12, GREEN);
	SET(k, row3, col12, GREEN);
	SET(k, row3, col13, GREEN);
	SET(k, row3, col13, GREEN);
	SET(k, row3, col14, GREEN);
	SET(k, row3, col14, GREEN);
	SET(k, row3, col15, GREEN);
	SET(k, row3, col15, GREEN);
	SET(k, row3, col16, GREEN);
	SET(k, row3, col16, GREEN);
	SET(k, row3, col17, GREEN);
	SET(k, row3, col17, GREEN);

	// row 2
	SET(k, row2, col17, RED);
	SET(k, row2, col17, RED);
	SET(k, row2, col16, RED);
	SET(k, row2, col16, RED);
	SET(k, row2, col15, RED);
	SET(k, row2, col15, RED);
	SET(k, row2, col14, RED);
	SET(k, row2, col14, RED);
	SET(k, row2, col13, RED);
	SET(k, row2, col13, RED);
	SET(k, row2, col12, RED);
	SET(k, row2, col12, RED);
	SET(k, row2, col11, RED);
	SET(k, row2, col11, RED);
	SET(k, row2, col10, RED);
	SET(k, row2, col10, RED);
	SET(k, row2, col09, RED);
	SET(k, row2, col09, RED);
	SET(k, row2, col08, RED);
	SET(k, row2, col08, RED);
	SET(k, row2, col07, RED);
	SET(k, row2, col07, RED);
	SET(k, row2, col06, RED);
	SET(k, row2, col06, RED);
	SET(k, row2, col05, RED);
	SET(k, row2, col05, RED);
	SET(k, row2, col04, RED);
	SET(k, row2, col04, RED);
	SET(k, row2, col03, RED);
	SET(k, row2, col03, RED);
	SET(k, row2, col02, RED);
	SET(k, row2, col02, RED);
	SET(k, row2, col01, RED);
	SET(k, row2, col01, RED);

	SET(k, row2, col01, RED);
	SET(k, row2, col01, RED);
	SET(k, row2, col02, RED);
	SET(k, row2, col02, RED);
	SET(k, row2, col03, RED);
	SET(k, row2, col03, RED);
	SET(k, row2, col04, RED);
	SET(k, row2, col04, RED);
	SET(k, row2, col05, RED);
	SET(k, row2, col05, RED);
	SET(k, row2, col06, RED);
	SET(k, row2, col06, RED);
	SET(k, row2, col07, RED);
	SET(k, row2, col07, RED);
	SET(k, row2, col08, RED);
	SET(k, row2, col08, RED);
	SET(k, row2, col09, RED);
	SET(k, row2, col09, RED);
	SET(k, row2, col10, RED);
	SET(k, row2, col10, RED);
	SET(k, row2, col11, RED);
	SET(k, row2, col11, RED);
	SET(k, row2, col12, RED);
	SET(k, row2, col12, RED);
	SET(k, row2, col13, RED);
	SET(k, row2, col13, RED);
	SET(k, row2, col14, RED);
	SET(k, row2, col14, RED);
	SET(k, row2, col15, RED);
	SET(k, row2, col15, RED);
	SET(k, row2, col16, RED);
	SET(k, row2, col16, RED);
	SET(k, row2, col17, RED);
	SET(k, row2, col17, RED);

	// row 1
	SET(k, row1, col17, RED);
	SET(k, row1, col17, RED);
	SET(k, row1, col16, RED);
	SET(k, row1, col16, RED);
	SET(k, row1, col15, RED);
	SET(k, row1, col15, RED);
	SET(k, row1, col14, RED);
	SET(k, row1, col14, RED);
	SET(k, row1, col13, RED);
	SET(k, row1, col13, RED);
	SET(k, row1, col12, RED);
	SET(k, row1, col12, RED);
	SET(k, row1, col11, RED);
	SET(k, row1, col11, RED);
	SET(k, row1, col10, RED);
	SET(k, row1, col10, RED);
	SET(k, row1, col09, RED);
	SET(k, row1, col09, RED);
	SET(k, row1, col08, RED);
	SET(k, row1, col08, RED);
	SET(k, row1, col07, RED);
	SET(k, row1, col07, RED);
	SET(k, row1, col06, RED);
	SET(k, row1, col06, RED);
	SET(k, row1, col05, RED);
	SET(k, row1, col05, RED);
	SET(k, row1, col04, RED);
	SET(k, row1, col04, RED);
	SET(k, row1, col03, RED);
	SET(k, row1, col03, RED);
	SET(k, row1, col02, RED);
	SET(k, row1, col02, RED);
	SET(k, row1, col01, RED);
	SET(k, row1, col01, RED);

	SET(k, row1, col01, RED);
	SET(k, row1, col01, RED);
	SET(k, row1, col02, RED);
	SET(k, row1, col02, RED);
	SET(k, row1, col03, RED);
	SET(k, row1, col03, RED);
	SET(k, row1, col04, RED);
	SET(k, row1, col04, RED);
	SET(k, row1, col05, RED);
	SET(k, row1, col05, RED);
	SET(k, row1, col06, RED);
	SET(k, row1, col06, RED);
	SET(k, row1, col07, RED);
	SET(k, row1, col07, RED);
	SET(k, row1, col08, RED);
	SET(k, row1, col08, RED);
	SET(k, row1, col09, RED);
	SET(k, row1, col09, RED);
	SET(k, row1, col10, RED);
	SET(k, row1, col10, RED);
	SET(k, row1, col11, RED);
	SET(k, row1, col11, RED);
	SET(k, row1, col12, RED);
	SET(k, row1, col12, RED);
	SET(k, row1, col13, RED);
	SET(k, row1, col13, RED);
	SET(k, row1, col14, RED);
	SET(k, row1, col14, RED);
	SET(k, row1, col15, RED);
	SET(k, row1, col15, RED);
	SET(k, row1, col16, RED);
	SET(k, row1, col16, RED);
	SET(k, row1, col17, RED);
	SET(k, row1, col17, RED);
}

void display_reset(void) {
	int i;
	for (i = 0; i < LED_COUNT; i++) {
		LEDS[i] = 0;
	}
}

void display_render(void) {
	leds_render();
}
