/*******************************************************************************
* File Name: Capsense.c
* Version 2.60
*
* Description:
*  This file provides the source code for scanning APIs for the CapSense CSD
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Capsense.h"
#include "Capsense_PVT.h"
#include "cypins.h"
#include "cyapicallbacks.h"

#if(0u != Capsense_CSHL_API_GENERATE)
    #include "Capsense_CSHL.h"
#endif /* (0u != Capsense_CSHL_API_GENERATE) */

/* This definition is intended to prevent unexpected linker error.
   For more details please see the IAR Technical Note 49981 */
#if defined(__ICCARM__)
    extern void Capsense_EnableSensor(uint32 sensor);
    extern void Capsense_DisableSensor(uint32 sensor);
#endif /* (__ICCARM__) */

/* SmartSense functions */
#if (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)
    uint8 Capsense_lowLevelTuningDone = 0u;
    uint8 Capsense_scanSpeedTbl[((Capsense_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
#endif /* (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */

#if(Capsense_PRS_OPTIONS != Capsense__PRS_NONE)
    uint8 Capsense_prescalersTuningDone = 0u;
#endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_NONE) */

/* Global software variables */
volatile uint8 Capsense_csdStatusVar = 0u;   /* CapSense CSD status, variable */
volatile uint8 Capsense_sensorIndex = 0u;    /* Index of scannig sensor */

/* Global array of Raw Counts */
uint16 Capsense_sensorRaw[Capsense_TOTAL_SENSOR_COUNT] = {0u};

/* Backup variables for trim registers*/
#if (Capsense_IDAC1_POLARITY == Capsense__IDAC_SINK)
    uint8 Capsense_csdTrim2;
#else
    uint8 Capsense_csdTrim1;
#endif /* (Capsense_IDAC1_POLARITY == Capsense__IDAC_SINK) */

/* Global array of un-scanned sensors state */
uint8 Capsense_unscannedSnsDriveMode[Capsense_PORT_PIN_CONFIG_TBL_ZISE];

/* Backup array for Capsense_sensorEnableMask */
uint8 Capsense_sensorEnableMaskBackup[(((Capsense_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

/* Configured constants and arrays by Customizer */
uint8 Capsense_sensorEnableMask[(((Capsense_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)] = {
0x1u, };

reg32 * Capsense_pcTable[] = {
    (reg32 *)Capsense_Sns__Button0__BTN__PC, 
};

const uint8 Capsense_portTable[] = {
    Capsense_Sns__Button0__BTN__PORT, 
};

const uint32 Capsense_maskTable[] = {
    Capsense_Sns__Button0__BTN__MASK, 
};

const uint8 Capsense_pinShiftTbl[] = {
    (uint8) Capsense_Sns__Button0__BTN__SHIFT, 
};

uint8 Capsense_modulationIDAC[Capsense_TOTAL_SENSOR_COUNT] = {
    51u, 
};
uint8 Capsense_compensationIDAC[Capsense_TOTAL_SENSOR_COUNT] = {
    51u, 
};

uint32 Capsense_widgetResolution[Capsense_RESOLUTIONS_TBL_SIZE] = {
    Capsense_RESOLUTION_10_BITS,
};

uint8 Capsense_senseClkDividerVal[Capsense_TOTAL_SCANSLOT_COUNT] = {
    2u, 
};
uint8 Capsense_sampleClkDividerVal[Capsense_TOTAL_SCANSLOT_COUNT] = {
    2u, 
};


const uint8 Capsense_widgetNumber[Capsense_TOTAL_SENSOR_COUNT] = {
    0u, /* Button0__BTN */
};

reg32* const Capsense_prtSelTbl[Capsense_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_HSIOM_PORT_SEL0),
    ((reg32 *) CYREG_HSIOM_PORT_SEL1),
    ((reg32 *) CYREG_HSIOM_PORT_SEL2),
    ((reg32 *) CYREG_HSIOM_PORT_SEL3),
    ((reg32 *) CYREG_HSIOM_PORT_SEL4),
    ((reg32 *) CYREG_HSIOM_PORT_SEL5),
    ((reg32 *) CYREG_HSIOM_PORT_SEL6),
};

reg32* const Capsense_prtCfgTbl[Capsense_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_PC),
    ((reg32 *) CYREG_GPIO_PRT1_PC),
    ((reg32 *) CYREG_GPIO_PRT2_PC),
    ((reg32 *) CYREG_GPIO_PRT3_PC),
    ((reg32 *) CYREG_GPIO_PRT4_PC),
    ((reg32 *) CYREG_GPIO_PRT5_PC),
    ((reg32 *) CYREG_GPIO_PRT6_PC),
};

reg32* const Capsense_prtDRTbl[Capsense_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_DR),
    ((reg32 *) CYREG_GPIO_PRT1_DR),
    ((reg32 *) CYREG_GPIO_PRT2_DR),
    ((reg32 *) CYREG_GPIO_PRT3_DR),
    ((reg32 *) CYREG_GPIO_PRT4_DR),
    ((reg32 *) CYREG_GPIO_PRT5_DR),
    ((reg32 *) CYREG_GPIO_PRT6_DR),
};



/*******************************************************************************
* Function Name: Capsense_Init
********************************************************************************
*
* Summary:
*  API initializes default CapSense configuration provided by the customizer that defines
*  the mode of component operations and resets all the sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_immunityIndex - defines immunity level.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_Init(void)
{
    uint32 curSensor;

    #if(Capsense_IS_SHIELD_ENABLE)

        #if((Capsense_CSH_TANK_PREGARGE_OPTION == Capsense__CAPPRIOBUF) || (0u != Capsense_CSH_TANK_ENABLE))
            uint32 newRegValue;

            newRegValue = Capsense_CTANK_CONNECTION_REG;
            newRegValue &= ~(Capsense_CSD_CTANK_CONNECTION_MASK);
            newRegValue |= Capsense_CSD_CTANK_TO_AMUXBUS_B;
            Capsense_CTANK_CONNECTION_REG = newRegValue;

            #if(Capsense_CSH_TANK_PREGARGE_OPTION == Capsense__CAPPRIOBUF)
                newRegValue = Capsense_CTANK_PORT_PC_REG;
                newRegValue &= ~(Capsense_CSD_CTANK_PC_MASK);
                newRegValue |= Capsense_CTANK_PC_STRONG_MODE;
                Capsense_CTANK_PORT_PC_REG = newRegValue;

                newRegValue = Capsense_CTANK_PORT_DR_REG;
                newRegValue &= ~(Capsense_CTANK_DR_MASK);
                newRegValue |= Capsense_CTANK_DR_CONFIG;
                Capsense_CTANK_PORT_DR_REG = newRegValue;
            #endif /* (Capsense_CSH_TANK_PREGARGE_OPTION == Capsense__CAPPRIOBUF) */

        #endif /* ((Capsense_CSH_TANK_PREGARGE_OPTION == Capsense__CAPPRIOBUF) || (Capsense_CSH_TANK_ENABLE)) */

        Capsense_EnableShieldElectrode((uint32)Capsense_SHIELD_PIN_NUMBER, (uint32)Capsense_SHIELD_PORT_NUMBER);

    #endif /* (Capsense_IS_SHIELD_ENABLE) */

    for(curSensor = 0u; curSensor < Capsense_TOTAL_SENSOR_COUNT; curSensor++)
    {
        Capsense_SetUnscannedSensorState(curSensor, Capsense_CONNECT_INACTIVE_SNS);
    }

    Capsense_CsdHwBlockInit();
    Capsense_SetShieldDelay(Capsense_SHIELD_DELAY );

    /* Clear all sensors */
    Capsense_ClearSensors();
}

/*******************************************************************************
* Function Name: Capsense_CsdHwBlockInit
********************************************************************************
*
* Summary:
*  Initialises the hardware parameters of the CSD block
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_csdTrim1 - Contains IDAC trim register value for Sourcing Mode.
*  Capsense_csdTrim2 - Contains IDAC trim register value for Sinking Mode.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_CsdHwBlockInit(void)
{
    uint32 newRegValue;

        /* Set trim registers for CSD Mode */
    #if (Capsense_IDAC1_POLARITY == Capsense__IDAC_SINK)
        /* iDAC1 Sinking Mode */
        Capsense_csdTrim2 = (uint8)Capsense_CSD_TRIM2_REG;
        newRegValue = Capsense_csdTrim2;
        newRegValue &= Capsense_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)Capsense_SFLASH_CSD_TRIM2_REG & (uint32)Capsense_CSFLASH_TRIM_IDAC1_MASK);

        #if (Capsense_IDAC_CNT == 2u)
            /* iDAC2 Sinking Mode */
            newRegValue &= Capsense_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)Capsense_SFLASH_CSD_TRIM2_REG & (uint32)Capsense_CSFLASH_TRIM_IDAC2_MASK);
        #endif /* (Capsense_IDAC_CNT == 2u) */
        Capsense_CSD_TRIM2_REG = newRegValue;
    #else
        /* iDAC1 Sourcing Mode */
        Capsense_csdTrim1 = (uint8)Capsense_CSD_TRIM1_REG;
        newRegValue = Capsense_csdTrim1;
        newRegValue &= Capsense_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)Capsense_SFLASH_CSD_TRIM1_REG & (uint32)Capsense_CSFLASH_TRIM_IDAC1_MASK);
        #if (Capsense_IDAC_CNT == 2u)
             /* iDAC2 Sourcing Mode */
            newRegValue &= Capsense_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)Capsense_SFLASH_CSD_TRIM1_REG & (uint32)Capsense_CSFLASH_TRIM_IDAC2_MASK);
        #endif
        Capsense_CSD_TRIM1_REG = newRegValue;
    #endif /* (Capsense_IDAC1_POLARITY == Capsense__IDAC_SINK) */

    /* Configure CSD and IDAC */
    #if (Capsense_IDAC_CNT > 1u)
        Capsense_CSD_IDAC_REG = Capsense_DEFAULT_CSD_IDAC_CONFIG;
        Capsense_CSD_CFG_REG = Capsense_DEFAULT_CSD_CONFIG;
    #else
        Capsense_CSD_IDAC_REG &= ~(Capsense_CSD_IDAC1_MODE_MASK  | Capsense_CSD_IDAC1_DATA_MASK);
        Capsense_CSD_IDAC_REG |= Capsense_DEFAULT_CSD_IDAC_CONFIG;

        Capsense_CSD_CFG_REG &= ~(Capsense_CSD_CONFIG_MASK);
        Capsense_CSD_CFG_REG |= (Capsense_DEFAULT_CSD_CONFIG);
    #endif /* (Capsense_IDAC_CNT > 1u) */


    /* Connect Cmod to AMUX bus */
    newRegValue = Capsense_CMOD_CONNECTION_REG;
    newRegValue &= ~(Capsense_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Capsense_CSD_CMOD_TO_AMUXBUS_A;
    Capsense_CMOD_CONNECTION_REG = newRegValue;

    /* Configure Dead Band PWM if it is enabled */
    #if(Capsense_CSD_4B_PWM_MODE != Capsense__PWM_OFF)
        Capsense_CSD_4B_PWM_REG = Capsense_DEFAULT_CSD_4B_PWM_CONFIG;
    #endif /* (Capsense_CSD_4B_PWM_MODE != Capsense__PWM_OFF) */

    /* Setup ISR */
    CyIntDisable(Capsense_ISR_NUMBER);
    #if !defined(CY_EXTERNAL_INTERRUPT_CONFIG)
        (void)CyIntSetVector(Capsense_ISR_NUMBER, &Capsense_ISR);
        CyIntSetPriority(Capsense_ISR_NUMBER, Capsense_ISR_PRIORITY);
    #endif /* (CY_EXTERNAL_INTERRUPT_CONFIG) */
}


