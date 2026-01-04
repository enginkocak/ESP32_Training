#include "led.h"
#include "driver/gpio.h"

#define LED_GPIO GPIO_NUM_16

static bool s_led_on = false;

void led_init(void)
{
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io);
    led_set(false);
}

void led_set(bool on)
{
    s_led_on = on;
    gpio_set_level(LED_GPIO, on ? 1 : 0);
}

bool led_get(void)
{
    return s_led_on;
}
