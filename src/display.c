/*

# Guitar Tuner
Copyright 2021 Chet Zema

This component interprets the Polytune LEDs state and converts it to a format suitable for an LED string.

*/

#include <stdio.h>
#include <stdint.h>

#include "leds.h"

#define RED   0x00F00000
#define GREEN 0x0000F000
#define BLUE  0x000000F0

// If the current LED is already on, leave it on.  Otherwise switch it based on row/col/color.
//#define SET(K, ROW, COL, COLOR) (LEDS[K] = LEDS[K] == 0 ? (ROW & COL) * COLOR : LEDS[K]); K++
#define SET(K, ROW, COL, COLOR) if ((ROW) & (COL)) LEDS[(K)] = (COLOR); (K++)

void display_update(uint8_t q1, uint8_t q2, uint8_t q3, uint8_t q4) {
	if (q4 != 0) return; // If q4 doesn't hold all zeros then we are out of sync (the device transmits 32 bits but only has 3 HC595's).

	uint8_t row05 = (q1 & 0b00000001) != 1; //   q1->a
	uint8_t row01 = (q1 & 0b00000010) != 2; //   q1->b
	uint8_t row02 = (q1 & 0b00000100) != 4; //   q1->c
	uint8_t row03 = (q1 & 0b00001000) != 8; //   q1->d
	uint8_t row04 = (q1 & 0b00010000) != 16; //  q1->e
	uint8_t row06 = (q1 & 0b00100000) != 32; //  q1->f
	uint8_t row07 = (q1 & 0b01000000) != 64; //  q1->g
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

	//Display_Row(row01, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row02, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row03, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row04, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);
	//Display_Row(row05, col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12, col13, col14, col15, col16, col17);

	//Display_Row(1, 0, 0, 0, 0, 0,				0,				0,				0,			 0, row06 & col12, row06 & col13, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row07 & col06, row06 & col06, row06 & col08, row06 & col11, row06 & col14, row06 & col15, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row07 & col05, row07 & col07, row06 & col07, row06 & col16, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row06 & col05, row06 & col09, row06 & col10, row06 & col17, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row06 & col01, row06 & col02, row06 & col03, row06 & col04, 0, 0, 0, 0, 0, 0, 0, 0);
	//Display_Row(1, 0, 0, 0, 0, 0, row07 & col01, row07 & col02, row07 & col03, row07 & col04, 0, 0, 0, 0, 0, 0, 0, 0);

	uint16_t k = 0;

	// unused
	// k = 0
	SET(k, 1, 1, RED);
	SET(k, 1, 1, GREEN);
	SET(k, 1, 1, BLUE);
	SET(k, 1, 1, RED);
	SET(k, 1, 1, GREEN);
	SET(k, 1, 1, BLUE);
	
	// row 11 (bottom)
	// k = 6
	SET(k, row07, col04, RED);
	SET(k, row07, col04, RED);
	SET(k, row07, col03, RED);
	SET(k, row07, col03, RED);
	SET(k, row07, col02, RED);
	SET(k, row07, col02, RED);
	SET(k, row07, col01, RED);
	SET(k, row07, col01, RED);

	SET(k, row07, col01, RED);
	SET(k, row07, col01, RED);
	SET(k, row07, col02, RED);
	SET(k, row07, col02, RED);
	SET(k, row07, col03, RED);
	SET(k, row07, col03, RED);
	SET(k, row07, col04, RED);
	SET(k, row07, col04, RED);

	// row 10
	// k = 22
	SET(k, row06, col04, RED);
	SET(k, row06, col04, RED);
	SET(k, row06, col03, RED);
	SET(k, row06, col03, RED);
	SET(k, row06, col02, RED);
	SET(k, row06, col02, RED);
	SET(k, row06, col01, RED);
	SET(k, row06, col01, RED);

	SET(k, row06, col01, RED);
	SET(k, row06, col01, RED);
	SET(k, row06, col02, RED);
	SET(k, row06, col02, RED);
	SET(k, row06, col03, RED);
	SET(k, row06, col03, RED);
	SET(k, row06, col04, RED);
	SET(k, row06, col04, RED);

	// row 9
	// k = 38
	SET(k, row06, col17, RED);
	SET(k, row06, col17, RED);
	SET(k, row06, col10, RED);
	SET(k, row06, col10, RED);
	SET(k, row06, col09, RED);
	SET(k, row06, col09, RED);
	SET(k, row06, col05, RED);
	SET(k, row06, col05, RED);

	SET(k, row06, col05, RED);
	SET(k, row06, col05, RED);
	SET(k, row06, col09, RED);
	SET(k, row06, col09, RED);
	SET(k, row06, col10, RED);
	SET(k, row06, col10, RED);
	SET(k, row06, col17, RED);
	SET(k, row06, col17, RED);

	// row 8
	// k = 54
	SET(k, row06, col16, RED);
	SET(k, row06, col16, RED);
	SET(k, row06, col07, RED);
	SET(k, row06, col07, RED);
	SET(k, row07, col07, RED);
	SET(k, row07, col07, RED);
	SET(k, row07, col05, RED);
	SET(k, row07, col05, RED);

	SET(k, row07, col05, RED);
	SET(k, row07, col05, RED);
	SET(k, row07, col07, RED);
	SET(k, row07, col07, RED);
	SET(k, row06, col07, RED);
	SET(k, row06, col07, RED);
	SET(k, row06, col16, RED);
	SET(k, row06, col16, RED);

	// row 7
	// k = 70
	SET(k, row06, col11, RED);
	SET(k, row06, col11, RED);
	SET(k, row06, col08, RED);
	SET(k, row06, col08, RED);
	SET(k, row06, col06, RED);
	SET(k, row06, col06, RED);
	SET(k, row07, col06, RED);
	SET(k, row07, col06, RED);

	SET(k, row07, col06, RED);
	SET(k, row07, col06, RED);
	SET(k, row06, col06, RED);
	SET(k, row06, col06, RED);
	SET(k, row06, col08, RED);
	SET(k, row06, col08, RED);
	SET(k, row06, col11, RED);
	SET(k, row06, col11, RED);

	// row 7 (sharp, bottom)
	// k = 86
	SET(k, row06, col14, RED);
	SET(k, row06, col14, RED);
	SET(k, row06, col15, RED);
	SET(k, row06, col15, RED);
	SET(k, row06, col15, RED);
	SET(k, row06, col15, RED);
	SET(k, row06, col14, RED);
	SET(k, row06, col14, RED);

	// row 6 (sharp, top)
	// k = 94
	SET(k, row06, col12, RED);
	SET(k, row06, col12, RED);
	SET(k, row06, col13, RED);
	SET(k, row06, col13, RED);
	SET(k, row06, col13, RED);
	SET(k, row06, col13, RED);
	SET(k, row06, col12, RED);
	SET(k, row06, col12, RED);
	
	k += 8; // Skip 8 leds.

	// row 5
	// k = 110
	SET(k, row05, col01, RED);
	SET(k, row05, col01, RED);
	SET(k, row05, col02, RED);
	SET(k, row05, col02, RED);
	SET(k, row05, col03, RED);
	SET(k, row05, col03, RED);
	SET(k, row05, col04, RED);
	SET(k, row05, col04, RED);
	SET(k, row05, col05, RED);
	SET(k, row05, col05, RED);
	SET(k, row05, col06, RED);
	SET(k, row05, col06, RED);
	SET(k, row05, col07, RED);
	SET(k, row05, col07, RED);
	SET(k, row05, col08, RED);
	SET(k, row05, col08, RED);
	SET(k, row05, col09, GREEN);
	SET(k, row05, col09, GREEN);
	SET(k, row05, col10, RED);
	SET(k, row05, col10, RED);
	SET(k, row05, col11, RED);
	SET(k, row05, col11, RED);
	SET(k, row05, col12, RED);
	SET(k, row05, col12, RED);
	SET(k, row05, col13, RED);
	SET(k, row05, col13, RED);
	SET(k, row05, col14, RED);
	SET(k, row05, col14, RED);
	SET(k, row05, col15, RED);
	SET(k, row05, col15, RED);
	SET(k, row05, col16, RED);
	SET(k, row05, col16, RED);
	SET(k, row05, col17, RED);
	SET(k, row05, col17, RED);

	// k = 144
	SET(k, row05, col17, RED);
	SET(k, row05, col17, RED);
	SET(k, row05, col16, RED);
	SET(k, row05, col16, RED);
	SET(k, row05, col15, RED);
	SET(k, row05, col15, RED);
	SET(k, row05, col14, RED);
	SET(k, row05, col14, RED);
	SET(k, row05, col13, RED);
	SET(k, row05, col13, RED);
	SET(k, row05, col12, RED);
	SET(k, row05, col12, RED);
	SET(k, row05, col11, RED);
	SET(k, row05, col11, RED);
	SET(k, row05, col10, RED);
	SET(k, row05, col10, RED);
	SET(k, row05, col09, GREEN);
	SET(k, row05, col09, GREEN);
	SET(k, row05, col08, RED);
	SET(k, row05, col08, RED);
	SET(k, row05, col07, RED);
	SET(k, row05, col07, RED);
	SET(k, row05, col06, RED);
	SET(k, row05, col06, RED);
	SET(k, row05, col05, RED);
	SET(k, row05, col05, RED);
	SET(k, row05, col04, RED);
	SET(k, row05, col04, RED);
	SET(k, row05, col03, RED);
	SET(k, row05, col03, RED);
	SET(k, row05, col02, RED);
	SET(k, row05, col02, RED);
	SET(k, row05, col01, RED);
	SET(k, row05, col01, RED);

	// row 4
	SET(k, row04, col01, RED);
	SET(k, row04, col01, RED);
	SET(k, row04, col02, RED);
	SET(k, row04, col02, RED);
	SET(k, row04, col03, RED);
	SET(k, row04, col03, RED);
	SET(k, row04, col04, RED);
	SET(k, row04, col04, RED);
	SET(k, row04, col05, RED);
	SET(k, row04, col05, RED);
	SET(k, row04, col06, RED);
	SET(k, row04, col06, RED);
	SET(k, row04, col07, RED);
	SET(k, row04, col07, RED);
	SET(k, row04, col08, RED);
	SET(k, row04, col08, RED);
	SET(k, row04, col09, GREEN);
	SET(k, row04, col09, GREEN);
	SET(k, row04, col10, RED);
	SET(k, row04, col10, RED);
	SET(k, row04, col11, RED);
	SET(k, row04, col11, RED);
	SET(k, row04, col12, RED);
	SET(k, row04, col12, RED);
	SET(k, row04, col13, RED);
	SET(k, row04, col13, RED);
	SET(k, row04, col14, RED);
	SET(k, row04, col14, RED);
	SET(k, row04, col15, RED);
	SET(k, row04, col15, RED);
	SET(k, row04, col16, RED);
	SET(k, row04, col16, RED);
	SET(k, row04, col17, RED);
	SET(k, row04, col17, RED);

	SET(k, row04, col17, RED);
	SET(k, row04, col17, RED);
	SET(k, row04, col16, RED);
	SET(k, row04, col16, RED);
	SET(k, row04, col15, RED);
	SET(k, row04, col15, RED);
	SET(k, row04, col14, RED);
	SET(k, row04, col14, RED);
	SET(k, row04, col13, RED);
	SET(k, row04, col13, RED);
	SET(k, row04, col12, RED);
	SET(k, row04, col12, RED);
	SET(k, row04, col11, RED);
	SET(k, row04, col11, RED);
	SET(k, row04, col10, RED);
	SET(k, row04, col10, RED);
	SET(k, row04, col09, GREEN);
	SET(k, row04, col09, GREEN);
	SET(k, row04, col08, RED);
	SET(k, row04, col08, RED);
	SET(k, row04, col07, RED);
	SET(k, row04, col07, RED);
	SET(k, row04, col06, RED);
	SET(k, row04, col06, RED);
	SET(k, row04, col05, RED);
	SET(k, row04, col05, RED);
	SET(k, row04, col04, RED);
	SET(k, row04, col04, RED);
	SET(k, row04, col03, RED);
	SET(k, row04, col03, RED);
	SET(k, row04, col02, RED);
	SET(k, row04, col02, RED);
	SET(k, row04, col01, RED);
	SET(k, row04, col01, RED);

	// row 3
	SET(k, row03, col01, GREEN);
	SET(k, row03, col01, GREEN);
	SET(k, row03, col02, GREEN);
	SET(k, row03, col02, GREEN);
	SET(k, row03, col03, GREEN);
	SET(k, row03, col03, GREEN);
	SET(k, row03, col04, GREEN);
	SET(k, row03, col04, GREEN);
	SET(k, row03, col05, GREEN);
	SET(k, row03, col05, GREEN);
	SET(k, row03, col06, GREEN);
	SET(k, row03, col06, GREEN);
	SET(k, row03, col07, GREEN);
	SET(k, row03, col07, GREEN);
	SET(k, row03, col08, GREEN);
	SET(k, row03, col08, GREEN);
	SET(k, row03, col09, GREEN);
	SET(k, row03, col09, GREEN);
	SET(k, row03, col10, GREEN);
	SET(k, row03, col10, GREEN);
	SET(k, row03, col11, GREEN);
	SET(k, row03, col11, GREEN);
	SET(k, row03, col12, GREEN);
	SET(k, row03, col12, GREEN);
	SET(k, row03, col13, GREEN);
	SET(k, row03, col13, GREEN);
	SET(k, row03, col14, GREEN);
	SET(k, row03, col14, GREEN);
	SET(k, row03, col15, GREEN);
	SET(k, row03, col15, GREEN);
	SET(k, row03, col16, GREEN);
	SET(k, row03, col16, GREEN);
	SET(k, row03, col17, GREEN);
	SET(k, row03, col17, GREEN);

	SET(k, row03, col17, GREEN);
	SET(k, row03, col17, GREEN);
	SET(k, row03, col16, GREEN);
	SET(k, row03, col16, GREEN);
	SET(k, row03, col15, GREEN);
	SET(k, row03, col15, GREEN);
	SET(k, row03, col14, GREEN);
	SET(k, row03, col14, GREEN);
	SET(k, row03, col13, GREEN);
	SET(k, row03, col13, GREEN);
	SET(k, row03, col12, GREEN);
	SET(k, row03, col12, GREEN);
	SET(k, row03, col11, GREEN);
	SET(k, row03, col11, GREEN);
	SET(k, row03, col10, GREEN);
	SET(k, row03, col10, GREEN);
	SET(k, row03, col09, GREEN);
	SET(k, row03, col09, GREEN);
	SET(k, row03, col08, GREEN);
	SET(k, row03, col08, GREEN);
	SET(k, row03, col07, GREEN);
	SET(k, row03, col07, GREEN);
	SET(k, row03, col06, GREEN);
	SET(k, row03, col06, GREEN);
	SET(k, row03, col05, GREEN);
	SET(k, row03, col05, GREEN);
	SET(k, row03, col04, GREEN);
	SET(k, row03, col04, GREEN);
	SET(k, row03, col03, GREEN);
	SET(k, row03, col03, GREEN);
	SET(k, row03, col02, GREEN);
	SET(k, row03, col02, GREEN);
	SET(k, row03, col01, GREEN);
	SET(k, row03, col01, GREEN);

	// row 2
	SET(k, row02, col01, RED);
	SET(k, row02, col01, RED);
	SET(k, row02, col02, RED);
	SET(k, row02, col02, RED);
	SET(k, row02, col03, RED);
	SET(k, row02, col03, RED);
	SET(k, row02, col04, RED);
	SET(k, row02, col04, RED);
	SET(k, row02, col05, RED);
	SET(k, row02, col05, RED);
	SET(k, row02, col06, RED);
	SET(k, row02, col06, RED);
	SET(k, row02, col07, RED);
	SET(k, row02, col07, RED);
	SET(k, row02, col08, RED);
	SET(k, row02, col08, RED);
	SET(k, row02, col09, GREEN);
	SET(k, row02, col09, GREEN);
	SET(k, row02, col10, RED);
	SET(k, row02, col10, RED);
	SET(k, row02, col11, RED);
	SET(k, row02, col11, RED);
	SET(k, row02, col12, RED);
	SET(k, row02, col12, RED);
	SET(k, row02, col13, RED);
	SET(k, row02, col13, RED);
	SET(k, row02, col14, RED);
	SET(k, row02, col14, RED);
	SET(k, row02, col15, RED);
	SET(k, row02, col15, RED);
	SET(k, row02, col16, RED);
	SET(k, row02, col16, RED);
	SET(k, row02, col17, RED);
	SET(k, row02, col17, RED);

	SET(k, row02, col17, RED);
	SET(k, row02, col17, RED);
	SET(k, row02, col16, RED);
	SET(k, row02, col16, RED);
	SET(k, row02, col15, RED);
	SET(k, row02, col15, RED);
	SET(k, row02, col14, RED);
	SET(k, row02, col14, RED);
	SET(k, row02, col13, RED);
	SET(k, row02, col13, RED);
	SET(k, row02, col12, RED);
	SET(k, row02, col12, RED);
	SET(k, row02, col11, RED);
	SET(k, row02, col11, RED);
	SET(k, row02, col10, RED);
	SET(k, row02, col10, RED);
	SET(k, row02, col09, GREEN);
	SET(k, row02, col09, GREEN);
	SET(k, row02, col08, RED);
	SET(k, row02, col08, RED);
	SET(k, row02, col07, RED);
	SET(k, row02, col07, RED);
	SET(k, row02, col06, RED);
	SET(k, row02, col06, RED);
	SET(k, row02, col05, RED);
	SET(k, row02, col05, RED);
	SET(k, row02, col04, RED);
	SET(k, row02, col04, RED);
	SET(k, row02, col03, RED);
	SET(k, row02, col03, RED);
	SET(k, row02, col02, RED);
	SET(k, row02, col02, RED);
	SET(k, row02, col01, RED);
	SET(k, row02, col01, RED);

	// row 1
	SET(k, row01, col01, RED);
	SET(k, row01, col01, RED);
	SET(k, row01, col02, RED);
	SET(k, row01, col02, RED);
	SET(k, row01, col03, RED);
	SET(k, row01, col03, RED);
	SET(k, row01, col04, RED);
	SET(k, row01, col04, RED);
	SET(k, row01, col05, RED);
	SET(k, row01, col05, RED);
	SET(k, row01, col06, RED);
	SET(k, row01, col06, RED);
	SET(k, row01, col07, RED);
	SET(k, row01, col07, RED);
	SET(k, row01, col08, RED);
	SET(k, row01, col08, RED);
	SET(k, row01, col09, GREEN);
	SET(k, row01, col09, GREEN);
	SET(k, row01, col10, RED);
	SET(k, row01, col10, RED);
	SET(k, row01, col11, RED);
	SET(k, row01, col11, RED);
	SET(k, row01, col12, RED);
	SET(k, row01, col12, RED);
	SET(k, row01, col13, RED);
	SET(k, row01, col13, RED);
	SET(k, row01, col14, RED);
	SET(k, row01, col14, RED);
	SET(k, row01, col15, RED);
	SET(k, row01, col15, RED);
	SET(k, row01, col16, RED);
	SET(k, row01, col16, RED);
	SET(k, row01, col17, RED);
	SET(k, row01, col17, RED);

	SET(k, row01, col17, RED);
	SET(k, row01, col17, RED);
	SET(k, row01, col16, RED);
	SET(k, row01, col16, RED);
	SET(k, row01, col15, RED);
	SET(k, row01, col15, RED);
	SET(k, row01, col14, RED);
	SET(k, row01, col14, RED);
	SET(k, row01, col13, RED);
	SET(k, row01, col13, RED);
	SET(k, row01, col12, RED);
	SET(k, row01, col12, RED);
	SET(k, row01, col11, RED);
	SET(k, row01, col11, RED);
	SET(k, row01, col10, RED);
	SET(k, row01, col10, RED);
	SET(k, row01, col09, GREEN);
	SET(k, row01, col09, GREEN);
	SET(k, row01, col08, RED);
	SET(k, row01, col08, RED);
	SET(k, row01, col07, RED);
	SET(k, row01, col07, RED);
	SET(k, row01, col06, RED);
	SET(k, row01, col06, RED);
	SET(k, row01, col05, RED);
	SET(k, row01, col05, RED);
	SET(k, row01, col04, RED);
	SET(k, row01, col04, RED);
	SET(k, row01, col03, RED);
	SET(k, row01, col03, RED);
	SET(k, row01, col02, RED);
	SET(k, row01, col02, RED);
	SET(k, row01, col01, RED);
	SET(k, row01, col01, RED);
}

void display_reset(void) {
	uint16_t i;
	
	for (i = 0; i < LED_COUNT; i++) {
		LEDS[i] = 0;
	}

	leds_render();
}

void display_render(void) {
	leds_render();
}
