/*******************************************************************************
* File Name: Capsense_CSHL.h
* Version 2.60
*
* Description:
*  This file provides constants and parameter values for the High Level APIs
*  for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_CSHL_Capsense_H)
#define CY_CAPSENSE_CSD_CSHL_Capsense_H

#include "Capsense.h"


/***************************************
*   Condition compilation parameters
***************************************/

#define Capsense_SIGNAL_SIZE                    (8u)
#define Capsense_AUTO_RESET                     (0u)
#define Capsense_RAW_FILTER_MASK                (8u)

/* Signal size definition */
#define Capsense_SIGNAL_SIZE_UINT8              (8u)
#define Capsense_SIGNAL_SIZE_UINT16             (16u)

/* Auto reset definition */
#define Capsense_AUTO_RESET_DISABLE             (0u)
#define Capsense_AUTO_RESET_ENABLE              (1u)

/* Mask for RAW and POS filters */
#define Capsense_MEDIAN_FILTER                  (0x01u)
#define Capsense_AVERAGING_FILTER               (0x02u)
#define Capsense_IIR2_FILTER                    (0x04u)
#define Capsense_IIR4_FILTER                    (0x08u)
#define Capsense_JITTER_FILTER                  (0x10u)
#define Capsense_IIR8_FILTER                    (0x20u)
#define Capsense_IIR16_FILTER                   (0x40u)
#define Capsense_RAW_FILTERS_ENABLED            (0x01u)
#define Capsense_RAW_FILTERS_DISABLED           (0x00u)

/***************************************
*           API Constants
***************************************/

/* Widgets constants definition */
#define Capsense_BUTTON0__BTN        (0u)

#define Capsense_TOTAL_DIPLEXED_SLIDERS_COUNT        (0u)
#define Capsense_TOTAL_LINEAR_SLIDERS_COUNT          (0u)
#define Capsense_TOTAL_RADIAL_SLIDERS_COUNT          (0u)
#define Capsense_TOTAL_TOUCH_PADS_COUNT              (0u)
#define Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT        (0u)
#define Capsense_TOTAL_TRACKPAD_GESTURES_COUNT       (0u)
#define Capsense_TOTAL_BUTTONS_COUNT                 (1u)
#define Capsense_TOTAL_MATRIX_BUTTONS_COUNT          (0u)
#define Capsense_TOTAL_GENERICS_COUNT                (0u)

#define Capsense_POS_FILTERS_MASK                    (0x0u)
#define Capsense_LINEAR_SLIDERS_POS_FILTERS_MASK     (0x0u)
#define Capsense_RADIAL_SLIDERS_POS_FILTERS_MASK     (0x0u)
#define Capsense_TOUCH_PADS_POS_FILTERS_MASK         (0x0u)
#define Capsense_TRACKPAD_GEST_POS_FILTERS_MASK      (0x0u)

#define Capsense_UNUSED_DEBOUNCE_COUNTER_INDEX       (1u)

#define Capsense_TOTAL_PROX_SENSORS_COUNT            (0u)

#define Capsense_END_OF_SLIDERS_INDEX                (0u)
#define Capsense_END_OF_TOUCH_PAD_INDEX              (0u)
#define Capsense_END_OF_BUTTONS_INDEX                (0u)
#define Capsense_END_OF_MATRIX_BUTTONS_INDEX         (0u)
#define Capsense_END_OF_WIDGETS_INDEX                (1u)



#define Capsense_TOTAL_SLIDERS_COUNT            ( Capsense_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Capsense_TOTAL_RADIAL_SLIDERS_COUNT )

#define Capsense_TOTAL_CENTROIDS_COUNT          ( Capsense_TOTAL_SLIDERS_COUNT + \
                                                         (Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
                                                         (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT * 4u))

#define Capsense_TOTAL_CENTROIDS_BASIC_COUNT    ( Capsense_TOTAL_SLIDERS_COUNT + \
                                                         (Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u))

#define Capsense_TOTAL_CENTROID_AXES_COUNT      ( Capsense_TOTAL_SLIDERS_COUNT + \
                                                         (Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
                                                         (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT * 2u))

#define Capsense_TOTAL_WIDGET_COUNT             ( Capsense_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Capsense_TOTAL_RADIAL_SLIDERS_COUNT + \
                                                          Capsense_TOTAL_TOUCH_PADS_COUNT + \
                                                          Capsense_TOTAL_BUTTONS_COUNT + \
                                                          Capsense_TOTAL_MATRIX_BUTTONS_COUNT )

