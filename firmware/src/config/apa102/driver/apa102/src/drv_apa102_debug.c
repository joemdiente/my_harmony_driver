#include "driver/apa102/drv_apa102_debug_helper.h"

void DRV_APA102_DEBUG_RGBDATA_PRINTER(DRV_APA102_OBJ* obj)
{
    uint8_t i = 0;
    
    SYS_CONSOLE_MESSAGE("=========DEBUG========\r\n");
    /* RGB Data Printer */
    for(i = 0; i < obj->ledMaxCnt; i++)
    {
        SYS_CONSOLE_PRINT("Index: %u, r:%u,g:%u,b:%u\r\n",
            i,
            obj->rgbData[i].rgbArray.red,
            obj->rgbData[i].rgbArray.green,
            obj->rgbData[i].rgbArray.blue
        );
    }
    SYS_CONSOLE_MESSAGE("=====END_OF_DEBUG======\r\n");
}