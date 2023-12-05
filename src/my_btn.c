#include "my_btn.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define SW0_NODE DT_ALIAS(userbtn)

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
static void button_debounce(struct k_work *work);

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;
static struct k_work_delayable button_work;

int my_btn_init(void)
{
    int ret;

    if (!gpio_is_ready_dt(&button))
    {
        printk("Error: button device %s is not ready\n", button.port->name);
        return 1;
    }
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret != 0)
    {
        printk("Error %d: failed to configure %s pin %d\n",
               ret, button.port->name, button.pin);
        return 1;
    }

    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_FALLING);
    if (ret != 0)
    {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
               ret, button.port->name, button.pin);
        return 1;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);

    k_work_init_delayable(&button_work, button_debounce);
    return 0;
}

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    k_work_reschedule(&button_work, K_MSEC(120));
}
static void button_debounce(struct k_work *work)
{
    ARG_UNUSED(work);

    printk("Button pressed\n");
}
