/*******************************************************************************
* File Name: Capesnse.h
* Version 2.60
*
* Description:
*  This file provides constants and parameter values for the CapSense CSD
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

#if !defined(CY_CAPSENSE_CSD_Capesnse_H)
#define CY_CAPSENSE_CSD_Capesnse_H

#include "cytypes.h"
#include "cyfitter.h"
#include "core_cm0_psoc4.h"
#include "CyLib.h"

#include "Capesnse_IP.h"
#include "Capesnse_SenseClk.h"
#include "Capesnse_SampleClk.h"

/* Constants set by Customizer */
#define Capesnse_TOTAL_SENSOR_COUNT            (5u)
#define Capesnse_TOTAL_SCANSLOT_COUNT          (5u)
#define Capesnse_INDEX_TABLE_SIZE              (0u)

/* Define Sensors */
#define Capesnse_SENSOR_LINEARSLIDER0_E0__LS    (0u)
#define Capesnse_SENSOR_LINEARSLIDER0_E1__LS    (1u)
#define Capesnse_SENSOR_LINEARSLIDER0_E2__LS    (2u)
#define Capesnse_SENSOR_LINEARSLIDER0_E3__LS    (3u)
#define Capesnse_SENSOR_LINEARSLIDER0_E4__LS    (4u)

#define Capesnse_TOTAL_SENSOR_MASK (((Capesnse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)

/* Interrupt handler */
CY_ISR_PROTO(Capesnse_ISR);

/* ISR Number and Priority to work with CyLib functions */
#define Capesnse_ISR_NUMBER        (Capesnse_ISR__INTC_NUMBER)
#define Capesnse_ISR_PRIORITY      (Capesnse_ISR__INTC_PRIOR_NUM)

/***************************************
*   Condition compilation parameters
***************************************/

#define Capesnse_CONNECT_INACTIVE_SNS       (0u)
#define Capesnse_IS_COMPLEX_SCANSLOTS       (0u)
#define Capesnse_COMPLEX_SCANSLOTS_NUM      (0u)
#define Capesnse_DEDICATED_SENSORS_NUM      (0u)
#define Capesnse_PORT_PIN_CONFIG_TBL_ZISE   (5u)

#define Capesnse_IDAC_CNT                   (2u)
#define Capesnse_IDAC1_POLARITY             (0u)
#define Capesnse_IDAC1_RANGE_VALUE          (0u)
#define Capesnse_IDAC2_RANGE_VALUE          (0u)

#define Capesnse_MULTIPLE_FREQUENCY_SET     (1u)
#define Capesnse_FILTER_ENABLE              (0u)
#define Capesnse_PRS_OPTIONS                (0u)

#define Capesnse_WATER_PROOF                (0u)

#define Capesnse_TUNING_METHOD              (2u)
#define Capesnse_TUNER_API_GENERATE         (0u)

#define Capesnse_CSHL_API_GENERATE          (1u)

#define Capesnse_CMOD_PREGARGE_OPTION       (1u)
#define Capesnse_CSH_TANK_PREGARGE_OPTION   (1u)
#define Capesnse_IS_SHIELD_ENABLE           (0u)
#define Capesnse_CSH_TANK_ENABLE            (0u)

#define Capesnse_SHIELD_DELAY               (0u)
#define Capesnse_AUTOCALIBRATION_ENABLE     (0u)

#define Capesnse_IS_OVERSAMPLING_EN         (1u)

#define Capesnse_CSD_4B_PWM_MODE            (0u)
#define Capesnse_CSD_4B_PWM_COUNT           (0u)

/**************************************
*    Enumerated Types and Parameters
**************************************/

/* Current source definitions */
#define Capesnse__IDAC_SOURCE 0
#define Capesnse__IDAC_SINK 1

#define Capesnse__IDAC_4X 0
#define Capesnse__IDAC_8X 1

#define Capesnse__PRS_NONE 0
#define Capesnse__PRS_8BITS 1
#define Capesnse__PRS_12BITS 2
#define Capesnse__PRS_AUTO 3

/* Connection of inactive sensors definitions */
#define Capesnse__GROUND 0
#define Capesnse__HIZ_ANALOG 1
#define Capesnse__SHIELD 2

/* Precharge options definitions */
#define Capesnse__CAPPRIOBUF 0
#define Capesnse__CAPPRVREF 1

/* Method of tuning */
#define Capesnse__TUNING_NONE 0
#define Capesnse__TUNING_MANUAL 1
#define Capesnse__TUNING_AUTO 2

/* Dead band PWM modulator options */
#define Capesnse__PWM_OFF 0
#define Capesnse__PWM_HIGH 2
#define Capesnse__PWM_LOW 3


#define Capesnse_DELAY_EXTRACYCLES_NUM          (9u)
#define Capesnse_GLITCH_ELIMINATION_TIMEOUT     (0u)
#define Capesnse_GLITCH_ELIMINATION_CYCLES_CALC (Capesnse_GLITCH_ELIMINATION_TIMEOUT * CYDEV_BCLK__SYSCLK__MHZ)

#if(Capesnse_GLITCH_ELIMINATION_CYCLES_CALC >= Capesnse_DELAY_EXTRACYCLES_NUM)
    #define Capesnse_GLITCH_ELIMINATION_CYCLES (Capesnse_GLITCH_ELIMINATION_CYCLES_CALC -\
                                                        Capesnse_DELAY_EXTRACYCLES_NUM)
#else
    #define Capesnse_GLITCH_ELIMINATION_CYCLES (Capesnse_GLITCH_ELIMINATION_CYCLES_CALC)
#endif /* (Capesnse_GLITCH_ELIMINATION_CYCLES_CALC >= Capesnse_DELAY_EXTRACYCLES_NUM) */

#define Capesnse_INITIAL_CLK_DIVIDER            (CYDEV_BCLK__HFCLK__KHZ / CYDEV_BCLK__SYSCLK__KHZ)


