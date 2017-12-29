/*******************************************************************************
* File Name: Capsense_PM.c
* Version 2.60
*
* Description:
*  This file provides Sleep APIs for CapSense CSD Component.
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

static Capsense_BACKUP_STRUCT Capsense_backup =
{
    0x00u, /* enableState; */
};


/*******************************************************************************
* Function Name: Capsense_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the customer configuration of CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_backup - used to save the component state before entering the  sleep
*  mode and none-retention registers.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_SaveConfig(void)
{
    if ((Capsense_CSD_CFG_REG & Capsense_CSD_CFG_ENABLE) != 0u)
    {
        Capsense_backup.enableState = 1u;
    }
}


/*******************************************************************************
* Function Name: Capsense_Sleep
********************************************************************************
*
* Summary:
*  Disables the Active mode power.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_backup - used to save the component state before entering the sleep
*  mode.
*
*******************************************************************************/
void Capsense_Sleep(void)
{
    Capsense_SaveConfig();

    /* Disable interrupt */
    CyIntDisable(Capsense_ISR_NUMBER);

    Capsense_CSD_CFG_REG &= ~(Capsense_CSD_CFG_SENSE_COMP_EN | Capsense_CSD_CFG_SENSE_EN);

    #if(Capsense_IDAC_CNT == 2u)
        Capsense_CSD_CFG_REG &= ~(Capsense_CSD_CFG_ENABLE);
    #endif /* (Capsense_IDAC_CNT == 2u) */

    /* Disable Clocks */
    Capsense_SenseClk_Stop();
    Capsense_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Capsense_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the CapSense configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after Capsense_SaveConfig() routine. Otherwise
*  the component configuration will be overwritten with its initial setting.
*
* Global Variables:
*  Capsense_backup - used to save the component state before entering the sleep
*  mode and none-retention registers.
*
*******************************************************************************/
void Capsense_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: Capsense_Wakeup
********************************************************************************
*
* Summary:
*  Restores the CapSense configuration and non-retention register values.
*  Restores the enabled state of the component by setting the Active mode power template
*  bits for a number of components used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_backup - used to save the component state before entering the sleep
*  mode and none-retention registers.
*
*******************************************************************************/
void Capsense_Wakeup(void)
{
    /* Enable the Clocks */
    Capsense_SenseClk_Start();
    Capsense_SampleClk_Start();

    /* Restore CapSense Enable state */
    if (Capsense_backup.enableState != 0u)
    {
        Capsense_Enable();
    }
}


/* [] END OF FILE */
