/*******************************************************************************
* File Name: Capsense.h
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

#if !defined(CY_CAPSENSE_CSD_Capsense_H)
#define CY_CAPSENSE_CSD_Capsense_H

#include "cytypes.h"
#include "cyfitter.h"
#include "core_cm0_psoc4.h"
#include "CyLib.h"

#include "Capsense_IP.h"
#include "Capsense_SenseClk.h"
#include "Capsense_SampleClk.h"

/* Constants set by Customizer */
#define Capsense_TOTAL_SENSOR_COUNT            (1u)
#define Capsense_TOTAL_SCANSLOT_COUNT          (1u)
#define Capsense_INDEX_TABLE_SIZE              (0u)

/* Define Sensors */
#define Capsense_SENSOR_BUTTON0__BTN    (0u)

#define Capsense_TOTAL_SENSOR_MASK (((Capsense_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)

/* Interrupt handler */
CY_ISR_PROTO(Capsense_ISR);

/* ISR Number and Priority to work with CyLib functions */
#define Capsense_ISR_NUMBER        (Capsense_ISR__INTC_NUMBER)
#define Capsense_ISR_PRIORITY      (Capsense_ISR__INTC_PRIOR_NUM)

/***************************************
*   Condition compilation parameters
***************************************/

#define Capsense_CONNECT_INACTIVE_SNS       (0u)
#define Capsense_IS_COMPLEX_SCANSLOTS       (0u)
#define Capsense_COMPLEX_SCANSLOTS_NUM      (0u)
#define Capsense_DEDICATED_SENSORS_NUM      (0u)
#define Capsense_PORT_PIN_CONFIG_TBL_ZISE   (1u)

#define Capsense_IDAC_CNT                   (2u)
#define Capsense_IDAC1_POLARITY             (0u)
#define Capsense_IDAC1_RANGE_VALUE          (0u)
#define Capsense_IDAC2_RANGE_VALUE          (0u)

#define Capsense_MULTIPLE_FREQUENCY_SET     (1u)
#define Capsense_FILTER_ENABLE              (0u)
#define Capsense_PRS_OPTIONS                (0u)

#define Capsense_WATER_PROOF                (0u)

#define Capsense_TUNING_METHOD              (1u)
#define Capsense_TUNER_API_GENERATE         (1u)

#define Capsense_CSHL_API_GENERATE          (1u)

#define Capsense_CMOD_PREGARGE_OPTION       (1u)
#define Capsense_CSH_TANK_PREGARGE_OPTION   (1u)
#define Capsense_IS_SHIELD_ENABLE           (0u)
#define Capsense_CSH_TANK_ENABLE            (0u)

#define Capsense_SHIELD_DELAY               (0u)
#define Capsense_AUTOCALIBRATION_ENABLE     (1u)

#define Capsense_IS_OVERSAMPLING_EN         (1u)

#define Capsense_CSD_4B_PWM_MODE            (0u)
#define Capsense_CSD_4B_PWM_COUNT           (0u)

/**************************************
*    Enumerated Types and Parameters
**************************************/

/* Current source definitions */
#define Capsense__IDAC_SOURCE 0
#define Capsense__IDAC_SINK 1

#define Capsense__IDAC_4X 0
#define Capsense__IDAC_8X 1

#define Capsense__PRS_NONE 0
#define Capsense__PRS_8BITS 1
#define Capsense__PRS_12BITS 2
#define Capsense__PRS_AUTO 3

/* Connection of inactive sensors definitions */
#define Capsense__GROUND 0
#define Capsense__HIZ_ANALOG 1
#define Capsense__SHIELD 2

/* Precharge options definitions */
#define Capsense__CAPPRIOBUF 0
#define Capsense__CAPPRVREF 1

/* Method of tuning */
#define Capsense__TUNING_NONE 0
#define Capsense__TUNING_MANUAL 1
#define Capsense__TUNING_AUTO 2

/* Dead band PWM modulator options */
#define Capsense__PWM_OFF 0
#define Capsense__PWM_HIGH 2
#define Capsense__PWM_LOW 3


#define Capsense_DELAY_EXTRACYCLES_NUM          (9u)
#define Capsense_GLITCH_ELIMINATION_TIMEOUT     (0u)
#define Capsense_GLITCH_ELIMINATION_CYCLES_CALC (Capsense_GLITCH_ELIMINATION_TIMEOUT * CYDEV_BCLK__SYSCLK__MHZ)

#if(Capsense_GLITCH_ELIMINATION_CYCLES_CALC >= Capsense_DELAY_EXTRACYCLES_NUM)
    #define Capsense_GLITCH_ELIMINATION_CYCLES (Capsense_GLITCH_ELIMINATION_CYCLES_CALC -\
                                                        Capsense_DELAY_EXTRACYCLES_NUM)
#else
    #define Capsense_GLITCH_ELIMINATION_CYCLES (Capsense_GLITCH_ELIMINATION_CYCLES_CALC)
#endif /* (Capsense_GLITCH_ELIMINATION_CYCLES_CALC >= Capsense_DELAY_EXTRACYCLES_NUM) */

#define Capsense_INITIAL_CLK_DIVIDER            (CYDEV_BCLK__HFCLK__KHZ / CYDEV_BCLK__SYSCLK__KHZ)


/* Structure to save registers before going to sleep */
typedef struct
{
    uint8 enableState;
} Capsense_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/
void Capsense_Init(void);
void Capsense_CsdHwBlockInit(void);
void Capsense_Enable(void);
void Capsense_Start(void);
void Capsense_Stop(void);
void Capsense_SaveConfig(void);
void Capsense_Sleep(void);
void Capsense_RestoreConfig(void);
void Capsense_Wakeup(void);
uint32 Capsense_IsBusy(void);
void Capsense_ScanSensor(uint32 sensor);
void Capsense_ScanWidget(uint32 widget);
void Capsense_ScanEnabledWidgets(void);
uint16 Capsense_ReadSensorRaw(uint32 sensor);
void Capsense_WriteSensorRaw(uint32 sensor, uint16 value);
void Capsense_ClearSensors(void);
void Capsense_SetShieldDelay(uint32 delay);
uint32 Capsense_ReadCurrentScanningSensor(void);