/* Structure to save registers before going to sleep */
typedef struct
{
    uint8 enableState;
} Capesnse_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/
void Capesnse_Init(void);
void Capesnse_CsdHwBlockInit(void);
void Capesnse_Enable(void);
void Capesnse_Start(void);
void Capesnse_Stop(void);
void Capesnse_SaveConfig(void);
void Capesnse_Sleep(void);
void Capesnse_RestoreConfig(void);
void Capesnse_Wakeup(void);
uint32 Capesnse_IsBusy(void);
void Capesnse_ScanSensor(uint32 sensor);
void Capesnse_ScanWidget(uint32 widget);
void Capesnse_ScanEnabledWidgets(void);
uint16 Capesnse_ReadSensorRaw(uint32 sensor);
void Capesnse_WriteSensorRaw(uint32 sensor, uint16 value);
void Capesnse_ClearSensors(void);
void Capesnse_SetShieldDelay(uint32 delay);
uint32 Capesnse_ReadCurrentScanningSensor(void);

uint32 Capesnse_GetScanResolution(uint32 widget);
uint32 Capesnse_GetSenseClkDivider(uint32 sensor);
uint32 Capesnse_GetModulatorClkDivider(uint32 sensor);
uint32 Capesnse_GetModulationIDAC(uint32 sensor);
uint32 Capesnse_GetCompensationIDAC(uint32 sensor);
uint32 Capesnse_GetIDACRange(void);

#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    void Capesnse_SetScanResolution(uint32 widget, uint32 resolution);
    void Capesnse_SetSenseClkDivider(uint32 sensor, uint32 senseClk);
    void Capesnse_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk);
    void Capesnse_SetModulationIDAC(uint32 sensor, uint32 modIdacValue);
    void Capesnse_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue);
    void Capesnse_SetIDACRange(uint32 iDacRange);
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) */


void Capesnse_SetDriveModeAllPins(uint32 driveMode);
void Capesnse_RestoreDriveModeAllPins(void);
void Capesnse_SetUnscannedSensorState(uint32 sensor, uint32 sensorState);
void Capesnse_NormalizeWidgets(uint32 widgetsNum, uint8 dataPtr[]);

/***************************************
*           API Constants
***************************************/

/* PWM Resolution */
#define Capesnse_CNT_MSB_RESOLUTION_8_BITS  (0x00u)
#define Capesnse_CNT_MSB_RESOLUTION_9_BITS  (0x01u)
#define Capesnse_CNT_MSB_RESOLUTION_10_BITS (0x03u)
#define Capesnse_CNT_MSB_RESOLUTION_11_BITS (0x07u)
#define Capesnse_CNT_MSB_RESOLUTION_12_BITS (0x0Fu)
#define Capesnse_CNT_MSB_RESOLUTION_13_BITS (0x1Fu)
#define Capesnse_CNT_MSB_RESOLUTION_14_BITS (0x3Fu)
#define Capesnse_CNT_MSB_RESOLUTION_15_BITS (0x7Fu)
#define Capesnse_CNT_MSB_RESOLUTION_16_BITS (0xFFu)

#define Capesnse_ONE_CYCLE                  (0x00010000Lu)

#define Capesnse_WINDOW_APPEND              (0xFFFF0000Lu)
#define Capesnse_RESOLUTION_6_BITS          (0x003F0000Lu)
#define Capesnse_RESOLUTION_7_BITS          (0x007F0000Lu)
#define Capesnse_RESOLUTION_8_BITS          (0x00FF0000Lu)
#define Capesnse_RESOLUTION_9_BITS          (0x01FF0000Lu)
#define Capesnse_RESOLUTION_10_BITS         (0x03FF0000Lu)
#define Capesnse_RESOLUTION_11_BITS         (0x07FF0000Lu)
#define Capesnse_RESOLUTION_12_BITS         (0x0FFF0000Lu)
#define Capesnse_RESOLUTION_13_BITS         (0x1FFF0000Lu)
#define Capesnse_RESOLUTION_14_BITS         (0x3FFF0000Lu)
#define Capesnse_RESOLUTION_15_BITS         (0x7FFF0000Lu)
#define Capesnse_RESOLUTION_16_BITS         (0xFFFF0000Lu)

#define Capesnse_RESOLUTION_OFFSET          (16u)
#define Capesnse_MSB_RESOLUTION_OFFSET      (22u)
#define Capesnse_RESOLUTIONS_TBL_SIZE       (1u)

/* Software Status Register Bit Masks */
#define Capesnse_SW_STS_BUSY                (0x01u)
/* Software Control Register Bit Masks */
#define Capesnse_SW_CTRL_SINGLE_SCAN        (0x80u)

/* Software Control Register Bit Masks for scanning one widget */
#define Capesnse_SW_CTRL_WIDGET_SCAN        (0x40u)

/* Flag for complex scan slot */
#define Capesnse_COMPLEX_SS_FLAG            (0x80u)

/* Flag for scanning is complete */
#define Capesnse_NOT_SENSOR                 (0xFFFFFFFFLu)

/* Number of bits for each pin in PC register. */
#define Capesnse_PC_PIN_CFG_SIZE            (0x03u)

/* Number of bits for each pin in HSIOM register. */
#define Capesnse_HSIOM_PIN_CFG_SIZE         (0x04u)

#define Capesnse_CFG_REG_TBL_SIZE           (7u)

/***************************************
*             Registers
***************************************/

#define Capesnse_CSD_ID_REG             (*(reg32 *)  Capesnse_CSD_FFB__CSD_ID)
#define Capesnse_CSD_ID_PTR             ( (reg32 *)  Capesnse_CSD_FFB__CSD_ID)

#define Capesnse_CSD_CFG_REG            (*(reg32 *)  Capesnse_CSD_FFB__CSD_CONFIG)
#define Capesnse_CSD_CFG_PTR            ( (reg32 *)  Capesnse_CSD_FFB__CSD_CONFIG)

#define Capesnse_CSD_IDAC_REG           (*(reg32 *)  Capesnse_IDAC1_cy_psoc4_idac__CSD_IDAC)
#define Capesnse_CSD_IDAC_PTR           ( (reg32 *)  Capesnse_IDAC1_cy_psoc4_idac__CSD_IDAC)

#define Capesnse_CSD_CNT_REG            (*(reg32 *) Capesnse_CSD_FFB__CSD_COUNTER)
#define Capesnse_CSD_CNT_PTR            ( (reg32 *) Capesnse_CSD_FFB__CSD_COUNTER)

#define Capesnse_CSD_STAT_REG           (*(reg32 *) Capesnse_CSD_FFB__CSD_STATUS)
#define Capesnse_CSD_STAT_PTR           ( (reg32 *) Capesnse_CSD_FFB__CSD_STATUS)

#define Capesnse_CSD_INTR_REG           (*(reg32 *) Capesnse_CSD_FFB__CSD_INTR)
#define Capesnse_CSD_INTR_PTR           ( (reg32 *) Capesnse_CSD_FFB__CSD_INTR)

