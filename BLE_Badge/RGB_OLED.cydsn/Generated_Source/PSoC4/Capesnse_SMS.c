/*******************************************************************************
* File Name: Capesnse_SMS.c
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

#include "Capesnse_SMS.h"
#include "Capesnse_PVT.h"

#include "cytypes.h"

#if (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)


/*  Real Charge Divider values */
#if ((0u != Capesnse_IS_M0S8PERI_BLOCK) && (0u == CY_PSOC4_4000))
    /*  Dividers are not chained */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            4u,     4u,     4u,    8u,    8u,    8u,    8u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u,    16u
        };
    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
        };
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */

#elif (0u != Capesnse_IS_M0S8PERI_BLOCK)
    /*  Dividers are not chained (PSoC 4000) */
    #if (CYDEV_BCLK__HFCLK__MHZ >= 12u)
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };

    #elif (CYDEV_BCLK__HFCLK__MHZ >= 6u)
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
        };
    #else
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u,    1u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */

#else
    /*  Dividers are chained (PSoC 4100, PSoC 4200) */
    #if (CYDEV_BCLK__HFCLK__MHZ > 24u)
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            2u,     2u,     2u,    4u,    4u,    4u,    4u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u,    8u
        };

    #elif (CYDEV_BCLK__HFCLK__MHZ >12u)
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    2u,    2u,    2u,    2u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u,    4u
        };
    #else   /* (CYDEV_BCLK__HFCLK__MHZ > 12u) */
        const uint8 Capesnse_prescalerTable[Capesnse_PRESCALERS_TBL_SIZE] = {
            1u,     1u,     1u,    1u,    1u,    1u,    1u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u,    2u
        };
    #endif /* (CYDEV_BCLK__HFCLK__MHZ > 24u) */
#endif /* ((0u != Capesnse_IS_M0S8PERI_BLOCK) && (0u == CY_PSOC4_4000)) */


uint8 Capesnse_noiseEnvelopeTbl[Capesnse_TOTAL_SENSOR_COUNT];
uint8 Capesnse_runningDifferenceTbl[Capesnse_TOTAL_SENSOR_COUNT];
uint8 Capesnse_signRegisterTbl[Capesnse_TOTAL_SENSOR_COUNT];
uint16 Capesnse_sampleMinTbl[Capesnse_TOTAL_SENSOR_COUNT];
uint16 Capesnse_sampleMaxTbl[Capesnse_TOTAL_SENSOR_COUNT];
uint16 Capesnse_previousSampleTbl[Capesnse_TOTAL_SENSOR_COUNT];
uint8 Capesnse_kValueTbl[Capesnse_TOTAL_SENSOR_COUNT];

uint8 Capesnse_sensorSensitivity[] = {
    2u, 2u, 2u, 2u, 2u, 
};

Capesnse_CONFIG_TYPE_P4_v2_60 Capesnse_config;
Capesnse_CONFIG_TYPE_POINTERS_P4_v2_60 const Capesnse_configPointers = {
    Capesnse_modulationIDAC,
    Capesnse_compensationIDAC,
    Capesnse_sensorSensitivity,
    Capesnse_senseClkDividerVal,
    Capesnse_sampleClkDividerVal,
    Capesnse_widgetNumber,
    Capesnse_widgetResolution,
    Capesnse_noiseEnvelopeTbl,
    Capesnse_runningDifferenceTbl,
    Capesnse_signRegisterTbl,
    Capesnse_sampleMinTbl,
    Capesnse_sampleMaxTbl,
    Capesnse_previousSampleTbl,
    Capesnse_kValueTbl,
    Capesnse_scanSpeedTbl,
    Capesnse_prescalerTable,
    (const uint8  *)Capesnse_rawDataIndex,
    (const uint8  *)Capesnse_numberOfSensors,
    &Capesnse_GetSensorRaw,
    &Capesnse_PreScan,
    &Capesnse_ReadSensorRaw,
    &Capesnse_GetBitValue,
    &Capesnse_SetBitValue
};