uint32 Capsense_GetScanResolution(uint32 widget);
uint32 Capsense_GetSenseClkDivider(uint32 sensor);
uint32 Capsense_GetModulatorClkDivider(uint32 sensor);
uint32 Capsense_GetModulationIDAC(uint32 sensor);
uint32 Capsense_GetCompensationIDAC(uint32 sensor);
uint32 Capsense_GetIDACRange(void);

#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    void Capsense_SetScanResolution(uint32 widget, uint32 resolution);
    void Capsense_SetSenseClkDivider(uint32 sensor, uint32 senseClk);
    void Capsense_SetModulatorClkDivider(uint32 sensor, uint32 modulatorClk);
    void Capsense_SetModulationIDAC(uint32 sensor, uint32 modIdacValue);
    void Capsense_SetCompensationIDAC(uint32 sensor, uint32 compIdacValue);
    void Capsense_SetIDACRange(uint32 iDacRange);
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE) */


void Capsense_SetDriveModeAllPins(uint32 driveMode);
void Capsense_RestoreDriveModeAllPins(void);
void Capsense_SetUnscannedSensorState(uint32 sensor, uint32 sensorState);
void Capsense_NormalizeWidgets(uint32 widgetsNum, uint8 dataPtr[]);

/***************************************
*           API Constants
***************************************/

/* PWM Resolution */
#define Capsense_CNT_MSB_RESOLUTION_8_BITS  (0x00u)
#define Capsense_CNT_MSB_RESOLUTION_9_BITS  (0x01u)
#define Capsense_CNT_MSB_RESOLUTION_10_BITS (0x03u)
#define Capsense_CNT_MSB_RESOLUTION_11_BITS (0x07u)
#define Capsense_CNT_MSB_RESOLUTION_12_BITS (0x0Fu)
#define Capsense_CNT_MSB_RESOLUTION_13_BITS (0x1Fu)
#define Capsense_CNT_MSB_RESOLUTION_14_BITS (0x3Fu)
#define Capsense_CNT_MSB_RESOLUTION_15_BITS (0x7Fu)
#define Capsense_CNT_MSB_RESOLUTION_16_BITS (0xFFu)

#define Capsense_ONE_CYCLE                  (0x00010000Lu)

#define Capsense_WINDOW_APPEND              (0xFFFF0000Lu)
#define Capsense_RESOLUTION_6_BITS          (0x003F0000Lu)
#define Capsense_RESOLUTION_7_BITS          (0x007F0000Lu)
#define Capsense_RESOLUTION_8_BITS          (0x00FF0000Lu)
#define Capsense_RESOLUTION_9_BITS          (0x01FF0000Lu)
#define Capsense_RESOLUTION_10_BITS         (0x03FF0000Lu)
#define Capsense_RESOLUTION_11_BITS         (0x07FF0000Lu)
#define Capsense_RESOLUTION_12_BITS         (0x0FFF0000Lu)
#define Capsense_RESOLUTION_13_BITS         (0x1FFF0000Lu)
#define Capsense_RESOLUTION_14_BITS         (0x3FFF0000Lu)
#define Capsense_RESOLUTION_15_BITS         (0x7FFF0000Lu)
#define Capsense_RESOLUTION_16_BITS         (0xFFFF0000Lu)

#define Capsense_RESOLUTION_OFFSET          (16u)
#define Capsense_MSB_RESOLUTION_OFFSET      (22u)
#define Capsense_RESOLUTIONS_TBL_SIZE       (1u)

/* Software Status Register Bit Masks */
#define Capsense_SW_STS_BUSY                (0x01u)
/* Software Control Register Bit Masks */
#define Capsense_SW_CTRL_SINGLE_SCAN        (0x80u)

/* Software Control Register Bit Masks for scanning one widget */
#define Capsense_SW_CTRL_WIDGET_SCAN        (0x40u)

/* Flag for complex scan slot */
#define Capsense_COMPLEX_SS_FLAG            (0x80u)

/* Flag for scanning is complete */
#define Capsense_NOT_SENSOR                 (0xFFFFFFFFLu)

/* Number of bits for each pin in PC register. */
#define Capsense_PC_PIN_CFG_SIZE            (0x03u)

/* Number of bits for each pin in HSIOM register. */
#define Capsense_HSIOM_PIN_CFG_SIZE         (0x04u)

#define Capsense_CFG_REG_TBL_SIZE           (7u)

/***************************************
*             Registers
***************************************/

#define Capsense_CSD_ID_REG             (*(reg32 *)  Capsense_CSD_FFB__CSD_ID)
#define Capsense_CSD_ID_PTR             ( (reg32 *)  Capsense_CSD_FFB__CSD_ID)

#define Capsense_CSD_CFG_REG            (*(reg32 *)  Capsense_CSD_FFB__CSD_CONFIG)
#define Capsense_CSD_CFG_PTR            ( (reg32 *)  Capsense_CSD_FFB__CSD_CONFIG)

#define Capsense_CSD_IDAC_REG           (*(reg32 *)  Capsense_IDAC1_cy_psoc4_idac__CSD_IDAC)
#define Capsense_CSD_IDAC_PTR           ( (reg32 *)  Capsense_IDAC1_cy_psoc4_idac__CSD_IDAC)

#define Capsense_CSD_CNT_REG            (*(reg32 *) Capsense_CSD_FFB__CSD_COUNTER)
#define Capsense_CSD_CNT_PTR            ( (reg32 *) Capsense_CSD_FFB__CSD_COUNTER)

#define Capsense_CSD_STAT_REG           (*(reg32 *) Capsense_CSD_FFB__CSD_STATUS)
#define Capsense_CSD_STAT_PTR           ( (reg32 *) Capsense_CSD_FFB__CSD_STATUS)

#define Capsense_CSD_INTR_REG           (*(reg32 *) Capsense_CSD_FFB__CSD_INTR)
#define Capsense_CSD_INTR_PTR           ( (reg32 *) Capsense_CSD_FFB__CSD_INTR)

#define Capsense_CSD_INTR_SET_REG       (*(reg32 *) Capsense_CSD_FFB__CSD_INTR_SET)
#define Capsense_CSD_INTR_SET_PTR       ( (reg32 *) Capsense_CSD_FFB__CSD_INTR_SET)

