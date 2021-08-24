/*

# Guitar Tuner
Copyright 2021 Chet Zema

*/

#ifndef LEDS_DOT_H
#define LEDS_DOT_H

#include <stdint.h>

#include <ws2811/ws2811.h>

#define LED_COUNT 450
extern ws2811_led_t *leds[LED_COUNT];

int leds_init(void);
int leds_render();
void leds_shutdown(void);

#endif