/*******************************************************************************
* Function Name: Capesnse_AutoTune
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
*   Capesnse_config: the structure which contains configuration parameters
*   Capesnse_senseClkDividerVal[]: used to store the Analog Switch
*   dividers for each sensor.
*   Capesnse_senseClkDividerVal[]: used to store the
*    Analog Switch divider for all sensors.
*   Capesnse_prescalersTuningDone - used to notify the Tuner GUI that
*   the pre-scalers tuning  is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_AutoTune(void)
{
    uint32 curSensor;
    uint32 widget;

    Capesnse_config.totalScanslotsNum = Capesnse_TOTAL_SCANSLOT_COUNT;
    Capesnse_config.totalWidgetsNum = Capesnse_END_OF_WIDGETS_INDEX;
    Capesnse_config.totalSensorsNum = Capesnse_TOTAL_SENSOR_COUNT;

    Capesnse_config.hfclkFreq_MHz = CYDEV_BCLK__HFCLK__MHZ;
    Capesnse_config.sensetivitySeed = Capesnse_SENSETIVITY_FACTOR;

    Capesnse_config.pointers = &Capesnse_configPointers;

    Capesnse_SetAnalogSwitchesSrcDirect();

    for(widget = 0u; widget < Capesnse_config.totalWidgetsNum; widget++)
    {
        Capesnse_widgetResolution[widget] = Capesnse_CALIBRATION_RESOLUTION;
    }


    for(curSensor = 0u; curSensor < Capesnse_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
        Capesnse_senseClkDividerVal[curSensor] = Capesnse_CALIBRATION_ASD;
        Capesnse_sampleClkDividerVal[curSensor] = Capesnse_CALIBRATION_MD;
    }

    Capesnse_DisableBaselineIDAC();

    CalibrateSensors_P4_v2_60(&Capesnse_config, Capesnse_CAL_RAW_COUNT);

    #if(0u != Capesnse_TOTAL_CENTROIDS_COUNT)
        Capesnse_NormalizeWidgets(Capesnse_END_OF_WIDGETS_INDEX, Capesnse_modulationIDAC);
    #endif /* (0u != Capesnse_TOTAL_CENTROIDS_COUNT)  */

    TunePrescalers_P4_v2_60(&Capesnse_config);

    #if(Capesnse_PRS_OPTIONS != Capesnse__PRS_NONE)
        Capesnse_prescalersTuningDone = 1u;
    #endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE) */

    CalibrateSensors_P4_v2_60(&Capesnse_config, Capesnse_CAL_RAW_COUNT);
    #if(0u != Capesnse_TOTAL_CENTROIDS_COUNT)
        Capesnse_NormalizeWidgets(Capesnse_END_OF_WIDGETS_INDEX, Capesnse_modulationIDAC);
    #endif /* (0u != Capesnse_TOTAL_CENTROIDS_COUNT)  */
    TuneSensitivity_P4_v2_60(&Capesnse_config);

    Capesnse_EnableBaselineIDAC(&Capesnse_config);

    for(curSensor = 0u; curSensor < Capesnse_TOTAL_SCANSLOT_COUNT; curSensor++)
    {
        Capesnse_UpdateThresholds(curSensor);
    }
}


/*******************************************************************************
* Function Name: Capesnse_UpdateThresholds
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
*  Capesnse_widgetNumber[] - contains widget Number for given sensor.
*  0 through 7 (sensor 0 is bit 0, sensor 1 is bit 1).
*  For other sensors the array element equals to 255.
*  Capesnse_numberOfSensors[widget] - Number of sensors in the widget.
*   Capesnse_fingerThreshold[] - contains the level of signal for each sensor
*   that determines if a finger present on the sensor.
*   Capesnse_negativeNoiseThreshold[] - negative Noise Threshold
*   Capesnse_hysteresis[] - the array with hysteresis values.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_UpdateThresholds(uint32 sensor)
{
    uint8 fingerThreshold;
    uint32 widget;

    widget = Capesnse_widgetNumber[sensor];

    #if(0u != Capesnse_FLEXIBLE_THRESHOLDS_EN)
        fingerThreshold = Capesnse_fingerThreshold[widget];
        /* Update Threshold manually (flexible) */
        Capesnse_noiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
        Capesnse_negativeNoiseThreshold[widget] = (uint8)(fingerThreshold >> 1u);
        Capesnse_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
    #else
        /* Calculate Finger Threshold and Noise Threshold with Smartsense (automatic) */
        CalculateThresholds_P4_v2_60(&Capesnse_config, (uint8)sensor, (uint8)widget, Capesnse_fingerThreshold, Capesnse_noiseThreshold);

        fingerThreshold = Capesnse_fingerThreshold[widget];

        /* Update Threshold based on calculated with Smartsense (automatic) */
        Capesnse_negativeNoiseThreshold[widget] = Capesnse_noiseThreshold[widget];

        if(fingerThreshold < Capesnse_THRESHOLD_1)
        {
            Capesnse_hysteresis[widget] = (uint8)(fingerThreshold >> 3u);
        }
        else if(fingerThreshold <  Capesnse_THRESHOLD_2)
        {
            Capesnse_hysteresis[widget] = (uint8)(fingerThreshold >> 4u);
        }
        else if(fingerThreshold <  Capesnse_THRESHOLD_3)
        {
            Capesnse_hysteresis[widget] = (uint8)(fingerThreshold >> 5u);
        }
        else if(fingerThreshold <  Capesnse_THRESHOLD_4)
        {
            Capesnse_hysteresis[widget] = (uint8)(fingerThreshold >> 6u);
        }
        else
        {
            Capesnse_hysteresis[widget] = 0u;
        }
    #endif /* (0u != Capesnse_FLEXIBLE_THRESHOLDS_EN)  */
}

