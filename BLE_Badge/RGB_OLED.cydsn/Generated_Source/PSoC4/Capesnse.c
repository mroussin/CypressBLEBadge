/*******************************************************************************
* File Name: Capesnse.c
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

#include "Capesnse.h"
#include "Capesnse_PVT.h"
#include "cypins.h"
#include "cyapicallbacks.h"

#if(0u != Capesnse_CSHL_API_GENERATE)
    #include "Capesnse_CSHL.h"
#endif /* (0u != Capesnse_CSHL_API_GENERATE) */

/* This definition is intended to prevent unexpected linker error.
   For more details please see the IAR Technical Note 49981 */
#if defined(__ICCARM__)
    extern void Capesnse_EnableSensor(uint32 sensor);
    extern void Capesnse_DisableSensor(uint32 sensor);
#endif /* (__ICCARM__) */

/* SmartSense functions */
#if (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)
    uint8 Capesnse_lowLevelTuningDone = 0u;
    uint8 Capesnse_scanSpeedTbl[((Capesnse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
#endif /* (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) */

#if(Capesnse_PRS_OPTIONS != Capesnse__PRS_NONE)
    uint8 Capesnse_prescalersTuningDone = 0u;
#endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE) */

/* Global software variables */
volatile uint8 Capesnse_csdStatusVar = 0u;   /* CapSense CSD status, variable */
volatile uint8 Capesnse_sensorIndex = 0u;    /* Index of scannig sensor */

/* Global array of Raw Counts */
uint16 Capesnse_sensorRaw[Capesnse_TOTAL_SENSOR_COUNT] = {0u};

/* Backup variables for trim registers*/
#if (Capesnse_IDAC1_POLARITY == Capesnse__IDAC_SINK)
    uint8 Capesnse_csdTrim2;
#else
    uint8 Capesnse_csdTrim1;
#endif /* (Capesnse_IDAC1_POLARITY == Capesnse__IDAC_SINK) */

/* Global array of un-scanned sensors state */
uint8 Capesnse_unscannedSnsDriveMode[Capesnse_PORT_PIN_CONFIG_TBL_ZISE];

/* Backup array for Capesnse_sensorEnableMask */
uint8 Capesnse_sensorEnableMaskBackup[(((Capesnse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)];

/* Configured constants and arrays by Customizer */
uint8 Capesnse_sensorEnableMask[(((Capesnse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)] = {
0x1Fu, };

reg32 * Capesnse_pcTable[] = {
    (reg32 *)Capesnse_Sns__LinearSlider0_e0__LS__PC, 
    (reg32 *)Capesnse_Sns__LinearSlider0_e1__LS__PC, 
    (reg32 *)Capesnse_Sns__LinearSlider0_e2__LS__PC, 
    (reg32 *)Capesnse_Sns__LinearSlider0_e3__LS__PC, 
    (reg32 *)Capesnse_Sns__LinearSlider0_e4__LS__PC, 
};

const uint8 Capesnse_portTable[] = {
    Capesnse_Sns__LinearSlider0_e0__LS__PORT, 
    Capesnse_Sns__LinearSlider0_e1__LS__PORT, 
    Capesnse_Sns__LinearSlider0_e2__LS__PORT, 
    Capesnse_Sns__LinearSlider0_e3__LS__PORT, 
    Capesnse_Sns__LinearSlider0_e4__LS__PORT, 
};

const uint32 Capesnse_maskTable[] = {
    Capesnse_Sns__LinearSlider0_e0__LS__MASK, 
    Capesnse_Sns__LinearSlider0_e1__LS__MASK, 
    Capesnse_Sns__LinearSlider0_e2__LS__MASK, 
    Capesnse_Sns__LinearSlider0_e3__LS__MASK, 
    Capesnse_Sns__LinearSlider0_e4__LS__MASK, 
};

const uint8 Capesnse_pinShiftTbl[] = {
    (uint8) Capesnse_Sns__LinearSlider0_e0__LS__SHIFT, 
    (uint8) Capesnse_Sns__LinearSlider0_e1__LS__SHIFT, 
    (uint8) Capesnse_Sns__LinearSlider0_e2__LS__SHIFT, 
    (uint8) Capesnse_Sns__LinearSlider0_e3__LS__SHIFT, 
    (uint8) Capesnse_Sns__LinearSlider0_e4__LS__SHIFT, 
};

uint8 Capesnse_modulationIDAC[Capesnse_TOTAL_SENSOR_COUNT];
uint8 Capesnse_compensationIDAC[Capesnse_TOTAL_SENSOR_COUNT];

uint32 Capesnse_widgetResolution[Capesnse_RESOLUTIONS_TBL_SIZE] = {
    Capesnse_RESOLUTION_12_BITS,
};

uint8 Capesnse_senseClkDividerVal[Capesnse_TOTAL_SCANSLOT_COUNT];
uint8 Capesnse_sampleClkDividerVal[Capesnse_TOTAL_SCANSLOT_COUNT];

const uint8 Capesnse_widgetNumber[Capesnse_TOTAL_SENSOR_COUNT] = {
    0u, 0u, 0u, 0u, 0u, /* LinearSlider0__LS */
};

reg32* const Capesnse_prtSelTbl[Capesnse_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_HSIOM_PORT_SEL0),
    ((reg32 *) CYREG_HSIOM_PORT_SEL1),
    ((reg32 *) CYREG_HSIOM_PORT_SEL2),
    ((reg32 *) CYREG_HSIOM_PORT_SEL3),
    ((reg32 *) CYREG_HSIOM_PORT_SEL4),
    ((reg32 *) CYREG_HSIOM_PORT_SEL5),
    ((reg32 *) CYREG_HSIOM_PORT_SEL6),
};

reg32* const Capesnse_prtCfgTbl[Capesnse_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_PC),
    ((reg32 *) CYREG_GPIO_PRT1_PC),
    ((reg32 *) CYREG_GPIO_PRT2_PC),
    ((reg32 *) CYREG_GPIO_PRT3_PC),
    ((reg32 *) CYREG_GPIO_PRT4_PC),
    ((reg32 *) CYREG_GPIO_PRT5_PC),
    ((reg32 *) CYREG_GPIO_PRT6_PC),
};

reg32* const Capesnse_prtDRTbl[Capesnse_CFG_REG_TBL_SIZE] = {
    ((reg32 *) CYREG_GPIO_PRT0_DR),
    ((reg32 *) CYREG_GPIO_PRT1_DR),
    ((reg32 *) CYREG_GPIO_PRT2_DR),
    ((reg32 *) CYREG_GPIO_PRT3_DR),
    ((reg32 *) CYREG_GPIO_PRT4_DR),
    ((reg32 *) CYREG_GPIO_PRT5_DR),
    ((reg32 *) CYREG_GPIO_PRT6_DR),
};



/*******************************************************************************
* Function Name: Capesnse_Init
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
*  Capesnse_immunityIndex - defines immunity level.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_Init(void)
{
    uint32 curSensor;

    #if(Capesnse_IS_SHIELD_ENABLE)

        #if((Capesnse_CSH_TANK_PREGARGE_OPTION == Capesnse__CAPPRIOBUF) || (0u != Capesnse_CSH_TANK_ENABLE))
            uint32 newRegValue;

            newRegValue = Capesnse_CTANK_CONNECTION_REG;
            newRegValue &= ~(Capesnse_CSD_CTANK_CONNECTION_MASK);
            newRegValue |= Capesnse_CSD_CTANK_TO_AMUXBUS_B;
            Capesnse_CTANK_CONNECTION_REG = newRegValue;

            #if(Capesnse_CSH_TANK_PREGARGE_OPTION == Capesnse__CAPPRIOBUF)
                newRegValue = Capesnse_CTANK_PORT_PC_REG;
                newRegValue &= ~(Capesnse_CSD_CTANK_PC_MASK);
                newRegValue |= Capesnse_CTANK_PC_STRONG_MODE;
                Capesnse_CTANK_PORT_PC_REG = newRegValue;

                newRegValue = Capesnse_CTANK_PORT_DR_REG;
                newRegValue &= ~(Capesnse_CTANK_DR_MASK);
                newRegValue |= Capesnse_CTANK_DR_CONFIG;
                Capesnse_CTANK_PORT_DR_REG = newRegValue;
            #endif /* (Capesnse_CSH_TANK_PREGARGE_OPTION == Capesnse__CAPPRIOBUF) */

        #endif /* ((Capesnse_CSH_TANK_PREGARGE_OPTION == Capesnse__CAPPRIOBUF) || (Capesnse_CSH_TANK_ENABLE)) */

        Capesnse_EnableShieldElectrode((uint32)Capesnse_SHIELD_PIN_NUMBER, (uint32)Capesnse_SHIELD_PORT_NUMBER);

    #endif /* (Capesnse_IS_SHIELD_ENABLE) */

    for(curSensor = 0u; curSensor < Capesnse_TOTAL_SENSOR_COUNT; curSensor++)
    {
        Capesnse_SetUnscannedSensorState(curSensor, Capesnse_CONNECT_INACTIVE_SNS);
    }

    Capesnse_CsdHwBlockInit();
    Capesnse_SetShieldDelay(Capesnse_SHIELD_DELAY );

    /* Clear all sensors */
    Capesnse_ClearSensors();
}

/*******************************************************************************
* Function Name: Capesnse_CsdHwBlockInit
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
*  Capesnse_csdTrim1 - Contains IDAC trim register value for Sourcing Mode.
*  Capesnse_csdTrim2 - Contains IDAC trim register value for Sinking Mode.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_CsdHwBlockInit(void)
{
    uint32 newRegValue;

        /* Set trim registers for CSD Mode */
    #if (Capesnse_IDAC1_POLARITY == Capesnse__IDAC_SINK)
        /* iDAC1 Sinking Mode */
        Capesnse_csdTrim2 = (uint8)Capesnse_CSD_TRIM2_REG;
        newRegValue = Capesnse_csdTrim2;
        newRegValue &= Capesnse_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)Capesnse_SFLASH_CSD_TRIM2_REG & (uint32)Capesnse_CSFLASH_TRIM_IDAC1_MASK);

        #if (Capesnse_IDAC_CNT == 2u)
            /* iDAC2 Sinking Mode */
            newRegValue &= Capesnse_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)Capesnse_SFLASH_CSD_TRIM2_REG & (uint32)Capesnse_CSFLASH_TRIM_IDAC2_MASK);
        #endif /* (Capesnse_IDAC_CNT == 2u) */
        Capesnse_CSD_TRIM2_REG = newRegValue;
    #else
        /* iDAC1 Sourcing Mode */
        Capesnse_csdTrim1 = (uint8)Capesnse_CSD_TRIM1_REG;
        newRegValue = Capesnse_csdTrim1;
        newRegValue &= Capesnse_CSD_IDAC1_TRIM_MASK;
        newRegValue |= (uint32)((uint32)Capesnse_SFLASH_CSD_TRIM1_REG & (uint32)Capesnse_CSFLASH_TRIM_IDAC1_MASK);
        #if (Capesnse_IDAC_CNT == 2u)
             /* iDAC2 Sourcing Mode */
            newRegValue &= Capesnse_CSD_IDAC2_TRIM_MASK;
            newRegValue |= (uint32)((uint32)Capesnse_SFLASH_CSD_TRIM1_REG & (uint32)Capesnse_CSFLASH_TRIM_IDAC2_MASK);
        #endif
        Capesnse_CSD_TRIM1_REG = newRegValue;
    #endif /* (Capesnse_IDAC1_POLARITY == Capesnse__IDAC_SINK) */

    /* Configure CSD and IDAC */
    #if (Capesnse_IDAC_CNT > 1u)
        Capesnse_CSD_IDAC_REG = Capesnse_DEFAULT_CSD_IDAC_CONFIG;
        Capesnse_CSD_CFG_REG = Capesnse_DEFAULT_CSD_CONFIG;
    #else
        Capesnse_CSD_IDAC_REG &= ~(Capesnse_CSD_IDAC1_MODE_MASK  | Capesnse_CSD_IDAC1_DATA_MASK);
        Capesnse_CSD_IDAC_REG |= Capesnse_DEFAULT_CSD_IDAC_CONFIG;

        Capesnse_CSD_CFG_REG &= ~(Capesnse_CSD_CONFIG_MASK);
        Capesnse_CSD_CFG_REG |= (Capesnse_DEFAULT_CSD_CONFIG);
    #endif /* (Capesnse_IDAC_CNT > 1u) */


    /* Connect Cmod to AMUX bus */
    newRegValue = Capesnse_CMOD_CONNECTION_REG;
    newRegValue &= ~(Capesnse_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Capesnse_CSD_CMOD_TO_AMUXBUS_A;
    Capesnse_CMOD_CONNECTION_REG = newRegValue;

    /* Configure Dead Band PWM if it is enabled */
    #if(Capesnse_CSD_4B_PWM_MODE != Capesnse__PWM_OFF)
        Capesnse_CSD_4B_PWM_REG = Capesnse_DEFAULT_CSD_4B_PWM_CONFIG;
    #endif /* (Capesnse_CSD_4B_PWM_MODE != Capesnse__PWM_OFF) */

    /* Setup ISR */
    CyIntDisable(Capesnse_ISR_NUMBER);
    #if !defined(CY_EXTERNAL_INTERRUPT_CONFIG)
        (void)CyIntSetVector(Capesnse_ISR_NUMBER, &Capesnse_ISR);
        CyIntSetPriority(Capesnse_ISR_NUMBER, Capesnse_ISR_PRIORITY);
    #endif /* (CY_EXTERNAL_INTERRUPT_CONFIG) */
}


