/*******************************************************************************
* File Name: Capsense_MBX.h
* Version 2.60
*
* Description:
*  This file provides constants and structure declarations for the tuner
*  communication for the CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_MBX_Capsense_H)
#define CY_CAPSENSE_CSD_MBX_Capsense_H

#include "Capsense.h"
#include "Capsense_PVT.h"

#if(0u != Capsense_CSHL_API_GENERATE)
    #include "Capsense_CSHL.h"
#endif /* (0u != Capsense_CSHL_API_GENERATE) */

#if(Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
    #include "Capsense_GESTURE.h"
#endif /* (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */
/***************************************
*   Condition compilation parameters
***************************************/
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    #define Capsense_PACKED_STRUCT_GNUC_ARMCC __attribute__ ((packed))
#else
    #define Capsense_PACKED_STRUCT_GNUC_ARMCC
#endif /* (defined(__GNUC__) || defined(__ARMCC_VERSION)) */

#if defined(__ICCARM__)
    #define Capsense_PACKED_STRUCT_IAR __packed
#else
    #define Capsense_PACKED_STRUCT_IAR
#endif /* (__ICCARM__) */

#if defined(__GNUC__)
    #define Capsense_NO_STRICT_VOLATILE __attribute__((optimize("-fno-strict-volatile-bitfields")))
#else
    #define Capsense_NO_STRICT_VOLATILE
#endif /* (defined(__GNUC__)) */


/***************************************
*           API Constants
***************************************/

/* Selected ID for this version of Tuner */
#define Capsense_TYPE_ID                (0x04u)

#define Capsense_BUSY_FLAG              (Capsense_TYPE_ID | 0x80u)
#define Capsense_HAVE_MSG               (sizeof(Capsense_MAILBOX) | 0x8000u)

#define Capsense_UPDATE_GESTURE_PARAMS  (Capsense_MAX_UINT_8)

#define Capsense_IS_ANY_GESTURES_EN     ((0u != Capsense_CLICK_GEST_ENABLED) ||\
                                                 (0u != Capsense_ZOOM_GEST_ENABLED)  ||\
                                                 (0u != Capsense_ST_SCROLL_GEST_ENABLED)||\
                                                 (0u != Capsense_DT_SCROLL_GEST_ENABLED)   ||\
                                                 (0u != Capsense_FLICK_GEST_ENABLED) ||\
                                                 (0u != Capsense_ROTATE_GEST_ENABLED)||\
                                                 (0u != Capsense_EDGE_SWIPE_GEST_ENABLED))

#define Capsense_MBX_TP_FILTERS_MASK    (Capsense_XY_IIR_MASK |\
                                                 Capsense_XY_JITTER_MASK |\
                                                 Capsense_XY_BALLISTIC_MASK |\
                                                 Capsense_XY_ADAPTIVE_IIR_MASK)

#define Capsense_MBX_TP_IIR_COEF_MASK   (0x07u)

/* Check Sum to identify data */
#define Capsense_CHECK_SUM      (11u)
#define Capsense_TOTAL_SENSOR_MASK_COUNT     (((Capsense_TOTAL_SENSOR_COUNT - 1u) / 8u) + 1u)

#define Capsense_WIDGET_CSHL_PARAMETERS_COUNT           (Capsense_TOTAL_WIDGET_COUNT + \
                                                                 Capsense_TOTAL_TOUCH_PADS_COUNT + \
                                                                 Capsense_TOTAL_MATRIX_BUTTONS_COUNT)

#define Capsense_WIDGET_RESOLUTION_PARAMETERS_COUNT     (Capsense_WIDGET_CSHL_PARAMETERS_COUNT + \
                                                                 Capsense_TOTAL_GENERICS_COUNT)


/***************************************
*      Type defines for mailboxes
***************************************/
typedef Capsense_PACKED_STRUCT_IAR struct
{
    uint16 apiResolution;

    uint8 fingerThreshold;
    uint8 noiseThreshold;
    uint8 hysteresis;
    uint8 debounce;

    uint8 modulatorIDAC;
    #if (Capsense_IDAC_CNT == 2u)
        uint8 compensationIDAC;
    #endif /* ( (Capsense_IDAC_CNT == 2u) */

    uint8 analogSwitchDivider;
    uint8 modulatorDivider;
    uint8 idacRange;
    uint8 scanResolution;
}Capsense_PACKED_STRUCT_GNUC_ARMCC Capsense_INBOX_CSD_CONFIG;