#define Capesnse_CSD_INTR_SET_REG       (*(reg32 *) Capesnse_CSD_FFB__CSD_INTR_SET)
#define Capesnse_CSD_INTR_SET_PTR       ( (reg32 *) Capesnse_CSD_FFB__CSD_INTR_SET)

#define Capesnse_CSD_4B_PWM_REG         (*(reg32 *) Capesnse_CSD_FFB__CSD_PWM)
#define Capesnse_CSD_4B_PWM_PTR         ( (reg32 *) Capesnse_CSD_FFB__CSD_PWM)

#define Capesnse_CSD_TRIM1_REG          (*(reg32 *) Capesnse_IDAC1_cy_psoc4_idac__CSD_TRIM1)
#define Capesnse_CSD_TRIM1_PTR          ( (reg32 *) Capesnse_IDAC1_cy_psoc4_idac__CSD_TRIM1)

#define Capesnse_CSD_TRIM2_REG          (*(reg32 *) Capesnse_IDAC1_cy_psoc4_idac__CSD_TRIM2)
#define Capesnse_CSD_TRIM2_PTR          ( (reg32 *) Capesnse_IDAC1_cy_psoc4_idac__CSD_TRIM2)

#if (2u == Capesnse_M0S8CSD_BLOCK_CNT)
    #if (Capesnse_CSD_FFB__CSD_NUMBER != 1u)
        #define Capesnse_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
        #define Capesnse_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

        #define Capesnse_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
        #define Capesnse_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
    #else
        #define Capesnse_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)
        #define Capesnse_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)

        #define Capesnse_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
        #define Capesnse_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
    #endif /* (Capesnse_CSD_FFB__CSD_NUMBER != 1u) */
#else
    #define Capesnse_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
    #define Capesnse_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

    #define Capesnse_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
    #define Capesnse_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
#endif /* (2u == Capesnse_M0S8CSD_BLOCK_CNT) */

/* Port function select */
#define Capesnse_PORT_SEL0_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL0 )
#define Capesnse_PORT_SEL0_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL0 )

#define Capesnse_PORT_SEL1_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL1 )
#define Capesnse_PORT_SEL1_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL1 )

#define Capesnse_PORT_SEL2_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL2 )
#define Capesnse_PORT_SEL2_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL2 )

#define Capesnse_PORT_SEL3_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL3 )
#define Capesnse_PORT_SEL3_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL3 )

#define Capesnse_PORT_SEL4_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL4 )
#define Capesnse_PORT_SEL4_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL4 )

#define Capesnse_PORT_SEL5_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL5 )
#define Capesnse_PORT_SEL5_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL5 )

#define Capesnse_PORT_SEL6_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL6 )
#define Capesnse_PORT_SEL6_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL6 )


#define Capesnse_PORT0_PC_REG          (*(reg32 *) CYREG_GPIO_PRT0_PC )
#define Capesnse_PORT0_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT0_PC )

#define Capesnse_PORT1_PC_REG          (*(reg32 *) CYREG_GPIO_PRT1_PC )
#define Capesnse_PORT1_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT1_PC )

#define Capesnse_PORT2_PC_REG          (*(reg32 *) CYREG_GPIO_PRT2_PC )
#define Capesnse_PORT2_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT2_PC )

#define Capesnse_PORT3_PC_REG          (*(reg32 *) CYREG_GPIO_PRT3_PC )
#define Capesnse_PORT3_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT3_PC )

#define Capesnse_PORT4_PC_REG          (*(reg32 *) CYREG_GPIO_PRT4_PC )
#define Capesnse_PORT4_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT4_PC )

#define Capesnse_PORT5_PC_REG          (*(reg32 *) CYREG_GPIO_PRT5_PC )
#define Capesnse_PORT5_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT5_PC )

#define Capesnse_PORT6_PC_REG          (*(reg32 *) CYREG_GPIO_PRT6_PC )
#define Capesnse_PORT6_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT6_PC )


#define Capesnse_PORT0_DR_REG          (*(reg32 *) CYREG_GPIO_PRT0_DR )
#define Capesnse_PORT0_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT0_DR )

#define Capesnse_PORT1_DR_REG          (*(reg32 *) CYREG_GPIO_PRT1_DR )
#define Capesnse_PORT1_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT1_DR )

#define Capesnse_PORT2_DR_REG          (*(reg32 *) CYREG_GPIO_PRT2_DR )
#define Capesnse_PORT2_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT2_DR )

#define Capesnse_PORT3_DR_REG          (*(reg32 *) CYREG_GPIO_PRT3_DR )
#define Capesnse_PORT3_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT3_DR )

#define Capesnse_PORT4_DR_REG          (*(reg32 *) CYREG_GPIO_PRT4_DR )
#define Capesnse_PORT4_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT4_DR )

#define Capesnse_PORT5_DR_REG          (*(reg32 *) CYREG_GPIO_PRT5_DR )
#define Capesnse_PORT5_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT5_DR )

#define Capesnse_PORT6_DR_REG          (*(reg32 *) CYREG_GPIO_PRT6_DR )
#define Capesnse_PORT6_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT6_DR )



#define Capesnse_CMOD_CONNECTION_REG        (*(reg32 *) Capesnse_Cmod__0__HSIOM)
#define Capesnse_CMOD_PORT_PC_REG           (*(reg32 *) Capesnse_Cmod__0__PC)
#define Capesnse_CMOD_PORT_DR_REG           (*(reg32 *) Capesnse_Cmod__0__DR)

#define Capesnse_CTANK_CONNECTION_REG       (*(reg32 *) Capesnse_Csh_tank__0__HSIOM)
#define Capesnse_CTANK_PORT_PC_REG          (*(reg32 *) Capesnse_Csh_tank__0__PC)
#define Capesnse_CTANK_PORT_DR_REG          (*(reg32 *) Capesnse_Csh_tank__0__DR)

/***************************************
*       Register Constants
***************************************/

/* Port configuration defines */
#define Capesnse_CSD_HSIOM_MASK                 (0x0000000FLu)

#define Capesnse_CSD_SENSE_PORT_MODE            (0x00000004Lu)
#define Capesnse_CSD_SHIELD_PORT_MODE           (0x00000005Lu)