/*******************************************************************************
* Function Name: Capesnse_Enable
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
void Capesnse_Enable(void)
{
    uint32 newRegValue;

    /* Enable Clocks */
    Capesnse_SenseClk_Stop();
    Capesnse_SampleClk_Stop();

    Capesnse_SampleClk_SetDividerValue((uint16)Capesnse_INITIAL_CLK_DIVIDER);
    Capesnse_SenseClk_SetDividerValue((uint16)Capesnse_INITIAL_CLK_DIVIDER);

    #if (0u == Capesnse_IS_M0S8PERI_BLOCK)
        Capesnse_SenseClk_Start();
        Capesnse_SampleClk_Start();
    #else
        Capesnse_SampleClk_Start();
        Capesnse_SenseClk_StartEx(Capesnse_SampleClk__DIV_ID);
    #endif /* (0u == Capesnse_IS_M0S8PERI_BLOCK) */

    /* Enable the CSD block */
    newRegValue = Capesnse_CSD_CFG_REG;
    newRegValue |= (Capesnse_CSD_CFG_ENABLE | Capesnse_CSD_CFG_SENSE_COMP_EN
                                                   | Capesnse_CSD_CFG_SENSE_EN);
    Capesnse_CSD_CFG_REG = newRegValue;

    /* Enable interrupt */
    CyIntEnable(Capesnse_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Capesnse_Start
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
*   Capesnse_initVar - used to check the initial configuration,
*   modified on the first function call.
*  Capesnse_lowLevelTuningDone - Used to notify the Tuner GUI that
*   tuning of the scanning parameters is done.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_Start(void)
{

    static uint8 Capesnse_initVar = 0u;

    #if((0u != Capesnse_AUTOCALIBRATION_ENABLE) && (Capesnse_TUNING_METHOD != Capesnse__TUNING_AUTO))
        uint32 curSensor;
        uint32 rawLevel;
        uint32 widget;
    #endif /* ((0u != Capesnse_AUTOCALIBRATION_ENABLE) && (Capesnse_TUNING_METHOD != Capesnse__TUNING_AUTO)) */

    if (Capesnse_initVar == 0u)
    {
        Capesnse_Init();
        Capesnse_initVar = 1u;
    }
    Capesnse_Enable();

    /* AutoTunning start */
    #if(Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)
        #if(0u != Capesnse_CSHL_API_GENERATE)
            Capesnse_AutoTune();
            Capesnse_lowLevelTuningDone = 1u;
        #endif /* (0u != Capesnse_CSHL_API_GENERATE) */
    #else
        #if(0u != Capesnse_AUTOCALIBRATION_ENABLE)

            #if(Capesnse_IDAC_CNT > 1u)
                Capesnse_CSD_IDAC_REG &= ~(Capesnse_CSD_IDAC2_MODE_MASK);
            #endif /* (Capesnse_IDAC_CNT > 1u) */

            for(curSensor = 0u; curSensor < Capesnse_TOTAL_SCANSLOT_COUNT; curSensor++)
            {
                widget = Capesnse_widgetNumber[curSensor];
                rawLevel = Capesnse_widgetResolution[widget] >> Capesnse_RESOLUTION_OFFSET;

                /* Calibration level should be equal to 85% from scanning resolution */
                rawLevel = (rawLevel * 85u) / 100u;

                Capesnse_CalibrateSensor(curSensor, rawLevel, Capesnse_modulationIDAC);
            }

            #if(0u != Capesnse_TOTAL_CENTROIDS_COUNT)
                Capesnse_NormalizeWidgets(Capesnse_END_OF_WIDGETS_INDEX, Capesnse_modulationIDAC);
            #endif /* (0u != Capesnse_TOTAL_CENTROIDS_COUNT)  */

            #if(Capesnse_IDAC_CNT > 1u)
                for(curSensor = 0u; curSensor < Capesnse_TOTAL_SCANSLOT_COUNT; curSensor++)
                {
                    Capesnse_compensationIDAC[curSensor] = Capesnse_modulationIDAC[curSensor] / 2u;
                    Capesnse_modulationIDAC[curSensor] = (Capesnse_modulationIDAC[curSensor] + 1u) / 2u;
                }
                Capesnse_CSD_IDAC_REG |= Capesnse_CSD_IDAC2_MODE_FIXED;
            #endif /* (Capesnse_IDAC_CNT > 1u) */

        #endif /* (0u != Capesnse_AUTOCALIBRATION_ENABLE) */

    #endif /* ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) */

    /* Initialize Advanced Centroid */
    #if(Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
        Capesnse_AdvancedCentroidInit();
    #endif /* (Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */

    /* Connect sense comparator input to AMUXA */
    Capesnse_CSD_CFG_REG |= Capesnse_CSD_CFG_SENSE_INSEL;
}


/*******************************************************************************
* Function Name: Capesnse_Stop
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
*  Capesnse_csdTrim1 - Contains the IDAC trim register value for the Sourcing Mode.
*  Capesnse_csdTrim2 - Contains the IDAC trim register value for vSinking Mode.
*
* Side Effects:
*  This function should be called after scans are completed.
*
*******************************************************************************/
void Capesnse_Stop(void)
{
    /* Disable interrupt */
    CyIntDisable(Capesnse_ISR_NUMBER);

    Capesnse_CSD_CFG_REG &= ~(Capesnse_CSD_CFG_SENSE_COMP_EN | Capesnse_CSD_CFG_SENSE_EN);

    #if(Capesnse_IDAC_CNT == 2u)
        Capesnse_CSD_CFG_REG &= ~(Capesnse_CSD_CFG_ENABLE);
    #endif /* (Capesnse_IDAC_CNT == 2u) */

    /* Disable the Clocks */
    Capesnse_SenseClk_Stop();
    Capesnse_SampleClk_Stop();
}


/*******************************************************************************
* Function Name: Capesnse_FindNextSensor
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
*  Capesnse_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*
* Side Effects:
*  This function affects the  current scanning and should not
*  be used outside the component.
*
*******************************************************************************/
uint8 Capesnse_FindNextSensor(uint8 snsIndex)
{
    uint32 enableFlag;

    /* Check if sensor enabled */
    do
    {
        /* Proceed with next sensor */
        snsIndex++;
        if(snsIndex == Capesnse_TOTAL_SENSOR_COUNT)
        {
            break;
        }
        enableFlag = Capesnse_GetBitValue(Capesnse_sensorEnableMask, (uint32)snsIndex);
    }
    while(enableFlag == 0u);

    return ((uint8)snsIndex);
}


/*******************************************************************************
* Function Name: Capesnse_ScanSensor
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
*  Capesnse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and mode of scan as single scan.
*  Capesnse_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to the provided sensor argument.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_ScanSensor(uint32 sensor)
{
    /* Clears status/control variable and set sensorIndex */
    Capesnse_csdStatusVar = 0u;
    Capesnse_sensorIndex = (uint8)sensor;

    /* Start of sensor scan */
    Capesnse_csdStatusVar = (Capesnse_SW_STS_BUSY | Capesnse_SW_CTRL_SINGLE_SCAN);
    Capesnse_PreScan(sensor);
}


