/*******************************************************************************
* File Name: Speaker.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Speaker.h"

static Speaker_BACKUP_STRUCT  Speaker_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Speaker_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Speaker_SUT.c usage_Speaker_Sleep_Wakeup
*******************************************************************************/
void Speaker_Sleep(void)
{
    #if defined(Speaker__PC)
        Speaker_backup.pcState = Speaker_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Speaker_backup.usbState = Speaker_CR1_REG;
            Speaker_USB_POWER_REG |= Speaker_USBIO_ENTER_SLEEP;
            Speaker_CR1_REG &= Speaker_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Speaker__SIO)
        Speaker_backup.sioState = Speaker_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Speaker_SIO_REG &= (uint32)(~Speaker_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Speaker_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Speaker_Sleep() for an example usage.
*******************************************************************************/
void Speaker_Wakeup(void)
{
    #if defined(Speaker__PC)
        Speaker_PC = Speaker_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Speaker_USB_POWER_REG &= Speaker_USBIO_EXIT_SLEEP_PH1;
            Speaker_CR1_REG = Speaker_backup.usbState;
            Speaker_USB_POWER_REG &= Speaker_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Speaker__SIO)
        Speaker_SIO_REG = Speaker_backup.sioState;
    #endif
}


/* [] END OF FILE */