/*******************************************************************************
* Function Name: Capsense_Enable
********************************************************************************
*
* Summary:
*  Enables the CSD block and related resources to an active mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  None.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_Enable(void)
{
    uint32 newRegValue;

    /* Enable Clocks */
    Capsense_SenseClk_Stop();
    Capsense_SampleClk_Stop();

    Capsense_SampleClk_SetDividerValue((uint16)Capsense_INITIAL_CLK_DIVIDER);
    Capsense_SenseClk_SetDividerValue((uint16)Capsense_INITIAL_CLK_DIVIDER);

    #if (0u == Capsense_IS_M0S8PERI_BLOCK)
        Capsense_SenseClk_Start();
        Capsense_SampleClk_Start();
    #else
        Capsense_SampleClk_Start();
        Capsense_SenseClk_StartEx(Capsense_SampleClk__DIV_ID);
    #endif /* (0u == Capsense_IS_M0S8PERI_BLOCK) */

    /* Enable the CSD block */
    newRegValue = Capsense_CSD_CFG_REG;
    newRegValue |= (Capsense_CSD_CFG_ENABLE | Capsense_CSD_CFG_SENSE_COMP_EN
                                                   | Capsense_CSD_CFG_SENSE_EN);
    Capsense_CSD_CFG_REG = newRegValue;

    /* Enable interrupt */
    CyIntEnable(Capsense_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Capsense_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin the component operation. CapSense_Start()
*  calls the CapSense_Init() function, and then calls the CapSense_Enable()
*  function. Initializes the registers and starts the CSD method of the CapSense
*  component. Resets all the sensors to an inactive state. Enables interrupts for
*  sensors scanning. When the SmartSense tuning mode is selected, the tuning procedure
*  is applied for all the sensors. The CapSense_Start() routine must be called before
*  any other API routines.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*   Capsense_initVar - used to check the initial configuration,
*   modified on the first function call.
*  Capsense_lowLevelTuningDone - Used to notify the Tuner GUI that
*   tuning of the scanning parameters is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_Start(void)
{

    static uint8 Capsense_initVar = 0u;

    #if((0u != Capsense_AUTOCALIBRATION_ENABLE) && (Capsense_TUNING_METHOD != Capsense__TUNING_AUTO))
        uint32 curSensor;
        uint32 rawLevel;
        uint32 widget;
    #endif /* ((0u != Capsense_AUTOCALIBRATION_ENABLE) && (Capsense_TUNING_METHOD != Capsense__TUNING_AUTO)) */

    if (Capsense_initVar == 0u)
    {
        Capsense_Init();
        Capsense_initVar = 1u;
    }
    Capsense_Enable();

    /* AutoTunning start */
    #if(Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)
        #if(0u != Capsense_CSHL_API_GENERATE)
            Capsense_AutoTune();
            Capsense_lowLevelTuningDone = 1u;
        #endif /* (0u != Capsense_CSHL_API_GENERATE) */
    #else
        #if(0u != Capsense_AUTOCALIBRATION_ENABLE)

            #if(Capsense_IDAC_CNT > 1u)
                Capsense_CSD_IDAC_REG &= ~(Capsense_CSD_IDAC2_MODE_MASK);
            #endif /* (Capsense_IDAC_CNT > 1u) */

            for(curSensor = 0u; curSensor < Capsense_TOTAL_SCANSLOT_COUNT; curSensor++)
            {
                widget = Capsense_widgetNumber[curSensor];
                rawLevel = Capsense_widgetResolution[widget] >> Capsense_RESOLUTION_OFFSET;

                /* Calibration level should be equal to 85% from scanning resolution */
                rawLevel = (rawLevel * 85u) / 100u;

                Capsense_CalibrateSensor(curSensor, rawLevel, Capsense_modulationIDAC);
            }

            #if(0u != Capsense_TOTAL_CENTROIDS_COUNT)
                Capsense_NormalizeWidgets(Capsense_END_OF_WIDGETS_INDEX, Capsense_modulationIDAC);
            #endif /* (0u != Capsense_TOTAL_CENTROIDS_COUNT)  */

            #if(Capsense_IDAC_CNT > 1u)
                for(curSensor = 0u; curSensor < Capsense_TOTAL_SCANSLOT_COUNT; curSensor++)
                {
                    Capsense_compensationIDAC[curSensor] = Capsense_modulationIDAC[curSensor] / 2u;
                    Capsense_modulationIDAC[curSensor] = (Capsense_modulationIDAC[curSensor] + 1u) / 2u;
                }
                Capsense_CSD_IDAC_REG |= Capsense_CSD_IDAC2_MODE_FIXED;
            #endif /* (Capsense_IDAC_CNT > 1u) */

        #endif /* (0u != Capsense_AUTOCALIBRATION_ENABLE) */

    #endif /* ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */

    /* Initialize Advanced Centroid */
    #if(Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
        Capsense_AdvancedCentroidInit();
    #endif /* (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */

    /* Connect sense comparator input to AMUXA */
    Capsense_CSD_CFG_REG |= Capsense_CSD_CFG_SENSE_INSEL;
}


/*******************************************************************************
* Function Name: Capsense_Stop
********************************************************************************
*
* Summary:
*  Stops the sensor scanning, disables component interrupts, and resets all the
*  sensors to an inactive state. Disables the Active mode power template bits for
*  the subcomponents used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_csdTrim1 - Contains the IDAC trim register value for the Sourcing Mode.
*  Capsense_csdTrim2 - Contains the IDAC trim register value for vSinking Mode.
*
* Side Effects:
*  This function should be called after scans are completed.
*
*******************************************************************************/
void Capsense_Stop(void)
{
    /* Disable interrupt */
    CyIntDisable(Capsense_ISR_NUMBER);

    Capsense_CSD_CFG_REG &= ~(Capsense_CSD_CFG_SENSE_COMP_EN | Capsense_CSD_CFG_SENSE_EN);

    #if(Capsense_IDAC_CNT == 2u)
        Capsense_CSD_CFG_REG &= ~(Capsense_CSD_CFG_ENABLE);
    #endif /* (Capsense_IDAC_CNT == 2u) */

    /* Disable the Clocks */
    Capsense_SenseClk_Stop();
    Capsense_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Capsense_FindNextSensor
********************************************************************************
*
* Summary:
*  Finds the next sensor to scan.
*
* Parameters:
*  snsIndex:  Current index of sensor.
*
* Return:
*  Returns the next sensor index to scan.
*
* Global Variables:
*  Capsense_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*
* Side Effects:
*  This function affects the  current scanning and should not
*  be used outside the component.
*
*******************************************************************************/
uint8 Capsense_FindNextSensor(uint8 snsIndex)
{
    uint32 enableFlag;

    /* Check if sensor enabled */
    do
    {
        /* Proceed with next sensor */
        snsIndex++;
        if(snsIndex == Capsense_TOTAL_SENSOR_COUNT)
        {
            break;
        }
        enableFlag = Capsense_GetBitValue(Capsense_sensorEnableMask, (uint32)snsIndex);
    }
    while(enableFlag == 0u);

    return ((uint8)snsIndex);
}


/*******************************************************************************
* Function Name: Capsense_ScanSensor
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a sensor. After scanning is complete,
*  the ISR copies the measured sensor raw data to the global raw sensor array.
*  Use of the ISR ensures this function is non-blocking.
*  Each sensor has a unique number within the sensor array.
*  This number is assigned by the CapSense customizer in sequence.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Capsense_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and mode of scan as single scan.
*  Capsense_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to the provided sensor argument.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_ScanSensor(uint32 sensor)
{
    /* Clears status/control variable and set sensorIndex */
    Capsense_csdStatusVar = 0u;
    Capsense_sensorIndex = (uint8)sensor;

    /* Start of sensor scan */
    Capsense_csdStatusVar = (Capsense_SW_STS_BUSY | Capsense_SW_CTRL_SINGLE_SCAN);
    Capsense_PreScan(sensor);
}


#if(0u != Capsense_CSHL_API_GENERATE)
/*******************************************************************************
* Function Name: Capsense_ScanWidget
********************************************************************************
*
* Summary:
*  Sets the scan settings and starts scanning a widget.
*
* Parameters:
*  uint32 widget: Widget number.
*
* Return:
*  None
*
* Global Variables:
*  Capsense_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Capsense_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Capsense_FindNextSensor or
*  Capsense_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*  Capsense_sensorEnableMaskBackup[] - used to backup bit masks of the enabled
*   sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_ScanWidget(uint32 widget)
{
    uint32 sensorsPerWidget;
    uint32 lastSensor;
    uint32 snsIndex;

    /* Get first sensor in widget */
    Capsense_sensorIndex = Capsense_rawDataIndex[widget];

    /* Get number of sensors in widget */
    sensorsPerWidget = Capsense_numberOfSensors[widget];

    /* Check if generic Sensor */
    if(sensorsPerWidget == 0u)
    {
        sensorsPerWidget = 1u;
    }

    /* Get last sensor in widget */
    lastSensor = (Capsense_rawDataIndex[widget] + sensorsPerWidget) - 1u;

    /* Backup sensorEnableMask array */
    for(snsIndex = 0u; snsIndex < Capsense_TOTAL_SENSOR_MASK; snsIndex++)
    {
        /* Backup sensorEnableMask array */
        Capsense_sensorEnableMaskBackup[snsIndex] = Capsense_sensorEnableMask[snsIndex];
    }

    /* Update sensorEnableMask to scan the sensors that belong to widget */
    for(snsIndex = 0u; snsIndex < Capsense_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        /* Update sensorEnableMask array bits to scan the widget only */
        if((snsIndex >= Capsense_sensorIndex) && (snsIndex <= lastSensor))
        {
            /* Set sensor bit to scan */
            Capsense_SetBitValue(Capsense_sensorEnableMask, snsIndex, 1u);
        }
        else
        {
            /* Reset sensor bit to do not scan */
            Capsense_SetBitValue(Capsense_sensorEnableMask, snsIndex, 0u);
        }
    }

    /* Check end of scan condition */
    if(Capsense_sensorIndex < Capsense_TOTAL_SENSOR_COUNT)
    {
        /* Set widget busy bit in status/control variable*/
        Capsense_csdStatusVar = (Capsense_SW_STS_BUSY | Capsense_SW_CTRL_WIDGET_SCAN);
        /* Scan first sensor of widget*/
        Capsense_PreScan((uint32)Capsense_sensorIndex);
    }
}
#endif /* (0u != Capsense_CSHL_API_GENERATE) */