#define Capesnse_AMUXA_CONNECTION_MODE          (0x00000006Lu)
#define Capesnse_AMUXB_CONNECTION_MODE          (0x00000007Lu)

#define Capesnse_CSD_PIN_MODE_MASK              (0x00000007Lu)
#define Capesnse_CSD_PIN_DRIVE_MASK             (0x00000001Lu)

#define Capesnse_CSD_PIN_DM_STRONG              (0x00000006Lu)
#define Capesnse_CSD_PIN_DM_HIGH_Z              (0x00000000Lu)

#define Capesnse_CSD_CMOD_CONNECTION_MASK   (uint32)(Capesnse_Cmod__0__HSIOM_MASK)
#define Capesnse_CSD_CMOD_TO_AMUXBUS_A      (uint32)(Capesnse_AMUXA_CONNECTION_MODE << Capesnse_Cmod__0__HSIOM_SHIFT)
#define Capesnse_CSD_CMOD_TO_AMUXBUS_B      (uint32)(Capesnse_AMUXB_CONNECTION_MODE << Capesnse_Cmod__0__HSIOM_SHIFT)

#if(0u != Capesnse_CSH_TANK_ENABLE)
    #define Capesnse_CSD_CTANK_CONNECTION_MASK  (uint32)(Capesnse_Csh_tank__0__HSIOM_MASK)
    #define Capesnse_CSD_CTANK_TO_AMUXBUS_A     (uint32)(Capesnse_AMUXA_CONNECTION_MODE << Capesnse_Csh_tank__0__HSIOM_SHIFT)
    #define Capesnse_CSD_CTANK_TO_AMUXBUS_B     (uint32)(Capesnse_AMUXB_CONNECTION_MODE << Capesnse_Csh_tank__0__HSIOM_SHIFT)
#endif /* (0u != Capesnse_CSH_TANK_ENABLE) */

#define  Capesnse_CSD_CMOD_PC_MASK          (uint32)(Capesnse_CSD_PIN_MODE_MASK <<\
                                                            ((uint32)Capesnse_Cmod__0__SHIFT * Capesnse_PC_PIN_CFG_SIZE))
#define  Capesnse_CMOD_PC_STRONG_MODE       (uint32)(Capesnse_CSD_PIN_DM_STRONG <<\
                                                            ((uint32)Capesnse_Cmod__0__SHIFT * Capesnse_PC_PIN_CFG_SIZE))
#define  Capesnse_CMOD_PC_HIGH_Z_MODE       (uint32)(Capesnse_CSD_PIN_DM_HIGH_Z <<\
                                                            ((uint32)Capesnse_Cmod__0__SHIFT * Capesnse_PC_PIN_CFG_SIZE))

#if(0u != Capesnse_CSH_TANK_ENABLE)
    #define  Capesnse_CSD_CTANK_PC_MASK         (uint32)(Capesnse_CSD_PIN_MODE_MASK <<\
                                                                ((uint32)Capesnse_Csh_tank__0__SHIFT * Capesnse_PC_PIN_CFG_SIZE))
    #define  Capesnse_CTANK_PC_STRONG_MODE      (uint32)(Capesnse_CSD_PIN_DM_STRONG <<\
                                                                ((uint32)Capesnse_Csh_tank__0__SHIFT * Capesnse_PC_PIN_CFG_SIZE))
    #define  Capesnse_CTANK_PC_HIGH_Z_MODE      (uint32)(Capesnse_CSD_PIN_DM_HIGH_Z <<\
                                                                ((uint32)Capesnse_Csh_tank__0__SHIFT * Capesnse_PC_PIN_CFG_SIZE))
#endif /* (0u != Capesnse_CSH_TANK_ENABLE) */

#define  Capesnse_CMOD_DR_VSSIO             (uint32)(Capesnse_Cmod__0__MASK)
#define  Capesnse_CMOD_DR_VDDIO             (0x00000000Lu)
#define  Capesnse_CMOD_DR_MASK              (uint32)(Capesnse_Cmod__0__MASK)

#if(0u != Capesnse_CSH_TANK_ENABLE)
    #define  Capesnse_CTANK_DR_VSSIO            (uint32)(Capesnse_Csh_tank__0__MASK)
    #define  Capesnse_CTANK_DR_VDDIO            (0x00000000Lu)
    #define  Capesnse_CTANK_DR_MASK             (uint32)(Capesnse_Csh_tank__0__MASK)
#endif /* (0u != Capesnse_CSH_TANK_ENABLE) */

#define  Capesnse_CMOD_PIN_NUMBER               (Capesnse_Cmod__0__SHIFT)
#define  Capesnse_CMOD_PRT_NUMBER               (Capesnse_Cmod__0__PORT)

#if(0u != Capesnse_CSH_TANK_ENABLE)
    #define  Capesnse_CTANK_PIN_NUMBER          (Capesnse_Csh_tank__0__SHIFT)
    #define  Capesnse_CTANK_PRT_NUMBER          (Capesnse_Csh_tank__0__PORT)
#endif /* (0u != Capesnse_CSH_TANK_ENABLE) */

/* ID register fields position */
#define Capesnse_CSD_ID_ID__POS                 (0x00u)
#define Capesnse_CSD_ID_REV__POS                (0x10u)

#define Capesnse_CSD_ID_ID                      (0xFFLu << Capesnse_CSD_ID_ID__POS)
#define Capesnse_CSD_ID_REV                     (0xFFLu << Capesnse_CSD_ID_REV__POS)

