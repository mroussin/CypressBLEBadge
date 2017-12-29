/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_SPI_Master_H)
#define CY_SCB_PVT_SPI_Master_H

#include "SPI_Master.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SPI_Master_SetI2CExtClkInterruptMode(interruptMask) SPI_Master_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SPI_Master_ClearI2CExtClkInterruptSource(interruptMask) SPI_Master_CLEAR_INTR_I2C_EC(interruptMask)
#define SPI_Master_GetI2CExtClkInterruptSource()                (SPI_Master_INTR_I2C_EC_REG)
#define SPI_Master_GetI2CExtClkInterruptMode()                  (SPI_Master_INTR_I2C_EC_MASK_REG)
#define SPI_Master_GetI2CExtClkInterruptSourceMasked()          (SPI_Master_INTR_I2C_EC_MASKED_REG)

#if (!SPI_Master_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SPI_Master_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SPI_Master_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SPI_Master_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SPI_Master_CLEAR_INTR_SPI_EC(interruptMask)
    #define SPI_Master_GetExtSpiClkInterruptSource()                 (SPI_Master_INTR_SPI_EC_REG)
    #define SPI_Master_GetExtSpiClkInterruptMode()                   (SPI_Master_INTR_SPI_EC_MASK_REG)
    #define SPI_Master_GetExtSpiClkInterruptSourceMasked()           (SPI_Master_INTR_SPI_EC_MASKED_REG)
#endif /* (!SPI_Master_CY_SCBIP_V1) */

#if(SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SPI_Master_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SPI_Master_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SPI_Master_CUSTOM_INTR_HANDLER)
    extern cyisraddress SPI_Master_customIntrHandler;
#endif /* !defined (CY_REMOVE_SPI_Master_CUSTOM_INTR_HANDLER) */
#endif /* (SPI_Master_SCB_IRQ_INTERNAL) */

extern SPI_Master_BACKUP_STRUCT SPI_Master_backup;

#if(SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SPI_Master_scbMode;
    extern uint8 SPI_Master_scbEnableWake;
    extern uint8 SPI_Master_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SPI_Master_mode;
    extern uint8 SPI_Master_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SPI_Master_rxBuffer;
    extern uint8   SPI_Master_rxDataBits;
    extern uint32  SPI_Master_rxBufferSize;

    extern volatile uint8 * SPI_Master_txBuffer;
    extern uint8   SPI_Master_txDataBits;
    extern uint32  SPI_Master_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SPI_Master_numberOfAddr;
    extern uint8 SPI_Master_subAddrSize;
#endif /* (SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SPI_Master_SCB_MODE_I2C_CONST_CFG || \
        SPI_Master_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SPI_Master_IntrTxMask;
#endif /* (! (SPI_Master_SCB_MODE_I2C_CONST_CFG || \
              SPI_Master_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SPI_Master_SCB_MODE_I2C_RUNTM_CFG     (SPI_Master_SCB_MODE_I2C      == SPI_Master_scbMode)
    #define SPI_Master_SCB_MODE_SPI_RUNTM_CFG     (SPI_Master_SCB_MODE_SPI      == SPI_Master_scbMode)
    #define SPI_Master_SCB_MODE_UART_RUNTM_CFG    (SPI_Master_SCB_MODE_UART     == SPI_Master_scbMode)
    #define SPI_Master_SCB_MODE_EZI2C_RUNTM_CFG   (SPI_Master_SCB_MODE_EZI2C    == SPI_Master_scbMode)
    #define SPI_Master_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SPI_Master_SCB_MODE_UNCONFIG == SPI_Master_scbMode)

    /* Defines wakeup enable */
    #define SPI_Master_SCB_WAKE_ENABLE_CHECK       (0u != SPI_Master_scbEnableWake)
#endif /* (SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SPI_Master_CY_SCBIP_V1)
    #define SPI_Master_SCB_PINS_NUMBER    (7u)
#else
    #define SPI_Master_SCB_PINS_NUMBER    (2u)
#endif /* (!SPI_Master_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SPI_Master_H) */


/* [] END OF FILE */
