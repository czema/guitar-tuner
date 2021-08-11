/*

# Guitar Tuner
Copyright 2021 Chet Zema

*/

#ifndef DISPLAY_DOT_H
#define DISPLAY_DOT_H

#include <stdint.h>

void display_update(uint8_t, uint8_t, uint8_t, uint8_t);
void display_clear(void);
void display_render(void);

#endif