/*******************************************************************************
* Function Name: Capesnse_SetAnalogSwitchesSrcDirect
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
void Capesnse_SetAnalogSwitchesSrcDirect(void)
{
    Capesnse_CSD_CFG_REG &= ~(Capesnse_CSD_CFG_PRS_SELECT);
}


/*******************************************************************************
* Function Name: Capesnse_DisableBaselineIDAC
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
void Capesnse_DisableBaselineIDAC(void)
{
    uint32 newRegValue;

    newRegValue = Capesnse_CSD_IDAC_REG;

    newRegValue &= ~(Capesnse_CSD_IDAC1_MODE_MASK | Capesnse_CSD_IDAC2_MODE_MASK);
    newRegValue |= Capesnse_CSD_IDAC1_MODE_VARIABLE;

    Capesnse_CSD_IDAC_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Capesnse_EnableBaselineIDAC
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
*  Capesnse_compensationIDAC[] - used to store a 7-bit idac value for all the sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void  Capesnse_EnableBaselineIDAC(const Capesnse_CONFIG_TYPE_P4_v2_60 *config)
{
    uint32 curSensor;
    uint32 newRegValue;

    for(curSensor = 0u; curSensor < config->totalScanslotsNum; curSensor++)
    {
        Capesnse_compensationIDAC[curSensor] = Capesnse_modulationIDAC[curSensor] / 2u;
        Capesnse_modulationIDAC[curSensor] = (Capesnse_modulationIDAC[curSensor] + 1u) / 2u;
    }

    CyIntDisable(Capesnse_ISR_NUMBER);

    newRegValue = Capesnse_CSD_IDAC_REG;
    newRegValue &= ~(Capesnse_CSD_IDAC1_MODE_MASK | Capesnse_CSD_IDAC2_MODE_MASK);
    newRegValue |= (Capesnse_CSD_IDAC1_MODE_VARIABLE | Capesnse_CSD_IDAC2_MODE_FIXED);
    Capesnse_CSD_IDAC_REG = newRegValue;

    CyIntEnable(Capesnse_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Capesnse_SetSensitivity
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
* Capesnse_sensorSensitivity[] - This 8-bits array contains Sensitivity in the LSB nibble
*                                and Noise Reset Counter value in the MSB nibble.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_SetSensitivity(uint32 sensor, uint32 value)
{
    if(value > 10u)
    {
        value = 10u;
    }

    /* Clear SensorSensitivity value in LSB nibble */
    Capesnse_sensorSensitivity[sensor] &= (uint8)~Capesnse_SENSITIVITY_MASK;
    /* Set SensorSensitivity value in LSB nibble */
    Capesnse_sensorSensitivity[sensor] |= (uint8)value;
}


/*******************************************************************************
* Function Name: Capesnse_GetSensitivityCoefficient
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
*  Capesnse_kValueTbl[] - This 8-bits array contains the K value for each sensor.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint8 Capesnse_GetSensitivityCoefficient(uint32 sensor)
{
    return (Capesnse_kValueTbl[sensor]);
}


/*******************************************************************************
* Function Name: Capesnse_GetNoiseEnvelope
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
*  Capesnse_noiseEnvelopeTbl[] - array with Noise Envelope.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Capesnse_GetNoiseEnvelope(uint32 sensor)
{
    return((uint16)((uint16)Capesnse_noiseEnvelopeTbl[sensor] << 1u) + 1u);
}


/*******************************************************************************
* Function Name: Capesnse_GetNormalizedDiffCountData
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
*  Capesnse_sensorSignal[] - array with difference counts for sensors
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Capesnse_GetNormalizedDiffCountData(uint32 sensor)
{
    return (uint16)(((uint32)Capesnse_sensorSignal[sensor] << 7u) / Capesnse_kValueTbl[sensor]);
}

#endif /* (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) */

/* [] END OF FILE */
