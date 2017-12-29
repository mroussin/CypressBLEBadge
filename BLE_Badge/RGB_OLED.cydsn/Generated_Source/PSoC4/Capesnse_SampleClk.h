/*******************************************************************************
* File Name: Capesnse_SampleClk.h
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

#if !defined(CY_CLOCK_Capesnse_SampleClk_H)
#define CY_CLOCK_Capesnse_SampleClk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Capesnse_SampleClk_StartEx(uint32 alignClkDiv);
#define Capesnse_SampleClk_Start() \
    Capesnse_SampleClk_StartEx(Capesnse_SampleClk__PA_DIV_ID)

#else

void Capesnse_SampleClk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Capesnse_SampleClk_Stop(void);

void Capesnse_SampleClk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Capesnse_SampleClk_GetDividerRegister(void);
uint8  Capesnse_SampleClk_GetFractionalDividerRegister(void);

#define Capesnse_SampleClk_Enable()                         Capesnse_SampleClk_Start()
#define Capesnse_SampleClk_Disable()                        Capesnse_SampleClk_Stop()
#define Capesnse_SampleClk_SetDividerRegister(clkDivider, reset)  \
    Capesnse_SampleClk_SetFractionalDividerRegister((clkDivider), 0u)
#define Capesnse_SampleClk_SetDivider(clkDivider)           Capesnse_SampleClk_SetDividerRegister((clkDivider), 1u)
#define Capesnse_SampleClk_SetDividerValue(clkDivider)      Capesnse_SampleClk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Capesnse_SampleClk_DIV_ID     Capesnse_SampleClk__DIV_ID

#define Capesnse_SampleClk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Capesnse_SampleClk_CTRL_REG   (*(reg32 *)Capesnse_SampleClk__CTRL_REGISTER)
#define Capesnse_SampleClk_DIV_REG    (*(reg32 *)Capesnse_SampleClk__DIV_REGISTER)

#define Capesnse_SampleClk_CMD_DIV_SHIFT          (0u)
#define Capesnse_SampleClk_CMD_PA_DIV_SHIFT       (8u)
#define Capesnse_SampleClk_CMD_DISABLE_SHIFT      (30u)
#define Capesnse_SampleClk_CMD_ENABLE_SHIFT       (31u)

#define Capesnse_SampleClk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Capesnse_SampleClk_CMD_DISABLE_SHIFT))
#define Capesnse_SampleClk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Capesnse_SampleClk_CMD_ENABLE_SHIFT))

#define Capesnse_SampleClk_DIV_FRAC_MASK  (0x000000F8u)
#define Capesnse_SampleClk_DIV_FRAC_SHIFT (3u)
#define Capesnse_SampleClk_DIV_INT_MASK   (0xFFFFFF00u)
#define Capesnse_SampleClk_DIV_INT_SHIFT  (8u)

#else 

#define Capesnse_SampleClk_DIV_REG        (*(reg32 *)Capesnse_SampleClk__REGISTER)
#define Capesnse_SampleClk_ENABLE_REG     Capesnse_SampleClk_DIV_REG
#define Capesnse_SampleClk_DIV_FRAC_MASK  Capesnse_SampleClk__FRAC_MASK
#define Capesnse_SampleClk_DIV_FRAC_SHIFT (16u)
#define Capesnse_SampleClk_DIV_INT_MASK   Capesnse_SampleClk__DIVIDER_MASK
#define Capesnse_SampleClk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Capesnse_SampleClk_H) */

/* [] END OF FILE */