#if(0u != Capesnse_CSHL_API_GENERATE)
/*******************************************************************************
* Function Name: Capesnse_ScanWidget
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
*  Capesnse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Capesnse_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Capesnse_FindNextSensor or
*  Capesnse_sensorEnableMask[] - used to store bit masks of the enabled sensors.
*  Capesnse_sensorEnableMaskBackup[] - used to backup bit masks of the enabled
*   sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_ScanWidget(uint32 widget)
{
    uint32 sensorsPerWidget;
    uint32 lastSensor;
    uint32 snsIndex;

    /* Get first sensor in widget */
    Capesnse_sensorIndex = Capesnse_rawDataIndex[widget];

    /* Get number of sensors in widget */
    sensorsPerWidget = Capesnse_numberOfSensors[widget];

    /* Check if generic Sensor */
    if(sensorsPerWidget == 0u)
    {
        sensorsPerWidget = 1u;
    }

    /* Get last sensor in widget */
    lastSensor = (Capesnse_rawDataIndex[widget] + sensorsPerWidget) - 1u;

    /* Backup sensorEnableMask array */
    for(snsIndex = 0u; snsIndex < Capesnse_TOTAL_SENSOR_MASK; snsIndex++)
    {
        /* Backup sensorEnableMask array */
        Capesnse_sensorEnableMaskBackup[snsIndex] = Capesnse_sensorEnableMask[snsIndex];
    }

    /* Update sensorEnableMask to scan the sensors that belong to widget */
    for(snsIndex = 0u; snsIndex < Capesnse_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        /* Update sensorEnableMask array bits to scan the widget only */
        if((snsIndex >= Capesnse_sensorIndex) && (snsIndex <= lastSensor))
        {
            /* Set sensor bit to scan */
            Capesnse_SetBitValue(Capesnse_sensorEnableMask, snsIndex, 1u);
        }
        else
        {
            /* Reset sensor bit to do not scan */
            Capesnse_SetBitValue(Capesnse_sensorEnableMask, snsIndex, 0u);
        }
    }

    /* Check end of scan condition */
    if(Capesnse_sensorIndex < Capesnse_TOTAL_SENSOR_COUNT)
    {
        /* Set widget busy bit in status/control variable*/
        Capesnse_csdStatusVar = (Capesnse_SW_STS_BUSY | Capesnse_SW_CTRL_WIDGET_SCAN);
        /* Scan first sensor of widget*/
        Capesnse_PreScan((uint32)Capesnse_sensorIndex);
    }
}
#endif /* (0u != Capesnse_CSHL_API_GENERATE) */


/*******************************************************************************
* Function Name: Capesnse_ScanEnableWidgets
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
*  Capesnse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Sets the busy status(scan in progress) and clears the single scan mode.
*  Capesnse_sensorIndex - used to store a sensor scanning sensor number.
*  Sets to 0xFF and provided to function Capesnse_FindNextSensor or
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_ScanEnabledWidgets(void)
{
    /* Clears status/control variable and set sensorIndex */
    Capesnse_csdStatusVar = 0u;
    Capesnse_sensorIndex = 0xFFu;

    /* Find next sensor */
    Capesnse_sensorIndex = (uint8)Capesnse_FindNextSensor(Capesnse_sensorIndex);

    /* Check end of scan condition */
    if(Capesnse_sensorIndex < Capesnse_TOTAL_SENSOR_COUNT)
    {
        Capesnse_csdStatusVar |= Capesnse_SW_STS_BUSY;
        Capesnse_PreScan((uint32)Capesnse_sensorIndex);
    }
}