/*******************************************************************************
* Function Name: Capsense_ScanEnableWidgets
********************************************************************************
*
* Summary:
*  This is the preferred method to scan all of the enabled widgets.
*  The API starts scanning a sensor within the enabled widgets.
*  The ISR continues scanning the sensors until all the enabled widgets are scanned.
*  Use of the ISR ensures this function is non-blocking.
*  All the widgets are enabled by default except proximity widgets.
*  The proximity widgets must be manually enabled as their long scan time
*  is incompatible with a fast response required of other widget types.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Capsense_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Capsense_FindNextSensor or
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_ScanEnabledWidgets(void)
{
    /* Clears status/control variable and set sensorIndex */
    Capsense_csdStatusVar = 0u;
    Capsense_sensorIndex = 0xFFu;

    /* Find next sensor */
    Capsense_sensorIndex = (uint8)Capsense_FindNextSensor(Capsense_sensorIndex);

    /* Check end of scan condition */
    if(Capsense_sensorIndex < Capsense_TOTAL_SENSOR_COUNT)
    {
        Capsense_csdStatusVar |= Capsense_SW_STS_BUSY;
        Capsense_PreScan((uint32)Capsense_sensorIndex);
    }
}


/*******************************************************************************
* Function Name: Capsense_IsBusy
********************************************************************************
*
* Summary:
*  Returns the state of the CapSense component. 1 means that scanning in
*  progress and 0 means that scanning is complete.
*
* Parameters:
*  None
*
* Return:
*  Returns the state of scanning. 1 - scanning in progress, 0 - scanning
*  completed.
*
* Global Variables:
*  Capsense_csdStatusVar - used to provide the status and mode of the scanning process.
*  Checks the busy status.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_IsBusy(void)
{
    return ((uint32)((0u != (Capsense_csdStatusVar & Capsense_SW_STS_BUSY)) ? 1u : 0u));
}


/*******************************************************************************
* Function Name: Capsense_ReadSensorRaw
********************************************************************************
*
* Summary:
*  Returns sensor raw data from the global CapSense_sensorRaw[ ] array.
*  Each scan sensor has a unique number within the sensor array.
*  This number is assigned by the CapSense customizer in sequence.
*  Raw data can be used to perform calculations outside of the CapSense
*  provided framework.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  Returns the current raw data value for a defined sensor number.
*
* Global Variables:
*  Capsense_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Capsense_ReadSensorRaw(uint32 sensor)
{
    return Capsense_sensorRaw[sensor];
}


/*******************************************************************************
* Function Name: Capsense_WriteSensorRaw
********************************************************************************
*
* Summary:
*  This API writes the raw count value passed as an argument to the sensor Raw count array.
*
* Parameters:
*  sensor:  Sensor number.
*  value: Raw count value.
*
* Global Variables:
*  Capsense_sensorRaw[] - used to store sensors raw data.
*
* Return:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_WriteSensorRaw(uint32 sensor, uint16 value)
{
    Capsense_sensorRaw[sensor] = value;
}


#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capsense_SetScanResolution
    ********************************************************************************
    *
    * Summary:
    *  Sets a value of the sensor scan resolution for a widget.
    *  The MIN resolution can be set 8-bit. The MAX Resolution can be set 16 bit.
    *  This function is not available for the tuning mode "None".
    *
    * Parameters:
    *  widget:     Widget index.
    *  resolution: Represents the resolution value. The following defines which are available in the
    *              Capsense.h file should be used:
    *              Capsense_RESOLUTION_6_BITS
    *              Capsense_RESOLUTION_7_BITS
    *              Capsense_RESOLUTION_8_BITS
    *              Capsense_RESOLUTION_9_BITS
    *              Capsense_RESOLUTION_10_BITS
    *              Capsense_RESOLUTION_11_BITS
    *              Capsense_RESOLUTION_12_BITS
    *              Capsense_RESOLUTION_13_BITS
    *              Capsense_RESOLUTION_14_BITS
    *              Capsense_RESOLUTION_15_BITS
    *              Capsense_RESOLUTION_16_BITS
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Capsense_widgetResolution[] - used to store scan resolution of each widget.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_SetScanResolution(uint32 widget, uint32 resolution)
    {
        Capsense_widgetResolution[widget] = resolution;
    }
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */


/*******************************************************************************
* Function Name: Capsense_GetScanResolution
********************************************************************************
*
* Summary:
*  Returns the resolution value for the appropriate sensor.
*  This function is not available for tuning mode "None".
*
* Parameters:
*  widget:     Widget index.
*
* Return:
*  resolution: Returns the resolution value for the appropriate sensor.
*              The resolution values are defined in the Capsense.h file
*              The defines are encountered below:
*              Capsense_RESOLUTION_6_BITS
*              Capsense_RESOLUTION_7_BITS
*              Capsense_RESOLUTION_8_BITS
*              Capsense_RESOLUTION_9_BITS
*              Capsense_RESOLUTION_10_BITS
*              Capsense_RESOLUTION_11_BITS
*              Capsense_RESOLUTION_12_BITS
*              Capsense_RESOLUTION_13_BITS
*              Capsense_RESOLUTION_14_BITS
*              Capsense_RESOLUTION_15_BITS
*              Capsense_RESOLUTION_16_BITS
*
* Global Variables:
*  Capsense_widgetResolution[] - used to store scan resolution of every widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_GetScanResolution(uint32 widget)
{
    return(Capsense_widgetResolution[widget]);
}


/*******************************************************************************
* Function Name: Capsense_ClearSensors
********************************************************************************
*
* Summary:
*  Resets all the sensors to the non-sampling state by sequentially disconnecting
*  all the sensors from Analog MUX Bus and putting them to an inactive state.
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
void Capsense_ClearSensors(void)
{
    uint32 snsIndex;

    for (snsIndex = 0u; snsIndex < Capsense_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        Capsense_DisableScanSlot(snsIndex);
    }
}


#if (Capsense_IS_COMPLEX_SCANSLOTS)
    /*******************************************************************************
    * Function Name: Capsense_EnableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Configures the selected slot to measure during the next measurement
    *  cycle. The corresponding pin/pins are set to Analog High-Z mode and
    *  connected to the Analog Mux Bus. This also enables the comparator function.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Constants:
    *  Capsense_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  Capsense_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  Capsense_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  Bit 7 (msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_EnableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;
        /* Read sensor type: single or complex */
        uint8 snsType = Capsense_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & Capsense_COMPLEX_SS_FLAG) == 0u)
        {
            /* Enable sensor (single) */
            Capsense_EnableSensor(slot);
        }
        else
        {
            /* Enable complex sensor */
            snsType &= ~Capsense_COMPLEX_SS_FLAG;
            index = &Capsense_indexTable[snsType];
            snsNumber = Capsense_maskTable[slot];

            for (j = 0u; j < snsNumber; j++)
            {
                Capsense_EnableSensor(index[j]);
            }
        }
    }


    /*******************************************************************************
    * Function Name: Capsense_DisableScanSlot
    ********************************************************************************
    *
    * Summary:
    *  Disables the selected slot. The corresponding pin/pins is/are disconnected
    *  from the Analog Mux Bus and connected to GND, High_Z or Shield electrode.
    *
    * Parameters:
    *  slot:  Slot number.
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Capsense_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  Capsense_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  Capsense_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  7bit(msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_DisableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;

        /* Read sensor type: single or complex */
        uint8 snsType = Capsense_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & Capsense_COMPLEX_SS_FLAG) == 0u)
        {
            /* Disable sensor (single) */
            Capsense_DisableSensor(slot);
        }
        else
        {
            /* Disable complex sensor */
            snsType &= ~Capsense_COMPLEX_SS_FLAG;
            index = &Capsense_indexTable[snsType];
            snsNumber = Capsense_maskTable[slot];

            for (j=0; j < snsNumber; j++)
            {
                Capsense_DisableSensor(index[j]);
            }
        }
    }
#endif  /* Capsense_IS_COMPLEX_SCANSLOTS */


/*******************************************************************************
* Function Name: Capsense_EnableSensor
********************************************************************************
*
* Summary:
*  Configures the selected sensor to measure during the next measurement cycle.
*  The corresponding pins are set to Analog High-Z mode and connected to the
*  Analog Mux Bus. This also enables the comparator function.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Capsense_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Capsense_pinShiftTbl[] - used to store position of pin that
*  configured as sensor in port.
*  Capsense_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Capsense_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_EnableSensor(uint32 sensor)
{
    uint8  pinModeShift;
    uint8  pinHSIOMShift;
    uint8 interruptState;
    uint32 newRegisterValue;
    uint32 port;

    port = (uint32) Capsense_portTable[sensor];
    pinModeShift = Capsense_pinShiftTbl[sensor]  * Capsense_PC_PIN_CFG_SIZE;
    pinHSIOMShift = Capsense_pinShiftTbl[sensor] * Capsense_HSIOM_PIN_CFG_SIZE;

    interruptState = CyEnterCriticalSection();

    newRegisterValue = *Capsense_prtSelTbl[port];
    newRegisterValue &= ~(Capsense_CSD_HSIOM_MASK << pinHSIOMShift);
    newRegisterValue |= (uint32)((uint32)Capsense_CSD_SENSE_PORT_MODE << pinHSIOMShift);

    *Capsense_prtCfgTbl[port] &= (uint32)~((uint32)Capsense_CSD_PIN_MODE_MASK << pinModeShift);
    *Capsense_prtSelTbl[port] = newRegisterValue;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Capsense_DisableSensor
********************************************************************************
*
* Summary:
*  Disables the selected sensor. The corresponding pin is disconnected from the
*  Analog Mux Bus and connected to GND, High_Z or Shield electrode.
*
* Parameters:
*  sensor:  Sensor number
*
* Return:
*  None
*
* Global Variables:
*  Capsense_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Capsense_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*  Capsense_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Capsense_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_DisableSensor(uint32 sensor)
{
    uint8 interruptState;
    uint32 newRegisterValue;

    uint32 port = (uint32) Capsense_portTable[sensor];
    uint8  pinHSIOMShift = Capsense_pinShiftTbl[sensor] * Capsense_HSIOM_PIN_CFG_SIZE;
    uint8  pinModeShift = Capsense_pinShiftTbl[sensor]  * Capsense_PC_PIN_CFG_SIZE;

    uint32 inactiveConnect = Capsense_SNS_HIZANALOG_CONNECT;
    uint32 sensorState = Capsense_unscannedSnsDriveMode[sensor];

    *Capsense_prtSelTbl[port] &= ~(Capsense_CSD_HSIOM_MASK << pinHSIOMShift);

    #if(Capsense_IS_SHIELD_ENABLE != 0)
    if(sensorState != (uint32)Capsense__SHIELD)
    {
    #else
        /* Connected to Ground if shield is disabled */
        if(sensorState == (uint32)Capsense__SHIELD)
        {
            sensorState = (uint32)Capsense__GROUND;
        }
    #endif /* (Capsense_IS_SHIELD_ENABLE) */
        if(sensorState != (uint32)Capsense__HIZ_ANALOG)
        {
            /* Connected to Ground */
            inactiveConnect = (uint32)Capsense_SNS_GROUND_CONNECT;
        }

        interruptState = CyEnterCriticalSection();

        /* Set drive mode */
        newRegisterValue = *Capsense_prtCfgTbl[port];
        newRegisterValue &= ~(Capsense_CSD_PIN_MODE_MASK << pinModeShift);
        newRegisterValue |=  (uint32)(inactiveConnect << pinModeShift);
        *Capsense_prtCfgTbl[port] =  newRegisterValue;

        CyExitCriticalSection(interruptState);

        *Capsense_prtDRTbl[port]  &=  (uint32)~(uint32)((uint32)1u << Capsense_pinShiftTbl[sensor]);
    #if(Capsense_IS_SHIELD_ENABLE != 0)
    }
    else
    {
        /* Connect to Shield */
        *Capsense_prtSelTbl[port] |= (Capsense_CSD_SHIELD_PORT_MODE << pinHSIOMShift);
    }
    #endif /* (Capsense_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Capsense_SetDriveModeAllPins
