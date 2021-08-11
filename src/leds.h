/*

# Guitar Tuner
Copyright 2021 Chet Zema

*/

#ifndef LEDS_DOT_H
#define LEDS_DOT_H

#include <stdint.h>

int leds_init(void);
int leds_render(uint32_t *data, uint32_t len);
void leds_shutdown(void);

#endif