#define Capsense_ANY_POS_FILTER                 ( Capsense_MEDIAN_FILTER | \
                                                          Capsense_AVERAGING_FILTER | \
                                                          Capsense_IIR2_FILTER | \
                                                          Capsense_IIR4_FILTER | \
                                                          Capsense_JITTER_FILTER )

#define Capsense_IS_DIPLEX_SLIDER               ( Capsense_TOTAL_DIPLEXED_SLIDERS_COUNT > 0u)

#define Capsense_IS_NON_DIPLEX_SLIDER           ( (Capsense_TOTAL_LINEAR_SLIDERS_COUNT - \
                                                           Capsense_TOTAL_DIPLEXED_SLIDERS_COUNT) > 0u)
#define Capsense_ADD_SLIDER_TYPE                ((Capsense_TOTAL_RADIAL_SLIDERS_COUNT > 0u) ? \
                                                        ((Capsense_TOTAL_TOUCH_PADS_COUNT > 0u) || \
                                                         (Capsense_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) : 0u)

#define Capsense_TOTAL_PROX_SENSOR_COUNT        (Capsense_TOTAL_PROX_SENSORS_COUNT)

#define Capsense_WIDGETS_TBL_SIZE               ( Capsense_TOTAL_WIDGET_COUNT + \
                                                          Capsense_TOTAL_GENERICS_COUNT)

#define Capsense_WIDGET_PARAM_TBL_SIZE          (Capsense_TOTAL_BUTTONS_COUNT + \
                                                         Capsense_TOTAL_SLIDERS_COUNT +\
                                                         Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u + \
                                                         Capsense_TOTAL_TRACKPAD_GESTURES_COUNT * 2u +\
                                                         Capsense_TOTAL_MATRIX_BUTTONS_COUNT * 2u)

#define Capsense_THRESHOLD_TBL_SIZE         (Capsense_WIDGET_PARAM_TBL_SIZE)
#define Capsense_DEBOUNCE_CNT_TBL_SIZE      (Capsense_WIDGET_PARAM_TBL_SIZE)
#define Capsense_RAW_DATA_INDEX_TBL_SIZE    (Capsense_WIDGET_PARAM_TBL_SIZE +\
                                                     Capsense_TOTAL_GENERICS_COUNT)

#define Capsense_RES_MULT                   (256u)


#define Capsense_NOT_WIDGET                     (0xFFFFFFFFu)

/*Types of centroids */
#define Capsense_TYPE_RADIAL_SLIDER             (0x01u)
#define Capsense_TYPE_LINEAR_SLIDER             (0x02u)
#define Capsense_TYPE_GENERIC                   (0xFFu)

/* Defines if sensors or widgets are active */
#define Capsense_SENSOR_IS_ACTIVE               (0x01u)
#define Capsense_SENSOR_1_IS_ACTIVE             (0x01u)
#define Capsense_SENSOR_2_IS_ACTIVE             (0x02u)
#define Capsense_WIDGET_IS_ACTIVE               (0x01u)

/* Defines diplex type of Slider */
#define Capsense_IS_DIPLEX                      (0x80u)

/* Defines fingers positions on Slider  */
#define Capsense_POS_PREV                       (0u)
#define Capsense_POS                            (1u)
#define Capsense_POS_NEXT                       (2u)
#define Capsense_CENTROID_ROUND_VALUE           (0x7F00u)
#define Capsense_MAXIMUM_CENTROID               (0xFFu)

#define Capsense_NEGATIVE_NOISE_THRESHOLD       (20u)
#define Capsense_LOW_BASELINE_RESET             (5u)


/***************************************
*        Function Prototypes
***************************************/

void Capsense_InitializeSensorBaseline(uint32 sensor);
void Capsense_InitializeAllBaselines(void);
void Capsense_InitializeEnabledBaselines(void);
void Capsense_UpdateSensorBaseline(uint32 sensor);
void Capsense_UpdateBaselineNoThreshold(uint32 sensor);
void Capsense_UpdateEnabledBaselines(void);
void Capsense_UpdateWidgetBaseline(uint32 widget);
uint16 Capsense_GetBaselineData(uint32 sensor);
void Capsense_SetBaselineData(uint32 sensor, uint16 data);
void Capsense_BaseInit(uint32 sensor);

#if (Capsense_IS_DIPLEX_SLIDER)
    uint8 Capsense_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold, const uint8 *diplex);
#else
    uint8 Capsense_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold);
#endif /* (Capsense_IS_DIPLEX_SLIDER) */