#define Capsense_CSD_4B_PWM_REG         (*(reg32 *) Capsense_CSD_FFB__CSD_PWM)
#define Capsense_CSD_4B_PWM_PTR         ( (reg32 *) Capsense_CSD_FFB__CSD_PWM)

#define Capsense_CSD_TRIM1_REG          (*(reg32 *) Capsense_IDAC1_cy_psoc4_idac__CSD_TRIM1)
#define Capsense_CSD_TRIM1_PTR          ( (reg32 *) Capsense_IDAC1_cy_psoc4_idac__CSD_TRIM1)

#define Capsense_CSD_TRIM2_REG          (*(reg32 *) Capsense_IDAC1_cy_psoc4_idac__CSD_TRIM2)
#define Capsense_CSD_TRIM2_PTR          ( (reg32 *) Capsense_IDAC1_cy_psoc4_idac__CSD_TRIM2)

#if (2u == Capsense_M0S8CSD_BLOCK_CNT)
    #if (Capsense_CSD_FFB__CSD_NUMBER != 1u)
        #define Capsense_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
        #define Capsense_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

        #define Capsense_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
        #define Capsense_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
    #else
        #define Capsense_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)
        #define Capsense_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)

        #define Capsense_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
        #define Capsense_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
    #endif /* (Capsense_CSD_FFB__CSD_NUMBER != 1u) */
#else
    #define Capsense_SFLASH_CSD_TRIM1_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
    #define Capsense_SFLASH_CSD_TRIM1_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

    #define Capsense_SFLASH_CSD_TRIM2_REG   (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
    #define Capsense_SFLASH_CSD_TRIM2_PTR   ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
#endif /* (2u == Capsense_M0S8CSD_BLOCK_CNT) */

/* Port function select */
#define Capsense_PORT_SEL0_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL0 )
#define Capsense_PORT_SEL0_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL0 )

#define Capsense_PORT_SEL1_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL1 )
#define Capsense_PORT_SEL1_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL1 )

#define Capsense_PORT_SEL2_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL2 )
#define Capsense_PORT_SEL2_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL2 )

#define Capsense_PORT_SEL3_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL3 )
#define Capsense_PORT_SEL3_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL3 )

#define Capsense_PORT_SEL4_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL4 )
#define Capsense_PORT_SEL4_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL4 )

#define Capsense_PORT_SEL5_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL5 )
#define Capsense_PORT_SEL5_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL5 )

#define Capsense_PORT_SEL6_REG          (*(reg32 *) CYREG_HSIOM_PORT_SEL6 )
#define Capsense_PORT_SEL6_PTR          (*(reg32 *) CYREG_HSIOM_PORT_SEL6 )


#define Capsense_PORT0_PC_REG          (*(reg32 *) CYREG_GPIO_PRT0_PC )
#define Capsense_PORT0_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT0_PC )

#define Capsense_PORT1_PC_REG          (*(reg32 *) CYREG_GPIO_PRT1_PC )
#define Capsense_PORT1_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT1_PC )

#define Capsense_PORT2_PC_REG          (*(reg32 *) CYREG_GPIO_PRT2_PC )
#define Capsense_PORT2_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT2_PC )

#define Capsense_PORT3_PC_REG          (*(reg32 *) CYREG_GPIO_PRT3_PC )
#define Capsense_PORT3_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT3_PC )

#define Capsense_PORT4_PC_REG          (*(reg32 *) CYREG_GPIO_PRT4_PC )
#define Capsense_PORT4_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT4_PC )

#define Capsense_PORT5_PC_REG          (*(reg32 *) CYREG_GPIO_PRT5_PC )
#define Capsense_PORT5_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT5_PC )

#define Capsense_PORT6_PC_REG          (*(reg32 *) CYREG_GPIO_PRT6_PC )
#define Capsense_PORT6_PC_PTR          (*(reg32 *) CYREG_GPIO_PRT6_PC )


#define Capsense_PORT0_DR_REG          (*(reg32 *) CYREG_GPIO_PRT0_DR )
#define Capsense_PORT0_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT0_DR )

#define Capsense_PORT1_DR_REG          (*(reg32 *) CYREG_GPIO_PRT1_DR )
#define Capsense_PORT1_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT1_DR )

#define Capsense_PORT2_DR_REG          (*(reg32 *) CYREG_GPIO_PRT2_DR )
#define Capsense_PORT2_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT2_DR )

#define Capsense_PORT3_DR_REG          (*(reg32 *) CYREG_GPIO_PRT3_DR )
#define Capsense_PORT3_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT3_DR )

#define Capsense_PORT4_DR_REG          (*(reg32 *) CYREG_GPIO_PRT4_DR )
#define Capsense_PORT4_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT4_DR )

#define Capsense_PORT5_DR_REG          (*(reg32 *) CYREG_GPIO_PRT5_DR )
#define Capsense_PORT5_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT5_DR )

#define Capsense_PORT6_DR_REG          (*(reg32 *) CYREG_GPIO_PRT6_DR )
#define Capsense_PORT6_DR_PTR          (*(reg32 *) CYREG_GPIO_PRT6_DR )



#define Capsense_CMOD_CONNECTION_REG        (*(reg32 *) Capsense_Cmod__0__HSIOM)
#define Capsense_CMOD_PORT_PC_REG           (*(reg32 *) Capsense_Cmod__0__PC)
#define Capsense_CMOD_PORT_DR_REG           (*(reg32 *) Capsense_Cmod__0__DR)

#define Capsense_CTANK_CONNECTION_REG       (*(reg32 *) Capsense_Csh_tank__0__HSIOM)
#define Capsense_CTANK_PORT_PC_REG          (*(reg32 *) Capsense_Csh_tank__0__PC)
#define Capsense_CTANK_PORT_DR_REG          (*(reg32 *) Capsense_Csh_tank__0__DR)

/***************************************
*       Register Constants
***************************************/

/* Port configuration defines */
#define Capsense_CSD_HSIOM_MASK                 (0x0000000FLu)

#define Capsense_CSD_SENSE_PORT_MODE            (0x00000004Lu)
#define Capsense_CSD_SHIELD_PORT_MODE           (0x00000005Lu)

