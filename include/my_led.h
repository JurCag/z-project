#ifndef _MY_LED_H
#define _MY_LED_H

#include <zephyr/shell/shell.h>

int my_led_init(void);
int my_led_led_on(const struct shell *sh, size_t argc, char **argv);
int my_led_led_off(const struct shell *sh, size_t argc, char **argv);
int my_led_led_toggle(const struct shell *sh, size_t argc, char **argv);

#endif