#if(Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
#if((0u != Capsense_TRACKPAD_GEST_POS_FILTERS_MASK) || (0u != Capsense_IS_ANY_GESTURES_EN))
    typedef Capsense_PACKED_STRUCT_IAR struct
    {
        #if(0u != Capsense_CLICK_GEST_ENABLED)
            uint16 clickRadiusX;            /* Click X Radius Pixels */
            uint16 clickRadiusY;            /* Click Y Radius Pixels */
            uint16 doubleClickRadius;       /* Double click max radius */
            uint16 stDoubleClickTimeoutMax; /* Double click max timeout */
            uint16 stDoubleClickTimeoutMin; /* Double click min timeout */
            uint16 stClickTimeoutMax;       /* Single click max timeout */
            uint16 stClickTimeoutMin;       /* Single click min timeout */
            uint16 dtClickTimeoutMax;       /* Two finger click max timeout */
            uint16 dtClickTimeoutMin;       /* Two finger click min timeout */
        #endif /* (0u != Capsense_CLICK_GEST_ENABLED) */

        #if(0u != Capsense_EDGE_SWIPE_GEST_ENABLED)
            uint16 edgeSwipeTime;          /* Edge Swipe Time */
            uint16 edgeSwipeCompleteTimeout;  /* Edge Swipe Complete Timeout */
            uint8  edgeSwipeActiveDistance; /* Active Edge Swipe threshold */
            uint8  bottomAngleThreshold;    /* Bottom angle threshold */
            uint8  topAngleThreshold;       /* Top angle threshold */
            uint8  widthOfDisambiguation;   /* Width of disambiguation region */
        #endif /* (0u != Capsense_EDGE_SWIPE_GEST_ENABLED) */

        #if(0u != Capsense_ZOOM_GEST_ENABLED)
            uint8 dtPanToZoomDebounce;      /* Debounce Scroll to Zoom count */
            uint8 dtZoomDebounce;           /* Debounce Zoom count */
            uint8 zoomActiveDistanceX;      /* Zoom Active distance threshold X */
            uint8 zoomActiveDistanceY;      /* Zoom Active distance threshold Y */
        #endif /* (0u != Capsense_ZOOM_GEST_ENABLED) */

        #if(0u != Capsense_ST_SCROLL_GEST_ENABLED)
            uint8 stScrThreshold1X;         /* One finger Scroll Threshold 1X */
            uint8 stScrThreshold2X;         /* One finger Scroll Threshold 2X */
            uint8 stScrThreshold3X;         /* One finger Scroll Threshold 3X */
            uint8 stScrThreshold4X;         /* One finger Scroll Threshold 4X */
            uint8 stScrThreshold1Y;         /* One finger Scroll Threshold 1Y; */
            uint8 stScrThreshold2Y;         /* One finger Scroll Threshold 2Y; */
            uint8 stScrThreshold3Y;         /* One finger Scroll Threshold 3Y; */
            uint8 stScrThreshold4Y;         /* One finger Scroll Threshold 4Y; */
            uint8 stScrStep1;               /* One finger Scroll Step 1 */
            uint8 stScrStep2;               /* One finger Scroll Step 2 */
            uint8 stScrStep3;               /* One finger Scroll Step 3 */
            uint8 stScrStep4;               /* One finger Scroll Step 4 */
            uint8 stScrDebounce;            /* One finger Scroll Debounce count */
            uint8 stInScrActiveDistanceX;   /* One finger Inertial scroll Active distance threshold X */
            uint8 stInScrActiveDistanceY;   /* One finger Inertial scroll Active distance threshold Y */
            uint8 stInScrCountLevel;        /* One finger Inertial scroll count level X */
        #endif /* (0u != Capsense_ST_SCROLL_GEST_ENABLED) */

        #if(0u != Capsense_DT_SCROLL_GEST_ENABLED)
            uint8 dtScrThreshold1X;         /* Two finger Scroll Threshold 1X */
            uint8 dtScrThreshold2X;         /* Two finger Scroll Threshold 2X */
            uint8 dtScrThreshold3X;         /* Two finger Scroll Threshold 3X */
            uint8 dtScrThreshold4X;         /* Two finger Scroll Threshold 4X */
            uint8 dtScrThreshold1Y;         /* Two finger Scroll Threshold 1Y; */
            uint8 dtScrThreshold2Y;         /* Two finger Scroll Threshold 2Y; */
            uint8 dtScrThreshold3Y;         /* Two finger Scroll Threshold 3Y; */
            uint8 dtScrThreshold4Y;         /* Two finger Scroll Threshold 4Y; */
            uint8 dtScrStep1;               /* Two finger Scroll Step 1 */
            uint8 dtScrStep2;               /* Two finger Scroll Step 2 */
            uint8 dtScrStep3;               /* Two finger Scroll Step 3 */
            uint8 dtScrStep4;               /* Two finger Scroll Step 4 */
            uint8 dtScrDebounce;            /* Two finger Scroll Debounce count */
            uint8 dtInScrActiveDistanceX;   /* Two finger Inertial scroll Active distance threshold X */
            uint8 dtInScrActiveDistanceY;   /* Two finger Inertial scroll Active distance threshold Y */
            uint8 dtInScrCountLevel;        /* Two finger Inertial scroll count level Y */
        #endif /* (0u != Capsense_ST_SCROLL_GEST_ENABLED) */

        #if(0u != Capsense_FLICK_GEST_ENABLED)
            uint16 flickSampleTime;         /* Flick Sample Time */
            uint8 flickActiveDistanceX;     /* Flick Active distance threshold X */
            uint8 flickActiveDistanceY;     /* Flick Active distance threshold Y */
        #endif /* (0u != Capsense_FLICK_GEST_ENABLED) */

        #if(0u != Capsense_ROTATE_GEST_ENABLED)
            uint8 rotateDebounce;           /* Rotate debounce limit */
        #endif /* (0u != Capsense_ROTATE_GEST_ENABLED) */

        #if(0u != Capsense_TRACKPAD_GEST_POS_FILTERS_MASK)
            uint8 filtersMask;
        #endif /* (0u != Capsense_TRACKPAD_GEST_POS_FILTERS_MASK) */

        #if(0u != (Capsense_XY_ADAPTIVE_IIR_MASK & Capsense_TRACKPAD_GEST_POS_FILTERS_MASK))
            uint8 filterDivisor;
            uint8 largeMovThreshold;
            uint8 littleMovThreshold;
            uint8 maxFilterCoef;
            uint8 minFilterCoef;
            uint8 noMovThreshold;
        #endif /* (0u != (Capsense_XY_ADAPTIVE_IIR_MASK & Capsense_TRACKPAD_GEST_POS_FILTERS_MASK)) */

    }Capsense_PACKED_STRUCT_GNUC_ARMCC Capsense_INBOX_TMG_CONFIG;