/* CSD_CONFIG register fields position */
#define Capesnse_CSD_CFG_DSI_SAMPLE_EN_POS      (0x00u)
#define Capesnse_CSD_CFG_SAMPLE_SYNC_POS        (0x01u)
#define Capesnse_CSD_CFG_FILTER_EN_POS          (0x03u)
#define Capesnse_CSD_CFG_PRS_CLEAR_POS          (0x05u)
#define Capesnse_CSD_CFG_PRS_SELECT_POS         (0x06u)
#define Capesnse_CSD_CFG_PRS_12_8_POS           (0x07u)
#define Capesnse_CSD_CFG_DSI_SENSE_EN_POS       (0x08u)
#define Capesnse_CSD_CFG_SHIELD_DELAY_POS       (0x09u)
#define Capesnse_CSD_CFG_SENSE_COMP_BW_POS      (0x0Bu)
#define Capesnse_CSD_CFG_SENSE_EN_POS           (0x0Cu)
#define Capesnse_CSD_CFG_REFBUF_EN_POS          (0x0Du)
#define Capesnse_CSD_CFG_CHARGE_IO_POS          (0x0Eu)
#define Capesnse_CSD_CFG_COMP_PIN_POS           (0x0Fu)
#define Capesnse_CSD_CFG_POLARITY_POS           (0x10u)
#define Capesnse_CSD_CFG_MUTUAL_CAP_POS         (0x12u)
#define Capesnse_CSD_CFG_SENSE_COMP_EN_POS      (0x13u)
#define Capesnse_CSD_CFG_REFBUF_OUTSEL_POS      (0x15u)
#define Capesnse_CSD_CFG_SENSE_INSEL_POS        (0x16u)
#define Capesnse_CSD_REFBUF_DRV_POS             (0x17u)
#define Capesnse_CSD_CFG_DDFTSEL_POS            (0x1Au)
#define Capesnse_CSD_CFG_ADFTEN_POS             (0x1Du)
#define Capesnse_CSD_CFG_DDFTCOMP_POS           (0x1Eu)
#define Capesnse_CSD_CFG_ENABLE_POS             (0x1Fu)

/* CSD_STATUS register fields position */
#define Capesnse_CSD_STAT_CSD_CHARGE_POS        (0x00u)
#define Capesnse_CSD_STAT_CSD_SENSE_POS         (0x01u)
#define Capesnse_CSD_STAT_COMP_OUT_POS          (0x02u)
#define Capesnse_CSD_STAT_SAMPLE_POS            (0x03u)

/* CSD_IDAC control register fields position */
#define Capesnse_CSD_IDAC1_MODE_MASK_POS        (0x08u)
#define Capesnse_CSD_IDAC1_MODE_FIXED_POS       (0x08u)
#define Capesnse_CSD_IDAC1_MODE_VARIABLE_POS    (0x08u)
#define Capesnse_CSD_IDAC1_MODE_DSI_POS         (0x08u)
#define Capesnse_CSD_IDAC1_RANGE_8X_POS         (0x0Au)
#define Capesnse_CSD_IDAC1_DATA_MASK_POS        (0x00u)

#define Capesnse_CSD_IDAC2_MODE_MASK_POS        (0x18u)
#define Capesnse_CSD_IDAC2_MODE_FIXED_POS       (0x18u)
#define Capesnse_CSD_IDAC2_MODE_VARIABLE_POS    (0x18u)
#define Capesnse_CSD_IDAC2_MODE_DSI_POS         (0x18u)
#define Capesnse_CSD_IDAC2_RANGE_8X_POS         (0x1Au)
#define Capesnse_CSD_IDAC2_DATA_MASK_POS        (0x10u)

/* CSD_COUNTER register fields position */
#define Capesnse_CSD_CNT_COUNTER_POS            (0x00u)
#define Capesnse_CSD_CNT_PERIOD_POS             (0x10u)

/* CSD_CONFIRG register definitions */
#define Capesnse_CSD_CFG_DSI_SAMPLE_EN          (0x01Lu << Capesnse_CSD_CFG_DSI_SAMPLE_EN_POS)
#define Capesnse_CSD_CFG_SAMPLE_SYNC            (0x01Lu << Capesnse_CSD_CFG_SAMPLE_SYNC_POS)
#define Capesnse_CSD_CFG_FILTER_EN              (0x01Lu << Capesnse_CSD_CFG_FILTER_EN_POS)
#define Capesnse_CSD_CFG_PRS_CLEAR              (0x01Lu << Capesnse_CSD_CFG_PRS_CLEAR_POS)
#define Capesnse_CSD_CFG_PRS_SELECT             (0x01Lu << Capesnse_CSD_CFG_PRS_SELECT_POS)
#define Capesnse_CSD_CFG_PRS_12_8               (0x01Lu << Capesnse_CSD_CFG_PRS_12_8_POS)
#define Capesnse_CSD_CFG_DSI_SENSE_EN           (0x01Lu << Capesnse_CSD_CFG_DSI_SENSE_EN_POS)
#define Capesnse_CSD_CFG_SHIELD_DELAY_MASK      (0x03Lu << Capesnse_CSD_CFG_SHIELD_DELAY_POS)
#define Capesnse_CSD_CFG_SENSE_COMP_BW          (0x01Lu << Capesnse_CSD_CFG_SENSE_COMP_BW_POS)
#define Capesnse_CSD_CFG_SENSE_EN               (0x01Lu << Capesnse_CSD_CFG_SENSE_EN_POS)
#define Capesnse_CSD_CFG_REFBUF_EN              (0x01Lu << Capesnse_CSD_CFG_REFBUF_EN_POS)
#define Capesnse_CSD_CFG_CHARGE_IO              (0x01Lu << Capesnse_CSD_CFG_CHARGE_IO_POS)
#define Capesnse_CSD_CFG_COMP_PIN_CH2           (0x01Lu << Capesnse_CSD_CFG_COMP_PIN_POS)
#define Capesnse_CSD_CFG_POLARITY_VDDIO         (0x01Lu << Capesnse_CSD_CFG_POLARITY_POS)
#define Capesnse_CSD_CFG_MUTUAL_CAP             (0x01Lu << Capesnse_CSD_CFG_MUTUAL_CAP_POS)
#define Capesnse_CSD_CFG_SENSE_COMP_EN          (0x01Lu << Capesnse_CSD_CFG_SENSE_COMP_EN_POS)
#define Capesnse_CSD_CFG_REFBUF_OUTSEL          (0x01Lu << Capesnse_CSD_CFG_REFBUF_OUTSEL_POS)
#define Capesnse_CSD_CFG_SENSE_INSEL            (0x01Lu << Capesnse_CSD_CFG_SENSE_INSEL_POS)
#define Capesnse_CSD_REFBUF_DRV_MASK            (0x03Lu << Capesnse_CSD_REFBUF_DRV_POS)
#define Capesnse_CSD_REFBUF_DRV_LOW             (0x01Lu << Capesnse_CSD_REFBUF_DRV_POS)
#define Capesnse_CSD_REFBUF_DRV_MEDIUM          (0x02Lu << Capesnse_CSD_REFBUF_DRV_POS)
#define Capesnse_CSD_REFBUF_DRV_HIGH            (0x03Lu << Capesnse_CSD_REFBUF_DRV_POS)
#define Capesnse_CSD_CFG_DDFTSEL_MASK           (0x07Lu << Capesnse_CSD_CFG_DDFTSEL_POS)
#define Capesnse_CSD_CFG_ADFTEN                 (0x01Lu << Capesnse_CSD_CFG_ADFTEN_POS)
#define Capesnse_CSD_CFG_DDFTCOMP_MASK          (0x03Lu << Capesnse_CSD_CFG_DDFTCOMP_POS)
#define Capesnse_CSD_CFG_DDFTCOMP_REFCOMP       (0x01Lu << Capesnse_CSD_CFG_DDFTCOMP_POS)
#define Capesnse_CSD_CFG_DDFTCOMP_SENSECOMP     (0x02Lu << Capesnse_CSD_CFG_DDFTCOMP_POS)
#define Capesnse_CSD_CFG_ENABLE                 (0x01Lu << Capesnse_CSD_CFG_ENABLE_POS)

