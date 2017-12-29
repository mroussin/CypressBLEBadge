/*******************************************************************************
* File Name: Capesnse_INT.c
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

#include "Capesnse.h"
#include "Capesnse_PVT.h"
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and code here
********************************************************************************/
/* `#START Capesnse_ISR_INTC` */

/* `#END` */


/*******************************************************************************
* Function Name: Capesnse_ISR
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
*  Capesnse_csv - used to provide the status and mode of the scanning process.
*   Mode - a single scan or scan of all the enabled sensors.
*   Status - scan is in progress or ready for new scan.
*  Capesnse_sensorIndex - used to store a sensor scanning sensor number.
*
* Reentrant:
*  No
*
*******************************************************************************/
CY_ISR(Capesnse_ISR)
{
    static uint8 Capesnse_snsIndexTmp;

    #ifdef Capesnse_ISR_ENTRY_CALLBACK
        Capesnse_ISR_EntryCallback();
    #endif /* Capesnse_ISR_ENTRY_CALLBACK */

    /*  Place your Interrupt code here. */
    /* `#START Capesnse_ISR_ENTER` */

    /* `#END` */

    CyIntDisable(Capesnse_ISR_NUMBER);

    Capesnse_CSD_INTR_REG = 1u;

    Capesnse_PostScan((uint32)Capesnse_sensorIndex);

    if ((Capesnse_csdStatusVar & Capesnse_SW_CTRL_SINGLE_SCAN) != 0u)
    {
        Capesnse_csdStatusVar &= (uint8)~Capesnse_SW_STS_BUSY;
    }
    else
    {
        /* Proceed scanning */
        Capesnse_sensorIndex = Capesnse_FindNextSensor(Capesnse_sensorIndex);

        /* Check end of scan */
        if(Capesnse_sensorIndex < Capesnse_TOTAL_SENSOR_COUNT)
        {
            Capesnse_PreScan((uint32)Capesnse_sensorIndex);
        }
        else
        {
            Capesnse_csdStatusVar &= (uint8)~Capesnse_SW_STS_BUSY;

             /* Check if widget has been scanned */
            if((Capesnse_csdStatusVar & Capesnse_SW_CTRL_WIDGET_SCAN) != 0u)
            {
                /* Restore sensorEnableMask array */
                for(Capesnse_snsIndexTmp = 0u;
                    Capesnse_snsIndexTmp < Capesnse_TOTAL_SENSOR_MASK;
                    Capesnse_snsIndexTmp++)
                {
                    /* Backup sensorEnableMask array */
                    Capesnse_sensorEnableMask[Capesnse_snsIndexTmp] = Capesnse_sensorEnableMaskBackup[Capesnse_snsIndexTmp];
                }
            }
        }
    }
    CyIntEnable(Capesnse_ISR_NUMBER);

    /*  Place your Interrupt code here. */
    /* `#START Capesnse_ISR_EXIT` */

    /* `#END` */

    #ifdef Capesnse_ISR_EXIT_CALLBACK
        Capesnse_ISR_ExitCallback();
    #endif /* Capesnse_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