********************************************************************************
*
* Summary:
* This API sets the drive mode of port pins used by the CapSense
* component (sensors, guard, shield, shield tank and Cmod) to drive the
* mode specified by the argument.
*
* Parameters:
*  driveMode:  Drive mode definition.
*
* Return:
*  None
*
* Global Variables:
*  Capsense_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Capsense_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*
* Side Effects:
*  This API shall be called only after CapSense component is stopped.
*
*******************************************************************************/
void Capsense_SetDriveModeAllPins(uint32 driveMode)
{
    uint8 curSensor;
    #if (Capsense_IS_COMPLEX_SCANSLOTS)
        uint8 snsNumber;
        uint8 snsType;
    #endif  /* Capsense_IS_COMPLEX_SCANSLOTS */
    uint32 prtNumberTmp;
    uint32 pinNumberTmp;

    for(curSensor = 0u; curSensor < Capsense_TOTAL_SENSOR_COUNT; curSensor++)
    {
        #if (Capsense_IS_COMPLEX_SCANSLOTS)
            /* Read sensor type: single or complex */
            snsType = Capsense_portTable[curSensor];

            /* Check if sensor is complex */
            if ((snsType & Capsense_COMPLEX_SS_FLAG) == 0u)
            {
                /* The sensor is not complex */
                snsNumber = curSensor;
            }
            else
            {
                /* Get dedicated sensor ID of the complex sensor */
                snsType &= ~Capsense_COMPLEX_SS_FLAG;
                snsNumber = Capsense_indexTable[snsType];
            }

            prtNumberTmp = Capsense_portTable[snsNumber];
            pinNumberTmp = Capsense_pinShiftTbl[snsNumber];
        #else
            prtNumberTmp = Capsense_portTable[curSensor];
            pinNumberTmp = Capsense_pinShiftTbl[curSensor];
        #endif  /* Capsense_IS_COMPLEX_SCANSLOTS */

        Capsense_SetPinDriveMode(driveMode, pinNumberTmp, prtNumberTmp);
    }

    Capsense_SetPinDriveMode(driveMode, (uint32)Capsense_CMOD_PIN_NUMBER, (uint32)Capsense_CMOD_PRT_NUMBER);

    #if(0u != Capsense_CSH_TANK_ENABLE)
        Capsense_SetPinDriveMode(driveMode, (uint32)Capsense_CTANK_PIN_NUMBER, (uint32)Capsense_CTANK_PRT_NUMBER);
    #endif /* (0u != Capsense_CSH_TANK_ENABLE) */

    #if(0u != Capsense_IS_SHIELD_ENABLE)
        Capsense_SetPinDriveMode(driveMode, (uint32)Capsense_SHIELD_PIN_NUMBER, (uint32)Capsense_SHIELD_PORT_NUMBER);
    #endif /* (0u != Capsense_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Capsense_RestoreDriveModeAllPins
********************************************************************************
*
* Summary:
*  This API restores the drive for all the CapSense port pins to the original
*  state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  Capsense_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*
* Side Effects:
*  This API is called only after the CapSense component is stopped.
*
*******************************************************************************/
void Capsense_RestoreDriveModeAllPins(void)
{
    uint32 newRegisterValue;

    Capsense_SetDriveModeAllPins(CY_SYS_PINS_DM_ALG_HIZ);

    newRegisterValue = Capsense_CMOD_CONNECTION_REG;
    newRegisterValue &= ~(Capsense_CSD_CMOD_CONNECTION_MASK);
    newRegisterValue |= Capsense_CSD_CMOD_TO_AMUXBUS_A;
    Capsense_CMOD_CONNECTION_REG = newRegisterValue;

    #if(0u != Capsense_CSH_TANK_ENABLE)
        newRegisterValue = Capsense_CTANK_CONNECTION_REG;
        newRegisterValue &= ~(Capsense_CSD_CTANK_CONNECTION_MASK);
        newRegisterValue |= Capsense_CSD_CTANK_TO_AMUXBUS_B;
        Capsense_CTANK_CONNECTION_REG = newRegisterValue;
    #endif/* (0u != Capsense_CSH_TANK_ENABLE) */

    #if(0u != Capsense_IS_SHIELD_ENABLE)
        newRegisterValue = *Capsense_prtSelTbl[Capsense_SHIELD_PORT_NUMBER];
        newRegisterValue &= ~(Capsense_CSD_HSIOM_MASK << ((uint32)Capsense_SHIELD_PIN_NUMBER * Capsense_HSIOM_PIN_CFG_SIZE));
        newRegisterValue |= (Capsense_CSD_SHIELD_PORT_MODE << ((uint32)Capsense_SHIELD_PIN_NUMBER * Capsense_HSIOM_PIN_CFG_SIZE));
        *Capsense_prtSelTbl[Capsense_SHIELD_PORT_NUMBER] = newRegisterValue;
    #endif /* (0u != Capsense_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Capsense_SetPinDriveMode
********************************************************************************
*
* Summary:
* This API sets the drive mode for the appropriate port/pin.
*
* Parameters:
*  driveMode:  Drive mode definition.
*  portNumber: contains port number (0, 1, 2).
*  pinNumber: contains pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  Capsense_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Capsense_prtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  This API is called only after CapSense component is stopped.
*
*******************************************************************************/
void Capsense_SetPinDriveMode(uint32 driveMode, uint32 pinNumber, uint32 portNumber)
{
    uint32  pinModeShift;
    uint32 newRegisterValue;

    pinModeShift  = pinNumber * Capsense_PC_PIN_CFG_SIZE;

    newRegisterValue = *Capsense_prtCfgTbl[portNumber];
    newRegisterValue &= ~(Capsense_CSD_PIN_MODE_MASK << pinModeShift);
    newRegisterValue |=  (uint32)((uint32)driveMode << pinModeShift);
    *Capsense_prtCfgTbl[portNumber] =  newRegisterValue;
}


/*******************************************************************************
* Function Name: Capsense_PreScan
********************************************************************************
*
* Summary:
*  Sets required settings, enables a sensor, removes Vref from AMUX and starts the
*  scanning process of the sensor.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
* Capsense_widgetNumber[] - This array contains numbers of widgets for each sensor.
* Capsense_widgetResolution[] - Contains the widget resolution.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_PreScan(uint32 sensor)
{
    uint8 widget;
    uint8 interruptState;
    uint32 newRegValue;
    uint32 counterResolution;

    #if(Capsense_PRS_OPTIONS == Capsense__PRS_AUTO)
        uint8 senseClkDivMath;
        uint8 sampleClkDivMath;
    #endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_AUTO) */

    #if ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
         (0 != Capsense_IS_OVERSAMPLING_EN))
        uint32 oversamplingFactor;
    #endif /* ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
               (0 != Capsense_IS_OVERSAMPLING_EN)) */

    /* Define widget sensor belongs to */
    widget = Capsense_widgetNumber[sensor];

    /* Recalculate Counter Resolution to MSB 16 bits */
    counterResolution = Capsense_widgetResolution[widget];

    #if ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
         (0 != Capsense_IS_OVERSAMPLING_EN))
        oversamplingFactor = Capsense_GetBitValue(Capsense_scanSpeedTbl, sensor);

        if(counterResolution < Capsense_RESOLUTION_16_BITS)
        {
            counterResolution <<= oversamplingFactor;
            counterResolution |= (uint32)(Capsense_RESOLUTION_8_BITS);
        }
    #endif /* ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
               (0 != Capsense_IS_OVERSAMPLING_EN)) */

    #if (0u != Capsense_IS_M0S8PERI_BLOCK)
        Capsense_SenseClk_Stop();
        Capsense_SampleClk_Stop();
    #endif /* (0u != Capsense_IS_M0S8PERI_BLOCK) */

    #if (0u != Capsense_MULTIPLE_FREQUENCY_SET)
        Capsense_SampleClk_SetDividerValue((uint16)Capsense_sampleClkDividerVal[sensor]);
        Capsense_SenseClk_SetDividerValue((uint16)Capsense_senseClkDividerVal[sensor]);
    #else
        Capsense_SampleClk_SetDividerValue((uint16)Capsense_sampleClkDividerVal);
        Capsense_SenseClk_SetDividerValue((uint16)Capsense_senseClkDividerVal);
    #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET) */

    #if (0u != Capsense_IS_M0S8PERI_BLOCK)
        Capsense_SampleClk_Start();
        Capsense_SenseClk_StartEx(Capsense_SampleClk__DIV_ID);

        #if(Capsense_PRS_OPTIONS == Capsense__PRS_NONE)
            CyIntDisable(Capsense_ISR_NUMBER);
            Capsense_CSD_CNT_REG = Capsense_ONE_CYCLE;
            while(0u != (Capsense_CSD_CNT_REG & Capsense_RESOLUTION_16_BITS))
            {
            /* Wait until scanning is complete */
            }
            Capsense_CSD_INTR_REG = 1u;
            CyIntClearPending(Capsense_ISR_NUMBER);
            CyIntEnable(Capsense_ISR_NUMBER);
        #endif /* Capsense_PRS_OPTIONS == Capsense__PRS_NONE */
    #endif /* (0u != Capsense_IS_M0S8PERI_BLOCK) */

