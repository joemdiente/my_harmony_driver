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
    /* clock speed */
    uint32_t clockSpeed;
} DRV_APA102_TRANSFER_SETUP;

/* Configuration */
typedef struct
{
    uint32_t ledCnt;   
    DRV_APA102_TRANSFER_SETUP transferParams;
    DRV_APA102_MODE drvOper;
}DRV_APA102_CONFIG_PARAMS;

/* RGB Containers */
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} DRV_APA102_RGB_ARRAY;
typedef struct
{
    uint8_t* ledIndex; //Unused
    DRV_APA102_RGB_ARRAY rgbArray;
} DRV_APA102_RGB_DATA;

// function prototype of application callback function
typedef void (*DRV_APA102_APP_CALLBACK)(DRV_APA102_EVENT event, uintptr_t context);

// function prototype of callback registered by driver with the PLIB
typedef void (* DRV_APA102_PLIB_CALLBACK)( uintptr_t context);

// function prototypes of PLIB APIs
typedef bool (* DRV_APA102_PLIB_WRITE)( uint16_t, uint8_t *, uint32_t );
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
