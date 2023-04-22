/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"
#include "driver/apa102/drv_apa102.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
const DRV_APA102_PLIB_INTERFACE gDrvAPA102PLIBIntf[1] = 
{
    {
        .write = (DRV_APA102_PLIB_WRITE)FLEXCOM7_SPI_Write,
        .transferSetup = (DRV_APA102_PLIB_TRANSFER_SETUP)FLEXCOM7_SPI_TransferSetup,
        .callbackRegister = (DRV_APA102_PLIB_CALLBACK_REGISTER)FLEXCOM7_SPI_CallbackRegister,
        .IsBusy = (DRV_APA102_PLIB_ISBUSY)FLEXCOM7_SPI_IsBusy,
    },
};

const DRV_APA102_INIT gDrvAPA102InitObj[1] = 
{
    {
        .plibInterface = &gDrvAPA102PLIBIntf[0],
    },
};
/* SPI PLIB Interface Initialization for AT25DF Driver */
const DRV_AT25DF_PLIB_INTERFACE drvAT25DFPlibAPI = {

    /* SPI PLIB WriteRead function */
    .writeRead = (DRV_AT25DF_PLIB_WRITE_READ)FLEXCOM9_SPI_WriteRead,

    /* SPI PLIB Write function */
    .write = (DRV_AT25DF_PLIB_WRITE)FLEXCOM9_SPI_Write,
    
    /* SPI PLIB Read function */
    .read = (DRV_AT25DF_PLIB_READ)FLEXCOM9_SPI_Read,

    /* SPI PLIB Transfer Status function */
    .isBusy = (DRV_AT25DF_PLIB_IS_BUSY)FLEXCOM9_SPI_IsBusy,
    
    /* SPI PLIB Callback Register */
    .callbackRegister = (DRV_AT25DF_PLIB_CALLBACK_REGISTER)FLEXCOM9_SPI_CallbackRegister,
};
    
/* AT25DF Driver Initialization Data */
const DRV_AT25DF_INIT drvAT25DFInitData =
{
    /* SPI PLIB API  interface*/
    .spiPlib = &drvAT25DFPlibAPI,
                
    /* AT25DF Number of clients */
    .numClients = DRV_AT25DF_CLIENTS_NUMBER_IDX,

    /* FLASH Page Size in bytes */
    .pageSize = DRV_AT25DF_PAGE_SIZE,
    
    /* Total size of the FLASH in bytes */
    .flashSize = DRV_AT25DF_FLASH_SIZE,
    
    .blockStartAddress = 0x0,
    
    .chipSelectPin = DRV_AT25DF_CHIP_SELECT_PIN_IDX
};



// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)TC0_CH0_TimerCallbackRegister,
    .timerStart = (SYS_TIME_PLIB_START)TC0_CH0_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP)TC0_CH0_TimerStop ,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)TC0_CH0_TimerFrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)TC0_CH0_TimerPeriodSet,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET)TC0_CH0_TimerCompareSet,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET)TC0_CH0_TimerCounterGet,
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = TC0_CH0_IRQn,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void STDIO_BufferModeSet ( void )

  Summary:
    Sets the buffering mode for stdin and stdout

  Remarks:
 ********************************************************************************/
static void STDIO_BufferModeSet(void)
{
    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 21.6 deviated 2 times in this file.  Deviation record ID -  H3_MISRAC_2012_R_21_6_DR_3 */

    /* Make stdin unbuffered */
    setbuf(stdin, NULL);

    /* Make stdout unbuffered */
    setbuf(stdout, NULL);
}




/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */

    STDIO_BufferModeSet();
  

  
    DWDT_Initialize();
    CLK_Initialize();
	GIC_Initialize();
    MMU_Initialize();
    Matrix_Initialize();

    PIO_Initialize();



    FLEXCOM7_SPI_Initialize();

    FLEXCOM9_SPI_Initialize();

    FLEXCOM3_USART_Initialize();


    TC0_CH0_TimerInitialize(); 



    sysObj.drvAT25DF = DRV_AT25DF_Initialize(DRV_AT25DF_INDEX, (SYS_MODULE_INIT *)&drvAT25DFInitData);

    DRV_APA102_Initialize(0, (SYS_MODULE_INIT *) &gDrvAPA102InitObj[0]);
    
    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);


    APP_Initialize();



    /* MISRAC 2012 deviation block end */
}


/*******************************************************************************
 End of File
*/