#if (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
    uint8 Capsense_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint32 resolution, uint8 noiseThreshold);
#else
    uint8 Capsense_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint16 resolution, uint8 noiseThreshold);
#endif /* (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */


uint8 Capsense_GetFingerThreshold(uint32 widget);
uint8 Capsense_GetNoiseThreshold(uint32 widget);
uint8 Capsense_GetFingerHysteresis(uint32 widget);
uint8 Capsense_GetNegativeNoiseThreshold(uint32 widget);

#if(Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    void Capsense_SetFingerThreshold(uint32 widget, uint8 value);
    void Capsense_SetNoiseThreshold(uint32 widget, uint8 value);
    void Capsense_SetFingerHysteresis(uint32 widget, uint8 value);
    void Capsense_SetNegativeNoiseThreshold(uint32 widget, uint8 value);
    void Capsense_SetDebounce(uint32 widget, uint8 value);
    void Capsense_SetLowBaselineReset(uint32 sensor, uint8 value);
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */

uint8 Capsense_GetDiffCountData(uint32 sensor);
void Capsense_SetDiffCountData(uint32 sensor, uint8 value);

uint32 Capsense_CheckIsSensorActive(uint32 sensor);
uint32 Capsense_CheckIsWidgetActive(uint32 widget);
uint32 Capsense_CheckIsAnyWidgetActive(void);
void Capsense_EnableWidget(uint32 widget);
void Capsense_DisableWidget(uint32 widget);
void Capsense_EnableRawDataFilters(void);
void Capsense_DisableRawDataFilters(void);

#if (Capsense_TOTAL_MATRIX_BUTTONS_COUNT)
    uint32 Capsense_GetMatrixButtonPos(uint32 widget, uint8* pos);
#endif /* (Capsense_TOTAL_MATRIX_BUTTONS_COUNT) */

#if((Capsense_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capsense_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Capsense_GetCentroidPos(uint32 widget);
#endif /* ((Capsense_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capsense_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if((Capsense_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capsense_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Capsense_GetRadialCentroidPos(uint32 widget);
#endif /* #if((Capsense_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capsense_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if (Capsense_TOTAL_TOUCH_PADS_COUNT)
    uint32 Capsense_GetTouchCentroidPos(uint32 widget, uint16* pos);
#endif /* (Capsense_TOTAL_TOUCH_PADS_COUNT) */

uint32 Capsense_GetWidgetNumber(uint32 sensor);
uint8 Capsense_GetLowBaselineReset(uint32 sensor);
uint8 Capsense_GetDebounce(uint32 widget);

/* Filter function prototypes for High level APIs */

/* Median filter function prototype */
#if ( (0u != (Capsense_RAW_FILTER_MASK & Capsense_MEDIAN_FILTER)) || \
      (0u != (Capsense_POS_FILTERS_MASK & Capsense_MEDIAN_FILTER)) || \
      ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)) )
    uint16 Capsense_MedianFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* ( (0u != (Capsense_RAW_FILTER_MASK & Capsense_MEDIAN_FILTER)) || \
      (0u != (Capsense_POS_FILTERS_MASK & Capsense_MEDIAN_FILTER)) || \
      ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)) ) */

/* Averaging filter function prototype */
#if ( (0u != (Capsense_RAW_FILTER_MASK & Capsense_AVERAGING_FILTER)) || \
      (0u != (Capsense_POS_FILTERS_MASK & Capsense_AVERAGING_FILTER)) )
    uint16 Capsense_AveragingFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* Capsense_RAW_FILTER_MASK && Capsense_POS_FILTERS_MASK */

/* IIR2Filter(1/2prev + 1/2cur) filter function prototype */
#if ( (0u != (Capsense_RAW_FILTER_MASK & Capsense_IIR2_FILTER)) || \
      (0u != (Capsense_POS_FILTERS_MASK & Capsense_IIR2_FILTER)) )
    uint16 Capsense_IIR2Filter(uint16 x1, uint16 x2);
#endif /* Capsense_RAW_FILTER_MASK && Capsense_POS_FILTERS_MASK */

/* IIR4Filter(3/4prev + 1/4cur) filter function prototype */
#if ( (0u != (Capsense_RAW_FILTER_MASK & Capsense_IIR4_FILTER)) || \
      (0u != (Capsense_POS_FILTERS_MASK & Capsense_IIR4_FILTER)) )
    uint16 Capsense_IIR4Filter(uint16 x1, uint16 x2);
#endif /* Capsense_RAW_FILTER_MASK && Capsense_POS_FILTERS_MASK */

