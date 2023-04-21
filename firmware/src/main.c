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
const uint8_t config_menu[] = 
{
    "*** Change LED Values ***\r\n"
    "Format: x:y:r:g:b \r\n"
    "   Where: x = LED index \r\n"      
    "          y = Brightness (0-31) \r\n"    
    "          r = Red (1-255) \r\n"   
    "          g = Green  (1-255) \r\n"   
    "          b = Blue  (1-255) \r\n"   
    "ex: 1:30:255:255:255\r\n"
    "\r\n\r\n"
    "Config: "
};
typedef enum
{
    CMD_LED_DEMO_FADE = '1',
    CMD_LED_DEMO_BLINK = '2',
    CMD_LED_DEMO_RUN = '3',
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
int main ( void )
{    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    rgbHandle = DRV_APA102_Open(DRV_APA102_INSTANCE_0, 0);
    DRV_APA102_EventHandlerSet(rgbHandle, drvAPA102EventHandler, 0);
    
    confParam.ledCnt = 4;   
    confParam.transferParams.clkFreq = 1000000;
    confParam.drvOper = DRV_APA102_LED_STRIP;
    if(DRV_APA102_ConfigSet(rgbHandle,&confParam) != 0)
    {
        printf("Driver Configuration Setup Failed\r\n");
        DRV_APA102_Close(rgbHandle);
    };

    rgbData = DRV_APA102_AllocateBuffer(rgbHandle);
    DRV_APA102_InitializeLED(rgbHandle);

    while ( true )
    {
        printf ("\33[H\33[2J");
        printf("%s", main_menu);
        scanf("%c", &cmd);
        
        switch(cmd)
        {
            case CMD_LED_DEMO_FADE:
                printf ("\33[H\33[2J");
                printf("Fading LED Demo\r\n");
                demoLEDFade();
                printf("Press Any Key to Exit\r\n");
                scanf("%c", &cmd);
                
                break;

            case CMD_LED_DEMO_BLINK:
                printf ("\33[H\33[2J");
                printf("Blink LEDs every 1 Sec. Demo\r\n");
                printf("Press Any Key to Exit\r\n");

                scanf("%c", &cmd);
                break;

            case CMD_LED_DEMO_RUN:
                printf ("\33[H\33[2J");
                printf("Running LED Demo\r\n");
                printf("Press Any Key to Exit\r\n");
                scanf("%c", &cmd);
                break;
            case CMD_LED_CONFIG:
                
                printf("%s", config_menu);
                scanf("%c", &cmd);
                switch(cmd)
                {
                    
                    default:
                        break;
                }
                break;
            default:    
                break;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