/*******************************************************************************
* Function Name: Capesnse_IsBusy
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
*  Capesnse_csdStatusVar - used to provide the status and mode of the scanning process.
*  Checks the busy status.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capesnse_IsBusy(void)
{
    return ((uint32)((0u != (Capesnse_csdStatusVar & Capesnse_SW_STS_BUSY)) ? 1u : 0u));
}


/*******************************************************************************
* Function Name: Capesnse_ReadSensorRaw
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
*  Capesnse_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint16 Capesnse_ReadSensorRaw(uint32 sensor)
{
    return Capesnse_sensorRaw[sensor];
}


/*******************************************************************************
* Function Name: Capesnse_WriteSensorRaw
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
*  Capesnse_sensorRaw[] - used to store sensors raw data.
*
* Return:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_WriteSensorRaw(uint32 sensor, uint16 value)
{
    Capesnse_sensorRaw[sensor] = value;
}


#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capesnse_SetScanResolution
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
    *              Capesnse.h file should be used:
    *              Capesnse_RESOLUTION_6_BITS
    *              Capesnse_RESOLUTION_7_BITS
    *              Capesnse_RESOLUTION_8_BITS
    *              Capesnse_RESOLUTION_9_BITS
    *              Capesnse_RESOLUTION_10_BITS
    *              Capesnse_RESOLUTION_11_BITS
    *              Capesnse_RESOLUTION_12_BITS
    *              Capesnse_RESOLUTION_13_BITS
    *              Capesnse_RESOLUTION_14_BITS
    *              Capesnse_RESOLUTION_15_BITS
    *              Capesnse_RESOLUTION_16_BITS
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Capesnse_widgetResolution[] - used to store scan resolution of each widget.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capesnse_SetScanResolution(uint32 widget, uint32 resolution)
    {
        Capesnse_widgetResolution[widget] = resolution;
    }
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */


/*******************************************************************************
* Function Name: Capesnse_GetScanResolution
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
*              The resolution values are defined in the Capesnse.h file
*              The defines are encountered below:
*              Capesnse_RESOLUTION_6_BITS
*              Capesnse_RESOLUTION_7_BITS
*              Capesnse_RESOLUTION_8_BITS
*              Capesnse_RESOLUTION_9_BITS
*              Capesnse_RESOLUTION_10_BITS
*              Capesnse_RESOLUTION_11_BITS
*              Capesnse_RESOLUTION_12_BITS
*              Capesnse_RESOLUTION_13_BITS
*              Capesnse_RESOLUTION_14_BITS
*              Capesnse_RESOLUTION_15_BITS
*              Capesnse_RESOLUTION_16_BITS
*
* Global Variables:
*  Capesnse_widgetResolution[] - used to store scan resolution of every widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capesnse_GetScanResolution(uint32 widget)
{
    return(Capesnse_widgetResolution[widget]);
}


/*******************************************************************************
* Function Name: Capesnse_ClearSensors
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
void Capesnse_ClearSensors(void)
{
    uint32 snsIndex;

    for (snsIndex = 0u; snsIndex < Capesnse_TOTAL_SENSOR_COUNT; snsIndex++)
    {
        Capesnse_DisableScanSlot(snsIndex);
    }
}


#if (Capesnse_IS_COMPLEX_SCANSLOTS)
    /*******************************************************************************
    * Function Name: Capesnse_EnableScanSlot
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
    *  Capesnse_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  Capesnse_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  Capesnse_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  Bit 7 (msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capesnse_EnableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;
        /* Read sensor type: single or complex */
        uint8 snsType = Capesnse_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & Capesnse_COMPLEX_SS_FLAG) == 0u)
        {
            /* Enable sensor (single) */
            Capesnse_EnableSensor(slot);
        }
        else
        {
            /* Enable complex sensor */
            snsType &= ~Capesnse_COMPLEX_SS_FLAG;
            index = &Capesnse_indexTable[snsType];
            snsNumber = Capesnse_maskTable[slot];

            for (j = 0u; j < snsNumber; j++)
            {
                Capesnse_EnableSensor(index[j]);
            }
        }
    }


    /*******************************************************************************
    * Function Name: Capesnse_DisableScanSlot
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
    *  Capesnse_portTable[]  - used to store the port number that pin
    *  belongs to for each sensor.
    *  Capesnse_maskTable[]  - used to store the pin within the port for
    *  each sensor.
    *  Capesnse_indexTable[] - used to store indexes of complex sensors.
    *  The offset and position in this array are stored in a port and mask table for
    *  complex sensors.
    *  7bit(msb) is used to define the sensor type: single or complex.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capesnse_DisableScanSlot(uint32 slot)
    {
        uint8 j;
        uint8 snsNumber;
        const uint8 *index;

        /* Read sensor type: single or complex */
        uint8 snsType = Capesnse_portTable[slot];

        /* Check if sensor is complex */
        if ((snsType & Capesnse_COMPLEX_SS_FLAG) == 0u)
        {
            /* Disable sensor (single) */
            Capesnse_DisableSensor(slot);
        }
        else
        {
            /* Disable complex sensor */
            snsType &= ~Capesnse_COMPLEX_SS_FLAG;
            index = &Capesnse_indexTable[snsType];
            snsNumber = Capesnse_maskTable[slot];

            for (j=0; j < snsNumber; j++)
            {
                Capesnse_DisableSensor(index[j]);
            }
        }
    }
#endif  /* Capesnse_IS_COMPLEX_SCANSLOTS */


