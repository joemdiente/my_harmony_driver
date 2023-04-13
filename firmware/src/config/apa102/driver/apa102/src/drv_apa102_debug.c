#include "driver/apa102/drv_apa102_debug_helper.h"

void DRV_APA102_DEBUG_FILL_RGBDATA(DRV_APA102_OBJ* obj)
{
    uint8_t i = 0;
    
    SYS_CONSOLE_MESSAGE("=========DEBUG========\r\n");
    SYS_CONSOLE_PRINT("%s\r\n",__func__);
    /* RGBDATA Filler */
    for(i = 0; i < obj->confParam.ledCnt; i++)
    {
        obj->rgbData[i].red = i+1;
        obj->rgbData[i].green = i+2;
        obj->rgbData[i].blue = i+3;
    }
    SYS_CONSOLE_MESSAGE("=====END_OF_DEBUG======\r\n");
}
void DRV_APA102_DEBUG_RGBDATA_PRINTER(DRV_APA102_OBJ* obj)
{
    uint8_t i = 0;
    
    SYS_CONSOLE_MESSAGE("=========DEBUG========\r\n");
    SYS_CONSOLE_PRINT("%s\r\n",__func__);
    /* RGB Data Printer */
    for(i = 0; i < obj->confParam.ledCnt; i++)
    {
        SYS_CONSOLE_PRINT("Index: %u, r:%u,g:%u,b:%u\r\n",
            i,
            obj->rgbData[i].red,
            obj->rgbData[i].green,
            obj->rgbData[i].blue
        );
    }
    SYS_CONSOLE_MESSAGE("=====END_OF_DEBUG======\r\n");
}