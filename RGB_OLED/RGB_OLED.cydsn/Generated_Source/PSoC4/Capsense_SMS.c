/*******************************************************************************
* File Name: Capsense_SMS.c
* Version 2.60
*
* Description:
*  This file provides the source code of wrapper between CapSense CSD component
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

#include "Capsense_SMS.h"
#include "Capsense_PVT.h"

#include "cytypes.h"

#if (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)


/*  Real Charge Divider values */
#if ((0u != Capsense_IS_M0S8PERI_BLOCK) && (0u == CY_PSOC4_4000))
    /*  Dividers are not chained */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            4u,     4u,     4u,    8u,    8u,    8u,    8u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u
        };
    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
        };
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */

#elif (0u != Capsense_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained (PSoC 4000) */
    #if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };

    #elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
        };
    #else
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */

#else
    /*  Dividers are chained (PSoC 4100, PSoC 4200) */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
        };

    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        const uint8 Capsense_prescalerTable[Capsense_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* ((0u != Capsense_IS_M0S8PERI_BLOCK) && (0u == CY_PSOC4_4000)) */


uint8 Capsense_noiseEnvelopeTbl[Capsense_TOTAL_SENSOR_COUNT];
uint8 Capsense_runningDifferenceTbl[Capsense_TOTAL_SENSOR_COUNT];
uint8 Capsense_signRegisterTbl[Capsense_TOTAL_SENSOR_COUNT];
uint16 Capsense_sampleMinTbl[Capsense_TOTAL_SENSOR_COUNT];
uint16 Capsense_sampleMaxTbl[Capsense_TOTAL_SENSOR_COUNT];
uint16 Capsense_previousSampleTbl[Capsense_TOTAL_SENSOR_COUNT];
uint8 Capsense_kValueTbl[Capsense_TOTAL_SENSOR_COUNT];

uint8 Capsense_sensorSensitivity[] = {
    2u, 
};

Capsense_CONFIG_TYPE_P4_v2_60 Capsense_config;
Capsense_CONFIG_TYPE_POINTERS_P4_v2_60 const Capsense_configPointers = {
    Capsense_modulationIDAC,
    Capsense_compensationIDAC,
    Capsense_sensorSensitivity,
    Capsense_senseClkDividerVal,
    Capsense_sampleClkDividerVal,
    Capsense_widgetNumber,
    Capsense_widgetResolution,
    Capsense_noiseEnvelopeTbl,
    Capsense_runningDifferenceTbl,
    Capsense_signRegisterTbl,
    Capsense_sampleMinTbl,
    Capsense_sampleMaxTbl,
    Capsense_previousSampleTbl,
    Capsense_kValueTbl,
    Capsense_scanSpeedTbl,
    Capsense_prescalerTable,
    (const uint8  *)Capsense_rawDataIndex,
    (const uint8  *)Capsense_numberOfSensors,
    &Capsense_GetSensorRaw,
    &Capsense_PreScan,
    &Capsense_ReadSensorRaw,
    &Capsense_GetBitValue,
    &Capsense_SetBitValue
};


/*******************************************************************************
* Function Name: Capsense_AutoTune
********************************************************************************
*
* Summary:
*  Provides the tuning procedure for all sensors.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*   Capsense_config: the structure which contains configuration parameters
*   Capsense_senseClkDividerVal[]: used to store the Analog Switch
*   dividers for each sensor.
*   Capsense_senseClkDividerVal[]: used to store the
*    Analog Switch divider for all sensors.
*   Capsense_prescalersTuningDone - used to notify the Tuner GUI that
*   the pre-scalers tuning  is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_AutoTune(void)
{
    uint32 curSensor;
    uint32 widget;

    Capsense_config.totalScanslotsNum = Capsense_TOTAL_SCANSLOT_COUNT;
    Capsense_config.totalWidgetsNum = Capsense_END_OF_WIDGETS_INDEX;
    Capsense_config.totalSensorsNum = Capsense_TOTAL_SENSOR_COUNT;

    Capsense_config.hfclkFreq_MHz = CYDEV_BCLK__HFCLK__MHZ;
    Capsense_config.sensetivitySeed = Capsense_SENSETIVITY_FACTOR;

    Capsense_config.pointers = &Capsense_configPointers;

    Capsense_SetAnalogSwitchesSrcDirect();

    for(widget = 0u; widget < Capsense_config.totalWidgetsNum; widget++)
    {
        Capsense_widgetResolution[widget] = Capsense_CALIBRATION_RESOLUTION;
    }


    for(curSensor = 0u; curSensor < Capsense_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
        Capsense_senseClkDividerVal[curSensor] = Capsense_CALIBRATION_ASD;
        Capsense_sampleClkDividerVal[curSensor] = Capsense_CALIBRATION_MD;
    }

    Capsense_DisableBaselineIDAC();

    CalibrateSensors_P4_v2_60(&Capsense_config, Capsense_CAL_RAW_COUNT);

    #if(0u != Capsense_TOTAL_CENTROIDS_COUNT)
        Capsense_NormalizeWidgets(Capsense_END_OF_WIDGETS_INDEX, Capsense_modulationIDAC);
    #endif /* (0u != Capsense_TOTAL_CENTROIDS_COUNT)  */

    TunePrescalers_P4_v2_60(&Capsense_config);

    #if(Capsense_PRS_OPTIONS != Capsense__PRS_NONE)
        Capsense_prescalersTuningDone = 1u;
    #endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_NONE) */

    CalibrateSensors_P4_v2_60(&Capsense_config, Capsense_CAL_RAW_COUNT);
    #if(0u != Capsense_TOTAL_CENTROIDS_COUNT)
        Capsense_NormalizeWidgets(Capsense_END_OF_WIDGETS_INDEX, Capsense_modulationIDAC);
    #endif /* (0u != Capsense_TOTAL_CENTROIDS_COUNT)  */
    TuneSensitivity_P4_v2_60(&Capsense_config);

    Capsense_EnableBaselineIDAC(&Capsense_config);

    for(curSensor = 0u; curSensor < Capsense_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
        Capsense_UpdateThresholds(curSensor);
    }
}