/*******************************************************************************
* Function Name: Capesnse_EnableSensor
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
*  Capesnse_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Capesnse_pinShiftTbl[] - used to store position of pin that
*  configured as sensor in port.
*  Capesnse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Capesnse_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_EnableSensor(uint32 sensor)
{
    uint8  pinModeShift;
    uint8  pinHSIOMShift;
    uint8 interruptState;
    uint32 newRegisterValue;
    uint32 port;

    port = (uint32) Capesnse_portTable[sensor];
    pinModeShift = Capesnse_pinShiftTbl[sensor]  * Capesnse_PC_PIN_CFG_SIZE;
    pinHSIOMShift = Capesnse_pinShiftTbl[sensor] * Capesnse_HSIOM_PIN_CFG_SIZE;

    interruptState = CyEnterCriticalSection();

    newRegisterValue = *Capesnse_prtSelTbl[port];
    newRegisterValue &= ~(Capesnse_CSD_HSIOM_MASK << pinHSIOMShift);
    newRegisterValue |= (uint32)((uint32)Capesnse_CSD_SENSE_PORT_MODE << pinHSIOMShift);

    *Capesnse_prtCfgTbl[port] &= (uint32)~((uint32)Capesnse_CSD_PIN_MODE_MASK << pinModeShift);
    *Capesnse_prtSelTbl[port] = newRegisterValue;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Capesnse_DisableSensor
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
*  Capesnse_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Capesnse_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*  Capesnse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Capesnse_PrtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_DisableSensor(uint32 sensor)
{
    uint8 interruptState;
    uint32 newRegisterValue;

    uint32 port = (uint32) Capesnse_portTable[sensor];
    uint8  pinHSIOMShift = Capesnse_pinShiftTbl[sensor] * Capesnse_HSIOM_PIN_CFG_SIZE;
    uint8  pinModeShift = Capesnse_pinShiftTbl[sensor]  * Capesnse_PC_PIN_CFG_SIZE;

    uint32 inactiveConnect = Capesnse_SNS_HIZANALOG_CONNECT;
    uint32 sensorState = Capesnse_unscannedSnsDriveMode[sensor];

    *Capesnse_prtSelTbl[port] &= ~(Capesnse_CSD_HSIOM_MASK << pinHSIOMShift);

    #if(Capesnse_IS_SHIELD_ENABLE != 0)
    if(sensorState != (uint32)Capesnse__SHIELD)
    {
    #else
        /* Connected to Ground if shield is disabled */
        if(sensorState == (uint32)Capesnse__SHIELD)
        {
            sensorState = (uint32)Capesnse__GROUND;
        }
    #endif /* (Capesnse_IS_SHIELD_ENABLE) */
        if(sensorState != (uint32)Capesnse__HIZ_ANALOG)
        {
            /* Connected to Ground */
            inactiveConnect = (uint32)Capesnse_SNS_GROUND_CONNECT;
        }

        interruptState = CyEnterCriticalSection();

        /* Set drive mode */
        newRegisterValue = *Capesnse_prtCfgTbl[port];
        newRegisterValue &= ~(Capesnse_CSD_PIN_MODE_MASK << pinModeShift);
        newRegisterValue |=  (uint32)(inactiveConnect << pinModeShift);
        *Capesnse_prtCfgTbl[port] =  newRegisterValue;

        CyExitCriticalSection(interruptState);

        *Capesnse_prtDRTbl[port]  &=  (uint32)~(uint32)((uint32)1u << Capesnse_pinShiftTbl[sensor]);
    #if(Capesnse_IS_SHIELD_ENABLE != 0)
    }
    else
    {
        /* Connect to Shield */
        *Capesnse_prtSelTbl[port] |= (Capesnse_CSD_SHIELD_PORT_MODE << pinHSIOMShift);
    }
    #endif /* (Capesnse_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Capesnse_SetDriveModeAllPins
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
*  Capesnse_portTable[] - used to store the port number that pin
*  belongs to for each sensor.
*  Capesnse_pinShiftTbl[] - used to store position of pin that
*  configured as a sensor in the port.
*
* Side Effects:
*  This API shall be called only after CapSense component is stopped.
*
*******************************************************************************/
void Capesnse_SetDriveModeAllPins(uint32 driveMode)
{
    uint8 curSensor;
    #if (Capesnse_IS_COMPLEX_SCANSLOTS)
        uint8 snsNumber;
        uint8 snsType;
    #endif  /* Capesnse_IS_COMPLEX_SCANSLOTS */
    uint32 prtNumberTmp;
    uint32 pinNumberTmp;

    for(curSensor = 0u; curSensor < Capesnse_TOTAL_SENSOR_COUNT; curSensor++)
    {
        #if (Capesnse_IS_COMPLEX_SCANSLOTS)
            /* Read sensor type: single or complex */
            snsType = Capesnse_portTable[curSensor];

            /* Check if sensor is complex */
            if ((snsType & Capesnse_COMPLEX_SS_FLAG) == 0u)
            {
                /* The sensor is not complex */
                snsNumber = curSensor;
            }
            else
            {
                /* Get dedicated sensor ID of the complex sensor */
                snsType &= ~Capesnse_COMPLEX_SS_FLAG;
                snsNumber = Capesnse_indexTable[snsType];
            }

            prtNumberTmp = Capesnse_portTable[snsNumber];
            pinNumberTmp = Capesnse_pinShiftTbl[snsNumber];
        #else
            prtNumberTmp = Capesnse_portTable[curSensor];
            pinNumberTmp = Capesnse_pinShiftTbl[curSensor];
        #endif  /* Capesnse_IS_COMPLEX_SCANSLOTS */

        Capesnse_SetPinDriveMode(driveMode, pinNumberTmp, prtNumberTmp);
    }

    Capesnse_SetPinDriveMode(driveMode, (uint32)Capesnse_CMOD_PIN_NUMBER, (uint32)Capesnse_CMOD_PRT_NUMBER);

    #if(0u != Capesnse_CSH_TANK_ENABLE)
        Capesnse_SetPinDriveMode(driveMode, (uint32)Capesnse_CTANK_PIN_NUMBER, (uint32)Capesnse_CTANK_PRT_NUMBER);
    #endif /* (0u != Capesnse_CSH_TANK_ENABLE) */

    #if(0u != Capesnse_IS_SHIELD_ENABLE)
        Capesnse_SetPinDriveMode(driveMode, (uint32)Capesnse_SHIELD_PIN_NUMBER, (uint32)Capesnse_SHIELD_PORT_NUMBER);
    #endif /* (0u != Capesnse_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Capesnse_RestoreDriveModeAllPins
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
*  Capesnse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*
* Side Effects:
*  This API is called only after the CapSense component is stopped.
*
*******************************************************************************/
void Capesnse_RestoreDriveModeAllPins(void)
{
    uint32 newRegisterValue;

    Capesnse_SetDriveModeAllPins(CY_SYS_PINS_DM_ALG_HIZ);

    newRegisterValue = Capesnse_CMOD_CONNECTION_REG;
    newRegisterValue &= ~(Capesnse_CSD_CMOD_CONNECTION_MASK);
    newRegisterValue |= Capesnse_CSD_CMOD_TO_AMUXBUS_A;
    Capesnse_CMOD_CONNECTION_REG = newRegisterValue;

    #if(0u != Capesnse_CSH_TANK_ENABLE)
        newRegisterValue = Capesnse_CTANK_CONNECTION_REG;
        newRegisterValue &= ~(Capesnse_CSD_CTANK_CONNECTION_MASK);
        newRegisterValue |= Capesnse_CSD_CTANK_TO_AMUXBUS_B;
        Capesnse_CTANK_CONNECTION_REG = newRegisterValue;
    #endif/* (0u != Capesnse_CSH_TANK_ENABLE) */

    #if(0u != Capesnse_IS_SHIELD_ENABLE)
        newRegisterValue = *Capesnse_prtSelTbl[Capesnse_SHIELD_PORT_NUMBER];
        newRegisterValue &= ~(Capesnse_CSD_HSIOM_MASK << ((uint32)Capesnse_SHIELD_PIN_NUMBER * Capesnse_HSIOM_PIN_CFG_SIZE));
        newRegisterValue |= (Capesnse_CSD_SHIELD_PORT_MODE << ((uint32)Capesnse_SHIELD_PIN_NUMBER * Capesnse_HSIOM_PIN_CFG_SIZE));
        *Capesnse_prtSelTbl[Capesnse_SHIELD_PORT_NUMBER] = newRegisterValue;
    #endif /* (0u != Capesnse_IS_SHIELD_ENABLE) */
}


/*******************************************************************************
* Function Name: Capesnse_SetPinDriveMode
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
*  Capesnse_prtSelTbl[] - Contains pointers to the HSIOM
*  registers for each port.
*  Capesnse_prtCfgTb[] - Contains pointers to the port config
*  registers for each port.
*
* Side Effects:
*  This API is called only after CapSense component is stopped.
*
*******************************************************************************/
void Capesnse_SetPinDriveMode(uint32 driveMode, uint32 pinNumber, uint32 portNumber)
{
    uint32  pinModeShift;
    uint32 newRegisterValue;

    pinModeShift  = pinNumber * Capesnse_PC_PIN_CFG_SIZE;

    newRegisterValue = *Capesnse_prtCfgTbl[portNumber];
    newRegisterValue &= ~(Capesnse_CSD_PIN_MODE_MASK << pinModeShift);
    newRegisterValue |=  (uint32)((uint32)driveMode << pinModeShift);
    *Capesnse_prtCfgTbl[portNumber] =  newRegisterValue;
}


/*******************************************************************************
* Function Name: Capesnse_PreScan
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
* Capesnse_widgetNumber[] - This array contains numbers of widgets for each sensor.
* Capesnse_widgetResolution[] - Contains the widget resolution.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_PreScan(uint32 sensor)
{
    uint8 widget;
    uint8 interruptState;
    uint32 newRegValue;
    uint32 counterResolution;

    #if(Capesnse_PRS_OPTIONS == Capesnse__PRS_AUTO)
        uint8 senseClkDivMath;
        uint8 sampleClkDivMath;
    #endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_AUTO) */

    #if ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
         (0 != Capesnse_IS_OVERSAMPLING_EN))
        uint32 oversamplingFactor;
    #endif /* ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
               (0 != Capesnse_IS_OVERSAMPLING_EN)) */

    /* Define widget sensor belongs to */
    widget = Capesnse_widgetNumber[sensor];

    /* Recalculate Counter Resolution to MSB 16 bits */
    counterResolution = Capesnse_widgetResolution[widget];

    #if ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
         (0 != Capesnse_IS_OVERSAMPLING_EN))
        oversamplingFactor = Capesnse_GetBitValue(Capesnse_scanSpeedTbl, sensor);

        if(counterResolution < Capesnse_RESOLUTION_16_BITS)
        {
            counterResolution <<= oversamplingFactor;
            counterResolution |= (uint32)(Capesnse_RESOLUTION_8_BITS);
        }
    #endif /* ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
               (0 != Capesnse_IS_OVERSAMPLING_EN)) */

    #if (0u != Capesnse_IS_M0S8PERI_BLOCK)
        Capesnse_SenseClk_Stop();
        Capesnse_SampleClk_Stop();
    #endif /* (0u != Capesnse_IS_M0S8PERI_BLOCK) */

    #if (0u != Capesnse_MULTIPLE_FREQUENCY_SET)
        Capesnse_SampleClk_SetDividerValue((uint16)Capesnse_sampleClkDividerVal[sensor]);
        Capesnse_SenseClk_SetDividerValue((uint16)Capesnse_senseClkDividerVal[sensor]);
    #else
        Capesnse_SampleClk_SetDividerValue((uint16)Capesnse_sampleClkDividerVal);
        Capesnse_SenseClk_SetDividerValue((uint16)Capesnse_senseClkDividerVal);
    #endif /* (0u != Capesnse_MULTIPLE_FREQUENCY_SET) */

    #if (0u != Capesnse_IS_M0S8PERI_BLOCK)
        Capesnse_SampleClk_Start();
        Capesnse_SenseClk_StartEx(Capesnse_SampleClk__DIV_ID);

        #if(Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE)
            CyIntDisable(Capesnse_ISR_NUMBER);
            Capesnse_CSD_CNT_REG = Capesnse_ONE_CYCLE;
            while(0u != (Capesnse_CSD_CNT_REG & Capesnse_RESOLUTION_16_BITS))
            {
            /* Wait until scanning is complete */
            }
            Capesnse_CSD_INTR_REG = 1u;
            CyIntClearPending(Capesnse_ISR_NUMBER);
            CyIntEnable(Capesnse_ISR_NUMBER);
        #endif /* Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE */
    #endif /* (0u != Capesnse_IS_M0S8PERI_BLOCK) */

