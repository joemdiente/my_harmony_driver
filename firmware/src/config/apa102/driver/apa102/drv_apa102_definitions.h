/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _DRV_APA102_DEFINITIONS_H    /* Guard against multiple inclusion */
#define _DRV_APA102_DEFINITIONS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "stddef.h"
#include "driver/driver.h"
#include <stdint.h>
#include <stdbool.h>

/* For SPI PLIB*/
typedef enum
{
    DRV_APA102_CLOCK_PHASE_VALID_TRAILING_EDGE = 0,
    DRV_APA102_CLOCK_PHASE_VALID_LEADING_EDGE = 1,

    /* Force the compiler to reserve 32-bit memory space for each enum */
    DRV_APA102_CLOCK_PHASE_INVALID = 0xFFFFFFFF

} DRV_APA102_CLOCK_PHASE;

typedef enum
{
    DRV_APA102_CLOCK_POLARITY_IDLE_LOW = 0,
    DRV_APA102_CLOCK_POLARITY_IDLE_HIGH = 1,

    /* Force the compiler to reserve 32-bit memory space for each enum */
    DRV_APA102_CLOCK_POLARITY_INVALID = 0xFFFFFFFF

} DRV_APA102_CLOCK_POLARITY;

typedef enum
{
    DRV_APA102_DATA_BITS_8 = 0,
    DRV_APA102_DATA_BITS_9 = 1,
    DRV_APA102_DATA_BITS_10 = 2,
    DRV_APA102_DATA_BITS_11 = 3,
    DRV_APA102_DATA_BITS_12 = 4,
    DRV_APA102_DATA_BITS_13 = 5,
    DRV_APA102_DATA_BITS_14 = 6,
    DRV_APA102_DATA_BITS_15 = 7,
    DRV_APA102_DATA_BITS_16 = 8,
	DRV_APA102_DATA_BITS_32 = 9,

    /* Force the compiler to reserve 32-bit memory space for each enum */
    DRV_APA102_DATA_BITS_INVALID = 0xFFFFFFFF

} DRV_APA102_DATA_BITS;

typedef enum
{
    DRV_APA102_CS_POLARITY_ACTIVE_LOW = 0,
    DRV_APA102_CS_POLARITY_ACTIVE_HIGH = 1

} DRV_APA102_CS_POLARITY;
typedef enum
{
    DRV_APA102_ERROR_NONE = 0,
    DRV_APA102_ERROR_BUS,
} DRV_APA102_ERROR;

typedef enum
{
    DRV_APA102_EVENT_WRITE_DONE = 0,
    DRV_APA102_EVENT_ERROR,
} DRV_APA102_EVENT;

typedef enum
{
    DRV_APA102_LED_STRIP = 0,
    DRV_APA102_LED_ARRAY,
} DRV_APA102_MODE;

/* SPI Configuration 
 * APA102-2020 Data Sheet
 * FOSC 800KHz - 1.2MHz
 */
typedef struct
{
    uint32_t    clkFreq;
    DRV_APA102_CLOCK_PHASE clkPhase;
    DRV_APA102_CS_POLARITY clkPol;
    DRV_APA102_DATA_BITS   dataBits;
} DRV_APA102_TRANSFER_SETUP;

/* Configuration */
typedef struct
{
    uint32_t ledCnt;   
    DRV_APA102_TRANSFER_SETUP transferParams;
    DRV_APA102_MODE drvOper;
}DRV_APA102_CONFIG_PARAMS;

/* APA102 Protocol [32 bits/frame]
* Start Frame   LED Frame/s                     End Frame
*   [0*32]   -   [LED 1]   -   [LED n + 1]   -   [1*32]
* LED Frame
*   [111 - Global(5b) - Blue(8b) - Green(8b) - Red(8b)] 
* 
* ::Global is brightness setting
* Brightness   -   Driving Current
* 00000        -       0/31       
* 00001        -       1/31
* .....        -       ....
* 11111        -       31/31  
* ::Blue/Green/Red are LEDs
* Data         -   Duty Cycle
* 00000000     -     0/256
* 00000001     -     1/256
* ........     -     .....
* 11111111     -     255/256
*/
typedef struct __attribute__((__packed__))
{
    //Global - Forms as 8-bit and padBits in LSB
    uint8_t ledBrightness :5;
    uint8_t padBits       :3;
    //RGB
    uint8_t blue          :8;
    uint8_t green         :8;
    uint8_t red           :8; 
} DRV_APA102_LED_FRAMES;

/* Payload */
typedef struct __attribute__((__packed__))
{
    uint32_t startFrame         :32;
//    DRV_APA102_LED_FRAMES* ledFrames;
    uint32_t endFrame           :32;
} DRV_APA102_PAYLOAD;

// function prototype of application callback function
typedef void (*DRV_APA102_APP_CALLBACK)(DRV_APA102_EVENT event, uintptr_t context);

// function prototype of callback registered by driver with the PLIB
typedef void (* DRV_APA102_PLIB_CALLBACK)( uintptr_t context);

// function prototypes of PLIB APIs
typedef bool (* DRV_APA102_PLIB_WRITE)( uint8_t*, uint16_t);
typedef DRV_APA102_ERROR (* DRV_APA102_PLIB_ERROR_GET)( void );
typedef bool (* DRV_APA102_PLIB_TRANSFER_SETUP)(DRV_APA102_TRANSFER_SETUP* setup, uint32_t srcClkFreq);
typedef void (* DRV_APA102_PLIB_CALLBACK_REGISTER)(DRV_APA102_PLIB_CALLBACK, uintptr_t);

typedef struct
{
    DRV_APA102_PLIB_WRITE                          write;
    DRV_APA102_PLIB_ERROR_GET                      errorGet;
    DRV_APA102_PLIB_TRANSFER_SETUP                 transferSetup;
    DRV_APA102_PLIB_CALLBACK_REGISTER              callbackRegister;
} DRV_APA102_PLIB_INTERFACE;

typedef struct
{
    const DRV_APA102_PLIB_INTERFACE* plibInterface;
}DRV_APA102_INIT;

#endif /*  _DRV_APA102_DEFINITIONS_H */

/* *****************************************************************************
 End of File
 */
