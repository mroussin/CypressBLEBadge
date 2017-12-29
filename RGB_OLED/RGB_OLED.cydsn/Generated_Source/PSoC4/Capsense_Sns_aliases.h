/*******************************************************************************
* File Name: Capsense_Sns.h  
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

#if !defined(CY_PINS_Capsense_Sns_ALIASES_H) /* Pins Capsense_Sns_ALIASES_H */
#define CY_PINS_Capsense_Sns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Capsense_Sns_0			(Capsense_Sns__0__PC)
#define Capsense_Sns_0_PS		(Capsense_Sns__0__PS)
#define Capsense_Sns_0_PC		(Capsense_Sns__0__PC)
#define Capsense_Sns_0_DR		(Capsense_Sns__0__DR)
#define Capsense_Sns_0_SHIFT	(Capsense_Sns__0__SHIFT)
#define Capsense_Sns_0_INTR	((uint16)((uint16)0x0003u << (Capsense_Sns__0__SHIFT*2u)))

#define Capsense_Sns_INTR_ALL	 ((uint16)(Capsense_Sns_0_INTR))
#define Capsense_Sns_Button0__BTN			(Capsense_Sns__Button0__BTN__PC)
#define Capsense_Sns_Button0__BTN_PS		(Capsense_Sns__Button0__BTN__PS)
#define Capsense_Sns_Button0__BTN_PC		(Capsense_Sns__Button0__BTN__PC)
#define Capsense_Sns_Button0__BTN_DR		(Capsense_Sns__Button0__BTN__DR)
#define Capsense_Sns_Button0__BTN_SHIFT	(Capsense_Sns__Button0__BTN__SHIFT)
#define Capsense_Sns_Button0__BTN_INTR	((uint16)((uint16)0x0003u << (Capsense_Sns__0__SHIFT*2u)))


#endif /* End Pins Capsense_Sns_ALIASES_H */


/* [] END OF FILE */