#define Capsense_AMUXA_CONNECTION_MODE          (0x00000006Lu)
#define Capsense_AMUXB_CONNECTION_MODE          (0x00000007Lu)

#define Capsense_CSD_PIN_MODE_MASK              (0x00000007Lu)
#define Capsense_CSD_PIN_DRIVE_MASK             (0x00000001Lu)

#define Capsense_CSD_PIN_DM_STRONG              (0x00000006Lu)
#define Capsense_CSD_PIN_DM_HIGH_Z              (0x00000000Lu)

#define Capsense_CSD_CMOD_CONNECTION_MASK   (uint32)(Capsense_Cmod__0__HSIOM_MASK)
#define Capsense_CSD_CMOD_TO_AMUXBUS_A      (uint32)(Capsense_AMUXA_CONNECTION_MODE << Capsense_Cmod__0__HSIOM_SHIFT)
#define Capsense_CSD_CMOD_TO_AMUXBUS_B      (uint32)(Capsense_AMUXB_CONNECTION_MODE << Capsense_Cmod__0__HSIOM_SHIFT)

#if(0u != Capsense_CSH_TANK_ENABLE)
    #define Capsense_CSD_CTANK_CONNECTION_MASK  (uint32)(Capsense_Csh_tank__0__HSIOM_MASK)
    #define Capsense_CSD_CTANK_TO_AMUXBUS_A     (uint32)(Capsense_AMUXA_CONNECTION_MODE << Capsense_Csh_tank__0__HSIOM_SHIFT)
    #define Capsense_CSD_CTANK_TO_AMUXBUS_B     (uint32)(Capsense_AMUXB_CONNECTION_MODE << Capsense_Csh_tank__0__HSIOM_SHIFT)
#endif /* (0u != Capsense_CSH_TANK_ENABLE) */

#define  Capsense_CSD_CMOD_PC_MASK          (uint32)(Capsense_CSD_PIN_MODE_MASK <<\
                                                            ((uint32)Capsense_Cmod__0__SHIFT * Capsense_PC_PIN_CFG_SIZE))
#define  Capsense_CMOD_PC_STRONG_MODE       (uint32)(Capsense_CSD_PIN_DM_STRONG <<\
                                                            ((uint32)Capsense_Cmod__0__SHIFT * Capsense_PC_PIN_CFG_SIZE))
#define  Capsense_CMOD_PC_HIGH_Z_MODE       (uint32)(Capsense_CSD_PIN_DM_HIGH_Z <<\
                                                            ((uint32)Capsense_Cmod__0__SHIFT * Capsense_PC_PIN_CFG_SIZE))

#if(0u != Capsense_CSH_TANK_ENABLE)
    #define  Capsense_CSD_CTANK_PC_MASK         (uint32)(Capsense_CSD_PIN_MODE_MASK <<\
                                                                ((uint32)Capsense_Csh_tank__0__SHIFT * Capsense_PC_PIN_CFG_SIZE))
    #define  Capsense_CTANK_PC_STRONG_MODE      (uint32)(Capsense_CSD_PIN_DM_STRONG <<\
                                                                ((uint32)Capsense_Csh_tank__0__SHIFT * Capsense_PC_PIN_CFG_SIZE))
    #define  Capsense_CTANK_PC_HIGH_Z_MODE      (uint32)(Capsense_CSD_PIN_DM_HIGH_Z <<\
                                                                ((uint32)Capsense_Csh_tank__0__SHIFT * Capsense_PC_PIN_CFG_SIZE))
#endif /* (0u != Capsense_CSH_TANK_ENABLE) */

#define  Capsense_CMOD_DR_VSSIO             (uint32)(Capsense_Cmod__0__MASK)
#define  Capsense_CMOD_DR_VDDIO             (0x00000000Lu)
#define  Capsense_CMOD_DR_MASK              (uint32)(Capsense_Cmod__0__MASK)

#if(0u != Capsense_CSH_TANK_ENABLE)
    #define  Capsense_CTANK_DR_VSSIO            (uint32)(Capsense_Csh_tank__0__MASK)
    #define  Capsense_CTANK_DR_VDDIO            (0x00000000Lu)
    #define  Capsense_CTANK_DR_MASK             (uint32)(Capsense_Csh_tank__0__MASK)
#endif /* (0u != Capsense_CSH_TANK_ENABLE) */

#define  Capsense_CMOD_PIN_NUMBER               (Capsense_Cmod__0__SHIFT)
#define  Capsense_CMOD_PRT_NUMBER               (Capsense_Cmod__0__PORT)

#if(0u != Capsense_CSH_TANK_ENABLE)
    #define  Capsense_CTANK_PIN_NUMBER          (Capsense_Csh_tank__0__SHIFT)
    #define  Capsense_CTANK_PRT_NUMBER          (Capsense_Csh_tank__0__PORT)
#endif /* (0u != Capsense_CSH_TANK_ENABLE) */

/* ID register fields position */
#define Capsense_CSD_ID_ID__POS                 (0x00u)
#define Capsense_CSD_ID_REV__POS                (0x10u)

#define Capsense_CSD_ID_ID                      (0xFFLu << Capsense_CSD_ID_ID__POS)
#define Capsense_CSD_ID_REV                     (0xFFLu << Capsense_CSD_ID_REV__POS)

