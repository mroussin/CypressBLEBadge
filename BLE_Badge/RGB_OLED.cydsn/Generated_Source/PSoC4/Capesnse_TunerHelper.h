/*******************************************************************************
* File Name: Capesnse_TunerHelper.h
* Version 2.60
*
* Description:
*  This file provides constants and structure declarations for the tuner helper
*  APIs for the CapSense CSD component.
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_Capesnse_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_Capesnse_H

#include "Capesnse.h"

#if(0u != Capesnse_CSHL_API_GENERATE)
    #include "Capesnse_CSHL.h"
#endif /* (0u != Capesnse_CSHL_API_GENERATE) */

#if (Capesnse_TUNER_API_GENERATE)
    #include "Capesnse_MBX.h"
#endif /* (Capesnse_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define Capesnse_DEFAULT_MAILBOXES_NUMBER       (1u)

#define Capesnse_CENTROID_POSITION_BUFFER_SIZE  (2u)
#define Capesnse_MB_POSITION_BUFFER_SIZE        (2u)

#define Capesnse_DEFAULT_TRACKPAD_INDEX         (0u)

#define Capesnse_FIRST_FINGER_INDEX             (0u)
#define Capesnse_SECOND_FINGER_INDEX            (1u)

#define Capesnse_FIRST_FINGER_X_INDEX           (0u)
#define Capesnse_FIRST_FINGER_Y_INDEX           (1u)

#define Capesnse_SECOND_FINGER_X_INDEX          (2u)
#define Capesnse_SECOND_FINGER_Y_INDEX          (3u)

#define Capesnse_GESTURE_TYPE_MASK              (0xF0u)
#define Capesnse_INERTIAL_SCROLL                (0xB0u)
#define Capesnse_NON_INERTIAL_SCROLL            (0xC0u)


/***************************************
*        Function Prototypes
***************************************/

void Capesnse_TunerStart(void);
void Capesnse_TunerComm(void);

#if (Capesnse_TUNER_API_GENERATE)
   Capesnse_NO_STRICT_VOLATILE void Capesnse_ProcessAllWidgets(volatile Capesnse_OUTBOX *outbox);
#endif /* (Capesnse_TUNER_API_GENERATE) */


/***************************************
*     Vars with External Linkage
***************************************/
#if (Capesnse_TUNER_API_GENERATE)
    extern volatile Capesnse_MAILBOXES Capesnse_mailboxesComm;
#endif /* (Capesnse_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_Capesnse_H)*/


/* [] END OF FILE */