/* IIR8Filter(7/8prev + 1/8cur) filter function prototype - RawCounts only */
#if (0u != (Capsense_RAW_FILTER_MASK & Capsense_IIR8_FILTER))
    uint16 Capsense_IIR8Filter(uint16 x1, uint16 x2);
#endif /* Capsense_RAW_FILTER_MASK */

/* IIR16Filter(15/16prev + 1/16cur) filter function prototype - RawCounts only */
#if (0u != (Capsense_RAW_FILTER_MASK & Capsense_IIR16_FILTER))
    uint16 Capsense_IIR16Filter(uint16 x1, uint16 x2);
#endif /* Capsense_RAW_FILTER_MASK */

/* JitterFilter filter function prototype */
#if ( (0u != (Capsense_RAW_FILTER_MASK & Capsense_JITTER_FILTER)) || \
      (0u != (Capsense_POS_FILTERS_MASK & Capsense_JITTER_FILTER)) || \
      (0u != (Capsense_TRACKPAD_GEST_POS_FILTERS_MASK & Capsense_JITTER_FILTER)))
    uint16 Capsense_JitterFilter(uint16 x1, uint16 x2);
#endif /* ( (0u != (Capsense_RAW_FILTER_MASK & Capsense_JITTER_FILTER)) || \
       *    (0u != (Capsense_POS_FILTERS_MASK & Capsense_JITTER_FILTER)) )
       *    (0u != (Capsense_TRACKPAD_GEST_POS_FILTERS_MASK & Capsense_JITTER_FILTER)) )
       */


/***************************************
*     Vars with External Linkage
***************************************/
extern uint16 Capsense_sensorBaseline[Capsense_TOTAL_SENSOR_COUNT];
extern uint8  Capsense_sensorBaselineLow[Capsense_TOTAL_SENSOR_COUNT];
extern uint8 Capsense_sensorSignal[Capsense_TOTAL_SENSOR_COUNT];
extern uint8  Capsense_sensorOnMask[Capsense_TOTAL_SENSOR_MASK];

extern uint8 Capsense_lowBaselineResetCnt[Capsense_TOTAL_SENSOR_COUNT];
extern uint8 Capsense_lowBaselineReset[Capsense_TOTAL_SENSOR_COUNT];

/* Generated by Customizer */
#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    extern uint8 Capsense_fingerThreshold[Capsense_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Capsense_noiseThreshold[Capsense_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Capsense_negativeNoiseThreshold[Capsense_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Capsense_hysteresis[Capsense_WIDGET_PARAM_TBL_SIZE];
    extern uint8  Capsense_debounce[Capsense_WIDGET_PARAM_TBL_SIZE];
#else
    extern const uint8 Capsense_fingerThreshold[Capsense_WIDGET_PARAM_TBL_SIZE];
    extern const uint8 Capsense_noiseThreshold[Capsense_WIDGET_PARAM_TBL_SIZE];
    extern const uint8 Capsense_hysteresis[Capsense_WIDGET_PARAM_TBL_SIZE];
    extern const uint8  Capsense_debounce[Capsense_WIDGET_PARAM_TBL_SIZE];
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */

extern const uint8 Capsense_rawDataIndex[Capsense_RAW_DATA_INDEX_TBL_SIZE];
extern const uint8 Capsense_numberOfSensors[Capsense_RAW_DATA_INDEX_TBL_SIZE];

#if (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT)
    #if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
        extern uint8 Capsense_posFiltersMask[Capsense_TOTAL_CENTROID_AXES_COUNT];
    #else
        extern const uint8 Capsense_posFiltersMask[Capsense_TOTAL_CENTROID_AXES_COUNT];
    #endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */

    extern uint32 Capsense_centroidMult[Capsense_TOTAL_CENTROID_AXES_COUNT];
#endif /* (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) */

/***************************************
*        Obsolete Names
***************************************/
#define Capsense_SensorRaw              Capsense_sensorRaw
#define Capsense_SensorEnableMask       Capsense_sensorEnableMask
#define Capsense_SensorBaseline         Capsense_sensorBaseline
#define Capsense_SensorBaselineLow      Capsense_sensorBaselineLow
#define Capsense_SensorSignal           Capsense_sensorSignal
#define Capsense_SensorOnMask           Capsense_sensorOnMask
#define Capsense_LowBaselineResetCnt    Capsense_lowBaselineResetCnt

#endif /* CY_CAPSENSE_CSD_CSHL_Capsense_H */

/* [] END OF FILE */
