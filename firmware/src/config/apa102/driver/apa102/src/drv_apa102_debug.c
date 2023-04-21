#include <stdio.h>
#include "driver/apa102/drv_apa102_debug_helper.h"

void DRV_APA102_DEBUG_FILL_RGBDATA(DRV_APA102_PAYLOAD_LED_FRAMES* led_frames, DRV_APA102_CONFIG_PARAMS confParam)
{
    uint8_t i = 0;
    
    printf("DEBUG: Entered %s\r\n", __func__);
    /* RGBDATA Filler */
    for(i = 0; i < confParam.ledCnt; i++)
    {
        led_frames[i].padBits = 0b111;
        led_frames[i].ledBrightness = 0b00000;
        led_frames[i].red = 'r';
        led_frames[i].green = 'g';
        led_frames[i].blue = 'b';
    }
}
void DRV_APA102_DEBUG_RGBDATA_PRINTER(DRV_APA102_PAYLOAD_LED_FRAMES* led_frames, DRV_APA102_CONFIG_PARAMS confParam)
{
    uint8_t i = 0;

    printf("DEBUG: Entered %s\r\n", __func__);
    /* RGB Data Printer */
    for(i = 0; i < confParam.ledCnt; i++)
    {
        printf("Index: %u, r:%u,g:%u,b:%u\r\n",      
            i,
            led_frames[i].red,
            led_frames[i].green,
            led_frames[i].blue
        );
    }
}