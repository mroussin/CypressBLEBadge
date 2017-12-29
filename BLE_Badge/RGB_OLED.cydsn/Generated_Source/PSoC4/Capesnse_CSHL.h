/*******************************************************************************
* File Name: Capesnse_CSHL.h
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

#if !defined(CY_CAPSENSE_CSD_CSHL_Capesnse_H)
#define CY_CAPSENSE_CSD_CSHL_Capesnse_H

#include "Capesnse.h"


/***************************************
*   Condition compilation parameters
***************************************/

#define Capesnse_SIGNAL_SIZE                    (8u)
#define Capesnse_AUTO_RESET                     (0u)
#define Capesnse_RAW_FILTER_MASK                (8u)

/* Signal size definition */
#define Capesnse_SIGNAL_SIZE_UINT8              (8u)
#define Capesnse_SIGNAL_SIZE_UINT16             (16u)

/* Auto reset definition */
#define Capesnse_AUTO_RESET_DISABLE             (0u)
#define Capesnse_AUTO_RESET_ENABLE              (1u)

/* Mask for RAW and POS filters */
#define Capesnse_MEDIAN_FILTER                  (0x01u)
#define Capesnse_AVERAGING_FILTER               (0x02u)
#define Capesnse_IIR2_FILTER                    (0x04u)
#define Capesnse_IIR4_FILTER                    (0x08u)
#define Capesnse_JITTER_FILTER                  (0x10u)
#define Capesnse_IIR8_FILTER                    (0x20u)
#define Capesnse_IIR16_FILTER                   (0x40u)
#define Capesnse_RAW_FILTERS_ENABLED            (0x01u)
#define Capesnse_RAW_FILTERS_DISABLED           (0x00u)

/***************************************
*           API Constants
***************************************/

/* Widgets constants definition */
#define Capesnse_LINEARSLIDER0__LS        (0u)

#define Capesnse_TOTAL_DIPLEXED_SLIDERS_COUNT        (0u)
#define Capesnse_TOTAL_LINEAR_SLIDERS_COUNT          (1u)
#define Capesnse_TOTAL_RADIAL_SLIDERS_COUNT          (0u)
#define Capesnse_TOTAL_TOUCH_PADS_COUNT              (0u)
#define Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT        (0u)
#define Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT       (0u)
#define Capesnse_TOTAL_BUTTONS_COUNT                 (0u)
#define Capesnse_TOTAL_MATRIX_BUTTONS_COUNT          (0u)
#define Capesnse_TOTAL_GENERICS_COUNT                (0u)

#define Capesnse_POS_FILTERS_MASK                    (0x8u)
#define Capesnse_LINEAR_SLIDERS_POS_FILTERS_MASK     (0x8u)
#define Capesnse_RADIAL_SLIDERS_POS_FILTERS_MASK     (0x0u)
#define Capesnse_TOUCH_PADS_POS_FILTERS_MASK         (0x0u)
#define Capesnse_TRACKPAD_GEST_POS_FILTERS_MASK      (0x0u)

#define Capesnse_UNUSED_DEBOUNCE_COUNTER_INDEX       (0u)

#define Capesnse_TOTAL_PROX_SENSORS_COUNT            (0u)

#define Capesnse_END_OF_SLIDERS_INDEX                (0u)
#define Capesnse_END_OF_TOUCH_PAD_INDEX              (0u)
#define Capesnse_END_OF_BUTTONS_INDEX                (0u)
#define Capesnse_END_OF_MATRIX_BUTTONS_INDEX         (0u)
#define Capesnse_END_OF_WIDGETS_INDEX                (1u)



#define Capesnse_TOTAL_SLIDERS_COUNT            ( Capesnse_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Capesnse_TOTAL_RADIAL_SLIDERS_COUNT )

#define Capesnse_TOTAL_CENTROIDS_COUNT          ( Capesnse_TOTAL_SLIDERS_COUNT + \
                                                         (Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
                                                         (Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT * 4u))

#define Capesnse_TOTAL_CENTROIDS_BASIC_COUNT    ( Capesnse_TOTAL_SLIDERS_COUNT + \
                                                         (Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u))

#define Capesnse_TOTAL_CENTROID_AXES_COUNT      ( Capesnse_TOTAL_SLIDERS_COUNT + \
                                                         (Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u) +\
                                                         (Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT * 2u))

#define Capesnse_TOTAL_WIDGET_COUNT             ( Capesnse_TOTAL_LINEAR_SLIDERS_COUNT + \
                                                          Capesnse_TOTAL_RADIAL_SLIDERS_COUNT + \
                                                          Capesnse_TOTAL_TOUCH_PADS_COUNT + \
                                                          Capesnse_TOTAL_BUTTONS_COUNT + \
                                                          Capesnse_TOTAL_MATRIX_BUTTONS_COUNT )

