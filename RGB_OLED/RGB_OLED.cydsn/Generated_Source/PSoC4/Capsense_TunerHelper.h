/*******************************************************************************
* File Name: Capsense_TunerHelper.h
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

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_Capsense_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_Capsense_H

#include "Capsense.h"

#if(0u != Capsense_CSHL_API_GENERATE)
    #include "Capsense_CSHL.h"
#endif /* (0u != Capsense_CSHL_API_GENERATE) */

#if (Capsense_TUNER_API_GENERATE)
    #include "Capsense_MBX.h"
#endif /* (Capsense_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define Capsense_DEFAULT_MAILBOXES_NUMBER       (1u)

#define Capsense_CENTROID_POSITION_BUFFER_SIZE  (2u)
#define Capsense_MB_POSITION_BUFFER_SIZE        (2u)

#define Capsense_DEFAULT_TRACKPAD_INDEX         (0u)

#define Capsense_FIRST_FINGER_INDEX             (0u)
#define Capsense_SECOND_FINGER_INDEX            (1u)

#define Capsense_FIRST_FINGER_X_INDEX           (0u)
#define Capsense_FIRST_FINGER_Y_INDEX           (1u)

#define Capsense_SECOND_FINGER_X_INDEX          (2u)
#define Capsense_SECOND_FINGER_Y_INDEX          (3u)

#define Capsense_GESTURE_TYPE_MASK              (0xF0u)
#define Capsense_INERTIAL_SCROLL                (0xB0u)
#define Capsense_NON_INERTIAL_SCROLL            (0xC0u)


/***************************************
*        Function Prototypes
***************************************/

void Capsense_TunerStart(void);
void Capsense_TunerComm(void);

#if (Capsense_TUNER_API_GENERATE)
   Capsense_NO_STRICT_VOLATILE void Capsense_ProcessAllWidgets(volatile Capsense_OUTBOX *outbox);
#endif /* (Capsense_TUNER_API_GENERATE) */


/***************************************
*     Vars with External Linkage
***************************************/
#if (Capsense_TUNER_API_GENERATE)
    extern volatile Capsense_MAILBOXES Capsense_mailboxesComm;
#endif /* (Capsense_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_Capsense_H)*/


/* [] END OF FILE */
