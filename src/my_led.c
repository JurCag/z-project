#include "../include/my_led.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)

static void toggle_led(struct k_work *work);
static int set_led_state(int state);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static struct k_work_delayable led_work;
static struct k_work_sync led_sync;

static int led_state = 0;
static int led_period = 1000;

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

int my_led_on(void)
{
    return set_led_state(1);
}

int my_led_off(void)
{

    return set_led_state(0);
}

int my_led_toggle(int ms)
{
    led_period = ms;
    if (led_period <= 0)
    {
        return 1;
    }
    
    k_work_reschedule(&led_work, K_NO_WAIT);
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

static int set_led_state(int state)
{
    k_work_cancel_delayable_sync(&led_work, &led_sync);

    gpio_pin_set_dt(&led, state);
    led_state = state;
    return 0;
}