#if(Capsense_PRS_OPTIONS != Capsense__PRS_NONE)

    #if(Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)
        if(Capsense_prescalersTuningDone != 0u)
        {
    #endif /* (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */

            CyIntDisable(Capsense_ISR_NUMBER);

            newRegValue = Capsense_CSD_CFG_REG;
            newRegValue |= Capsense_CSD_CFG_PRS_SELECT;

            #if(Capsense_PRS_OPTIONS == Capsense__PRS_AUTO)

                newRegValue &= ~(Capsense_PRS_MODE_MASK);

                #if (0u != Capsense_MULTIPLE_FREQUENCY_SET)
                    senseClkDivMath = Capsense_senseClkDividerVal[sensor];
                    sampleClkDivMath = Capsense_sampleClkDividerVal[sensor];
                #else
                    senseClkDivMath = Capsense_senseClkDividerVal;
                    sampleClkDivMath = Capsense_sampleClkDividerVal;
                #endif /* ( Capsense_MULTIPLE_FREQUENCY_SET) */

                #if(0u == Capsense_IS_M0S8PERI_BLOCK)
                    senseClkDivMath *= sampleClkDivMath;
                #endif /* (0u == Capsense_IS_M0S8PERI_BLOCK) */

                if((senseClkDivMath * Capsense_RESOLUTION_12_BITS) <
                   (sampleClkDivMath * Capsense_widgetResolution[widget]))
                {
                    newRegValue |= Capsense_CSD_PRS_12_BIT;
                }
            #endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_AUTO) */

            Capsense_CSD_CFG_REG = newRegValue;

            CyIntEnable(Capsense_ISR_NUMBER);

    #if(Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)
        }
    #endif /* (Capsense_PRS_OPTIONS != Capsense__PRS_NONE) */

#endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_NONE) */

    /* Set Idac Value */
    CyIntDisable(Capsense_ISR_NUMBER);
    newRegValue = Capsense_CSD_IDAC_REG;

#if (Capsense_IDAC_CNT == 1u)
    newRegValue &= ~(Capsense_CSD_IDAC1_DATA_MASK);
    newRegValue |= Capsense_modulationIDAC[sensor];
#else
    newRegValue &= ~(Capsense_CSD_IDAC1_DATA_MASK | Capsense_CSD_IDAC2_DATA_MASK);
    newRegValue |= (Capsense_modulationIDAC[sensor] |
                            (uint32)((uint32)Capsense_compensationIDAC[sensor] <<
                            Capsense_CSD_IDAC2_DATA_OFFSET));
#endif /* (Capsense_IDAC_CNT == 1u) */

    Capsense_CSD_IDAC_REG = newRegValue;

#if(Capsense_CMOD_PREGARGE_OPTION == Capsense__CAPPRIOBUF)
    newRegValue = Capsense_CMOD_CONNECTION_REG;
    newRegValue &= ~(Capsense_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Capsense_CSD_CMOD_TO_AMUXBUS_A;
    Capsense_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = Capsense_CMOD_PORT_PC_REG;
    newRegValue &= ~(Capsense_CSD_CMOD_PC_MASK);
    newRegValue |= Capsense_CMOD_PC_HIGH_Z_MODE;
    Capsense_CMOD_PORT_PC_REG = newRegValue;
#endif /* (Capsense_CMOD_PREGARGE_OPTION == Capsense__CAPPRIOBUF) */

    /* Disconnect Vref Buffer from AMUX */
    newRegValue = Capsense_CSD_CFG_REG;
    newRegValue &= ~(Capsense_PRECHARGE_CONFIG_MASK);
    newRegValue |= Capsense_CTANK_PRECHARGE_CONFIG;

    CyIntEnable(Capsense_ISR_NUMBER);

    /* Enable Sensor */
    Capsense_EnableScanSlot(sensor);

    interruptState = CyEnterCriticalSection();
    Capsense_CSD_CFG_REG = newRegValue;

    /* `#START Capsense_PreSettlingDelay_Debug` */

    /* `#END` */

#ifdef Capsense_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK
    Capsense_PreScan_Pre_SettlingDelay_Debug_Callback();
#endif /* Capsense_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK */

    CyDelayCycles(Capsense_GLITCH_ELIMINATION_CYCLES);

    /* `#START Capsense_PreScan_Debug` */

    /* `#END` */

#ifdef Capsense_PRE_SCAN_DEBUG_CALLBACK
    Capsense_PreScan_Debug_Callback();
#endif /* Capsense_PRE_SCAN_DEBUG_CALLBACK */

    Capsense_CSD_CNT_REG = counterResolution;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Capsense_PostScan
********************************************************************************
*
* Summary:
*  Stores the results of measurement in the Capsense_sensorRaw[] array,
*  sets the scanning sensor in the non-sampling state, turns off Idac(Current Source IDAC),
*  disconnects the IDAC(Sink mode), and applies Vref on AMUX.
*
* Parameters:
*  sensor:  Sensor number.
*
* Return:
*  None
*
* Global Variables:
*  Capsense_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_PostScan(uint32 sensor)
{
    uint32 newRegValue;
#if ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
     (0 != Capsense_IS_OVERSAMPLING_EN))
    uint32 oversamplingFactor;
    uint32 widget;
#endif /* ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
           (0 != Capsense_IS_OVERSAMPLING_EN)) */

    /* `#START Capsense_PostScan_Debug` */

    /* `#END` */

#ifdef Capsense_POST_SCAN_DEBUG_CALLBACK
    Capsense_PostScan_Debug_Callback();
#endif /* Capsense_POST_SCAN_DEBUG_Callback */

    /* Read SlotResult from Raw Counter */
    Capsense_sensorRaw[sensor]  = (uint16)Capsense_CSD_CNT_REG;

#if ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
     (0 != Capsense_IS_OVERSAMPLING_EN))
    widget = Capsense_widgetNumber[sensor];
    if(Capsense_widgetResolution[widget] < Capsense_RESOLUTION_16_BITS)
    {
        oversamplingFactor = Capsense_GetBitValue(Capsense_scanSpeedTbl, sensor);
        Capsense_sensorRaw[sensor] >>= oversamplingFactor;
    }
#endif /* ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) &&\
           (0 != Capsense_IS_OVERSAMPLING_EN)) */

    /* Disable Sensor */
    Capsense_DisableScanSlot(sensor);

    CyIntDisable(Capsense_ISR_NUMBER);

