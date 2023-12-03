#include "../include/my_led.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdlib.h>

#define LED0_NODE DT_ALIAS(led0)

static void toggle_led(struct k_work *work);
static int set_led_state(const struct shell *sh, int state);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static struct k_work_delayable led_work;
static struct k_work_sync led_sync;

static int led_state = 0;
static int led_period = 1000;

SHELL_STATIC_SUBCMD_SET_CREATE(
    led_subcmds,
    SHELL_CMD_ARG(on, NULL, "Turn LED on.", &my_led_led_on, 1, 0),
    SHELL_CMD_ARG(off, NULL, "Turn LED off.", &my_led_led_off, 1, 0),
    SHELL_CMD_ARG(toggle, NULL, "Toggle LED with given period (ms).", &my_led_led_toggle, 1, 1),
    SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(led, &led_subcmds, "Led control commands", NULL);

int my_led_init(void)
{
    if (!gpio_is_ready_dt(&led))
    {
        return 1;
    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_LOW) < 0)
    {
        return 1;
    }

    k_work_init_delayable(&led_work, toggle_led);
    return 0;
}

int my_led_led_on(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    return set_led_state(sh, 1);
}

int my_led_led_off(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    return set_led_state(sh, 0);
}

int my_led_led_toggle(const struct shell *sh, size_t argc, char **argv)
{
    if (argc > 1)
    {
        led_period = atoi(argv[1]);
        if (led_period <= 0)
        {
            shell_print(sh, "Invalid period!");
            return -EINVAL;
        }
    }

    k_work_reschedule(&led_work, K_NO_WAIT);
    shell_print(sh, "Flashing LED with period %d ms", led_period);
    return 0;
}

static void toggle_led(struct k_work *work)
{
    ARG_UNUSED(work);

    if (led_state == 0)
    {
        gpio_pin_set_dt(&led, 1);
        led_state = 1;
    }
    else
    {
        gpio_pin_set_dt(&led, 0);
        led_state = 0;
    }

    k_work_reschedule(&led_work, K_MSEC(led_period));
}

static int set_led_state(const struct shell *sh, int state)
{
    k_work_cancel_delayable_sync(&led_work, &led_sync);

    gpio_pin_set_dt(&led, state);
    led_state = state;

    if (state == 1)
    {
        shell_print(sh, "Led ON");
    }
    else
    {
        shell_print(sh, "Led OFF");
    }

    return 0;
}