/* CSD_STATUS register definitions */
#define Capesnse_CSD_STAT_CSD_CHARGE            (0x01Lu << Capesnse_CSD_STAT_CSD_CHARGE_POS)
#define Capesnse_CSD_STAT_CSD_SENSE             (0x01Lu << Capesnse_CSD_STAT_CSD_SENSE_POS)
#define Capesnse_CSD_STAT_COMP_OUT              (0x01Lu << Capesnse_CSD_STAT_COMP_OUT_POS)
#define Capesnse_CSD_STAT_SAMPLE                (0x01Lu << Capesnse_CSD_STAT_SAMPLE_POS)

/* CSD_IDAC control register definitions */
#define Capesnse_CSD_IDAC1_MODE_MASK            (0x03Lu << Capesnse_CSD_IDAC1_MODE_MASK_POS)
#define Capesnse_CSD_IDAC1_MODE_FIXED           (0x01Lu << Capesnse_CSD_IDAC1_MODE_FIXED_POS)
#define Capesnse_CSD_IDAC1_MODE_VARIABLE        (0x02Lu << Capesnse_CSD_IDAC1_MODE_VARIABLE_POS)
#define Capesnse_CSD_IDAC1_MODE_DSI             (0x03Lu << Capesnse_CSD_IDAC1_MODE_DSI_POS)
#define Capesnse_CSD_IDAC1_RANGE_8X             (0x01Lu << Capesnse_CSD_IDAC1_RANGE_8X_POS)
#define Capesnse_CSD_IDAC1_DATA_MASK            (0xFFLu << Capesnse_CSD_IDAC1_DATA_MASK_POS)

#define Capesnse_CSD_IDAC2_MODE_MASK            (0x03Lu << Capesnse_CSD_IDAC2_MODE_MASK_POS)
#define Capesnse_CSD_IDAC2_MODE_FIXED           (0x01Lu << Capesnse_CSD_IDAC2_MODE_FIXED_POS)
#define Capesnse_CSD_IDAC2_MODE_VARIABLE        (0x02Lu << Capesnse_CSD_IDAC2_MODE_VARIABLE_POS)
#define Capesnse_CSD_IDAC2_MODE_DSI             (0x03Lu << Capesnse_CSD_IDAC2_MODE_DSI_POS)
#define Capesnse_CSD_IDAC2_RANGE_8X             (0x01Lu << Capesnse_CSD_IDAC2_RANGE_8X_POS)
#define Capesnse_CSD_IDAC2_DATA_MASK            (0x7FLu << Capesnse_CSD_IDAC2_DATA_MASK_POS)

#define Capesnse_CSD_IDAC2_DATA_OFFSET          (16u)

#define Capesnse_CSD_IDAC1_TRIM_MASK            (0xFFFFFFF0u)
#define Capesnse_CSD_IDAC2_TRIM_MASK            (0xFFFFFF0Fu)

#define Capesnse_CSFLASH_TRIM_IDAC1_MASK        (0x0Fu)
#define Capesnse_CSFLASH_TRIM_IDAC2_MASK        (0xF0)

#define Capesnse_CSD_4B_PWM_MODE_OFFSET         (4u)
#define Capesnse_CSD_4B_PWM_MODE_DEFAULT        (Capesnse_CSD_4B_PWM_MODE << Capesnse_CSD_4B_PWM_MODE_OFFSET)

#define Capesnse_CSD_4B_PWM_COUNT_MASK          (0x0000000Fu)
#define Capesnse_CSD_4B_PWM_MODE_MASK           (0x00000030u)

#define Capesnse_CSD_IDAC_PRECHARGE_CONFIG      (Capesnse_CSD_IDAC1_MODE_FIXED |\
                                                        Capesnse_PRECHARGE_IDAC1_VAL)

/* CSD_COUNTER register definitions */
#define Capesnse_CSD_CNT_COUNTER                (0xFFLu << Capesnse_CSD_CNT_COUNTER_POS )
#define Capesnse_CSD_CNT_PERIOD                 (0xFFLu << Capesnse_CSD_CNT_PERIOD_POS)

#define Capesnse_CSD_PRS_8_BIT                  (0x00000000u)
#define Capesnse_CSD_PRS_12_BIT                 (Capesnse_CSD_CFG_PRS_12_8)
#define Capesnse_PRS_MODE_MASK                  (Capesnse_CSD_CFG_PRS_12_8)

/***************************************
*    Initial Parameter Constants
***************************************/

#if (0u != Capesnse_FILTER_ENABLE)
    #define Capesnse_DEFAULT_FILTER_STATE       (Capesnse_CSD_CFG_FILTER_EN)

#else
    #define Capesnse_DEFAULT_FILTER_STATE       (0u)

#endif /* (Capesnse_PRS_OPTIONS == Capesnse_PRS_8BITS) */

#define Capesnse_DEFAULT_CSD_4B_PWM_CONFIG      (Capesnse_CSD_4B_PWM_MODE_DEFAULT | Capesnse_CSD_4B_PWM_COUNT)

/* Set PRS */
#if (Capesnse_PRS_OPTIONS == Capesnse__PRS_8BITS)
    #define Capesnse_DEFAULT_MODULATION_MODE    Capesnse_CSD_CFG_PRS_SELECT

#elif (Capesnse_PRS_OPTIONS == Capesnse__PRS_12BITS)
    #define Capesnse_DEFAULT_MODULATION_MODE    (Capesnse_CSD_CFG_PRS_12_8 |\
                                                        Capesnse_CSD_CFG_PRS_SELECT)