#if(Capsense_CMOD_PREGARGE_OPTION == Capsense__CAPPRIOBUF)
    newRegValue = Capsense_CMOD_CONNECTION_REG;
    newRegValue &= ~(Capsense_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Capsense_CSD_CMOD_TO_AMUXBUS_B;
    Capsense_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = Capsense_CMOD_PORT_PC_REG;
    newRegValue &= ~(Capsense_CSD_CMOD_PC_MASK);
    newRegValue |= Capsense_CMOD_PC_STRONG_MODE;
    Capsense_CMOD_PORT_PC_REG = newRegValue;

    newRegValue = Capsense_CMOD_PORT_DR_REG;
    newRegValue &= ~(Capsense_CMOD_DR_MASK);
    newRegValue |= Capsense_CMOD_DR_CONFIG;
    Capsense_CMOD_PORT_DR_REG = newRegValue;
#endif /* (Capsense_CMOD_PREGARGE_OPTION == Capsense__CAPPRIOBUF) */

    /* Connect Vref Buffer to AMUX bus  */
    newRegValue = Capsense_CSD_CFG_REG;
    newRegValue &= ~(Capsense_PRECHARGE_CONFIG_MASK);
    newRegValue |= Capsense_CMOD_PRECHARGE_CONFIG;
    Capsense_CSD_CFG_REG = newRegValue;

    /* Set Idac Value = 0 */
#if (Capsense_IDAC_CNT == 1u)
    Capsense_CSD_IDAC_REG &= ~(Capsense_CSD_IDAC1_DATA_MASK);
#else
    Capsense_CSD_IDAC_REG &= ~(Capsense_CSD_IDAC1_DATA_MASK | Capsense_CSD_IDAC2_DATA_MASK);
#endif /* (Capsense_IDAC_CNT == 1u) */

    CyIntEnable(Capsense_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Capsense_EnableShieldElectrode
********************************************************************************
*
* Summary:
*  This API enables or disables the shield electrode on a specified port pin.
*
* Parameters:
*  portNumber: contains the shield electrode port number (0, 1, 2).
*  pinNumber: contains the shield electrode pin number (0, 1, 2, ... , 7).
*
* Return:
*  None
*
* Global Variables:
*  Capsense_prtCfgTbl[] - Contains pointers to the port config registers for each port.
*  Capsense_prtSelTbl[] - Contains pointers to the HSIOM registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_EnableShieldElectrode(uint32 pinNumber, uint32 portNumber)
{
    uint32 newRegValue;

    *Capsense_prtCfgTbl[portNumber] &= ~(Capsense_CSD_PIN_MODE_MASK << (pinNumber * Capsense_PC_PIN_CFG_SIZE));
    newRegValue = *Capsense_prtSelTbl[portNumber];
    newRegValue &= ~(Capsense_CSD_HSIOM_MASK << (pinNumber * Capsense_HSIOM_PIN_CFG_SIZE));
    newRegValue |= (Capsense_CSD_SHIELD_PORT_MODE << (pinNumber * Capsense_HSIOM_PIN_CFG_SIZE));
    *Capsense_prtSelTbl[portNumber] = newRegValue;
}


/*******************************************************************************
* Function Name: Capsense_SetShieldDelay
********************************************************************************
*
* Summary:
*  This API sets a shield delay.
*
* Parameters:
*  delay:  shield delay value:
*                               0 - no delay
*                               1 - 1 cycle delay
*                               2 - 2 cycles delay
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
void Capsense_SetShieldDelay(uint32 delay)
{
    uint32 newRegValue;

    delay &= 0x03u;

    newRegValue = Capsense_CSD_CFG_REG;
    newRegValue &= (uint32)(~(uint32)Capsense_SHIELD_DELAY);
    newRegValue |= (delay << Capsense_CSD_CFG_SHIELD_DELAY_POS);
    Capsense_CSD_CFG_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Capsense_ReadCurrentScanningSensor
********************************************************************************
*
* Summary:
*  This API returns scanning sensor number when sensor scan is in progress.
*  When sensor scan is completed the API returns
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) (when no sensor is scanned).
*
* Parameters:
*   None.
*
* Return:
*  Returns Sensor number if sensor is being scanned and
*  $INSTANCE_NAME`_NOT_SENSOR (0xFFFFFFFF) if scanning is complete.
*
* Global Variables:
*  Capsense_sensorIndex - the sensor number is being scanned.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_ReadCurrentScanningSensor(void)
{
    return ((uint32)(( 0u != (Capsense_csdStatusVar & Capsense_SW_STS_BUSY)) ?
                     Capsense_sensorIndex : Capsense_NOT_SENSOR));
}


/*******************************************************************************
* Function Name: Capsense_GetBitValue
********************************************************************************
*
* Summary:
*  The API returns a bit status of the bit in the table array which contains the masks.
*
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*
* Return:
*  0 - bit is not set; 1 - bit is set.
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_GetBitValue(const uint8 table[], uint32 position)
{
    uint32 offset;
    uint8 enMask;

    /* position is divided by 8 to calculate the element of the
       table[] array that contains the enable bit
       for an appropriate sensor.
    */
    offset = (position >> 3u);

    /* The enMask calculation for the appropriate sensor. Operation (position & 0x07u)
       intends to calculate the enable bit offset for the 8-bit element of the
       table[] array.
    */
    enMask = 0x01u << (position & 0x07u);

    return ((0u !=(table[offset] & enMask)) ? 1Lu : 0Lu);
}


/*******************************************************************************
* Function Name: Capsense_SetBitValue
********************************************************************************
*
* Summary:
*  The API sets a bit status of the bit in the table array which contains masks.
*
* Parameters:
*  table[] - array with bit masks.
*  position - position of bit in the table[] array.
*  value: 0 - bit is not set; 1 - bit is set.
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
void Capsense_SetBitValue(uint8 table[], uint32 position, uint32 value)
{
    uint32 offset;
    uint8 enMask;

    /* position is divided by 8 to calculate the element of the
       table[] array that contains the enable bit
       for an appropriate sensor.
    */
    offset = (position >> 3u);

    /* The enMask calculation for the appropriate sensor. Operation (position & 0x07u)
       intends to calculate the enable bit offset for the 8-bit element of the
       table[] array.
    */
    enMask = 0x01u << (position & 0x07u);

    if(0u != value)
    {
        table[offset] |= enMask;
    }
    else
    {
        table[offset] &= (uint8)~(enMask);
    }
}



/*******************************************************************************
* Function Name: Capsense_GetSenseClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the sense clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (Capsense_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the sense clock divider of the sensor.
*
* Global Variables:
*  Capsense_senseClkDividerVal[] - stores the sense clock divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_GetSenseClkDivider(uint32 sensor)
{
    #if(0u != Capsense_MULTIPLE_FREQUENCY_SET)
        return((uint32)Capsense_senseClkDividerVal[sensor]);
    #else
        return((uint32)Capsense_senseClkDividerVal);
    #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET) */
}

#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capsense_SetSenseClkDivider
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the sense clock divider for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  senseClk: represents the sense clock value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Capsense_senseClkDividerVal[] - stores the sense clock divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_SetSenseClkDivider(uint32 sensor, uint32 senseClk)
    {
        #if(0u != Capsense_MULTIPLE_FREQUENCY_SET)
            Capsense_senseClkDividerVal[sensor] = (uint8)senseClk;
        #else
            Capsense_senseClkDividerVal = (uint8)senseClk;
        #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */


/*******************************************************************************
* Function Name: Capsense_GetModulatorClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the modulator sample clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The value of index can be
*  from 0 to (Capsense_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulator sample clock divider for the  sensor.
*
* Global Variables:
*  Capsense_sampleClkDividerVal[] - stores the modulator sample divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_GetModulatorClkDivider(uint32 sensor)
{
    #if(0u != Capsense_MULTIPLE_FREQUENCY_SET)
        return((uint32)Capsense_sampleClkDividerVal[sensor]);
    #else
        return((uint32)Capsense_sampleClkDividerVal);
    #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET) */
}