#define Capesnse_ANY_POS_FILTER                 ( Capesnse_MEDIAN_FILTER | \
                                                          Capesnse_AVERAGING_FILTER | \
                                                          Capesnse_IIR2_FILTER | \
                                                          Capesnse_IIR4_FILTER | \
                                                          Capesnse_JITTER_FILTER )

#define Capesnse_IS_DIPLEX_SLIDER               ( Capesnse_TOTAL_DIPLEXED_SLIDERS_COUNT > 0u)

#define Capesnse_IS_NON_DIPLEX_SLIDER           ( (Capesnse_TOTAL_LINEAR_SLIDERS_COUNT - \
                                                           Capesnse_TOTAL_DIPLEXED_SLIDERS_COUNT) > 0u)
#define Capesnse_ADD_SLIDER_TYPE                ((Capesnse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) ? \
                                                        ((Capesnse_TOTAL_TOUCH_PADS_COUNT > 0u) || \
                                                         (Capesnse_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) : 0u)

#define Capesnse_TOTAL_PROX_SENSOR_COUNT        (Capesnse_TOTAL_PROX_SENSORS_COUNT)

#define Capesnse_WIDGETS_TBL_SIZE               ( Capesnse_TOTAL_WIDGET_COUNT + \
                                                          Capesnse_TOTAL_GENERICS_COUNT)

#define Capesnse_WIDGET_PARAM_TBL_SIZE          (Capesnse_TOTAL_BUTTONS_COUNT + \
                                                         Capesnse_TOTAL_SLIDERS_COUNT +\
                                                         Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u + \
                                                         Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT * 2u +\
                                                         Capesnse_TOTAL_MATRIX_BUTTONS_COUNT * 2u)

#define Capesnse_THRESHOLD_TBL_SIZE         (Capesnse_WIDGET_PARAM_TBL_SIZE)
#define Capesnse_DEBOUNCE_CNT_TBL_SIZE      (Capesnse_WIDGET_PARAM_TBL_SIZE)
#define Capesnse_RAW_DATA_INDEX_TBL_SIZE    (Capesnse_WIDGET_PARAM_TBL_SIZE +\
                                                     Capesnse_TOTAL_GENERICS_COUNT)

#define Capesnse_RES_MULT                   (256u)


#define Capesnse_NOT_WIDGET                     (0xFFFFFFFFu)

/*Types of centroids */
#define Capesnse_TYPE_RADIAL_SLIDER             (0x01u)
#define Capesnse_TYPE_LINEAR_SLIDER             (0x02u)
#define Capesnse_TYPE_GENERIC                   (0xFFu)

/* Defines if sensors or widgets are active */
#define Capesnse_SENSOR_IS_ACTIVE               (0x01u)
#define Capesnse_SENSOR_1_IS_ACTIVE             (0x01u)
#define Capesnse_SENSOR_2_IS_ACTIVE             (0x02u)
#define Capesnse_WIDGET_IS_ACTIVE               (0x01u)

/* Defines diplex type of Slider */
#define Capesnse_IS_DIPLEX                      (0x80u)

/* Defines fingers positions on Slider  */
#define Capesnse_POS_PREV                       (0u)
#define Capesnse_POS                            (1u)
#define Capesnse_POS_NEXT                       (2u)
#define Capesnse_CENTROID_ROUND_VALUE           (0x7F00u)
#define Capesnse_MAXIMUM_CENTROID               (0xFFu)

#define Capesnse_NEGATIVE_NOISE_THRESHOLD       (20u)
#define Capesnse_LOW_BASELINE_RESET             (5u)


/***************************************
*        Function Prototypes
***************************************/

void Capesnse_InitializeSensorBaseline(uint32 sensor);
void Capesnse_InitializeAllBaselines(void);
void Capesnse_InitializeEnabledBaselines(void);
void Capesnse_UpdateSensorBaseline(uint32 sensor);
void Capesnse_UpdateBaselineNoThreshold(uint32 sensor);
void Capesnse_UpdateEnabledBaselines(void);
void Capesnse_UpdateWidgetBaseline(uint32 widget);
uint16 Capesnse_GetBaselineData(uint32 sensor);
void Capesnse_SetBaselineData(uint32 sensor, uint16 data);
void Capesnse_BaseInit(uint32 sensor);

#if (Capesnse_IS_DIPLEX_SLIDER)
    uint8 Capesnse_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold, const uint8 *diplex);