#else
    #define Capesnse_DEFAULT_MODULATION_MODE    (0u)
#endif /* (Capesnse_PRS_OPTIONS == Capesnse_PRS_8BITS) */

/* Set IDAC ranges */
#if (Capesnse_IDAC1_RANGE_VALUE == Capesnse__IDAC_8X)
    #define Capesnse_DEFAULT_IDAC1_RANGE        Capesnse_CSD_IDAC1_RANGE_8X
    #define Capesnse_DEFAULT_IDAC2_RANGE        Capesnse_CSD_IDAC2_RANGE_8X
#else
    #define Capesnse_DEFAULT_IDAC1_RANGE        (0u)
    #define Capesnse_DEFAULT_IDAC2_RANGE        (0u)
#endif /* (Capesnse_IDAC1_RANGE_VALUE == Capesnse_IDAC_RANGE_8X) */

/* Set IDAC polarity */
#if(Capesnse_IDAC1_POLARITY == Capesnse__IDAC_SINK)

    #define Capesnse_DEFAULT_IDAC_POLARITY      Capesnse_CSD_CFG_POLARITY_VDDIO
    #define Capesnse_CMOD_DR_CONFIG             Capesnse_CMOD_DR_VDDIO
    #define Capesnse_CTANK_DR_CONFIG            Capesnse_CTANK_DR_VDDIO
#else

    #define Capesnse_DEFAULT_IDAC_POLARITY      (0u)
    #define Capesnse_CMOD_DR_CONFIG             Capesnse_CMOD_DR_VSSIO
    #define Capesnse_CTANK_DR_CONFIG            Capesnse_CTANK_DR_VSSIO
#endif /* (Capesnse_IDAC_OPTIONS == Capesnse_IDAC_SINK) */

#define Capesnse_SNS_GROUND_CONNECT             (6u)
#define Capesnse_SNS_HIZANALOG_CONNECT          (0u)

/* Inactive sensors connection configuration */
#if (Capesnse_CONNECT_INACTIVE_SNS == Capesnse__GROUND)
    #define Capesnse_CSD_INACTIVE_CONNECT       (Capesnse_SNS_GROUND_CONNECT)
#else
    #define Capesnse_CSD_INACTIVE_CONNECT       (Capesnse_SNS_HIZANALOG_CONNECT)
#endif /* Capesnse_CONNECT_INACTIVE_SNS == Capesnse__GROUND */

#if(Capesnse_IS_SHIELD_ENABLE)
    #define Capesnse_SHIELD_PORT_NUMBER Capesnse_Shield__PORT
    #define Capesnse_SHIELD_PIN_NUMBER  Capesnse_Shield__SHIFT
#endif /* (Capesnse_IS_SHIELD_ENABLE) */

/* Defining default CSD configuration according to settings in customizer. */
#define Capesnse_DEFAULT_CSD_CONFIG             (Capesnse_DEFAULT_FILTER_STATE |\
                                                         Capesnse_DEFAULT_MODULATION_MODE |\
                                                         Capesnse_CSD_CFG_SENSE_COMP_BW |\
                                                         Capesnse_DEFAULT_IDAC_POLARITY |\
                                                         Capesnse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capesnse_CSD_REFBUF_DRV_HIGH)

#define Capesnse_CSD_CONFIG_MASK                (Capesnse_CSD_CFG_FILTER_EN |\
                                                         Capesnse_CSD_CFG_PRS_SELECT |\
                                                         Capesnse_CSD_CFG_PRS_12_8 |\
                                                         Capesnse_CSD_CFG_SENSE_COMP_BW |\
                                                         Capesnse_CSD_CFG_REFBUF_EN |\
                                                         Capesnse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capesnse_CSD_CFG_POLARITY_VDDIO |\
                                                         Capesnse_CSD_CFG_REFBUF_OUTSEL  |\
                                                         Capesnse_CSD_REFBUF_DRV_MASK)


/* Defining the default IDACs configuration according to settings in customizer. */
#if (Capesnse_IDAC_CNT == 1u)
    #define Capesnse_DEFAULT_CSD_IDAC_CONFIG    (Capesnse_CSD_IDAC1_MODE_VARIABLE |\
                                                             Capesnse_DEFAULT_IDAC1_RANGE)
#else
    #define Capesnse_DEFAULT_CSD_IDAC_CONFIG    (Capesnse_CSD_IDAC1_MODE_VARIABLE |\
                                                             Capesnse_CSD_IDAC2_MODE_FIXED |\
                                                             Capesnse_DEFAULT_IDAC1_RANGE |\
                                                             Capesnse_DEFAULT_IDAC2_RANGE)
#endif /* (Capesnse_IDAC_CNT == 1u) */

/* Defining mask intended for clearing bits related to pre-charging options. */
#define Capesnse_PRECHARGE_CONFIG_MASK          (Capesnse_CSD_CFG_REFBUF_EN |\
                                                         Capesnse_CSD_CFG_CHARGE_IO |\
                                                         Capesnse_CSD_CFG_COMP_PIN_CH2  |\
                                                         Capesnse_CSD_CFG_REFBUF_OUTSEL)

/* Cmod the precharge mode configuration */
#if(Capesnse_CMOD_PREGARGE_OPTION == Capesnse__CAPPRVREF)

    #define Capesnse_CMOD_PRECHARGE_CONFIG      (Capesnse_CSD_CFG_REFBUF_EN |\
                                                         Capesnse_CSD_CFG_COMP_PIN_CH2)

#else

    #define Capesnse_CMOD_PRECHARGE_CONFIG      (Capesnse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capesnse_CSD_CFG_REFBUF_EN |\
                                                         Capesnse_CSD_CFG_CHARGE_IO)

#endif /* (Capesnse_CMOD_PREGARGE_OPTION == Capesnse__CAPPRVREF) */

/* Ctank the precharge mode configuration */
#if(Capesnse_CSH_TANK_PREGARGE_OPTION == Capesnse__CAPPRVREF)

    #if(0u != Capesnse_IS_SHIELD_ENABLE)
        #define  Capesnse_CTANK_PRECHARGE_CONFIG    (Capesnse_CSD_CFG_REFBUF_EN |\
                                                             Capesnse_CSD_CFG_PRS_CLEAR |\
                                                             Capesnse_CSD_CFG_REFBUF_OUTSEL)
    #else
        #define  Capesnse_CTANK_PRECHARGE_CONFIG    (Capesnse_CSD_CFG_REFBUF_OUTSEL |\
                                                             Capesnse_CSD_CFG_PRS_CLEAR)
    #endif /* (0u != Capesnse_IS_SHIELD_ENABLE) */

