/*******************************************************************************
* File Name: Capsense_SMS.h
* Version 2.60
*
* Description:
*  This file provides the declarations of the wrapper between the CapSense CSD component
*  and Auto Tuning library.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_CAPSENSE_CSD_SMS_Capsense_H)
#define CY_CAPSENSE_CSD_SMS_Capsense_H

#include "Capsense.h"
#include "Capsense_PVT.h"

#if(0u != Capsense_CSHL_API_GENERATE)
    #include "Capsense_CSHL.h"
#endif /* (0u != Capsense_CSHL_API_GENERATE) */

#define Capsense_PRESCALERS_TBL_SIZE        (16u)

#define Capsense_CALIBRATION_FREQ_KHZ       (3000u)
#define Capsense_CALIBRATION_MD             (1u)

#define Capsense_MAX_RESOLUTION         (65535u)
#define Capsense_CALIBRATION_RESOLUTION (0x0FFF0000u)

#define Capsense_PRS_CORRECTION_DIVIDER (4u)
#define Capsense_SENSITIVITY_DIVIDER    (10u)

#define Capsense_SENSITIVITY_MASK           (0x0Fu)
#define Capsense_FLEXIBLE_THRESHOLDS_EN     (0u)
#define Capsense_CAL_RAW_COUNT              (3482u)

#define Capsense_THRESHOLD_1                (220u)
#define Capsense_THRESHOLD_2                (237u)
#define Capsense_THRESHOLD_3                (245u)
#define Capsense_THRESHOLD_4                (250u)


#if (0u != Capsense_IS_M0S8PERI_BLOCK)
    #define Capsense_CALIBRATION_ASD        (CYDEV_BCLK__HFCLK__KHZ /\
                                                     Capsense_CALIBRATION_FREQ_KHZ)
#else
    #define Capsense_MIN_IMO_FREQ_KHZ       (6000u)
    #if(CYDEV_BCLK__HFCLK__KHZ < Capsense_MIN_IMO_FREQ_KHZ)
        #define Capsense_CALIBRATION_ASD            (1u)
    #else
        #define Capsense_CALIBRATION_ASD    (CYDEV_BCLK__HFCLK__KHZ / Capsense_CALIBRATION_MD /\
                                                     Capsense_CALIBRATION_FREQ_KHZ)
    #endif /* (CYDEV_BCLK__HFCLK__KHZ < Capsense_MIN_IMO_FREQ_KHZ) */
#endif /* (0u != Capsense_IS_M0S8PERI_BLOCK) */

#if (0u != Capsense_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */
    #if(Capsense_PRS_OPTIONS != Capsense__PRS_NONE)
        #define Capsense_SENSETIVITY_FACTOR (1228u)
    #else
        #define Capsense_SENSETIVITY_FACTOR (2456u)
    #endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_NONE) */
#else
    /*  Dividers are chained */
    #if(Capsense_PRS_OPTIONS != Capsense__PRS_NONE)
        #define Capsense_SENSETIVITY_FACTOR (1228u/Capsense_CALIBRATION_MD)
    #else
        #define Capsense_SENSETIVITY_FACTOR (2456u/Capsense_CALIBRATION_MD)
    #endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_NONE) */
#endif /* (0u != Capsense_IS_M0S8PERI_BLOCK) */


/***************************************
*        Function Prototypes
***************************************/
void Capsense_AutoTune(void);
void Capsense_UpdateThresholds(uint32 sensor);
void Capsense_SetSensitivity(uint32 sensor, uint32 value);
void Capsense_SetAnalogSwitchesSrcDirect(void);
uint16 Capsense_GetNoiseEnvelope(uint32 sensor);
uint8 Capsense_GetSensitivityCoefficient(uint32 sensor);
uint16 Capsense_GetNormalizedDiffCountData(uint32 sensor);


/***************************************
*     Vars with External Linkage
***************************************/

/*  Real Charge Divider values */
#if (0u != Capsense_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */
    #if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
        extern const uint8 prescalerTable[Capsense_PRESCALERS_TBL_SIZE];
    #elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
        extern const uint8 prescalerTable[Capsense_PRESCALERS_TBL_SIZE];
    #else
        extern const uint8 prescalerTable[Capsense_PRESCALERS_TBL_SIZE];
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
#else
    /*  Dividers are chained */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        extern const uint8 prescalerTable[Capsense_PRESCALERS_TBL_SIZE];
    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        extern const uint8 prescalerTable[Capsense_PRESCALERS_TBL_SIZE];
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        extern const uint8 prescalerTable[Capsense_PRESCALERS_TBL_SIZE];
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* (0u != Capsense_IS_M0S8PERI_BLOCK) */


extern uint8 Capsense_noiseEnvelopeTbl[Capsense_TOTAL_SENSOR_COUNT];
extern uint8 Capsense_runningDifferenceTbl[Capsense_TOTAL_SENSOR_COUNT];
extern uint8 Capsense_signRegisterTbl[Capsense_TOTAL_SENSOR_COUNT];
extern uint16 Capsense_sampleMinTbl[Capsense_TOTAL_SENSOR_COUNT];
extern uint16 Capsense_sampleMaxTbl[Capsense_TOTAL_SENSOR_COUNT];
extern uint16 Capsense_previousSampleTbl[Capsense_TOTAL_SENSOR_COUNT];
extern uint8 Capsense_kValueTbl[Capsense_TOTAL_SENSOR_COUNT];


#endif  /* (CY_CAPSENSE_CSD_SMS_Capsense_H) */

/* [] END OF FILE */
