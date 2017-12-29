/*******************************************************************************
* File Name: Capesnse_PM.c
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

#include "Capesnse.h"

static Capesnse_BACKUP_STRUCT Capesnse_backup =
{
    0x00u, /* enableState; */
};


/*******************************************************************************
* Function Name: Capesnse_SaveConfig
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
*  Capesnse_backup - used to save the component state before entering the  sleep
*  mode and none-retention registers.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_SaveConfig(void)
{
    if ((Capesnse_CSD_CFG_REG & Capesnse_CSD_CFG_ENABLE) != 0u)
    {
        Capesnse_backup.enableState = 1u;
    }
}


/*******************************************************************************
* Function Name: Capesnse_Sleep
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
*  Capesnse_backup - used to save the component state before entering the sleep
*  mode.
*
*******************************************************************************/
void Capesnse_Sleep(void)
{
    Capesnse_SaveConfig();

    /* Disable interrupt */
    CyIntDisable(Capesnse_ISR_NUMBER);

    Capesnse_CSD_CFG_REG &= ~(Capesnse_CSD_CFG_SENSE_COMP_EN | Capesnse_CSD_CFG_SENSE_EN);

    #if(Capesnse_IDAC_CNT == 2u)
        Capesnse_CSD_CFG_REG &= ~(Capesnse_CSD_CFG_ENABLE);
    #endif /* (Capesnse_IDAC_CNT == 2u) */

    /* Disable Clocks */
    Capesnse_SenseClk_Stop();
    Capesnse_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Capesnse_RestoreConfig
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
*  Must be called only after Capesnse_SaveConfig() routine. Otherwise
*  the component configuration will be overwritten with its initial setting.
*
* Global Variables:
*  Capesnse_backup - used to save the component state before entering the sleep
*  mode and none-retention registers.
*
*******************************************************************************/
void Capesnse_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: Capesnse_Wakeup
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
*  Capesnse_backup - used to save the component state before entering the sleep
*  mode and none-retention registers.
*
*******************************************************************************/
void Capesnse_Wakeup(void)
{
    /* Enable the Clocks */
    Capesnse_SenseClk_Start();
    Capesnse_SampleClk_Start();

    /* Restore CapSense Enable state */
    if (Capesnse_backup.enableState != 0u)
    {
        Capesnse_Enable();
    }
}


/* [] END OF FILE */