#endif /* ((0u != Capsense_TRACKPAD_GEST_POS_FILTERS_MASK) || (0u != Capsense_IS_ANY_GESTURES_EN)) */
#endif /* (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */

/* Outbox structure definition */
typedef Capsense_PACKED_STRUCT_IAR struct
{
    uint8 onMask[Capsense_TOTAL_SENSOR_MASK];

    #if (0u != Capsense_TOTAL_CENTROIDS_COUNT)
        uint16 centroidPosition[Capsense_TOTAL_CENTROIDS_COUNT];
    #endif  /* (0u != Capsense_TOTAL_CENTROIDS_COUNT) */

    #if (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT)
        uint8 fingersNum[Capsense_TOTAL_TRACKPAD_GESTURES_COUNT];
        uint8 gestureID[Capsense_TOTAL_TRACKPAD_GESTURES_COUNT];
        #if((0u != Capsense_ST_SCROLL_GEST_ENABLED) || (0u != Capsense_DT_SCROLL_GEST_ENABLED))
            uint8 scrollCnt;
        #endif /* ((0u != Capsense_ST_SCROLL_GEST_ENABLED) || (0u != Capsense_DT_SCROLL_GEST_ENABLED)) */
    #endif /* (0u != Capsense_TOUCH_PADS_GESTURES_COUNT) */

    #if (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT)
        uint8 mbPosition[Capsense_TOTAL_MATRIX_BUTTONS_COUNT * 2u];
    #endif /* (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT) */

    #if (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)
        uint8 fingerThreshold[Capsense_THRESHOLD_TBL_SIZE];
        uint8 noiseThreshold[Capsense_THRESHOLD_TBL_SIZE];
        uint8 hysteresis[Capsense_WIDGET_PARAM_TBL_SIZE];
    #endif  /* (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */

    uint8 sensorSignal[Capsense_TOTAL_SENSOR_COUNT];
    uint16 rawData[Capsense_TOTAL_SENSOR_COUNT];
    uint16 baseLine[Capsense_TOTAL_SENSOR_COUNT];

    #if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
        (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL))
        uint8 noReadMessage;
    #endif /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
               (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)) */


    #if((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) || (0u != Capsense_AUTOCALIBRATION_ENABLE))
        uint8 modulationIDAC[Capsense_TOTAL_SENSOR_COUNT];
        #if (Capsense_IDAC_CNT == 2u)
            uint8 compensationIDAC[Capsense_TOTAL_SENSOR_COUNT];
        #endif /* (Capsense_IDAC_CNT == 2u) */
    #endif /* ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) || (0u != Capsense_AUTOCALIBRATION_ENABLE)) */

    #if (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO)
        uint8 analogSwitchDivider[Capsense_TOTAL_SENSOR_COUNT];
        uint8 modulatorDivider[Capsense_TOTAL_SENSOR_COUNT];

        uint8 scanResolution[Capsense_RESOLUTIONS_TBL_SIZE];
    #endif  /* (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */

    uint16 checkSum;

} Capsense_PACKED_STRUCT_GNUC_ARMCC Capsense_OUTBOX;


