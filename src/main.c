#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "sdkconfig.h"

#define BLINK_GPIO 2


void led_blink(void *pvParameter)
{
    while(1) {
        /* Blink off (output low) */
	    //printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
	    //printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void serial_handle(void *pvParameter)
{
    while(1)
    {
        printf("Hello World\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void adc_read(void *pvParameter)
{
    while(1)
    {
        int adcVal = adc1_get_raw(ADC1_CHANNEL_0);
        printf("ADC READ: %d\n", adcVal);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main() {

    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */

    // LED PIN
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    // ADC PIN
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_11db);

    xTaskCreate(&serial_handle, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&led_blink, "led_blink", 2048, NULL, 5, NULL );
    xTaskCreate(&adc_read, "adc_read", 2048, NULL, 5, NULL );

}
