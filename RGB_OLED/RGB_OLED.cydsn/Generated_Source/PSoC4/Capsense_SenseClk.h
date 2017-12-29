/*******************************************************************************
* File Name: Capsense_SenseClk.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Capsense_SenseClk_H)
#define CY_CLOCK_Capsense_SenseClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Capsense_SenseClk_StartEx(uint32 alignClkDiv);
#define Capsense_SenseClk_Start() \
    Capsense_SenseClk_StartEx(Capsense_SenseClk__PA_DIV_ID)

#else

void Capsense_SenseClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Capsense_SenseClk_Stop(void);

void Capsense_SenseClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Capsense_SenseClk_GetDividerRegister(void);
uint8  Capsense_SenseClk_GetFractionalDividerRegister(void);

#define Capsense_SenseClk_Enable()                         Capsense_SenseClk_Start()
#define Capsense_SenseClk_Disable()                        Capsense_SenseClk_Stop()
#define Capsense_SenseClk_SetDividerRegister(clkDivider, reset)  \
    Capsense_SenseClk_SetFractionalDividerRegister((clkDivider), 0u)
#define Capsense_SenseClk_SetDivider(clkDivider)           Capsense_SenseClk_SetDividerRegister((clkDivider), 1u)
#define Capsense_SenseClk_SetDividerValue(clkDivider)      Capsense_SenseClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Capsense_SenseClk_DIV_ID     Capsense_SenseClk__DIV_ID

#define Capsense_SenseClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Capsense_SenseClk_CTRL_REG   (*(reg32 *)Capsense_SenseClk__CTRL_REGISTER)
#define Capsense_SenseClk_DIV_REG    (*(reg32 *)Capsense_SenseClk__DIV_REGISTER)

#define Capsense_SenseClk_CMD_DIV_SHIFT          (0u)
#define Capsense_SenseClk_CMD_PA_DIV_SHIFT       (8u)
#define Capsense_SenseClk_CMD_DISABLE_SHIFT      (30u)
#define Capsense_SenseClk_CMD_ENABLE_SHIFT       (31u)

#define Capsense_SenseClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Capsense_SenseClk_CMD_DISABLE_SHIFT))
#define Capsense_SenseClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Capsense_SenseClk_CMD_ENABLE_SHIFT))

#define Capsense_SenseClk_DIV_FRAC_MASK  (0x000000F8u)
#define Capsense_SenseClk_DIV_FRAC_SHIFT (3u)
#define Capsense_SenseClk_DIV_INT_MASK   (0xFFFFFF00u)
#define Capsense_SenseClk_DIV_INT_SHIFT  (8u)

#else 

#define Capsense_SenseClk_DIV_REG        (*(reg32 *)Capsense_SenseClk__REGISTER)
#define Capsense_SenseClk_ENABLE_REG     Capsense_SenseClk_DIV_REG
#define Capsense_SenseClk_DIV_FRAC_MASK  Capsense_SenseClk__FRAC_MASK
#define Capsense_SenseClk_DIV_FRAC_SHIFT (16u)
#define Capsense_SenseClk_DIV_INT_MASK   Capsense_SenseClk__DIVIDER_MASK
#define Capsense_SenseClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Capsense_SenseClk_H) */

/* [] END OF FILE */