/* Inbox structure definition */
#if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
    (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL))
    typedef Capsense_PACKED_STRUCT_IAR struct
    {
        uint8 sensorIndex;

        #if(Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)
            Capsense_INBOX_CSD_CONFIG Capsense_inboxCsdCfg;
        #endif /* (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL) */

        #if(Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
        #if((0u != Capsense_TRACKPAD_GEST_POS_FILTERS_MASK) || (0u != Capsense_IS_ANY_GESTURES_EN))
            Capsense_INBOX_TMG_CONFIG Capsense_inboxTmgCfg;
        #endif /* ((0u != Capsense_TRACKPAD_GEST_POS_FILTERS_MASK) || (0u != Capsense_IS_ANY_GESTURES_EN)) */
        #endif /* (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */
    } Capsense_PACKED_STRUCT_GNUC_ARMCC Capsense_INBOX;

#endif  /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
            (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)) */


/* Mailbox structure definition */
typedef Capsense_PACKED_STRUCT_IAR struct
{
    uint8   type;               /* Must be first byte with values ranging from 0-119 */
    uint16  size;               /* Must be size of this data structure. Range between 0-127 */
    Capsense_OUTBOX  outbox;
    #if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
        (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL))
        Capsense_INBOX inbox;
    #endif  /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
                (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)) */
}Capsense_PACKED_STRUCT_GNUC_ARMCC Capsense_MAILBOX;


/* Mailboxes structure definition */
typedef Capsense_PACKED_STRUCT_IAR struct
{
    uint8   numMailBoxes;       /* This must always be first. Represents # of mailboxes */
    Capsense_MAILBOX    csdMailbox;
}Capsense_PACKED_STRUCT_GNUC_ARMCC Capsense_MAILBOXES;


extern uint16 Capsense_TMG_edgeSwipeCompleteTimeout;

/***************************************
*        Function Prototypes
***************************************/
Capsense_NO_STRICT_VOLATILE void Capsense_InitMailbox(volatile Capsense_MAILBOX *mbx);
Capsense_NO_STRICT_VOLATILE void Capsense_PostMessage(volatile Capsense_MAILBOX *mbx);

#if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
     (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL))
    Capsense_NO_STRICT_VOLATILE void Capsense_ReadMessage(volatile Capsense_MAILBOX *mbx);
#endif  /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
            (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)) */


#endif  /* (CY_CAPSENSE_CSD_MBX_Capsense_H) */


/* [] END OF FILE */
