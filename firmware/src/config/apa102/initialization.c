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
        .write = (DRV_APA102_PLIB_SPI_WRITE)FLEXCOM7_SPI_Write,
        .transferSetup = (DRV_APA102_PLIB_SPI_TRANSFER_SETUP)FLEXCOM7_SPI_TransferSetup,
        .spiCallbackRegister = (DRV_APA102_PLIB_SPI_CALLBACK_REGISTER)FLEXCOM7_SPI_CallbackRegister,
        .IsBusy = (DRV_APA102_PLIB_SPI_ISBUSY)FLEXCOM7_SPI_IsBusy,
        .startTc = (DRV_APA102_PLIB_TC_START)TC0_CH0_TimerStart,
        .stopTc = (DRV_APA102_PLIB_TC_STOP)TC0_CH0_TimerStop,
        .tcCallbackRegister = (DRV_APA102_PLIB_TC_CALLBACK_REGISTER)TC0_CH0_TimerCallbackRegister,
    },
};

const DRV_APA102_INIT gDrvAPA102InitObj[1] = 
{
    {
        .plibInterface = &gDrvAPA102PLIBIntf[0],
    },
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

    FLEXCOM3_USART_Initialize();


    TC0_CH0_TimerInitialize(); 




    DRV_APA102_Initialize(0, (SYS_MODULE_INIT *) &gDrvAPA102InitObj[0]);
    

    APP_Initialize();



    /* MISRAC 2012 deviation block end */
}


/*******************************************************************************
 End of File
*/