/* CSD_CONFIG register fields position */
#define Capsense_CSD_CFG_DSI_SAMPLE_EN_POS      (0x00u)
#define Capsense_CSD_CFG_SAMPLE_SYNC_POS        (0x01u)
#define Capsense_CSD_CFG_FILTER_EN_POS          (0x03u)
#define Capsense_CSD_CFG_PRS_CLEAR_POS          (0x05u)
#define Capsense_CSD_CFG_PRS_SELECT_POS         (0x06u)
#define Capsense_CSD_CFG_PRS_12_8_POS           (0x07u)
#define Capsense_CSD_CFG_DSI_SENSE_EN_POS       (0x08u)
#define Capsense_CSD_CFG_SHIELD_DELAY_POS       (0x09u)
#define Capsense_CSD_CFG_SENSE_COMP_BW_POS      (0x0Bu)
#define Capsense_CSD_CFG_SENSE_EN_POS           (0x0Cu)
#define Capsense_CSD_CFG_REFBUF_EN_POS          (0x0Du)
#define Capsense_CSD_CFG_CHARGE_IO_POS          (0x0Eu)
#define Capsense_CSD_CFG_COMP_PIN_POS           (0x0Fu)
#define Capsense_CSD_CFG_POLARITY_POS           (0x10u)
#define Capsense_CSD_CFG_MUTUAL_CAP_POS         (0x12u)
#define Capsense_CSD_CFG_SENSE_COMP_EN_POS      (0x13u)
#define Capsense_CSD_CFG_REFBUF_OUTSEL_POS      (0x15u)
#define Capsense_CSD_CFG_SENSE_INSEL_POS        (0x16u)
#define Capsense_CSD_REFBUF_DRV_POS             (0x17u)
#define Capsense_CSD_CFG_DDFTSEL_POS            (0x1Au)
#define Capsense_CSD_CFG_ADFTEN_POS             (0x1Du)
#define Capsense_CSD_CFG_DDFTCOMP_POS           (0x1Eu)
#define Capsense_CSD_CFG_ENABLE_POS             (0x1Fu)

/* CSD_STATUS register fields position */
#define Capsense_CSD_STAT_CSD_CHARGE_POS        (0x00u)
#define Capsense_CSD_STAT_CSD_SENSE_POS         (0x01u)
#define Capsense_CSD_STAT_COMP_OUT_POS          (0x02u)
#define Capsense_CSD_STAT_SAMPLE_POS            (0x03u)

/* CSD_IDAC control register fields position */
#define Capsense_CSD_IDAC1_MODE_MASK_POS        (0x08u)
#define Capsense_CSD_IDAC1_MODE_FIXED_POS       (0x08u)
#define Capsense_CSD_IDAC1_MODE_VARIABLE_POS    (0x08u)
#define Capsense_CSD_IDAC1_MODE_DSI_POS         (0x08u)
#define Capsense_CSD_IDAC1_RANGE_8X_POS         (0x0Au)
#define Capsense_CSD_IDAC1_DATA_MASK_POS        (0x00u)

#define Capsense_CSD_IDAC2_MODE_MASK_POS        (0x18u)
#define Capsense_CSD_IDAC2_MODE_FIXED_POS       (0x18u)
#define Capsense_CSD_IDAC2_MODE_VARIABLE_POS    (0x18u)
#define Capsense_CSD_IDAC2_MODE_DSI_POS         (0x18u)
#define Capsense_CSD_IDAC2_RANGE_8X_POS         (0x1Au)
#define Capsense_CSD_IDAC2_DATA_MASK_POS        (0x10u)

/* CSD_COUNTER register fields position */
#define Capsense_CSD_CNT_COUNTER_POS            (0x00u)
#define Capsense_CSD_CNT_PERIOD_POS             (0x10u)

/* CSD_CONFIRG register definitions */
#define Capsense_CSD_CFG_DSI_SAMPLE_EN          (0x01Lu << Capsense_CSD_CFG_DSI_SAMPLE_EN_POS)
#define Capsense_CSD_CFG_SAMPLE_SYNC            (0x01Lu << Capsense_CSD_CFG_SAMPLE_SYNC_POS)
#define Capsense_CSD_CFG_FILTER_EN              (0x01Lu << Capsense_CSD_CFG_FILTER_EN_POS)
#define Capsense_CSD_CFG_PRS_CLEAR              (0x01Lu << Capsense_CSD_CFG_PRS_CLEAR_POS)
#define Capsense_CSD_CFG_PRS_SELECT             (0x01Lu << Capsense_CSD_CFG_PRS_SELECT_POS)
#define Capsense_CSD_CFG_PRS_12_8               (0x01Lu << Capsense_CSD_CFG_PRS_12_8_POS)
#define Capsense_CSD_CFG_DSI_SENSE_EN           (0x01Lu << Capsense_CSD_CFG_DSI_SENSE_EN_POS)
#define Capsense_CSD_CFG_SHIELD_DELAY_MASK      (0x03Lu << Capsense_CSD_CFG_SHIELD_DELAY_POS)
#define Capsense_CSD_CFG_SENSE_COMP_BW          (0x01Lu << Capsense_CSD_CFG_SENSE_COMP_BW_POS)
#define Capsense_CSD_CFG_SENSE_EN               (0x01Lu << Capsense_CSD_CFG_SENSE_EN_POS)
#define Capsense_CSD_CFG_REFBUF_EN              (0x01Lu << Capsense_CSD_CFG_REFBUF_EN_POS)
#define Capsense_CSD_CFG_CHARGE_IO              (0x01Lu << Capsense_CSD_CFG_CHARGE_IO_POS)
#define Capsense_CSD_CFG_COMP_PIN_CH2           (0x01Lu << Capsense_CSD_CFG_COMP_PIN_POS)
#define Capsense_CSD_CFG_POLARITY_VDDIO         (0x01Lu << Capsense_CSD_CFG_POLARITY_POS)
#define Capsense_CSD_CFG_MUTUAL_CAP             (0x01Lu << Capsense_CSD_CFG_MUTUAL_CAP_POS)
#define Capsense_CSD_CFG_SENSE_COMP_EN          (0x01Lu << Capsense_CSD_CFG_SENSE_COMP_EN_POS)
#define Capsense_CSD_CFG_REFBUF_OUTSEL          (0x01Lu << Capsense_CSD_CFG_REFBUF_OUTSEL_POS)
#define Capsense_CSD_CFG_SENSE_INSEL            (0x01Lu << Capsense_CSD_CFG_SENSE_INSEL_POS)
#define Capsense_CSD_REFBUF_DRV_MASK            (0x03Lu << Capsense_CSD_REFBUF_DRV_POS)
#define Capsense_CSD_REFBUF_DRV_LOW             (0x01Lu << Capsense_CSD_REFBUF_DRV_POS)
#define Capsense_CSD_REFBUF_DRV_MEDIUM          (0x02Lu << Capsense_CSD_REFBUF_DRV_POS)
#define Capsense_CSD_REFBUF_DRV_HIGH            (0x03Lu << Capsense_CSD_REFBUF_DRV_POS)
#define Capsense_CSD_CFG_DDFTSEL_MASK           (0x07Lu << Capsense_CSD_CFG_DDFTSEL_POS)
#define Capsense_CSD_CFG_ADFTEN                 (0x01Lu << Capsense_CSD_CFG_ADFTEN_POS)
#define Capsense_CSD_CFG_DDFTCOMP_MASK          (0x03Lu << Capsense_CSD_CFG_DDFTCOMP_POS)
#define Capsense_CSD_CFG_DDFTCOMP_REFCOMP       (0x01Lu << Capsense_CSD_CFG_DDFTCOMP_POS)
#define Capsense_CSD_CFG_DDFTCOMP_SENSECOMP     (0x02Lu << Capsense_CSD_CFG_DDFTCOMP_POS)
#define Capsense_CSD_CFG_ENABLE                 (0x01Lu << Capsense_CSD_CFG_ENABLE_POS)

