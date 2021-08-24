/*

# Guitar Tuner
Copyright 2021 Chet Zema

This component sends the virtual display to the WS2812 LED string.

*/

#include <stdio.h>

#include <ws2811/ws2811.h>

#define ARRAY_SIZE(stuff)       (sizeof(stuff) / sizeof(stuff[0]))

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18 // pin #12
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_RGB	// WS2812/SK6812RGB integrated chip+leds
#define LED_COUNT               450

int led_count = LED_COUNT;

// Configuration for the WS2812 library.
ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .count = LED_COUNT,
            .invert = 0,
            .brightness = 128,
            .strip_type = STRIP_TYPE,
        },
        [1] =
        {
            .gpionum = 0,
            .count = 0,
            .invert = 0,
            .brightness = 0,
        },
    },
};

int dotspos[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
ws2811_led_t dotcolors[] =
{
    0x00200000,  // red
    0x00201000,  // orange
    0x00202000,  // yellow
    0x00002000,  // green
    0x00002020,  // lightblue
    0x00000020,  // blue
    0x00100010,  // purple
    0x00200010,  // pink
};

int leds_init() {
    ws2811_return_t ret;

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "LEDS: ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }

    puts("LEDS: RPI_WS281X Initialized.");
}

void clear(void)
{
    int i;

    for (i = 0; i < led_count; i++) {
        ledstring.channel[0].leds[i] = 0;
    }
}

void leds_shutdown(void) {
    clear();
    ws2811_render(&ledstring);
    ws2811_fini(&ledstring);
}

int leds_render(uint32_t *data, uint32_t len)
{
    ws2811_return_t ret;
    int i;
    int min_count = led_count;

    if (len < led_count) min_count = len;

    for (i = 0; i < min_count; i++) {
        ledstring.channel[0].leds[i] = data[i];
    }

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "LEDS: ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        ws2811_fini(&ledstring);
        return ret;
    }
}
