/*******************************************************************************
* File Name: Capesnse_Sns.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Capesnse_Sns_ALIASES_H) /* Pins Capesnse_Sns_ALIASES_H */
#define CY_PINS_Capesnse_Sns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Capesnse_Sns_0			(Capesnse_Sns__0__PC)
#define Capesnse_Sns_0_PS		(Capesnse_Sns__0__PS)
#define Capesnse_Sns_0_PC		(Capesnse_Sns__0__PC)
#define Capesnse_Sns_0_DR		(Capesnse_Sns__0__DR)
#define Capesnse_Sns_0_SHIFT	(Capesnse_Sns__0__SHIFT)
#define Capesnse_Sns_0_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__0__SHIFT*2u)))

#define Capesnse_Sns_1			(Capesnse_Sns__1__PC)
#define Capesnse_Sns_1_PS		(Capesnse_Sns__1__PS)
#define Capesnse_Sns_1_PC		(Capesnse_Sns__1__PC)
#define Capesnse_Sns_1_DR		(Capesnse_Sns__1__DR)
#define Capesnse_Sns_1_SHIFT	(Capesnse_Sns__1__SHIFT)
#define Capesnse_Sns_1_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__1__SHIFT*2u)))

#define Capesnse_Sns_2			(Capesnse_Sns__2__PC)
#define Capesnse_Sns_2_PS		(Capesnse_Sns__2__PS)
#define Capesnse_Sns_2_PC		(Capesnse_Sns__2__PC)
#define Capesnse_Sns_2_DR		(Capesnse_Sns__2__DR)
#define Capesnse_Sns_2_SHIFT	(Capesnse_Sns__2__SHIFT)
#define Capesnse_Sns_2_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__2__SHIFT*2u)))

#define Capesnse_Sns_3			(Capesnse_Sns__3__PC)
#define Capesnse_Sns_3_PS		(Capesnse_Sns__3__PS)
#define Capesnse_Sns_3_PC		(Capesnse_Sns__3__PC)
#define Capesnse_Sns_3_DR		(Capesnse_Sns__3__DR)
#define Capesnse_Sns_3_SHIFT	(Capesnse_Sns__3__SHIFT)
#define Capesnse_Sns_3_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__3__SHIFT*2u)))

#define Capesnse_Sns_4			(Capesnse_Sns__4__PC)
#define Capesnse_Sns_4_PS		(Capesnse_Sns__4__PS)
#define Capesnse_Sns_4_PC		(Capesnse_Sns__4__PC)
#define Capesnse_Sns_4_DR		(Capesnse_Sns__4__DR)
#define Capesnse_Sns_4_SHIFT	(Capesnse_Sns__4__SHIFT)
#define Capesnse_Sns_4_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__4__SHIFT*2u)))

#define Capesnse_Sns_INTR_ALL	 ((uint16)(Capesnse_Sns_0_INTR| Capesnse_Sns_1_INTR| Capesnse_Sns_2_INTR| Capesnse_Sns_3_INTR| Capesnse_Sns_4_INTR))
#define Capesnse_Sns_LinearSlider0_e0__LS			(Capesnse_Sns__LinearSlider0_e0__LS__PC)
#define Capesnse_Sns_LinearSlider0_e0__LS_PS		(Capesnse_Sns__LinearSlider0_e0__LS__PS)
#define Capesnse_Sns_LinearSlider0_e0__LS_PC		(Capesnse_Sns__LinearSlider0_e0__LS__PC)
#define Capesnse_Sns_LinearSlider0_e0__LS_DR		(Capesnse_Sns__LinearSlider0_e0__LS__DR)
#define Capesnse_Sns_LinearSlider0_e0__LS_SHIFT	(Capesnse_Sns__LinearSlider0_e0__LS__SHIFT)
#define Capesnse_Sns_LinearSlider0_e0__LS_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__0__SHIFT*2u)))

#define Capesnse_Sns_LinearSlider0_e1__LS			(Capesnse_Sns__LinearSlider0_e1__LS__PC)
#define Capesnse_Sns_LinearSlider0_e1__LS_PS		(Capesnse_Sns__LinearSlider0_e1__LS__PS)
#define Capesnse_Sns_LinearSlider0_e1__LS_PC		(Capesnse_Sns__LinearSlider0_e1__LS__PC)
#define Capesnse_Sns_LinearSlider0_e1__LS_DR		(Capesnse_Sns__LinearSlider0_e1__LS__DR)
#define Capesnse_Sns_LinearSlider0_e1__LS_SHIFT	(Capesnse_Sns__LinearSlider0_e1__LS__SHIFT)
#define Capesnse_Sns_LinearSlider0_e1__LS_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__1__SHIFT*2u)))

#define Capesnse_Sns_LinearSlider0_e2__LS			(Capesnse_Sns__LinearSlider0_e2__LS__PC)
#define Capesnse_Sns_LinearSlider0_e2__LS_PS		(Capesnse_Sns__LinearSlider0_e2__LS__PS)
#define Capesnse_Sns_LinearSlider0_e2__LS_PC		(Capesnse_Sns__LinearSlider0_e2__LS__PC)
#define Capesnse_Sns_LinearSlider0_e2__LS_DR		(Capesnse_Sns__LinearSlider0_e2__LS__DR)
#define Capesnse_Sns_LinearSlider0_e2__LS_SHIFT	(Capesnse_Sns__LinearSlider0_e2__LS__SHIFT)
#define Capesnse_Sns_LinearSlider0_e2__LS_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__2__SHIFT*2u)))

#define Capesnse_Sns_LinearSlider0_e3__LS			(Capesnse_Sns__LinearSlider0_e3__LS__PC)
#define Capesnse_Sns_LinearSlider0_e3__LS_PS		(Capesnse_Sns__LinearSlider0_e3__LS__PS)
#define Capesnse_Sns_LinearSlider0_e3__LS_PC		(Capesnse_Sns__LinearSlider0_e3__LS__PC)
#define Capesnse_Sns_LinearSlider0_e3__LS_DR		(Capesnse_Sns__LinearSlider0_e3__LS__DR)
#define Capesnse_Sns_LinearSlider0_e3__LS_SHIFT	(Capesnse_Sns__LinearSlider0_e3__LS__SHIFT)
#define Capesnse_Sns_LinearSlider0_e3__LS_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__3__SHIFT*2u)))

#define Capesnse_Sns_LinearSlider0_e4__LS			(Capesnse_Sns__LinearSlider0_e4__LS__PC)
#define Capesnse_Sns_LinearSlider0_e4__LS_PS		(Capesnse_Sns__LinearSlider0_e4__LS__PS)
#define Capesnse_Sns_LinearSlider0_e4__LS_PC		(Capesnse_Sns__LinearSlider0_e4__LS__PC)
#define Capesnse_Sns_LinearSlider0_e4__LS_DR		(Capesnse_Sns__LinearSlider0_e4__LS__DR)
#define Capesnse_Sns_LinearSlider0_e4__LS_SHIFT	(Capesnse_Sns__LinearSlider0_e4__LS__SHIFT)
#define Capesnse_Sns_LinearSlider0_e4__LS_INTR	((uint16)((uint16)0x0003u << (Capesnse_Sns__4__SHIFT*2u)))


#endif /* End Pins Capesnse_Sns_ALIASES_H */


/* [] END OF FILE */
