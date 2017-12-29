/*******************************************************************************
* File Name: Capesnse_SenseClk.h
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

#if !defined(CY_CLOCK_Capesnse_SenseClk_H)
#define CY_CLOCK_Capesnse_SenseClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Capesnse_SenseClk_StartEx(uint32 alignClkDiv);
#define Capesnse_SenseClk_Start() \
    Capesnse_SenseClk_StartEx(Capesnse_SenseClk__PA_DIV_ID)

#else

void Capesnse_SenseClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Capesnse_SenseClk_Stop(void);

void Capesnse_SenseClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Capesnse_SenseClk_GetDividerRegister(void);
uint8  Capesnse_SenseClk_GetFractionalDividerRegister(void);

#define Capesnse_SenseClk_Enable()                         Capesnse_SenseClk_Start()
#define Capesnse_SenseClk_Disable()                        Capesnse_SenseClk_Stop()
#define Capesnse_SenseClk_SetDividerRegister(clkDivider, reset)  \
    Capesnse_SenseClk_SetFractionalDividerRegister((clkDivider), 0u)
#define Capesnse_SenseClk_SetDivider(clkDivider)           Capesnse_SenseClk_SetDividerRegister((clkDivider), 1u)
#define Capesnse_SenseClk_SetDividerValue(clkDivider)      Capesnse_SenseClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Capesnse_SenseClk_DIV_ID     Capesnse_SenseClk__DIV_ID

#define Capesnse_SenseClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Capesnse_SenseClk_CTRL_REG   (*(reg32 *)Capesnse_SenseClk__CTRL_REGISTER)
#define Capesnse_SenseClk_DIV_REG    (*(reg32 *)Capesnse_SenseClk__DIV_REGISTER)

#define Capesnse_SenseClk_CMD_DIV_SHIFT          (0u)
#define Capesnse_SenseClk_CMD_PA_DIV_SHIFT       (8u)
#define Capesnse_SenseClk_CMD_DISABLE_SHIFT      (30u)
#define Capesnse_SenseClk_CMD_ENABLE_SHIFT       (31u)

#define Capesnse_SenseClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Capesnse_SenseClk_CMD_DISABLE_SHIFT))
#define Capesnse_SenseClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Capesnse_SenseClk_CMD_ENABLE_SHIFT))

#define Capesnse_SenseClk_DIV_FRAC_MASK  (0x000000F8u)
#define Capesnse_SenseClk_DIV_FRAC_SHIFT (3u)
#define Capesnse_SenseClk_DIV_INT_MASK   (0xFFFFFF00u)
#define Capesnse_SenseClk_DIV_INT_SHIFT  (8u)

#else 

#define Capesnse_SenseClk_DIV_REG        (*(reg32 *)Capesnse_SenseClk__REGISTER)
#define Capesnse_SenseClk_ENABLE_REG     Capesnse_SenseClk_DIV_REG
#define Capesnse_SenseClk_DIV_FRAC_MASK  Capesnse_SenseClk__FRAC_MASK
#define Capesnse_SenseClk_DIV_FRAC_SHIFT (16u)
#define Capesnse_SenseClk_DIV_INT_MASK   Capesnse_SenseClk__DIVIDER_MASK
#define Capesnse_SenseClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Capesnse_SenseClk_H) */

/* [] END OF FILE */