/*******************************************************************************
* Function Name: Capsense_UpdateThresholds
********************************************************************************
*
* Summary:
*  The API updates the Finger Threshold, Hysteresis, Noise Threshold, and
*  Negative Noise Threshold in the AutoTuning (Smartsense) Mode.
*
* Parameters:
*  sensor: sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Capsense_widgetNumber[] - contains widget Number for given sensor.
*  0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  For other sensors the array element equals to 255.
*  Capsense_numberOfSensors[widget] - Number of sensors in the widget.
*   Capsense_fingerThreshold[] - contains the level of signal for each sensor
*   that determines if a finger present on the sensor.
*   Capsense_negativeNoiseThreshold[] - negative Noise Threshold
*   Capsense_hysteresis[] - the array with hysteresis values.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_UpdateThresholds(uint32 sensor)
{
    uint8 fingerThreshold;
    uint32 widget;

    widget = Capsense_widgetNumber[sensor];

    #if(0u != Capsense_FLEXIBLE_THRESHOLDS_EN)
        fingerThreshold = Capsense_fingerThreshold[widget];
        /* Update Threshold manually (flexible) */
        Capsense_noiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
        Capsense_negativeNoiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
        Capsense_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
    #else
        /* Calculate Finger Threshold and Noise Threshold with Smartsense (automatic) */
        CalculateThresholds_P4_v2_60(&Capsense_config, (uint8)sensor, (uint8)widget, Capsense_fingerThreshold, Capsense_noiseThreshold);

        fingerThreshold = Capsense_fingerThreshold[widget];

        /* Update Threshold based on calculated with Smartsense (automatic) */
        Capsense_negativeNoiseThreshold[widget] = Capsense_noiseThreshold[widget];

        if(fingerThreshold < Capsense_THRESHOLD_1)
        {
            Capsense_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
        }
        else if(fingerThreshold <  Capsense_THRESHOLD_2)
        {
            Capsense_hysteresis[widget] = (uint8)(fingerThreshold >> 4u);
        }
        else if(fingerThreshold <  Capsense_THRESHOLD_3)
        {
            Capsense_hysteresis[widget] = (uint8)(fingerThreshold >> 5u);
        }
        else if(fingerThreshold <  Capsense_THRESHOLD_4)
        {
            Capsense_hysteresis[widget] = (uint8)(fingerThreshold >> 6u);
        }
        else
        {
            Capsense_hysteresis[widget] = 0u;
        }
    #endif /* (0u != Capsense_FLEXIBLE_THRESHOLDS_EN)  */
}