/* CSD_STATUS register definitions */
#define Capsense_CSD_STAT_CSD_CHARGE            (0x01Lu << Capsense_CSD_STAT_CSD_CHARGE_POS)
#define Capsense_CSD_STAT_CSD_SENSE             (0x01Lu << Capsense_CSD_STAT_CSD_SENSE_POS)
#define Capsense_CSD_STAT_COMP_OUT              (0x01Lu << Capsense_CSD_STAT_COMP_OUT_POS)
#define Capsense_CSD_STAT_SAMPLE                (0x01Lu << Capsense_CSD_STAT_SAMPLE_POS)

/* CSD_IDAC control register definitions */
#define Capsense_CSD_IDAC1_MODE_MASK            (0x03Lu << Capsense_CSD_IDAC1_MODE_MASK_POS)
#define Capsense_CSD_IDAC1_MODE_FIXED           (0x01Lu << Capsense_CSD_IDAC1_MODE_FIXED_POS)
#define Capsense_CSD_IDAC1_MODE_VARIABLE        (0x02Lu << Capsense_CSD_IDAC1_MODE_VARIABLE_POS)
#define Capsense_CSD_IDAC1_MODE_DSI             (0x03Lu << Capsense_CSD_IDAC1_MODE_DSI_POS)
#define Capsense_CSD_IDAC1_RANGE_8X             (0x01Lu << Capsense_CSD_IDAC1_RANGE_8X_POS)
#define Capsense_CSD_IDAC1_DATA_MASK            (0xFFLu << Capsense_CSD_IDAC1_DATA_MASK_POS)

#define Capsense_CSD_IDAC2_MODE_MASK            (0x03Lu << Capsense_CSD_IDAC2_MODE_MASK_POS)
#define Capsense_CSD_IDAC2_MODE_FIXED           (0x01Lu << Capsense_CSD_IDAC2_MODE_FIXED_POS)
#define Capsense_CSD_IDAC2_MODE_VARIABLE        (0x02Lu << Capsense_CSD_IDAC2_MODE_VARIABLE_POS)
#define Capsense_CSD_IDAC2_MODE_DSI             (0x03Lu << Capsense_CSD_IDAC2_MODE_DSI_POS)
#define Capsense_CSD_IDAC2_RANGE_8X             (0x01Lu << Capsense_CSD_IDAC2_RANGE_8X_POS)
#define Capsense_CSD_IDAC2_DATA_MASK            (0x7FLu << Capsense_CSD_IDAC2_DATA_MASK_POS)

#define Capsense_CSD_IDAC2_DATA_OFFSET          (16u)

#define Capsense_CSD_IDAC1_TRIM_MASK            (0xFFFFFFF0u)
#define Capsense_CSD_IDAC2_TRIM_MASK            (0xFFFFFF0Fu)

#define Capsense_CSFLASH_TRIM_IDAC1_MASK        (0x0Fu)
#define Capsense_CSFLASH_TRIM_IDAC2_MASK        (0xF0)

#define Capsense_CSD_4B_PWM_MODE_OFFSET         (4u)
#define Capsense_CSD_4B_PWM_MODE_DEFAULT        (Capsense_CSD_4B_PWM_MODE << Capsense_CSD_4B_PWM_MODE_OFFSET)

#define Capsense_CSD_4B_PWM_COUNT_MASK          (0x0000000Fu)
#define Capsense_CSD_4B_PWM_MODE_MASK           (0x00000030u)

#define Capsense_CSD_IDAC_PRECHARGE_CONFIG      (Capsense_CSD_IDAC1_MODE_FIXED |\
                                                        Capsense_PRECHARGE_IDAC1_VAL)

/* CSD_COUNTER register definitions */
#define Capsense_CSD_CNT_COUNTER                (0xFFLu << Capsense_CSD_CNT_COUNTER_POS )
#define Capsense_CSD_CNT_PERIOD                 (0xFFLu << Capsense_CSD_CNT_PERIOD_POS)

#define Capsense_CSD_PRS_8_BIT                  (0x00000000u)
#define Capsense_CSD_PRS_12_BIT                 (Capsense_CSD_CFG_PRS_12_8)
#define Capsense_PRS_MODE_MASK                  (Capsense_CSD_CFG_PRS_12_8)

/***************************************
*    Initial Parameter Constants
***************************************/

#if (0u != Capsense_FILTER_ENABLE)
    #define Capsense_DEFAULT_FILTER_STATE       (Capsense_CSD_CFG_FILTER_EN)

#else
    #define Capsense_DEFAULT_FILTER_STATE       (0u)

#endif /* (Capsense_PRS_OPTIONS == Capsense_PRS_8BITS) */

#define Capsense_DEFAULT_CSD_4B_PWM_CONFIG      (Capsense_CSD_4B_PWM_MODE_DEFAULT | Capsense_CSD_4B_PWM_COUNT)

/* Set PRS */
#if (Capsense_PRS_OPTIONS == Capsense__PRS_8BITS)
    #define Capsense_DEFAULT_MODULATION_MODE    Capsense_CSD_CFG_PRS_SELECT

#elif (Capsense_PRS_OPTIONS == Capsense__PRS_12BITS)
    #define Capsense_DEFAULT_MODULATION_MODE    (Capsense_CSD_CFG_PRS_12_8 |\
                                                        Capsense_CSD_CFG_PRS_SELECT)