#else

    #define  Capesnse_CTANK_PRECHARGE_CONFIG    (Capesnse_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capesnse_CSD_CFG_REFBUF_EN |\
                                                         Capesnse_CSD_CFG_CHARGE_IO |\
                                                         Capesnse_CSD_CFG_PRS_CLEAR |\
                                                         Capesnse_CSD_CFG_COMP_PIN_CH2)
#endif /* (Capesnse_CSH_TANK_PREGARGE_OPTION == Capesnse__CAPPRIOBUF) */

#define Capesnse_MAX_UINT_8                     (0xFFu)
#define Capesnse_MAX_UINT_16                    (0xFFFFu)
#define Capesnse_MAX_UINT_32                    (0xFFFFFFFFLu)

/***************************************
*     Vars with External Linkage
***************************************/
/* SmartSense functions */
#if (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO)
    extern uint8 Capesnse_lowLevelTuningDone;
    extern uint8 Capesnse_scanSpeedTbl[((Capesnse_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
    extern void Capesnse_AutoTune(void);
#endif /* (Capesnse_TUNING_METHOD == Capesnse__TUNING_AUTO) */

#if(Capesnse_PRS_OPTIONS != Capesnse__PRS_NONE)
    extern uint8 Capesnse_prescalersTuningDone;
#endif /* (Capesnse_PRS_OPTIONS == Capesnse__PRS_NONE) */

/* Global software variables */
extern volatile uint8 Capesnse_csdStatusVar;
extern volatile uint8 Capesnse_sensorIndex;
extern uint16 Capesnse_sensorRaw[Capesnse_TOTAL_SENSOR_COUNT];
extern uint8 Capesnse_unscannedSnsDriveMode[Capesnse_PORT_PIN_CONFIG_TBL_ZISE];
extern uint8 Capesnse_sensorEnableMaskBackup[Capesnse_TOTAL_SENSOR_MASK];
extern uint8 Capesnse_sensorEnableMask[Capesnse_TOTAL_SENSOR_MASK];

#if ((Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) || (0u != Capesnse_AUTOCALIBRATION_ENABLE))
    extern uint8 Capesnse_modulationIDAC[Capesnse_TOTAL_SENSOR_COUNT];
    extern uint8 Capesnse_compensationIDAC[Capesnse_TOTAL_SENSOR_COUNT];
#else
    extern const uint8 Capesnse_modulationIDAC[Capesnse_TOTAL_SENSOR_COUNT];
    extern const uint8 Capesnse_compensationIDAC[Capesnse_TOTAL_SENSOR_COUNT];
#endif /* ((Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) || (0u != Capesnse_AUTOCALIBRATION_ENABLE)) */

#if (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)
    extern uint32 Capesnse_widgetResolution[Capesnse_RESOLUTIONS_TBL_SIZE];
    #if (0u != Capesnse_MULTIPLE_FREQUENCY_SET)
        extern uint8 Capesnse_senseClkDividerVal[Capesnse_TOTAL_SCANSLOT_COUNT];
        extern uint8 Capesnse_sampleClkDividerVal[Capesnse_TOTAL_SCANSLOT_COUNT];
    #else
        extern uint8 Capesnse_senseClkDividerVal;
        extern uint8 Capesnse_sampleClkDividerVal;
    #endif /* (0u != Capesnse_MULTIPLE_FREQUENCY_SET) */
#else
    extern const uint32 Capesnse_widgetResolution[Capesnse_RESOLUTIONS_TBL_SIZE];
    #if (0u != Capesnse_MULTIPLE_FREQUENCY_SET)
        extern const uint8 Capesnse_senseClkDividerVal[Capesnse_TOTAL_SCANSLOT_COUNT];
        extern const uint8 Capesnse_sampleClkDividerVal[Capesnse_TOTAL_SCANSLOT_COUNT];
    #else
        extern const uint8 Capesnse_senseClkDividerVal;
        extern const uint8 Capesnse_sampleClkDividerVal;
    #endif /* (0u != Capesnse_MULTIPLE_FREQUENCY_SET) */
#endif /* (Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE)  */

extern const uint8 Capesnse_widgetNumber[Capesnse_TOTAL_SENSOR_COUNT];

extern reg32* const Capesnse_prtSelTbl[Capesnse_CFG_REG_TBL_SIZE];
extern reg32* const Capesnse_prtCfgTbl[Capesnse_CFG_REG_TBL_SIZE];
extern reg32* const Capesnse_prtDRTbl[Capesnse_CFG_REG_TBL_SIZE];
extern reg32 * Capesnse_pcTable[Capesnse_PORT_PIN_CONFIG_TBL_ZISE];

extern const uint8  Capesnse_portTable[Capesnse_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint32 Capesnse_maskTable[Capesnse_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint8  Capesnse_pinShiftTbl[Capesnse_PORT_PIN_CONFIG_TBL_ZISE];

#if (0u != Capesnse_INDEX_TABLE_SIZE)
extern const uint8 CYCODE Capesnse_indexTable[Capesnse_INDEX_TABLE_SIZE];
#endif /* (0u != Capesnse_INDEX_TABLE_SIZE)) */

/***************************************************
*    Obsolete Names (will be removed in future)
***************************************************/
#define  Capesnse_PrescalersTuningDone         Capesnse_prescalersTuningDone
#define  Capesnse_SensorRaw                    Capesnse_sensorRaw
#define  Capesnse_PRSResolutionTbl             Capesnse_prsResolutionTbl
#define  Capesnse_SensorEnableMask             Capesnse_sensorEnableMask
#define  Capesnse_Clk1DividerVal               Capesnse_senseClkDividerVal
#define  Capesnse_Clk2DividerVal               Capesnse_sampleClkDividerVal
#define  Capesnse_PrtSelTbl                    Capesnse_prtSelTbl
#define  Capesnse_PrtCfgTbl                    Capesnse_prtCfgTbl
#define  Capesnse_PrtDRTbl                     Capesnse_prtDRTbl
#define  Capesnse_idac1Settings                Capesnse_modulationIDAC
#define  Capesnse_idac2Settings                Capesnse_compensationIDAC

#endif /* CY_CAPSENSE_CSD_Capesnse_H */


 /* [] END OF FILE */
