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
//Driver Parameters
DRV_APA102_CONFIG_PARAMS confParam;
DRV_APA102_LED_FRAME* rgbData;
DRV_APA102_INSTRUCTION_CONFIG confInstr;

//Instances
#define DRV_APA102_INSTANCE_0          0

volatile uint8_t drvStatus;
char* drvEvents[4] = {"NONE", "RUNNING", "PENDING", "STOPPED"};

volatile bool WriteLEDError = true;

/* Main Menu + Options */
const uint8_t main_menu[] = {
    "*** APA102 RPi HAT Single Client Demonstration ***\r\n"
    "Connect APA102 RPi HAT in RPi Connector \r\n"
    "\r\n"
    "1: Fading LEDs\r\n"
    "2: Blink LEDs \r\n"
    "r: Run Demo\r\n"
    "s: Stop Demo\r\n"
    "0: Manual Update LEDs\r\n"
    "Press any key to clear screen and print menu\r\n"
};

typedef enum {
    CMD_LED_DEMO_FADE = '1',
    CMD_LED_DEMO_BLINK = '2',
    CMD_LED_INSTRUCTIONS_RUN = 'r',
    CMD_LED_INSTRUCTIONS_STOP = 's',
    CMD_LED_INSTRUCTION_MANUAL_UPDATE = '0',
    CMD_LED_CONFIG = 'c',
} CMD;
char cmd;

void drvAPA102EventHandler(DRV_APA102_EVENT event, uintptr_t context) {

    switch (event) {
        case DRV_APA102_EVENT_NONE:
            drvStatus = DRV_APA102_EVENT_NONE;
            break;
        case DRV_APA102_EVENT_RUNNING:
            drvStatus = DRV_APA102_EVENT_RUNNING;
            break;
        case DRV_APA102_EVENT_PENDING:
            drvStatus = DRV_APA102_EVENT_PENDING;
            break;
        case DRV_APA102_EVENT_STOPPED:
            drvStatus = DRV_APA102_EVENT_STOPPED;
            break;
        default:
            break;
    }
}

/*
 * Demo App LED Fade
 */
void demoLEDFade(void) {
    printf("\33[H\33[2J");
    printf("This sets up instructions for fading LED Demo\r\n");

    uint8_t i, x, y;

    /* Setup Instruction Running Parameters */
    confInstr.instrCnt = 31 * 2;
    confInstr.instrFreq = REPEAT;
    DRV_APA102_Instruction_OperSetup(rgbHandle, &confInstr);

    /* Fill-up Instruction List for LED Fade Demo */
    //Incrementing Brightness
    y = 0;
    for (i = 0; i < 31; i++) {
        rgbData = DRV_APA102_GetLedFramePtrAtIndex(rgbHandle, i);
        if (rgbData == NULL) {
            printf("ERROR: LED Frame Pointer Invalid\r\n");
            break;
        }

        for (x = 0; x < confParam.ledMaxCnt; x++) {
            rgbData[x].ledBrightness = y;
            rgbData[x].red = 255;
            rgbData[x].green = 255;
            rgbData[x].blue = 255;
        }
        y++;
    }
    //Decrementing Brightness
    y = 31;
    for (i = 31; i < 62; i++) {
        rgbData = DRV_APA102_GetLedFramePtrAtIndex(rgbHandle, i);
        for (x = 0; x < confParam.ledMaxCnt; x++) {
            rgbData[x].ledBrightness = y;
            rgbData[x].red = 255;
            rgbData[x].green = 255;
            rgbData[x].blue = 255;
        }
        y--;
    }

    printf("Press start demo using 's' from main menu\r\n");
    printf("Press any key to continue\r\n");
    scanf("%c", &cmd);
}
/*
 * Demo App Blink
 */
void demoLEDBlink(void) {

    printf("\33[H\33[2J");
    printf("This sets up instructions for Blinking LEDs per second Demo\r\n");

    uint8_t i, x;

    /* Setup Instruction Operation */
    confInstr.instrCnt = 2;
    confInstr.instrFreq = REPEAT;
    DRV_APA102_Instruction_OperSetup(rgbHandle, &confInstr);

    /* Fill-up Instruction List for LED Blink Demo */
    for (i = 0; i < confInstr.instrCnt; i++) {
        rgbData = DRV_APA102_GetLedFramePtrAtIndex(rgbHandle, x);
        if (rgbData == NULL) {
            printf("ERROR: LED Frame Pointer Invalid\r\n");
            break;
        }

        for (x = 0; x < confParam.ledMaxCnt; x++) {
            rgbData[x].red = 255;
            rgbData[x].green = 255;
            rgbData[x].blue = 255;
            if (i == 0) rgbData[x].ledBrightness = 0;
            else if (i == 1) rgbData[x].ledBrightness = 31;
        }
    }

    /* Setup Instruction Parameters */
    printf("Press start demo using 's' from main menu\r\n");
    printf("Press any key to continue\r\n");
    scanf("%c", &cmd);
}
bool configLED(DRV_APA102_LED_FRAME* userData);

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);

    rgbHandle = DRV_APA102_Open(DRV_APA102_INSTANCE_0, 0);
    DRV_APA102_EventHandlerSet(rgbHandle, drvAPA102EventHandler, 0);

    confParam.instrMaxCnt = 255;
    confParam.ledMaxCnt = 3;
    confParam.transferParams.clkFreq = 1000000;

    if (DRV_APA102_ConfigSet(rgbHandle, &confParam) != true) {
        printf("Driver Configuration Setup Failed\r\n");
        DRV_APA102_Close(rgbHandle);
    };

    if (DRV_APA102_AllocateBuffer(rgbHandle) != true) {
        printf("Buffer Allocation Failed\r\n");
        DRV_APA102_Close(rgbHandle);
    }

    DRV_APA102_Instruction_InitializeLED(rgbHandle);

    while (true) {
        printf("\33[H\33[2J");
        printf("%s", main_menu);
        printf("DRV_STATUS: ");
        printf("%s\r\n", drvEvents[drvStatus]);

        //User Input
        scanf("%c", &cmd);
        switch (cmd) {
            case CMD_LED_DEMO_FADE:
                demoLEDFade();
                break;

            case CMD_LED_DEMO_BLINK:
                demoLEDBlink();
                break;

            case CMD_LED_INSTRUCTIONS_RUN:
                DRV_APA102_Instruction_Start(rgbHandle);
                break;

            case CMD_LED_INSTRUCTIONS_STOP:
                DRV_APA102_Instruction_Stop(rgbHandle);
                DRV_APA102_Instruction_InitializeLED(rgbHandle);
                break;

            case CMD_LED_INSTRUCTION_MANUAL_UPDATE:
                DRV_APA102_Instruction_ManualUpdate(rgbHandle);
                break;

            case CMD_LED_CONFIG:
                printf("\33[H\33[2J");
                printf("*** Change LED Values ***\r\n");
                configLED(rgbData);
                printf("LED Config Done. Press Any Key to Continue\r\n");
                scanf("%c", &cmd);
                break;

            default:
                break;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE);
}
/*
 * Demo APIs
 * 
 */

/*
 * App Configuration
 */
bool configLED(DRV_APA102_LED_FRAME* userData) {
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
    for (i = 0; i <= 2; i++) {
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