#if(Capesnse_PRS_OPTIONS != Capesnse__PRS_NONE)

    #if(Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)
        if(Capesnse_prescalersTuningDone != 0u)
        {
    #endif /* (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) */

            CyIntDisable(Capesnse_ISR_NUMBER);

            newRegValue = Capesnse_CSD_CFG_REG;
            newRegValue |= Capesnse_CSD_CFG_PRS_SELECT;

            #if(Capesnse_PRS_OPTIONS == Capesnse__PRS_AUTO)

                newRegValue &= ~(Capesnse_PRS_MODE_MASK);

                #if (0u != Capesnse_MULTIPLE_FREQUENCY_SET)
                    senseClkDivMath = Capesnse_senseClkDividerVal[sensor];
                    sampleClkDivMath = Capesnse_sampleClkDividerVal[sensor];
                #else
                    senseClkDivMath = Capesnse_senseClkDividerVal;
                    sampleClkDivMath = Capesnse_sampleClkDividerVal;
                #endif /* ( Capesnse_MULTIPLE_FREQUENCY_SET) */

                #if(0u == Capesnse_IS_M0S8PERI_BLOCK)
                    senseClkDivMath *= sampleClkDivMath;
                #endif /* (0u == Capesnse_IS_M0S8PERI_BLOCK) */

                if((senseClkDivMath * Capesnse_RESOLUTION_12_BITS) <
                   (sampleClkDivMath * Capesnse_widgetResolution[widget]))
                {
                    newRegValue |= Capesnse_CSD_PRS_12_BIT;
                }
            #endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_AUTO) */

            Capesnse_CSD_CFG_REG = newRegValue;

            CyIntEnable(Capesnse_ISR_NUMBER);

    #if(Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)
        }
    #endif /* (Capesnse_PRS_OPTIONS != Capesnse__PRS_NONE) */

#endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE) */

    /* Set Idac Value */
    CyIntDisable(Capesnse_ISR_NUMBER);
    newRegValue = Capesnse_CSD_IDAC_REG;

#if (Capesnse_IDAC_CNT == 1u)
    newRegValue &= ~(Capesnse_CSD_IDAC1_DATA_MASK);
    newRegValue |= Capesnse_modulationIDAC[sensor];
#else
    newRegValue &= ~(Capesnse_CSD_IDAC1_DATA_MASK | Capesnse_CSD_IDAC2_DATA_MASK);
    newRegValue |= (Capesnse_modulationIDAC[sensor] |
                            (uint32)((uint32)Capesnse_compensationIDAC[sensor] <<
                            Capesnse_CSD_IDAC2_DATA_OFFSET));
#endif /* (Capesnse_IDAC_CNT == 1u) */

    Capesnse_CSD_IDAC_REG = newRegValue;

#if(Capesnse_CMOD_PREGARGE_OPTION == Capesnse__CAPPRIOBUF)
    newRegValue = Capesnse_CMOD_CONNECTION_REG;
    newRegValue &= ~(Capesnse_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Capesnse_CSD_CMOD_TO_AMUXBUS_A;
    Capesnse_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = Capesnse_CMOD_PORT_PC_REG;
    newRegValue &= ~(Capesnse_CSD_CMOD_PC_MASK);
    newRegValue |= Capesnse_CMOD_PC_HIGH_Z_MODE;
    Capesnse_CMOD_PORT_PC_REG = newRegValue;
#endif /* (Capesnse_CMOD_PREGARGE_OPTION == Capesnse__CAPPRIOBUF) */

    /* Disconnect Vref Buffer from AMUX */
    newRegValue = Capesnse_CSD_CFG_REG;
    newRegValue &= ~(Capesnse_PRECHARGE_CONFIG_MASK);
    newRegValue |= Capesnse_CTANK_PRECHARGE_CONFIG;

    CyIntEnable(Capesnse_ISR_NUMBER);

    /* Enable Sensor */
    Capesnse_EnableScanSlot(sensor);

    interruptState = CyEnterCriticalSection();
    Capesnse_CSD_CFG_REG = newRegValue;

    /* `#START Capesnse_PreSettlingDelay_Debug` */

    /* `#END` */

#ifdef Capesnse_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK
    Capesnse_PreScan_Pre_SettlingDelay_Debug_Callback();
#endif /* Capesnse_PRE_SCAN_PRE_SETTLING_DELAY_DEBUG_CALLBACK */

    CyDelayCycles(Capesnse_GLITCH_ELIMINATION_CYCLES);

    /* `#START Capesnse_PreScan_Debug` */

    /* `#END` */

#ifdef Capesnse_PRE_SCAN_DEBUG_CALLBACK
    Capesnse_PreScan_Debug_Callback();
#endif /* Capesnse_PRE_SCAN_DEBUG_CALLBACK */

    Capesnse_CSD_CNT_REG = counterResolution;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Capesnse_PostScan
********************************************************************************
*
* Summary:
*  Stores the results of measurement in the Capesnse_sensorRaw[] array,
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
*  Capesnse_sensorRaw[] - used to store sensors raw data.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_PostScan(uint32 sensor)
{
    uint32 newRegValue;
#if ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
     (0 != Capesnse_IS_OVERSAMPLING_EN))
    uint32 oversamplingFactor;
    uint32 widget;
#endif /* ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
           (0 != Capesnse_IS_OVERSAMPLING_EN)) */

    /* `#START Capesnse_PostScan_Debug` */

    /* `#END` */