#else
    #define Capsense_DEFAULT_MODULATION_MODE    (0u)
#endif /* (Capsense_PRS_OPTIONS == Capsense_PRS_8BITS) */

/* Set IDAC ranges */
#if (Capsense_IDAC1_RANGE_VALUE == Capsense__IDAC_8X)
    #define Capsense_DEFAULT_IDAC1_RANGE        Capsense_CSD_IDAC1_RANGE_8X
    #define Capsense_DEFAULT_IDAC2_RANGE        Capsense_CSD_IDAC2_RANGE_8X
#else
    #define Capsense_DEFAULT_IDAC1_RANGE        (0u)
    #define Capsense_DEFAULT_IDAC2_RANGE        (0u)
#endif /* (Capsense_IDAC1_RANGE_VALUE == Capsense_IDAC_RANGE_8X) */

/* Set IDAC polarity */
#if(Capsense_IDAC1_POLARITY == Capsense__IDAC_SINK)

    #define Capsense_DEFAULT_IDAC_POLARITY      Capsense_CSD_CFG_POLARITY_VDDIO
    #define Capsense_CMOD_DR_CONFIG             Capsense_CMOD_DR_VDDIO
    #define Capsense_CTANK_DR_CONFIG            Capsense_CTANK_DR_VDDIO
#else

    #define Capsense_DEFAULT_IDAC_POLARITY      (0u)
    #define Capsense_CMOD_DR_CONFIG             Capsense_CMOD_DR_VSSIO
    #define Capsense_CTANK_DR_CONFIG            Capsense_CTANK_DR_VSSIO
#endif /* (Capsense_IDAC_OPTIONS == Capsense_IDAC_SINK) */

#define Capsense_SNS_GROUND_CONNECT             (6u)
#define Capsense_SNS_HIZANALOG_CONNECT          (0u)

/* Inactive sensors connection configuration */
#if (Capsense_CONNECT_INACTIVE_SNS == Capsense__GROUND)
    #define Capsense_CSD_INACTIVE_CONNECT       (Capsense_SNS_GROUND_CONNECT)
#else
    #define Capsense_CSD_INACTIVE_CONNECT       (Capsense_SNS_HIZANALOG_CONNECT)
#endif /* Capsense_CONNECT_INACTIVE_SNS == Capsense__GROUND */

#if(Capsense_IS_SHIELD_ENABLE)
    #define Capsense_SHIELD_PORT_NUMBER Capsense_Shield__PORT
    #define Capsense_SHIELD_PIN_NUMBER  Capsense_Shield__SHIFT
#endif /* (Capsense_IS_SHIELD_ENABLE) */

/* Defining default CSD configuration according to settings in customizer. */
#define Capsense_DEFAULT_CSD_CONFIG             (Capsense_DEFAULT_FILTER_STATE |\
                                                         Capsense_DEFAULT_MODULATION_MODE |\
                                                         Capsense_CSD_CFG_SENSE_COMP_BW |\
                                                         Capsense_DEFAULT_IDAC_POLARITY |\
                                                         Capsense_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capsense_CSD_REFBUF_DRV_HIGH)

#define Capsense_CSD_CONFIG_MASK                (Capsense_CSD_CFG_FILTER_EN |\
                                                         Capsense_CSD_CFG_PRS_SELECT |\
                                                         Capsense_CSD_CFG_PRS_12_8 |\
                                                         Capsense_CSD_CFG_SENSE_COMP_BW |\
                                                         Capsense_CSD_CFG_REFBUF_EN |\
                                                         Capsense_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capsense_CSD_CFG_POLARITY_VDDIO |\
                                                         Capsense_CSD_CFG_REFBUF_OUTSEL  |\
                                                         Capsense_CSD_REFBUF_DRV_MASK)


/* Defining the default IDACs configuration according to settings in customizer. */
#if (Capsense_IDAC_CNT == 1u)
    #define Capsense_DEFAULT_CSD_IDAC_CONFIG    (Capsense_CSD_IDAC1_MODE_VARIABLE |\
                                                             Capsense_DEFAULT_IDAC1_RANGE)
#else
    #define Capsense_DEFAULT_CSD_IDAC_CONFIG    (Capsense_CSD_IDAC1_MODE_VARIABLE |\
                                                             Capsense_CSD_IDAC2_MODE_FIXED |\
                                                             Capsense_DEFAULT_IDAC1_RANGE |\
                                                             Capsense_DEFAULT_IDAC2_RANGE)
#endif /* (Capsense_IDAC_CNT == 1u) */

/* Defining mask intended for clearing bits related to pre-charging options. */
#define Capsense_PRECHARGE_CONFIG_MASK          (Capsense_CSD_CFG_REFBUF_EN |\
                                                         Capsense_CSD_CFG_CHARGE_IO |\
                                                         Capsense_CSD_CFG_COMP_PIN_CH2  |\
                                                         Capsense_CSD_CFG_REFBUF_OUTSEL)

/* Cmod the precharge mode configuration */
#if(Capsense_CMOD_PREGARGE_OPTION == Capsense__CAPPRVREF)

    #define Capsense_CMOD_PRECHARGE_CONFIG      (Capsense_CSD_CFG_REFBUF_EN |\
                                                         Capsense_CSD_CFG_COMP_PIN_CH2)

#else

    #define Capsense_CMOD_PRECHARGE_CONFIG      (Capsense_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capsense_CSD_CFG_REFBUF_EN |\
                                                         Capsense_CSD_CFG_CHARGE_IO)

#endif /* (Capsense_CMOD_PREGARGE_OPTION == Capsense__CAPPRVREF) */

/* Ctank the precharge mode configuration */
#if(Capsense_CSH_TANK_PREGARGE_OPTION == Capsense__CAPPRVREF)

    #if(0u != Capsense_IS_SHIELD_ENABLE)
        #define  Capsense_CTANK_PRECHARGE_CONFIG    (Capsense_CSD_CFG_REFBUF_EN |\
                                                             Capsense_CSD_CFG_PRS_CLEAR |\
                                                             Capsense_CSD_CFG_REFBUF_OUTSEL)
    #else
        #define  Capsense_CTANK_PRECHARGE_CONFIG    (Capsense_CSD_CFG_REFBUF_OUTSEL |\
                                                             Capsense_CSD_CFG_PRS_CLEAR)
    #endif /* (0u != Capsense_IS_SHIELD_ENABLE) */