#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capsense_SetModulatorClkDivider
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the modulator sample clock divider for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  modulatorClk: represents the modulator sample clock value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Capsense_sampleClkDividerVal[] - stores the modulator sample divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk)
    {
        #if(0u != Capsense_MULTIPLE_FREQUENCY_SET)
            Capsense_sampleClkDividerVal[sensor] = (uint8)modulatorClk;
        #else
            Capsense_sampleClkDividerVal = (uint8)modulatorClk;
        #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */

/*******************************************************************************
* Function Name: Capsense_GetModulationIDAC
********************************************************************************
*
* Summary:
*  This API returns a value of the modulation IDAC for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (Capsense_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulation IDAC of the sensor.
*
* Global Variables:
*  Capsense_modulationIDAC[] - stores modulation IDAC values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_GetModulationIDAC(uint32 sensor)
{
        return((uint32)Capsense_modulationIDAC[sensor]);
}

#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capsense_SetModulationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of the modulation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  compIdacValue: represents the modulation IDAC data register value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Capsense_modulationIDAC[] - array with modulation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_SetModulationIDAC(uint32 sensor, uint32 modIdacValue)
    {
        Capsense_modulationIDAC[sensor] = (uint8)modIdacValue;
    }
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */


#if(Capsense_IDAC_CNT > 1u)
    /*******************************************************************************
    * Function Name: Capsense_GetCompensationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API returns a value of the compensation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor: sensor index. The index value can be
    *  from 0 to (Capsense_TOTAL_SENSOR_COUNT-1).
    *
    * Return:
    *  This API returns the compensation IDAC of the sensor.
    *
    * Global Variables:
    *  Capsense_compensationIDAC[] - stores the compensation IDAC values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    uint32 Capsense_GetCompensationIDAC(uint32 sensor)
    {
        return((uint32)Capsense_compensationIDAC[sensor]);
    }
#endif /* (Capsense_IDAC_CNT > 1u) */


#if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (Capsense_IDAC_CNT > 1u))
    /*******************************************************************************
    * Function Name: Capsense_SetCompensationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API sets a value of compensation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor:  Sensor index.
    *  compIdacValue: represents the compensation IDAC data register value.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Capsense_compensationIDAC[] - an array with compensation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue)
    {
        Capsense_compensationIDAC[sensor] = (uint8)compIdacValue;
    }
#endif /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (Capsense_IDAC_CNT > 1u)) */

/*******************************************************************************
* Function Name: Capsense_GetIDACRange
********************************************************************************
*
* Summary:
*  This API returns a value that indicates the IDAC range used by the
*  component to scan sensors. The IDAC range is common for all the sensors.
*
* Parameters:
*  None
*
* Return:
*  This API Returns a value that indicates the IDAC range:
*      0 - IDAC range set to 4x
*      1 - IDAC range set to 8x
*
* Global Variables:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capsense_GetIDACRange(void)
{
    return((0u != (Capsense_CSD_IDAC_REG & Capsense_CSD_IDAC1_RANGE_8X)) ? 1uL : 0uL);
}

#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capsense_SetIDACRange
    ********************************************************************************
    *
    * Summary:
    *  This API sets the IDAC range to the 4x (1.2uA/bit) or 8x (2.4uA/bit) mode.
    *  The IDAC range is common for all the sensors and common for the modulation and
    *  compensation IDACs.
    *
    * Parameters:
    *  iDacRange:  represents value for IDAC range
    *  0 -  IDAC range set to 4x (1.2uA/bit)
    *  1 or >1 - IDAC range set to 8x (2.4uA/bit)
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  None
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capsense_SetIDACRange(uint32 iDacRange)
    {
        if(iDacRange != 0u)
        {
            /*  IDAC range = 8x (2.4uA/bit) */
            Capsense_CSD_IDAC_REG |= (Capsense_CSD_IDAC1_RANGE_8X |\
                                              Capsense_CSD_IDAC2_RANGE_8X);
        }
        else
        {
            /*  IDAC range = 4x (1.2uA/bit) */
            /*  IDAC range = 8x (2.4uA/bit) */
            Capsense_CSD_IDAC_REG &= ~(Capsense_CSD_IDAC1_RANGE_8X |\
                                               Capsense_CSD_IDAC2_RANGE_8X);
        }
    }
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */


#if((0u != Capsense_AUTOCALIBRATION_ENABLE) || (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: Capsense_GetSensorRaw
    ********************************************************************************
    *
    * Summary:
    *  The API updates and gets uprated raw data from the sensor.
    *
    * Parameters:
    *  sensor - Sensor number.
    *
    * Return:
    *  Returns the current raw data value for a defined sensor number.
    *
    * Global Variables:
    *  None
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    uint16  Capsense_GetSensorRaw(uint32 sensor)
    {
        uint32 curSample;
        uint32 avgVal = 0u;

        for(curSample = 0u; curSample < Capsense_AVG_SAMPLES_NUM; curSample++)
        {

            Capsense_ScanSensor((uint32)sensor);
            while(Capsense_IsBusy() == 1u)
            {
                /* Wait while sensor is busy */
            }
            avgVal += Capsense_ReadSensorRaw((uint32)sensor);
        }

        return((uint16)(avgVal / Capsense_AVG_SAMPLES_NUM));
    }
#endif /* ((0u != Capsense_AUTOCALIBRATION_ENABLE) && (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)) */


#if((0u != Capsense_AUTOCALIBRATION_ENABLE) && (Capsense_TUNING_METHOD != Capsense__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: Capsense_CalibrateSensor
    ********************************************************************************
    *
    * Summary:
    *  Computes the one sensor IDAC value, which provides the raw signal on
    *  a specified level, with a specified prescaler, speed, and resolution.
    *
    * Parameters:
    *  sensor - Sensor Number.
    *
    *  rawLevel -  Raw data level which should be reached during the calibration
    *              procedure.
    *
    *  idacLevelsTbl - Pointer to the modulatorIdac data register configuration table.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Capsense_CalibrateSensor(uint32 sensor, uint32 rawLevel, uint8 idacLevelsTbl[])
    {
        uint8 mask;
        uint16 rawData;

        rawData = 0u;

        mask = 0x80u;
        /* Init IDAC for null Channel */
        idacLevelsTbl[sensor] = 0x00u;

        do
        {
            /* Set bit for null Channel */
            idacLevelsTbl[sensor] |= mask;

            /* Scan null Channel and get Rawdata */
            rawData = Capsense_GetSensorRaw(sensor);

            /* Decrease IDAC until Rawdata reaches rawLevel */
            if(rawData < rawLevel)
            {
                /* Reset bit for null Channel  */
                idacLevelsTbl[sensor] &= (uint8)~mask;
            }

            mask >>= 1u;
        }
        while(mask > 0u);

    }
#endif /* ((0u != Capsense_AUTOCALIBRATION_ENABLE) && (Capsense_TUNING_METHOD != Capsense__TUNING_AUTO)) */


/*******************************************************************************
* Function Name: Capsense_SetUnscannedSensorState
********************************************************************************
*
* Summary:
*  This API sets a state for un-scanned sensors.
*  It is possible to set the state to Ground, High-Z, or the shield electrode.
*  The un-scanned sensor can be connected to a shield electrode only if the shield is
*  enabled. If the shield is disabled and this API is called with the parameter which
*  indicates the shield state, the un-scanned sensor will be connected to Ground.
*
* Parameters:
*  sensor - Sensor Number.
*  sensorState: This parameter indicates un-scanned sensor state:
*
*     Capsense__GROUND 0
*     Capsense__HIZ_ANALOG 1
*     Capsense__SHIELD 2
*
* Return:
*  None.
*
* Global Variables:
*  Capsense_unscannedSnsDriveMode[] - used to store the state for
*  un-scanned sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_SetUnscannedSensorState(uint32 sensor, uint32 sensorState)
{
    #if(Capsense_IS_COMPLEX_SCANSLOTS)
        uint8 snsType;
        uint8 curSensor;
        uint16 snsNumber;

        /* Read sensor type: single or complex */
        snsType = Capsense_portTable[sensor];

        /* Check if sensor is complex */
        if(0u != (snsType & Capsense_COMPLEX_SS_FLAG))
        {
            snsType &= (uint8)~Capsense_COMPLEX_SS_FLAG;
            snsNumber = (uint16)Capsense_maskTable[sensor];

            for (curSensor=0u; curSensor < snsNumber; curSensor++)
            {
                sensor = Capsense_indexTable[snsType + curSensor];
                Capsense_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
            }
        }
        else
        {
            Capsense_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
        }
    #else
        Capsense_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
    #endif /* (Capsense_IS_COMPLEX_SCANSLOTS) */
}

#if(0u != Capsense_TOTAL_CENTROIDS_COUNT)
/*******************************************************************************
* Function Name: Capsense_NormalizeWidgets
********************************************************************************
*
* Summary:
*  This API aligns all the parameters of the widget to the maximum parameter.
*
* Parameters:
*  uint32 widgetsNum: Number of widgets.
*  uint8 *dataPrt: pointer to array with widget parameters.
*
* Return:
*  None
*
* Global Variables:
*  Capsense_numberOfSensors[] - Number of sensors in the widget.
*  Capsense_rawDataIndex[currentWidget] - Contains the  1st sensor
*  position in the widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capsense_NormalizeWidgets(uint32 widgetsNum, uint8 dataPtr[])
{
    uint32 currentWidget;
    uint32 currentSensor;
    uint32 lastSensor;
    uint32 sensorsPerWidget;
    uint32 maxPerWidget;

    for(currentWidget = 0u; currentWidget < widgetsNum; currentWidget++)
    {
        maxPerWidget = 0u;

        sensorsPerWidget = Capsense_numberOfSensors[currentWidget];
        currentSensor = Capsense_rawDataIndex[currentWidget];
        lastSensor = currentSensor + sensorsPerWidget;

        while(currentSensor < lastSensor)
        {
            if(maxPerWidget < dataPtr[currentSensor])
            {
                maxPerWidget = dataPtr[currentSensor];
            }
            currentSensor++;
        }

        currentSensor = Capsense_rawDataIndex[currentWidget];

        while(currentSensor < lastSensor)
        {
            dataPtr[currentSensor] = (uint8)maxPerWidget;
            currentSensor++;
        }
    }
}
#endif /* (0u != Capsense_TOTAL_CENTROIDS_COUNT) */

/* [] END OF FILE */
