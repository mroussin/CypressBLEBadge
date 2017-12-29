/*******************************************************************************
* File Name: Capsense_INT.c
* Version 2.60
*
* Description:
*  This file provides the source code of the  Interrupt Service Routine (ISR)
*  for the CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Capsense.h"
#include "Capsense_PVT.h"
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and code here
********************************************************************************/
/* `#START Capsense_ISR_INTC` */

/* `#END` */


/*******************************************************************************
* Function Name: Capsense_ISR
********************************************************************************
*
* Summary:
*  This ISR is executed when the measure window is closed. The measure window
*  depends on the sensor scan resolution parameter.
*  The ISR has two modes:
*   1. Scans a single sensor and stores the measure result.
*   2. Stores the result of the current measure and starts the next scan till all
*      the enabled sensors are scanned.
*  This interrupt handler is only used in one channel designs.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_csv - used to provide the status and mode of the scanning process.
*   Mode - a single scan or scan of all the enabled sensors.
*   Status - scan is in progress or ready for new scan.
*  Capsense_sensorIndex - used to store a sensor scanning sensor number.
*
* Reentrant:
*  No
*
*******************************************************************************/
CY_ISR(Capsense_ISR)
{
    static uint8 Capsense_snsIndexTmp;

    #ifdef Capsense_ISR_ENTRY_CALLBACK
        Capsense_ISR_EntryCallback();
    #endif /* Capsense_ISR_ENTRY_CALLBACK */

    /*  Place your Interrupt code here. */
    /* `#START Capsense_ISR_ENTER` */

    /* `#END` */

    CyIntDisable(Capsense_ISR_NUMBER);

    Capsense_CSD_INTR_REG = 1u;

    Capsense_PostScan((uint32)Capsense_sensorIndex);

    if ((Capsense_csdStatusVar & Capsense_SW_CTRL_SINGLE_SCAN) != 0u)
    {
        Capsense_csdStatusVar &= (uint8)~Capsense_SW_STS_BUSY;
    }
    else
    {
        /* Proceed scanning */
        Capsense_sensorIndex = Capsense_FindNextSensor(Capsense_sensorIndex);

        /* Check end of scan */
        if(Capsense_sensorIndex < Capsense_TOTAL_SENSOR_COUNT)
        {
            Capsense_PreScan((uint32)Capsense_sensorIndex);
        }
        else
        {
            Capsense_csdStatusVar &= (uint8)~Capsense_SW_STS_BUSY;

             /* Check if widget has been scanned */
            if((Capsense_csdStatusVar & Capsense_SW_CTRL_WIDGET_SCAN) != 0u)
            {
                /* Restore sensorEnableMask array */
                for(Capsense_snsIndexTmp = 0u;
                    Capsense_snsIndexTmp < Capsense_TOTAL_SENSOR_MASK;
                    Capsense_snsIndexTmp++)
                {
                    /* Backup sensorEnableMask array */
                    Capsense_sensorEnableMask[Capsense_snsIndexTmp] = Capsense_sensorEnableMaskBackup[Capsense_snsIndexTmp];
                }
            }
        }
    }
    CyIntEnable(Capsense_ISR_NUMBER);

    /*  Place your Interrupt code here. */
    /* `#START Capsense_ISR_EXIT` */

    /* `#END` */

    #ifdef Capsense_ISR_EXIT_CALLBACK
        Capsense_ISR_ExitCallback();
    #endif /* Capsense_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
