/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>
#include <string.h>                     // Defines EXIT_FAILURE
#include "definitions.h"
#include "driver/apa102/drv_apa102.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

//Driver Handle
DRV_HANDLE rgbHandle;
//Transfer Setup
DRV_APA102_CONFIG_PARAMS confParam;
DRV_APA102_PAYLOAD_LED_FRAME* rgbData;
//Instances
#define DRV_APA102_INSTANCE_0          0

volatile bool isWriteToLEDDone = false;
volatile bool WriteLEDError = true;
uint8_t wrBuffer[2];
uint8_t rdBuffer[2];

/* Main Menu + Options */
const uint8_t main_menu[] = 
{
    "*** APA102 RPi HAT Single Client Demonstration ***\r\n"
    "Connect APA102 RPi HAT in RPi Connector \r\n"
    "\r\n"
    "1: Fading LEDs\r\n"
    "2: Blink LEDs every 1 sec\r\n"
    "3: Running LEDs\r\n"  
    "c: LED Configuration\r\n"
    "Press any key to clear screen and print menu\r\n\r\n"
};
typedef enum
{
    CMD_LED_DEMO_FADE = '1',
    CMD_LED_DEMO_BLINK = '2',
    CMD_LED_DEMO_RUN = '3',
    CMD_LED_TEST = '0',
    CMD_LED_CONFIG = 'c',
}CMD;
char cmd;

void drvAPA102EventHandler(DRV_APA102_EVENT event, uintptr_t context)
{
    printf("EventHandler Called!\r\n");
    switch(event)
    {
        case DRV_APA102_EVENT_NONE:
            break;
        case DRV_APA102_EVENT_WRITE_DONE:
            isWriteToLEDDone = true;
            break;
        case DRV_APA102_EVENT_WRITE_ERROR:
            WriteLEDError = true;
            break;
    }
}
void demoLEDFade(void)
{
    
}
bool configLED(DRV_APA102_PAYLOAD_LED_FRAME* userData);

int main ( void )
{    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    rgbHandle = DRV_APA102_Open(DRV_APA102_INSTANCE_0, 0);
    DRV_APA102_EventHandlerSet(rgbHandle, drvAPA102EventHandler, 0);
    
    confParam.instructionCnt = 2;
    confParam.ledCnt = 3;   
    confParam.transferParams.clkFreq = 1000000;
    confParam.drvOper = DRV_APA102_LED_STRIP;
    if(DRV_APA102_ConfigSet(rgbHandle,&confParam) != true)
    {
        printf("Driver Configuration Setup Failed\r\n");
        DRV_APA102_Close(rgbHandle);
    };

    if(DRV_APA102_AllocateBuffer(rgbHandle) != true)
    {
        printf("Buffer Allocation Failed\r\n");
    }
    
    uint16_t i;
    uint8_t c = 0;
    for(i = 0; i < confParam.instructionCnt; i++)
    {
        rgbData = DRV_APA102_GetLedFramePtrAtIndex(rgbHandle, i);
        memset(rgbData, c++, sizeof(rgbData)*confParam.ledCnt);
    }
    
    DRV_APA102_InitializeLED(rgbHandle);
    
    while (true)
    {
//        printf ("\33[H\33[2J");
//        printf("%s", main_menu);
        scanf("%c", &cmd);
        
        switch(cmd)
        {
            case CMD_LED_DEMO_FADE:
                printf ("\33[H\33[2J");
                printf("Fading LED Demo\r\n");
                printf("Press Any Key to Exit\r\n");
                demoLEDFade();
                scanf("%c", &cmd);
                
                break;

            case CMD_LED_DEMO_BLINK:
                printf ("\33[H\33[2J");
                printf("Blink LEDs every 1 Sec. Demo\r\n");
                printf("Press Any Key to Stop\r\n");
//                demoLEDBlink();
                scanf("%c", &cmd);
                break;

            case CMD_LED_DEMO_RUN:
                printf ("\33[H\33[2J");
                printf("Running LED Demo\r\n");
                printf("Press Any Key to Stop\r\n");
//                demoLEDRun();
                scanf("%c", &cmd);
                break;
                
            case CMD_LED_TEST:
//                DRV_APA102_WriteLEDs(rgbHandle, rgbData);
                break;
                
            case CMD_LED_CONFIG:
                printf ("\33[H\33[2J");
                printf ("*** Change LED Values ***\r\n");
                configLED(rgbData);
                printf ("LED Config Done. Press Any Key to Continue\r\n");
                scanf("%c", &cmd);
                break;
                
            default:    
                break;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}
/*
 * Demo APIs
 * 
 */

/*
 * App Configuration
 */
bool configLED(DRV_APA102_PAYLOAD_LED_FRAME* userData)
{
//    uint8_t index;
//    uint32_t tmp;
//    
//    printf("Enter LED Index: ");
//    scanf("%u\r\n", (unsigned int*) &index);
//    
//    //Brightness 
//    printf("Enter LED %u Brightness: ",index);
//    scanf("%30u\r\n", (unsigned int*) &tmp);    
//    if(tmp < 0 || tmp > 31)
//    {
//        printf("ERROR: Brightness outside of range\r\n");
//        return false;
//    }
//    userData->ledBrightness = tmp;
//    
//    //Red
//    printf("Enter LED %u Red: ",index);
//    scanf("%255u\r\n", (unsigned int*) &tmp);  
//    if(tmp < 0 || tmp > 255)
//    {
//        printf("ERROR: red value is outside of range\r\n");
//        return false;
//    }
//    userData->red = tmp;
//            
//    //Green
//    printf("Enter LED %u Green: ",index);
//    scanf("%255u\r\n", (unsigned int*) &tmp);  
//    if(tmp < 0 || tmp > 255)
//    {
//        printf("ERROR: Green value is outside of range\r\n");
//        return false;
//    }
//    userData->green = tmp;
//    
//    //Blue
//    printf("Enter LED %u Blue: ",index);
//    scanf("%255u\r\nc", (unsigned int*) &tmp);  
//    if(tmp < 0 || tmp > 255)
//    {
//        printf("ERROR: Blue value is outside of range\r\n");
//        return false;
//    }
//    userData->blue = tmp;
    
    //Hardcoded for now
    uint8_t i = 0;
    for (i = 0; i <= 2; i++)
    {
        userData[i].ledBrightness = 30;
    }
    userData[0].red = 255;
    userData[1].green = 255;
    userData[2].blue = 255;
    
    return true;
}

/*******************************************************************************
 End of File
*/