#else
    uint8 Capesnse_FindMaximum(uint8 offset, uint8 count, uint8 fingerThreshold);
#endif /* (Capesnse_IS_DIPLEX_SLIDER) */

#if (Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
    uint8 Capesnse_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint32 resolution, uint8 noiseThreshold);
#else
    uint8 Capesnse_CalcCentroid(uint8 maximum, uint8 offset, \
                                    uint8 count, uint16 resolution, uint8 noiseThreshold);
#endif /* (Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */


uint8 Capesnse_GetFingerThreshold(uint32 widget);
uint8 Capesnse_GetNoiseThreshold(uint32 widget);
uint8 Capesnse_GetFingerHysteresis(uint32 widget);
uint8 Capesnse_GetNegativeNoiseThreshold(uint32 widget);

#if(Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    void Capesnse_SetFingerThreshold(uint32 widget, uint8 value);
    void Capesnse_SetNoiseThreshold(uint32 widget, uint8 value);
    void Capesnse_SetFingerHysteresis(uint32 widget, uint8 value);
    void Capesnse_SetNegativeNoiseThreshold(uint32 widget, uint8 value);
    void Capesnse_SetDebounce(uint32 widget, uint8 value);
    void Capesnse_SetLowBaselineReset(uint32 sensor, uint8 value);
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */

uint8 Capesnse_GetDiffCountData(uint32 sensor);
void Capesnse_SetDiffCountData(uint32 sensor, uint8 value);

uint32 Capesnse_CheckIsSensorActive(uint32 sensor);
uint32 Capesnse_CheckIsWidgetActive(uint32 widget);
uint32 Capesnse_CheckIsAnyWidgetActive(void);
void Capesnse_EnableWidget(uint32 widget);
void Capesnse_DisableWidget(uint32 widget);
void Capesnse_EnableRawDataFilters(void);
void Capesnse_DisableRawDataFilters(void);

#if (Capesnse_TOTAL_MATRIX_BUTTONS_COUNT)
    uint32 Capesnse_GetMatrixButtonPos(uint32 widget, uint8* pos);
#endif /* (Capesnse_TOTAL_MATRIX_BUTTONS_COUNT) */

#if((Capesnse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capesnse_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Capesnse_GetCentroidPos(uint32 widget);
#endif /* ((Capesnse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capesnse_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if((Capesnse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capesnse_TOTAL_LINEAR_SLIDERS_COUNT > 0u))
    uint16 Capesnse_GetRadialCentroidPos(uint32 widget);
#endif /* #if((Capesnse_TOTAL_RADIAL_SLIDERS_COUNT > 0u) || (Capesnse_TOTAL_LINEAR_SLIDERS_COUNT > 0u)) */
#if (Capesnse_TOTAL_TOUCH_PADS_COUNT)
    uint32 Capesnse_GetTouchCentroidPos(uint32 widget, uint16* pos);
#endif /* (Capesnse_TOTAL_TOUCH_PADS_COUNT) */

uint32 Capesnse_GetWidgetNumber(uint32 sensor);
uint8 Capesnse_GetLowBaselineReset(uint32 sensor);
uint8 Capesnse_GetDebounce(uint32 widget);

/* Filter function prototypes for High level APIs */

/* Median filter function prototype */
#if ( (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_MEDIAN_FILTER)) || \
      (0u != (Capesnse_POS_FILTERS_MASK & Capesnse_MEDIAN_FILTER)) || \
      ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)) )
    uint16 Capesnse_MedianFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* ( (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_MEDIAN_FILTER)) || \
      (0u != (Capesnse_POS_FILTERS_MASK & Capesnse_MEDIAN_FILTER)) || \
      ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)) ) */

/* Averaging filter function prototype */
#if ( (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_AVERAGING_FILTER)) || \
      (0u != (Capesnse_POS_FILTERS_MASK & Capesnse_AVERAGING_FILTER)) )
    uint16 Capesnse_AveragingFilter(uint16 x1, uint16 x2, uint16 x3);
#endif /* Capesnse_RAW_FILTER_MASK && Capesnse_POS_FILTERS_MASK */

/* IIR2Filter(1/2prev + 1/2cur) filter function prototype */
#if ( (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_IIR2_FILTER)) || \
      (0u != (Capesnse_POS_FILTERS_MASK & Capesnse_IIR2_FILTER)) )
    uint16 Capesnse_IIR2Filter(uint16 x1, uint16 x2);
#endif /* Capesnse_RAW_FILTER_MASK && Capesnse_POS_FILTERS_MASK */