#ifdef Capesnse_POST_SCAN_DEBUG_CALLBACK
    Capesnse_PostScan_Debug_Callback();
#endif /* Capesnse_POST_SCAN_DEBUG_Callback */

    /* Read SlotResult from Raw Counter */
    Capesnse_sensorRaw[sensor]  = (uint16)Capesnse_CSD_CNT_REG;

#if ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
     (0 != Capesnse_IS_OVERSAMPLING_EN))
    widget = Capesnse_widgetNumber[sensor];
    if(Capesnse_widgetResolution[widget] < Capesnse_RESOLUTION_16_BITS)
    {
        oversamplingFactor = Capesnse_GetBitValue(Capesnse_scanSpeedTbl, sensor);
        Capesnse_sensorRaw[sensor] >>= oversamplingFactor;
    }
#endif /* ((Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) &&\
           (0 != Capesnse_IS_OVERSAMPLING_EN)) */

    /* Disable Sensor */
    Capesnse_DisableScanSlot(sensor);

    CyIntDisable(Capesnse_ISR_NUMBER);

#if(Capesnse_CMOD_PREGARGE_OPTION == Capesnse__CAPPRIOBUF)
    newRegValue = Capesnse_CMOD_CONNECTION_REG;
    newRegValue &= ~(Capesnse_CSD_CMOD_CONNECTION_MASK);
    newRegValue |= Capesnse_CSD_CMOD_TO_AMUXBUS_B;
    Capesnse_CMOD_CONNECTION_REG = newRegValue;

    newRegValue = Capesnse_CMOD_PORT_PC_REG;
    newRegValue &= ~(Capesnse_CSD_CMOD_PC_MASK);
    newRegValue |= Capesnse_CMOD_PC_STRONG_MODE;
    Capesnse_CMOD_PORT_PC_REG = newRegValue;

    newRegValue = Capesnse_CMOD_PORT_DR_REG;
    newRegValue &= ~(Capesnse_CMOD_DR_MASK);
    newRegValue |= Capesnse_CMOD_DR_CONFIG;
    Capesnse_CMOD_PORT_DR_REG = newRegValue;
#endif /* (Capesnse_CMOD_PREGARGE_OPTION == Capesnse__CAPPRIOBUF) */

    /* Connect Vref Buffer to AMUX bus  */
    newRegValue = Capesnse_CSD_CFG_REG;
    newRegValue &= ~(Capesnse_PRECHARGE_CONFIG_MASK);
    newRegValue |= Capesnse_CMOD_PRECHARGE_CONFIG;
    Capesnse_CSD_CFG_REG = newRegValue;

    /* Set Idac Value = 0 */
#if (Capesnse_IDAC_CNT == 1u)
    Capesnse_CSD_IDAC_REG &= ~(Capesnse_CSD_IDAC1_DATA_MASK);
#else
    Capesnse_CSD_IDAC_REG &= ~(Capesnse_CSD_IDAC1_DATA_MASK | Capesnse_CSD_IDAC2_DATA_MASK);
#endif /* (Capesnse_IDAC_CNT == 1u) */

    CyIntEnable(Capesnse_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: Capesnse_EnableShieldElectrode
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
*  Capesnse_prtCfgTbl[] - Contains pointers to the port config registers for each port.
*  Capesnse_prtSelTbl[] - Contains pointers to the HSIOM registers for each port.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_EnableShieldElectrode(uint32 pinNumber, uint32 portNumber)
{
    uint32 newRegValue;

    *Capesnse_prtCfgTbl[portNumber] &= ~(Capesnse_CSD_PIN_MODE_MASK << (pinNumber * Capesnse_PC_PIN_CFG_SIZE));
    newRegValue = *Capesnse_prtSelTbl[portNumber];
    newRegValue &= ~(Capesnse_CSD_HSIOM_MASK << (pinNumber * Capesnse_HSIOM_PIN_CFG_SIZE));
    newRegValue |= (Capesnse_CSD_SHIELD_PORT_MODE << (pinNumber * Capesnse_HSIOM_PIN_CFG_SIZE));
    *Capesnse_prtSelTbl[portNumber] = newRegValue;
}


/*******************************************************************************
* Function Name: Capesnse_SetShieldDelay
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
void Capesnse_SetShieldDelay(uint32 delay)
{
    uint32 newRegValue;

    delay &= 0x03u;

    newRegValue = Capesnse_CSD_CFG_REG;
    newRegValue &= (uint32)(~(uint32)Capesnse_SHIELD_DELAY);
    newRegValue |= (delay << Capesnse_CSD_CFG_SHIELD_DELAY_POS);
    Capesnse_CSD_CFG_REG = newRegValue;
}


/*******************************************************************************
* Function Name: Capesnse_ReadCurrentScanningSensor
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
*  Capesnse_sensorIndex - the sensor number is being scanned.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capesnse_ReadCurrentScanningSensor(void)
{
    return ((uint32)(( 0u != (Capesnse_csdStatusVar & Capesnse_SW_STS_BUSY)) ?
                     Capesnse_sensorIndex : Capesnse_NOT_SENSOR));
}


/*******************************************************************************
* Function Name: Capesnse_GetBitValue
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
uint32 Capesnse_GetBitValue(const uint8 table[], uint32 position)
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
* Function Name: Capesnse_SetBitValue
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
void Capesnse_SetBitValue(uint8 table[], uint32 position, uint32 value)
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
* Function Name: Capesnse_GetSenseClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the sense clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (Capesnse_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the sense clock divider of the sensor.
*
* Global Variables:
*  Capesnse_senseClkDividerVal[] - stores the sense clock divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capesnse_GetSenseClkDivider(uint32 sensor)
{
    #if(0u != Capesnse_MULTIPLE_FREQUENCY_SET)
        return((uint32)Capesnse_senseClkDividerVal[sensor]);
    #else
        return((uint32)Capesnse_senseClkDividerVal);
    #endif /* (0u != Capesnse_MULTIPLE_FREQUENCY_SET) */
}

#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capesnse_SetSenseClkDivider
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
    *  Capesnse_senseClkDividerVal[] - stores the sense clock divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capesnse_SetSenseClkDivider(uint32 sensor, uint32 senseClk)
    {
        #if(0u != Capesnse_MULTIPLE_FREQUENCY_SET)
            Capesnse_senseClkDividerVal[sensor] = (uint8)senseClk;
        #else
            Capesnse_senseClkDividerVal = (uint8)senseClk;
        #endif /* (0u != Capesnse_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */


/*******************************************************************************
* Function Name: Capesnse_GetModulatorClkDivider
********************************************************************************
*
* Summary:
*  This API returns a value of the modulator sample clock divider for the  sensor.
*
* Parameters:
*  sensor: sensor index. The value of index can be
*  from 0 to (Capesnse_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulator sample clock divider for the  sensor.
*
* Global Variables:
*  Capesnse_sampleClkDividerVal[] - stores the modulator sample divider values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capesnse_GetModulatorClkDivider(uint32 sensor)
{
    #if(0u != Capesnse_MULTIPLE_FREQUENCY_SET)
        return((uint32)Capesnse_sampleClkDividerVal[sensor]);
    #else
        return((uint32)Capesnse_sampleClkDividerVal);
    #endif /* (0u != Capesnse_MULTIPLE_FREQUENCY_SET) */
}

#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capesnse_SetModulatorClkDivider
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
    *  Capesnse_sampleClkDividerVal[] - stores the modulator sample divider values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capesnse_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk)
    {
        #if(0u != Capesnse_MULTIPLE_FREQUENCY_SET)
            Capesnse_sampleClkDividerVal[sensor] = (uint8)modulatorClk;
        #else
            Capesnse_sampleClkDividerVal = (uint8)modulatorClk;
        #endif /* (0u != Capesnse_MULTIPLE_FREQUENCY_SET) */
    }
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */

/*******************************************************************************
* Function Name: Capesnse_GetModulationIDAC
********************************************************************************
*
* Summary:
*  This API returns a value of the modulation IDAC for the  sensor.
*
* Parameters:
*  sensor: sensor index. The index value can be
*  from 0 to (Capesnse_TOTAL_SENSOR_COUNT-1).
*
* Return:
*  This API returns the modulation IDAC of the sensor.
*
* Global Variables:
*  Capesnse_modulationIDAC[] - stores modulation IDAC values.
*
* Side Effects:
*  None
*
*******************************************************************************/
uint32 Capesnse_GetModulationIDAC(uint32 sensor)
{
        return((uint32)Capesnse_modulationIDAC[sensor]);
}

#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capesnse_SetModulationIDAC
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
    *  Capesnse_modulationIDAC[] - array with modulation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capesnse_SetModulationIDAC(uint32 sensor, uint32 modIdacValue)
    {
        Capesnse_modulationIDAC[sensor] = (uint8)modIdacValue;
    }
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */


#if(Capesnse_IDAC_CNT > 1u)
    /*******************************************************************************
    * Function Name: Capesnse_GetCompensationIDAC
    ********************************************************************************
    *
    * Summary:
    *  This API returns a value of the compensation IDAC for the  sensor.
    *
    * Parameters:
    *  sensor: sensor index. The index value can be
    *  from 0 to (Capesnse_TOTAL_SENSOR_COUNT-1).
    *
    * Return:
    *  This API returns the compensation IDAC of the sensor.
    *
    * Global Variables:
    *  Capesnse_compensationIDAC[] - stores the compensation IDAC values.
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    uint32 Capesnse_GetCompensationIDAC(uint32 sensor)
    {
        return((uint32)Capesnse_compensationIDAC[sensor]);
    }
#endif /* (Capesnse_IDAC_CNT > 1u) */


#if((Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) && (Capesnse_IDAC_CNT > 1u))
    /*******************************************************************************
    * Function Name: Capesnse_SetCompensationIDAC
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
    *  Capesnse_compensationIDAC[] - an array with compensation IDAC values
    *
    * Side Effects:
    *  None
    *
    *******************************************************************************/
    void Capesnse_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue)
    {
        Capesnse_compensationIDAC[sensor] = (uint8)compIdacValue;
    }
#endif /* ((Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) && (Capesnse_IDAC_CNT > 1u)) */

/*******************************************************************************
* Function Name: Capesnse_GetIDACRange
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
uint32 Capesnse_GetIDACRange(void)
{
    return((0u != (Capesnse_CSD_IDAC_REG & Capesnse_CSD_IDAC1_RANGE_8X)) ? 1uL : 0uL);
}

#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    /*******************************************************************************
    * Function Name: Capesnse_SetIDACRange
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
    void Capesnse_SetIDACRange(uint32 iDacRange)
    {
        if(iDacRange != 0u)
        {
            /*  IDAC range = 8x (2.4uA/bit) */
            Capesnse_CSD_IDAC_REG |= (Capesnse_CSD_IDAC1_RANGE_8X |\
                                              Capesnse_CSD_IDAC2_RANGE_8X);
        }
        else
        {
            /*  IDAC range = 4x (1.2uA/bit) */
            /*  IDAC range = 8x (2.4uA/bit) */
            Capesnse_CSD_IDAC_REG &= ~(Capesnse_CSD_IDAC1_RANGE_8X |\
                                               Capesnse_CSD_IDAC2_RANGE_8X);
        }
    }
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */


#if((0u != Capesnse_AUTOCALIBRATION_ENABLE) || (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: Capesnse_GetSensorRaw
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
    uint16  Capesnse_GetSensorRaw(uint32 sensor)
    {
        uint32 curSample;
        uint32 avgVal = 0u;

        for(curSample = 0u; curSample < Capesnse_AVG_SAMPLES_NUM; curSample++)
        {

            Capesnse_ScanSensor((uint32)sensor);
            while(Capesnse_IsBusy() == 1u)
            {
                /* Wait while sensor is busy */
            }
            avgVal += Capesnse_ReadSensorRaw((uint32)sensor);
        }

        return((uint16)(avgVal / Capesnse_AVG_SAMPLES_NUM));
    }
#endif /* ((0u != Capesnse_AUTOCALIBRATION_ENABLE) && (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)) */


#if((0u != Capesnse_AUTOCALIBRATION_ENABLE) && (Capesnse_TUNING_METHOD != Capesnse__TUNING_AUTO))
    /*******************************************************************************
    * Function Name: Capesnse_CalibrateSensor
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
    void Capesnse_CalibrateSensor(uint32 sensor, uint32 rawLevel, uint8 idacLevelsTbl[])
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
            rawData = Capesnse_GetSensorRaw(sensor);

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
#endif /* ((0u != Capesnse_AUTOCALIBRATION_ENABLE) && (Capesnse_TUNING_METHOD != Capesnse__TUNING_AUTO)) */


/*******************************************************************************
* Function Name: Capesnse_SetUnscannedSensorState
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
*     Capesnse__GROUND 0
*     Capesnse__HIZ_ANALOG 1
*     Capesnse__SHIELD 2
*
* Return:
*  None.
*
* Global Variables:
*  Capesnse_unscannedSnsDriveMode[] - used to store the state for
*  un-scanned sensors.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_SetUnscannedSensorState(uint32 sensor, uint32 sensorState)
{
    #if(Capesnse_IS_COMPLEX_SCANSLOTS)
        uint8 snsType;
        uint8 curSensor;
        uint16 snsNumber;

        /* Read sensor type: single or complex */
        snsType = Capesnse_portTable[sensor];

        /* Check if sensor is complex */
        if(0u != (snsType & Capesnse_COMPLEX_SS_FLAG))
        {
            snsType &= (uint8)~Capesnse_COMPLEX_SS_FLAG;
            snsNumber = (uint16)Capesnse_maskTable[sensor];

            for (curSensor=0u; curSensor < snsNumber; curSensor++)
            {
                sensor = Capesnse_indexTable[snsType + curSensor];
                Capesnse_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
            }
        }
        else
        {
            Capesnse_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
        }
    #else
        Capesnse_unscannedSnsDriveMode[sensor] = (uint8)sensorState;
    #endif /* (Capesnse_IS_COMPLEX_SCANSLOTS) */
}

#if(0u != Capesnse_TOTAL_CENTROIDS_COUNT)
/*******************************************************************************
* Function Name: Capesnse_NormalizeWidgets
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
*  Capesnse_numberOfSensors[] - Number of sensors in the widget.
*  Capesnse_rawDataIndex[currentWidget] - Contains the  1st sensor
*  position in the widget.
*
* Side Effects:
*  None
*
*******************************************************************************/
void Capesnse_NormalizeWidgets(uint32 widgetsNum, uint8 dataPtr[])
{
    uint32 currentWidget;
    uint32 currentSensor;
    uint32 lastSensor;
    uint32 sensorsPerWidget;
    uint32 maxPerWidget;

    for(currentWidget = 0u; currentWidget < widgetsNum; currentWidget++)
    {
        maxPerWidget = 0u;

        sensorsPerWidget = Capesnse_numberOfSensors[currentWidget];
        currentSensor = Capesnse_rawDataIndex[currentWidget];
        lastSensor = currentSensor + sensorsPerWidget;

        while(currentSensor < lastSensor)
        {
            if(maxPerWidget < dataPtr[currentSensor])
            {
                maxPerWidget = dataPtr[currentSensor];
            }
            currentSensor++;
        }

        currentSensor = Capesnse_rawDataIndex[currentWidget];

        while(currentSensor < lastSensor)
        {
            dataPtr[currentSensor] = (uint8)maxPerWidget;
            currentSensor++;
        }
    }
}
#endif /* (0u != Capesnse_TOTAL_CENTROIDS_COUNT) */

/* [] END OF FILE */