/*******************************************************************************
* Function Name: Capsense_SetAnalogSwitchesSrcDirect
********************************************************************************
*
* Summary:
*  This API switches the charge clock source to prescaler.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_SetAnalogSwitchesSrcDirect(void)
{
    Capsense_CSD_CFG_REG &= ~(Capsense_CSD_CFG_PRS_SELECT);
}


/*******************************************************************************
* Function Name: Capsense_DisableBaselineIDAC
********************************************************************************
*
* Summary:
*  The API disables the Compensation IDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_DisableBaselineIDAC(void)
{
    uint32 newRegValue;

    newRegValue = Capsense_CSD_IDAC_REG;

    newRegValue &= ~(Capsense_CSD_IDAC1_MODE_MASK | Capsense_CSD_IDAC2_MODE_MASK);
    newRegValue |= Capsense_CSD_IDAC1_MODE_VARIABLE;

    Capsense_CSD_IDAC_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Capsense_EnableBaselineIDAC
********************************************************************************
*
* Summary:
*  The API enables the Compensation IDAC.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_compensationIDAC[] - used to store a 7-bit idac value for all the sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void  Capsense_EnableBaselineIDAC(const Capsense_CONFIG_TYPE_P4_v2_60 *config)
{
    uint32 curSensor;
    uint32 newRegValue;

    for(curSensor = 0u; curSensor < config->totalScanslotsNum; curSensor++)
    {
        Capsense_compensationIDAC[curSensor] = Capsense_modulationIDAC[curSensor] / 2u;
        Capsense_modulationIDAC[curSensor] = (Capsense_modulationIDAC[curSensor] + 1u) / 2u;
    }

    CyIntDisable(Capsense_ISR_NUMBER);

    newRegValue = Capsense_CSD_IDAC_REG;
    newRegValue &= ~(Capsense_CSD_IDAC1_MODE_MASK | Capsense_CSD_IDAC2_MODE_MASK);
    newRegValue |= (Capsense_CSD_IDAC1_MODE_VARIABLE | Capsense_CSD_IDAC2_MODE_FIXED);
    Capsense_CSD_IDAC_REG = newRegValue;

    CyIntEnable(Capsense_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Capsense_SetSensitivity
********************************************************************************
*
* Summary:
*  This API sets the sensitivity value for the sensor. The sensitivity
*  value is used during the auto-tuning algorithm executed as part of the CapSense_Start API.
*  This API is called by the application layer prior to calling the CapSense_Start API.
*  Calling this API after execution of CapSense_Start API has no effect.
*
* Parameters:
*  sensor  Sensor index
*  data    Sensitivity of the sensor. Possible values are below
*  1 - 0.1pf sensitivity
*  2 - 0.2pf sensitivity
*  3 - 0.3pf sensitivity
*  4 - 0.4pf sensitivity
*  5 - 0.5pf sensitivity
*  6 - 0.6pf sensitivity
*  7 - 0.7pf sensitivity
*  8 - 0.8pf sensitivity
*  9 - 0.9pf sensitivity
*  10 - 1.0pf sensitivity
*  All other values, set sensitivity to 1.0pf
*
*  Return Value:
*   None
*
* Global Variables:
* Capsense_sensorSensitivity[] - This 8-bits array contains Sensitivity in the LSB nibble
*                                and Noise Reset Counter value in the MSB nibble.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_SetSensitivity(uint32 sensor, uint32 value)
{
    if(value > 10u)
    {
        value = 10u;
    }

    /* Clear SensorSensitivity value in LSB nibble */
    Capsense_sensorSensitivity[sensor] &= (uint8)~Capsense_SENSITIVITY_MASK;
    /* Set SensorSensitivity value in LSB nibble */
    Capsense_sensorSensitivity[sensor] |= (uint8)value;
}


/*******************************************************************************
* Function Name: Capsense_GetSensitivityCoefficient
********************************************************************************
*
* Summary:
*  This API returns the K coefficient for the appropriate sensor.
*
* Parameters:
*  sensor:  Sensor index
*
*  Return Value:
*   K - value for the appropriate sensor.
*
* Global Variables:
*  Capsense_kValueTbl[] - This 8-bits array contains the K value for each sensor.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint8 Capsense_GetSensitivityCoefficient(uint32 sensor)
{
    return (Capsense_kValueTbl[sensor]);
}


/*******************************************************************************
* Function Name: Capsense_GetNoiseEnvelope
********************************************************************************
*
* Summary:
*  This function returns the noise value of a
*  component.
*
* Parameters:
*  sensor - Sensor number.
*
* Return:
*  The noise envelope value of the sensor
*  indicated by argument.
*
* Global Variables:
*  Capsense_noiseEnvelopeTbl[] - array with Noise Envelope.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Capsense_GetNoiseEnvelope(uint32 sensor)
{
    return((uint16)((uint16)Capsense_noiseEnvelopeTbl[sensor] << 1u) + 1u);
}


/*******************************************************************************
* Function Name: Capsense_GetNormalizedDiffCountData
********************************************************************************
*
* Summary:
*  This API returns normalized difference count data.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  This API returns a normalized count value of the sensor indicated by the
*  argument.
*
* Global Variables:
*  Capsense_sensorSignal[] - array with difference counts for sensors
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Capsense_GetNormalizedDiffCountData(uint32 sensor)
{
    return (uint16)(((uint32)Capsense_sensorSignal[sensor] << 7u) / Capsense_kValueTbl[sensor]);
}

#endif /* (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */

/* [] END OF FILE */