/* IIR4Filter(3/4prev + 1/4cur) filter function prototype */
#if ( (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_IIR4_FILTER)) || \
      (0u != (Capesnse_POS_FILTERS_MASK & Capesnse_IIR4_FILTER)) )
    uint16 Capesnse_IIR4Filter(uint16 x1, uint16 x2);
#endif /* Capesnse_RAW_FILTER_MASK && Capesnse_POS_FILTERS_MASK */

/* IIR8Filter(7/8prev + 1/8cur) filter function prototype - RawCounts only */
#if (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_IIR8_FILTER))
    uint16 Capesnse_IIR8Filter(uint16 x1, uint16 x2);
#endif /* Capesnse_RAW_FILTER_MASK */

/* IIR16Filter(15/16prev + 1/16cur) filter function prototype - RawCounts only */
#if (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_IIR16_FILTER))
    uint16 Capesnse_IIR16Filter(uint16 x1, uint16 x2);
#endif /* Capesnse_RAW_FILTER_MASK */

/* JitterFilter filter function prototype */
#if ( (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_JITTER_FILTER)) || \
      (0u != (Capesnse_POS_FILTERS_MASK & Capesnse_JITTER_FILTER)) || \
      (0u != (Capesnse_TRACKPAD_GEST_POS_FILTERS_MASK & Capesnse_JITTER_FILTER)))
    uint16 Capesnse_JitterFilter(uint16 x1, uint16 x2);
#endif /* ( (0u != (Capesnse_RAW_FILTER_MASK & Capesnse_JITTER_FILTER)) || \
       *    (0u != (Capesnse_POS_FILTERS_MASK & Capesnse_JITTER_FILTER)) )
       *    (0u != (Capesnse_TRACKPAD_GEST_POS_FILTERS_MASK & Capesnse_JITTER_FILTER)) )
       */


/***************************************
*     Vars with External Linkage
***************************************/
extern uint16 Capesnse_sensorBaseline[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8  Capesnse_sensorBaselineLow[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8 Capesnse_sensorSignal[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8  Capesnse_sensorOnMask[Capesnse_TOTAL_SENSOR_MASK];

extern uint8 Capesnse_lowBaselineResetCnt[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8 Capesnse_lowBaselineReset[Capesnse_TOTAL_SENSOR_COUNT];

/* Generated by Customizer */
#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    extern uint8 Capesnse_fingerThreshold[Capesnse_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Capesnse_noiseThreshold[Capesnse_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Capesnse_negativeNoiseThreshold[Capesnse_WIDGET_PARAM_TBL_SIZE];
    extern uint8 Capesnse_hysteresis[Capesnse_WIDGET_PARAM_TBL_SIZE];
    extern uint8  Capesnse_debounce[Capesnse_WIDGET_PARAM_TBL_SIZE];
#else
    extern const uint8 Capesnse_fingerThreshold[Capesnse_WIDGET_PARAM_TBL_SIZE];
    extern const uint8 Capesnse_noiseThreshold[Capesnse_WIDGET_PARAM_TBL_SIZE];
    extern const uint8 Capesnse_hysteresis[Capesnse_WIDGET_PARAM_TBL_SIZE];
    extern const uint8  Capesnse_debounce[Capesnse_WIDGET_PARAM_TBL_SIZE];
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */

extern const uint8 Capesnse_rawDataIndex[Capesnse_RAW_DATA_INDEX_TBL_SIZE];
extern const uint8 Capesnse_numberOfSensors[Capesnse_RAW_DATA_INDEX_TBL_SIZE];

#if (0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT)
    #if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
        extern uint8 Capesnse_posFiltersMask[Capesnse_TOTAL_CENTROID_AXES_COUNT];
    #else
        extern const uint8 Capesnse_posFiltersMask[Capesnse_TOTAL_CENTROID_AXES_COUNT];
    #endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */

    extern uint32 Capesnse_centroidMult[Capesnse_TOTAL_CENTROID_AXES_COUNT];
#endif /* (0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT) */

/***************************************
*        Obsolete Names
***************************************/
#define Capesnse_SensorRaw              Capesnse_sensorRaw
#define Capesnse_SensorEnableMask       Capesnse_sensorEnableMask
#define Capesnse_SensorBaseline         Capesnse_sensorBaseline
#define Capesnse_SensorBaselineLow      Capesnse_sensorBaselineLow
#define Capesnse_SensorSignal           Capesnse_sensorSignal
#define Capesnse_SensorOnMask           Capesnse_sensorOnMask
#define Capesnse_LowBaselineResetCnt    Capesnse_lowBaselineResetCnt

#endif /* CY_CAPSENSE_CSD_CSHL_Capesnse_H */

/* [] END OF FILE */
