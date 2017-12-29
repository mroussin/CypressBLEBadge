/*******************************************************************************
* File Name: OLED_Reset.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_OLED_Reset_H) /* Pins OLED_Reset_H */
#define CY_PINS_OLED_Reset_H

#include "cytypes.h"
#include "cyfitter.h"
#include "OLED_Reset_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} OLED_Reset_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   OLED_Reset_Read(void);
void    OLED_Reset_Write(uint8 value);
uint8   OLED_Reset_ReadDataReg(void);
#if defined(OLED_Reset__PC) || (CY_PSOC4_4200L) 
    void    OLED_Reset_SetDriveMode(uint8 mode);
#endif
void    OLED_Reset_SetInterruptMode(uint16 position, uint16 mode);
uint8   OLED_Reset_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void OLED_Reset_Sleep(void); 
void OLED_Reset_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(OLED_Reset__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define OLED_Reset_DRIVE_MODE_BITS        (3)
    #define OLED_Reset_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - OLED_Reset_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the OLED_Reset_SetDriveMode() function.
         *  @{
         */
        #define OLED_Reset_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define OLED_Reset_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define OLED_Reset_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define OLED_Reset_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define OLED_Reset_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define OLED_Reset_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define OLED_Reset_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define OLED_Reset_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define OLED_Reset_MASK               OLED_Reset__MASK
#define OLED_Reset_SHIFT              OLED_Reset__SHIFT
#define OLED_Reset_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in OLED_Reset_SetInterruptMode() function.
     *  @{
     */
        #define OLED_Reset_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define OLED_Reset_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define OLED_Reset_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define OLED_Reset_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(OLED_Reset__SIO)
    #define OLED_Reset_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(OLED_Reset__PC) && (CY_PSOC4_4200L)
    #define OLED_Reset_USBIO_ENABLE               ((uint32)0x80000000u)
    #define OLED_Reset_USBIO_DISABLE              ((uint32)(~OLED_Reset_USBIO_ENABLE))
    #define OLED_Reset_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define OLED_Reset_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define OLED_Reset_USBIO_ENTER_SLEEP          ((uint32)((1u << OLED_Reset_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << OLED_Reset_USBIO_SUSPEND_DEL_SHIFT)))
    #define OLED_Reset_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << OLED_Reset_USBIO_SUSPEND_SHIFT)))
    #define OLED_Reset_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << OLED_Reset_USBIO_SUSPEND_DEL_SHIFT)))
    #define OLED_Reset_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(OLED_Reset__PC)
    /* Port Configuration */
    #define OLED_Reset_PC                 (* (reg32 *) OLED_Reset__PC)
#endif
/* Pin State */
#define OLED_Reset_PS                     (* (reg32 *) OLED_Reset__PS)
/* Data Register */
#define OLED_Reset_DR                     (* (reg32 *) OLED_Reset__DR)
/* Input Buffer Disable Override */
#define OLED_Reset_INP_DIS                (* (reg32 *) OLED_Reset__PC2)

/* Interrupt configuration Registers */
#define OLED_Reset_INTCFG                 (* (reg32 *) OLED_Reset__INTCFG)
#define OLED_Reset_INTSTAT                (* (reg32 *) OLED_Reset__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define OLED_Reset_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(OLED_Reset__SIO)
    #define OLED_Reset_SIO_REG            (* (reg32 *) OLED_Reset__SIO)
#endif /* (OLED_Reset__SIO_CFG) */

/* USBIO registers */
#if !defined(OLED_Reset__PC) && (CY_PSOC4_4200L)
    #define OLED_Reset_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define OLED_Reset_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define OLED_Reset_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define OLED_Reset_DRIVE_MODE_SHIFT       (0x00u)
#define OLED_Reset_DRIVE_MODE_MASK        (0x07u << OLED_Reset_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins OLED_Reset_H */


/* [] END OF FILE */
