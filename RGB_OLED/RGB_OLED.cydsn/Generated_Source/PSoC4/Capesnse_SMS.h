/*******************************************************************************
* File Name: Capesnse_SMS.h
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
#if !defined(CY_CAPSENSE_CSD_SMS_Capesnse_H)
#define CY_CAPSENSE_CSD_SMS_Capesnse_H

#include "Capesnse.h"
#include "Capesnse_PVT.h"

#if(0u != Capesnse_CSHL_API_GENERATE)
    #include "Capesnse_CSHL.h"
#endif /* (0u != Capesnse_CSHL_API_GENERATE) */

#define Capesnse_PRESCALERS_TBL_SIZE        (16u)

#define Capesnse_CALIBRATION_FREQ_KHZ       (3000u)
#define Capesnse_CALIBRATION_MD             (1u)

#define Capesnse_MAX_RESOLUTION         (65535u)
#define Capesnse_CALIBRATION_RESOLUTION (0x0FFF0000u)

#define Capesnse_PRS_CORRECTION_DIVIDER (4u)
#define Capesnse_SENSITIVITY_DIVIDER    (10u)

#define Capesnse_SENSITIVITY_MASK           (0x0Fu)
#define Capesnse_FLEXIBLE_THRESHOLDS_EN     (0u)
#define Capesnse_CAL_RAW_COUNT              (3482u)

#define Capesnse_THRESHOLD_1                (220u)
#define Capesnse_THRESHOLD_2                (237u)
#define Capesnse_THRESHOLD_3                (245u)
#define Capesnse_THRESHOLD_4                (250u)


#if (0u != Capesnse_IS_M0S8PERI_BLOCK)
    #define Capesnse_CALIBRATION_ASD        (CYDEV_BCLK__HFCLK__KHZ /\
                                                     Capesnse_CALIBRATION_FREQ_KHZ)
#else
    #define Capesnse_MIN_IMO_FREQ_KHZ       (6000u)
    #if(CYDEV_BCLK__HFCLK__KHZ < Capesnse_MIN_IMO_FREQ_KHZ)
        #define Capesnse_CALIBRATION_ASD            (1u)
    #else
        #define Capesnse_CALIBRATION_ASD    (CYDEV_BCLK__HFCLK__KHZ / Capesnse_CALIBRATION_MD /\
                                                     Capesnse_CALIBRATION_FREQ_KHZ)
    #endif /* (CYDEV_BCLK__HFCLK__KHZ < Capesnse_MIN_IMO_FREQ_KHZ) */
#endif /* (0u != Capesnse_IS_M0S8PERI_BLOCK) */

#if (0u != Capesnse_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */
    #if(Capesnse_PRS_OPTIONS != Capesnse__PRS_NONE)
        #define Capesnse_SENSETIVITY_FACTOR (1228u)
    #else
        #define Capesnse_SENSETIVITY_FACTOR (2456u)
    #endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE) */
#else
    /*  Dividers are chained */
    #if(Capesnse_PRS_OPTIONS != Capesnse__PRS_NONE)
        #define Capesnse_SENSETIVITY_FACTOR (1228u/Capesnse_CALIBRATION_MD)
    #else
        #define Capesnse_SENSETIVITY_FACTOR (2456u/Capesnse_CALIBRATION_MD)
    #endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE) */
#endif /* (0u != Capesnse_IS_M0S8PERI_BLOCK) */


/***************************************
*        Function Prototypes
***************************************/
void Capesnse_AutoTune(void);
void Capesnse_UpdateThresholds(uint32 sensor);
void Capesnse_SetSensitivity(uint32 sensor, uint32 value);
void Capesnse_SetAnalogSwitchesSrcDirect(void);
uint16 Capesnse_GetNoiseEnvelope(uint32 sensor);
uint8 Capesnse_GetSensitivityCoefficient(uint32 sensor);
uint16 Capesnse_GetNormalizedDiffCountData(uint32 sensor);


/***************************************
*     Vars with External Linkage
***************************************/

/*  Real Charge Divider values */
#if (0u != Capesnse_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained */
    #if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
        extern const uint8 prescalerTable[Capesnse_PRESCALERS_TBL_SIZE];
    #elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
        extern const uint8 prescalerTable[Capesnse_PRESCALERS_TBL_SIZE];
    #else
        extern const uint8 prescalerTable[Capesnse_PRESCALERS_TBL_SIZE];
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
#else
    /*  Dividers are chained */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        extern const uint8 prescalerTable[Capesnse_PRESCALERS_TBL_SIZE];
    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        extern const uint8 prescalerTable[Capesnse_PRESCALERS_TBL_SIZE];
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        extern const uint8 prescalerTable[Capesnse_PRESCALERS_TBL_SIZE];
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* (0u != Capesnse_IS_M0S8PERI_BLOCK) */


extern uint8 Capesnse_noiseEnvelopeTbl[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8 Capesnse_runningDifferenceTbl[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8 Capesnse_signRegisterTbl[Capesnse_TOTAL_SENSOR_COUNT];
extern uint16 Capesnse_sampleMinTbl[Capesnse_TOTAL_SENSOR_COUNT];
extern uint16 Capesnse_sampleMaxTbl[Capesnse_TOTAL_SENSOR_COUNT];
extern uint16 Capesnse_previousSampleTbl[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8 Capesnse_kValueTbl[Capesnse_TOTAL_SENSOR_COUNT];


#endif  /* (CY_CAPSENSE_CSD_SMS_Capesnse_H) */

/* [] END OF FILE */