#else

    #define  Capsense_CTANK_PRECHARGE_CONFIG    (Capsense_CSD_CFG_REFBUF_OUTSEL |\
                                                         Capsense_CSD_CFG_REFBUF_EN |\
                                                         Capsense_CSD_CFG_CHARGE_IO |\
                                                         Capsense_CSD_CFG_PRS_CLEAR |\
                                                         Capsense_CSD_CFG_COMP_PIN_CH2)
#endif /* (Capsense_CSH_TANK_PREGARGE_OPTION == Capsense__CAPPRIOBUF) */

#define Capsense_MAX_UINT_8                     (0xFFu)
#define Capsense_MAX_UINT_16                    (0xFFFFu)
#define Capsense_MAX_UINT_32                    (0xFFFFFFFFLu)

/***************************************
*     Vars with External Linkage
***************************************/
/* SmartSense functions */
#if (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)
    extern uint8 Capsense_lowLevelTuningDone;
    extern uint8 Capsense_scanSpeedTbl[((Capsense_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u];
    extern void Capsense_AutoTune(void);
#endif /* (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */

#if(Capsense_PRS_OPTIONS != Capsense__PRS_NONE)
    extern uint8 Capsense_prescalersTuningDone;
#endif /* (Capsense_PRS_OPTIONS == Capsense__PRS_NONE) */

/* Global software variables */
extern volatile uint8 Capsense_csdStatusVar;
extern volatile uint8 Capsense_sensorIndex;
extern uint16 Capsense_sensorRaw[Capsense_TOTAL_SENSOR_COUNT];
extern uint8 Capsense_unscannedSnsDriveMode[Capsense_PORT_PIN_CONFIG_TBL_ZISE];
extern uint8 Capsense_sensorEnableMaskBackup[Capsense_TOTAL_SENSOR_MASK];
extern uint8 Capsense_sensorEnableMask[Capsense_TOTAL_SENSOR_MASK];

#if ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) || (0u != Capsense_AUTOCALIBRATION_ENABLE))
    extern uint8 Capsense_modulationIDAC[Capsense_TOTAL_SENSOR_COUNT];
    extern uint8 Capsense_compensationIDAC[Capsense_TOTAL_SENSOR_COUNT];
#else
    extern const uint8 Capsense_modulationIDAC[Capsense_TOTAL_SENSOR_COUNT];
    extern const uint8 Capsense_compensationIDAC[Capsense_TOTAL_SENSOR_COUNT];
#endif /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) || (0u != Capsense_AUTOCALIBRATION_ENABLE)) */

#if (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)
    extern uint32 Capsense_widgetResolution[Capsense_RESOLUTIONS_TBL_SIZE];
    #if (0u != Capsense_MULTIPLE_FREQUENCY_SET)
        extern uint8 Capsense_senseClkDividerVal[Capsense_TOTAL_SCANSLOT_COUNT];
        extern uint8 Capsense_sampleClkDividerVal[Capsense_TOTAL_SCANSLOT_COUNT];
    #else
        extern uint8 Capsense_senseClkDividerVal;
        extern uint8 Capsense_sampleClkDividerVal;
    #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET) */
#else
    extern const uint32 Capsense_widgetResolution[Capsense_RESOLUTIONS_TBL_SIZE];
    #if (0u != Capsense_MULTIPLE_FREQUENCY_SET)
        extern const uint8 Capsense_senseClkDividerVal[Capsense_TOTAL_SCANSLOT_COUNT];
        extern const uint8 Capsense_sampleClkDividerVal[Capsense_TOTAL_SCANSLOT_COUNT];
    #else
        extern const uint8 Capsense_senseClkDividerVal;
        extern const uint8 Capsense_sampleClkDividerVal;
    #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET) */
#endif /* (Capsense_TUNING_METHOD != Capsense__TUNING_NONE)  */

extern const uint8 Capsense_widgetNumber[Capsense_TOTAL_SENSOR_COUNT];

extern reg32* const Capsense_prtSelTbl[Capsense_CFG_REG_TBL_SIZE];
extern reg32* const Capsense_prtCfgTbl[Capsense_CFG_REG_TBL_SIZE];
extern reg32* const Capsense_prtDRTbl[Capsense_CFG_REG_TBL_SIZE];
extern reg32 * Capsense_pcTable[Capsense_PORT_PIN_CONFIG_TBL_ZISE];

extern const uint8  Capsense_portTable[Capsense_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint32 Capsense_maskTable[Capsense_PORT_PIN_CONFIG_TBL_ZISE];
extern const uint8  Capsense_pinShiftTbl[Capsense_PORT_PIN_CONFIG_TBL_ZISE];

#if (0u != Capsense_INDEX_TABLE_SIZE)
extern const uint8 CYCODE Capsense_indexTable[Capsense_INDEX_TABLE_SIZE];
#endif /* (0u != Capsense_INDEX_TABLE_SIZE)) */

/***************************************************
*    Obsolete Names (will be removed in future)
***************************************************/
#define  Capsense_PrescalersTuningDone         Capsense_prescalersTuningDone
#define  Capsense_SensorRaw                    Capsense_sensorRaw
#define  Capsense_PRSResolutionTbl             Capsense_prsResolutionTbl
#define  Capsense_SensorEnableMask             Capsense_sensorEnableMask
#define  Capsense_Clk1DividerVal               Capsense_senseClkDividerVal
#define  Capsense_Clk2DividerVal               Capsense_sampleClkDividerVal
#define  Capsense_PrtSelTbl                    Capsense_prtSelTbl
#define  Capsense_PrtCfgTbl                    Capsense_prtCfgTbl
#define  Capsense_PrtDRTbl                     Capsense_prtDRTbl
#define  Capsense_idac1Settings                Capsense_modulationIDAC
#define  Capsense_idac2Settings                Capsense_compensationIDAC

#endif /* CY_CAPSENSE_CSD_Capsense_H */


 /* [] END OF FILE */
