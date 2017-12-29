/*******************************************************************************
* File Name: Capsense_MBX.c
* Version 2.60
*
* Description:
*  This file provides the source code of Tuner communication APIs for the
*  CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Capsense_MBX.h"
#include "cyapicallbacks.h"

/*******************************************************************************
*  Place your includes, defines and code here
********************************************************************************/
/* `#START Capsense_MBX` */

/* `#END` */

#if (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)
    /*******************************************************************************
    * Function Name: Capsense_InitCsdParams
    ********************************************************************************
    *
    * Summary:
    *  Configures the CSD parameters to match the parameters in the inbox.
    *  Used only in the manual tuning mode to apply new scanning parameters from the Tuner
    *  GUI.
    *
    * Parameters:
    *  inbox:  Pointer to Inbox structure in RAM.
    *
    * Return:
    *  None. Contents of the structure are not modified.
    *
    * Global Variables:
    *  Capsense_modulationIDAC[]      - stores modulation IDAC values.
    *  Capsense_compensationIDAC[]    - stores compensation IDAC values.
    *  Capsense_widgetResolution[]    - used to store the scan resolution values for each widget.
    *  Capsense_senseClkDividerVal[]  - used to store the sense clock divider values for each widget.
    *  Capsense_sampleClkDividerVal[] - used to store the sample clock divider values for each widget.
    *  Capsense_fingerThreshold[]     - used to store the finger threshold values for each widget.
    *  Capsense_noiseThreshold[]      - used to store the noise threshold values for each widget.
    *  Capsense_hysteresis[]          - used to store the hysteresis values for each widget.
    *  Capsense_debounce[]            - used to store the debounce values for each widget.
    *
    *******************************************************************************/
    static void Capsense_InitCsdParams(volatile const Capsense_INBOX *inbox);
    static void Capsense_InitCsdParams(volatile const Capsense_INBOX *inbox)
    {
        /* Define widget sensor belongs to */
        uint8 sensor = inbox->sensorIndex;
        uint8 widget = Capsense_widgetNumber[sensor];

        /* Scanning parameters */
        #if(0u == Capsense_AUTOCALIBRATION_ENABLE)
            Capsense_modulationIDAC[sensor] = inbox->Capsense_inboxCsdCfg.modulatorIDAC;

            #if (Capsense_IDAC_CNT == 2u)
                Capsense_compensationIDAC[sensor] = inbox->Capsense_inboxCsdCfg.compensationIDAC;
            #endif /* ( Capsense_IDAC_CNT == 2u ) */
        #endif /* (0u != Capsense_AUTOCALIBRATION_ENABLE) */

        Capsense_SetIDACRange((uint32)inbox->Capsense_inboxCsdCfg.idacRange);

        Capsense_widgetResolution[widget] =
        (uint32)(~(Capsense_RESOLUTION_16_BITS << inbox->Capsense_inboxCsdCfg.scanResolution));

        Capsense_widgetResolution[widget] &= Capsense_RESOLUTION_16_BITS;

        #if (0u != Capsense_MULTIPLE_FREQUENCY_SET )
            Capsense_senseClkDividerVal[sensor] = inbox->Capsense_inboxCsdCfg.analogSwitchDivider;
            Capsense_sampleClkDividerVal[sensor] = inbox->Capsense_inboxCsdCfg.modulatorDivider;
        #else
            Capsense_senseClkDividerVal = inbox->Capsense_inboxCsdCfg.analogSwitchDivider;
            Capsense_sampleClkDividerVal = inbox->Capsense_inboxCsdCfg.modulatorDivider;
        #endif /* (0u != Capsense_MULTIPLE_FREQUENCY_SET ) */

        #if(0u != Capsense_CSHL_API_GENERATE)
            #if (0u != Capsense_TOTAL_GENERICS_COUNT)
                /* Exclude generic widget */
                if(widget < Capsense_END_OF_WIDGETS_INDEX)
                {
            #endif  /* 0u != Capsense_TOTAL_GENERICS_COUNT */

                /* High level parameters */
                Capsense_fingerThreshold[widget] = inbox->Capsense_inboxCsdCfg.fingerThreshold;
                Capsense_noiseThreshold[widget]  = inbox->Capsense_inboxCsdCfg.noiseThreshold;

                Capsense_hysteresis[widget] = inbox->Capsense_inboxCsdCfg.hysteresis;
                Capsense_debounce[widget]   = inbox->Capsense_inboxCsdCfg.debounce;

                #if(Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u)
                    Capsense_centroidMult[widget] = (uint16)((inbox->Capsense_inboxCsdCfg.apiResolution *
                                                            (uint32)Capsense_RES_MULT) / (uint32)Capsense_numberOfSensors[widget]);

                #endif /* (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT > 0u) */

            #if (0u != Capsense_TOTAL_GENERICS_COUNT)
                /* Exclude generic widget */
                }
            #endif  /* 0u != Capsense_TOTAL_GENERICS_COUNT */

            /* Re-Init baseline */
            Capsense_InitializeAllBaselines();
        #endif /* (0u != Capsense_CSHL_API_GENERATE) */
    }
#endif  /* (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL) */


    /*******************************************************************************
    * Function Name: Capsense_InitGesturesParams
    ********************************************************************************
    *
    * Summary:
    *  Configures the parameters of gestures to match the parameters in the inbox.
    *  Used only in the manual and auto tuning mode or if the gestures support is enabled.
    *  Intended apply new scanning parameters from the Tuner GUI.
    *
    * Parameters:
    *  inbox:  Pointer to Inbox structure in RAM.
    *
    * Return:
    *  None. Contents of the structure are not modified.
    *
    *******************************************************************************/
#if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT))
    static void Capsense_InitGesturesParams(volatile const Capsense_INBOX *inbox);
    static void Capsense_InitGesturesParams(volatile const Capsense_INBOX *inbox)
    {
        #if(0u != Capsense_IS_ANY_GESTURES_EN)
            #if(0u != Capsense_CLICK_GEST_ENABLED)
                Capsense_gesturesConfig.clickRadiusX            = (uint8)inbox->Capsense_inboxTmgCfg.clickRadiusX;
                Capsense_gesturesConfig.clickRadiusY            = (uint8)inbox->Capsense_inboxTmgCfg.clickRadiusY;
                Capsense_gesturesConfig.doubleClickRadius       = (uint8)inbox->Capsense_inboxTmgCfg.doubleClickRadius;
                Capsense_gesturesConfig.STDoubleClickTimeoutMax = inbox->Capsense_inboxTmgCfg.stDoubleClickTimeoutMax;
                Capsense_gesturesConfig.STDoubleClickTimeoutMin = inbox->Capsense_inboxTmgCfg.stDoubleClickTimeoutMin;
                Capsense_gesturesConfig.STClickTimeoutMax       = inbox->Capsense_inboxTmgCfg.stClickTimeoutMax;
                Capsense_gesturesConfig.STClickTimeoutMin       = inbox->Capsense_inboxTmgCfg.stClickTimeoutMin;
                Capsense_gesturesConfig.DTClickTimeoutMax       = inbox->Capsense_inboxTmgCfg.dtClickTimeoutMax;
                Capsense_gesturesConfig.DTClickTimeoutMin       = inbox->Capsense_inboxTmgCfg.dtClickTimeoutMin;
            #endif /* (0u != Capsense_CLICK_GEST_ENABLED) */

            #if(0u != Capsense_ZOOM_GEST_ENABLED)
                Capsense_gesturesConfig.dtScrollToZoomDebounce  = inbox->Capsense_inboxTmgCfg.dtPanToZoomDebounce;
                Capsense_gesturesConfig.DTZoomDebounce          = inbox->Capsense_inboxTmgCfg.dtZoomDebounce;
                Capsense_gesturesConfig.zoomActiveDistanceX     = inbox->Capsense_inboxTmgCfg.zoomActiveDistanceX;
                Capsense_gesturesConfig.zoomActiveDistanceY     = inbox->Capsense_inboxTmgCfg.zoomActiveDistanceY;
            #endif /* (0u != Capsense_ZOOM_GEST_ENABLED) */

            #if(0u != Capsense_ST_SCROLL_GEST_ENABLED)
                Capsense_gesturesConfig.stScrollDebounce        = inbox->Capsense_inboxTmgCfg.stScrDebounce;

                Capsense_gesturesConfig.stScrollThreshold1X     = inbox->Capsense_inboxTmgCfg.stScrThreshold1X;
                Capsense_gesturesConfig.stScrollThreshold2X     = inbox->Capsense_inboxTmgCfg.stScrThreshold2X;
                Capsense_gesturesConfig.stScrollThreshold3X     = inbox->Capsense_inboxTmgCfg.stScrThreshold3X;
                Capsense_gesturesConfig.stScrollThreshold4X     = inbox->Capsense_inboxTmgCfg.stScrThreshold4X;
                Capsense_gesturesConfig.stScrollThreshold1Y     = inbox->Capsense_inboxTmgCfg.stScrThreshold1Y;
                Capsense_gesturesConfig.stScrollThreshold2Y     = inbox->Capsense_inboxTmgCfg.stScrThreshold2Y;
                Capsense_gesturesConfig.stScrollThreshold3Y     = inbox->Capsense_inboxTmgCfg.stScrThreshold3Y;
                Capsense_gesturesConfig.stScrollThreshold4Y     = inbox->Capsense_inboxTmgCfg.stScrThreshold4Y;
                Capsense_gesturesConfig.stScrollStep1           = inbox->Capsense_inboxTmgCfg.stScrStep1;
                Capsense_gesturesConfig.stScrollStep2           = inbox->Capsense_inboxTmgCfg.stScrStep2;
                Capsense_gesturesConfig.stScrollStep3           = inbox->Capsense_inboxTmgCfg.stScrStep3;
                Capsense_gesturesConfig.stScrollStep4           = inbox->Capsense_inboxTmgCfg.stScrStep4;
                Capsense_gesturesConfig.stInScrActiveDistanceX  = inbox->Capsense_inboxTmgCfg.stInScrActiveDistanceX;
                Capsense_gesturesConfig.stInScrActiveDistanceY  = inbox->Capsense_inboxTmgCfg.stInScrActiveDistanceY;
                Capsense_gesturesConfig.stInScrCountLevel       = inbox->Capsense_inboxTmgCfg.stInScrCountLevel;
            #endif /* (0u != Capsense_ST_SCROLL_GEST_ENABLED) */

            #if(0u != Capsense_DT_SCROLL_GEST_ENABLED)
                Capsense_gesturesConfig.dtScrollDebounce        = inbox->Capsense_inboxTmgCfg.dtScrDebounce;
                Capsense_gesturesConfig.dtScrollThreshold1X     = inbox->Capsense_inboxTmgCfg.dtScrThreshold1X;
                Capsense_gesturesConfig.dtScrollThreshold2X     = inbox->Capsense_inboxTmgCfg.dtScrThreshold2X;
                Capsense_gesturesConfig.dtScrollThreshold3X     = inbox->Capsense_inboxTmgCfg.dtScrThreshold3X;
                Capsense_gesturesConfig.dtScrollThreshold4X     = inbox->Capsense_inboxTmgCfg.dtScrThreshold4X;
                Capsense_gesturesConfig.dtScrollThreshold1Y     = inbox->Capsense_inboxTmgCfg.dtScrThreshold1Y;
                Capsense_gesturesConfig.dtScrollThreshold2Y     = inbox->Capsense_inboxTmgCfg.dtScrThreshold2Y;
                Capsense_gesturesConfig.dtScrollThreshold3Y     = inbox->Capsense_inboxTmgCfg.dtScrThreshold3Y;
                Capsense_gesturesConfig.dtScrollThreshold4Y     = inbox->Capsense_inboxTmgCfg.dtScrThreshold4Y;
                Capsense_gesturesConfig.dtScrollStep1           = inbox->Capsense_inboxTmgCfg.dtScrStep1;
                Capsense_gesturesConfig.dtScrollStep2           = inbox->Capsense_inboxTmgCfg.dtScrStep2;
                Capsense_gesturesConfig.dtScrollStep3           = inbox->Capsense_inboxTmgCfg.dtScrStep3;
                Capsense_gesturesConfig.dtScrollStep4           = inbox->Capsense_inboxTmgCfg.dtScrStep4;
                Capsense_gesturesConfig.dtInScrActiveDistanceX  = inbox->Capsense_inboxTmgCfg.dtInScrActiveDistanceX;
                Capsense_gesturesConfig.dtInScrActiveDistanceY  = inbox->Capsense_inboxTmgCfg.dtInScrActiveDistanceY;
                Capsense_gesturesConfig.dtInScrCountLevel       = inbox->Capsense_inboxTmgCfg.dtInScrCountLevel;
            #endif /* (0u != Capsense_DT_SCROLL_GEST_ENABLED) */

            #if(0u != Capsense_FLICK_GEST_ENABLED)
                Capsense_gesturesConfig.flickSampleTime         = inbox->Capsense_inboxTmgCfg.flickSampleTime;
                Capsense_gesturesConfig.flickActiveDistanceX    = inbox->Capsense_inboxTmgCfg.flickActiveDistanceX;
                Capsense_gesturesConfig.flickActiveDistanceY    = inbox->Capsense_inboxTmgCfg.flickActiveDistanceY;
            #endif /* (0u != Capsense_FLICK_GEST_ENABLED) */

            #if(0u != Capsense_ROTATE_GEST_ENABLED)
                Capsense_gesturesConfig.rotateDebounce          = inbox->Capsense_inboxTmgCfg.rotateDebounce;
            #endif /* (0u != Capsense_ROTATE_GEST_ENABLED) */

            #if(0u != Capsense_EDGE_SWIPE_GEST_ENABLED)
                Capsense_gesturesConfig.edgeSwipeActiveDistance = inbox->Capsense_inboxTmgCfg.edgeSwipeActiveDistance;
                Capsense_gesturesConfig.bottomAngleThreshold    = inbox->Capsense_inboxTmgCfg.bottomAngleThreshold;
                Capsense_gesturesConfig.edgeSwipeTimeout        = inbox->Capsense_inboxTmgCfg.edgeSwipeTime;
                Capsense_TMG_edgeSwipeCompleteTimeout           = inbox->Capsense_inboxTmgCfg.edgeSwipeCompleteTimeout;
                Capsense_gesturesConfig.topAngleThreshold       = inbox->Capsense_inboxTmgCfg.topAngleThreshold;
                Capsense_gesturesConfig.widthOfDisambiguation   = inbox->Capsense_inboxTmgCfg.widthOfDisambiguation;
            #endif /* (0u != Capsense_EDGE_SWIPE_GEST_ENABLED) */

            Capsense_TMG_InitGestures(&Capsense_gesturesConfig);
        #endif /* (0u != Capsense_IS_ANY_GESTURES_EN) */

        #if(0u != Capsense_TP_GESTURE_POS_FILTERS_MASK)
            Capsense_posFiltersMask[Capsense_TRACKPAD__TPG] = inbox->Capsense_inboxTmgCfg.filtersMask;
        #endif /* (0u != Capsense_TP_GESTURE_POS_FILTERS_MASK) */

        #if(0u != (Capsense_XY_ADAPTIVE_IIR_MASK & Capsense_TP_GESTURE_POS_FILTERS_MASK))
            Capsense_adpFltOptions.divVal       = inbox->Capsense_inboxTmgCfg.filterDivisor;
            Capsense_adpFltOptions.largeMovTh   = inbox->Capsense_inboxTmgCfg.largeMovThreshold;
            Capsense_adpFltOptions.littleMovTh  = inbox->Capsense_inboxTmgCfg.littleMovThreshold;
            Capsense_adpFltOptions.maxK     = inbox->Capsense_inboxTmgCfg.maxFilterCoef;
            Capsense_adpFltOptions.minK     = inbox->Capsense_inboxTmgCfg.minFilterCoef;
            Capsense_adpFltOptions.noMovTh      = inbox->Capsense_inboxTmgCfg.noMovThreshold;
        #endif /* (0u != (Capsense_XY_ADAPTIVE_IIR_MASK & Capsense_TP_GESTURE_POS_FILTERS_MASK)) */
    }
#endif /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT)) */


/*******************************************************************************
* Function Name: Capsense_InitMailbox
********************************************************************************
*
* Summary:
*  Initializes parameters of the mailbox structure.
*  Sets the type and size of the mailbox structure.
*  Sets a check sum to check by the Tuner GUI and noReadMsg flag to indicate that this
*  is the first communication packet.
*
* Parameters:
*  mbx:  Pointer to Mailbox structure in RAM.
*
* Return:
*  None. Modifies the contents of mbx and mbx->outbox.
*
* Global Variables:
*  None
*
*******************************************************************************/
void Capsense_InitMailbox(volatile Capsense_MAILBOX *mbx)
{
    /* Restore TYPE_ID (clear busy flag) to indicate "action complete" */
    mbx->type = Capsense_TYPE_ID;
    /* Restore default value - clear "have_msg" */
    mbx->size = sizeof(Capsense_MAILBOX);

    /* Additional fields for async and reset handling */
    #if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
        (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL))
        mbx->outbox.noReadMessage = 1u;
    #endif  /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
                (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)) */

    mbx->outbox.checkSum = (uint16)(Capsense_CHECK_SUM);
}


/*******************************************************************************
* Function Name:  Capsense_PostMessage
********************************************************************************
*
* Summary:
*  This blocking function waits while the Tuner GUI reports that the mailbox content
*  could be modified (clears Capsense_BUSY_FLAG). Then loads the report
*  data to the outbox and sets a busy flag.
*  In the manual tuning mode the report data is:
*    - raw data, baseline, signal;
*  In the auto tuning mode the added data is:
*    - fingerThreshold;
*    - noiseThreshold;
*    - scanResolution;
*    - idacValue;
*    - prescaler.
*
* Parameters:
*  mbx:  Pointer to Mailbox structure in RAM.
*
* Return:
*  None. Modifies the contents of mbx->outbox.
*
* Global Variables:
*  None
*
*******************************************************************************/
void Capsense_PostMessage(volatile Capsense_MAILBOX *mbx)
{
    uint8 i;

    #if ( Capsense_TUNING_METHOD == Capsense__TUNING_AUTO )
        uint8 tmpResolutionIndex;
        uint16 tmpResolutionValue;
    #endif /* ( Capsense_TUNING_METHOD == Capsense__TUNING_AUTO ) */

    /* Check busy flag */
    while (mbx->type != Capsense_TYPE_ID){}

    /* Copy scan parameters */
    #if((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) || (0u != Capsense_AUTOCALIBRATION_ENABLE))
        /* Copy tuned idac values */
        for(i = 0u; i < Capsense_TOTAL_SENSOR_COUNT; i++)
        {
            mbx->outbox.modulationIDAC[i] = Capsense_modulationIDAC[i];
            #if (Capsense_IDAC_CNT == 2u)
                mbx->outbox.compensationIDAC[i] = Capsense_compensationIDAC[i];
            #endif /* (Capsense_IDAC_CNT == 2u) */
        }
    #endif /* ((Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) || (0u != Capsense_AUTOCALIBRATION_ENABLE)) */

    #if ( Capsense_TUNING_METHOD == Capsense__TUNING_AUTO )
        for(i = 0u; i < Capsense_TOTAL_SCANSLOT_COUNT; i++)
        {
            mbx->outbox.analogSwitchDivider[i] = Capsense_senseClkDividerVal[i];
            mbx->outbox.modulatorDivider[i] = Capsense_sampleClkDividerVal[i];
        }

        /* Widget resolution, take to account TP and MB */
        for(i = 0u; i < Capsense_WIDGET_RESOLUTION_PARAMETERS_COUNT; i++)
        {
            tmpResolutionValue = (uint16)(Capsense_widgetResolution[i] >> Capsense_MSB_RESOLUTION_OFFSET);
            tmpResolutionIndex = 0u;

            while(0u != tmpResolutionValue)
            {
                tmpResolutionIndex++;
                tmpResolutionValue >>= 1u;
            }

            mbx->outbox.scanResolution[i] = tmpResolutionIndex;
        }

        #if(0u != Capsense_CSHL_API_GENERATE)
            /* Parameters are changed in run time */
            for(i = 0u; i < Capsense_WIDGET_CSHL_PARAMETERS_COUNT; i++)
            {
                mbx->outbox.fingerThreshold[i] = Capsense_fingerThreshold[i];
                mbx->outbox.noiseThreshold[i]  = Capsense_noiseThreshold[i];
                mbx->outbox.hysteresis[i]      = Capsense_hysteresis[i];
            }
        #endif /* (0u != Capsense_CSHL_API_GENERATE) */

    #endif /* (Capsense_TUNING_METHOD == Capsense__TUNING_AUTO) */


    /* Copy all data - Raw, Base, Signal, OnMask */
    for(i = 0u; i < Capsense_TOTAL_SENSOR_COUNT; i++)
    {
        mbx->outbox.rawData[i]  = Capsense_sensorRaw[i];
        #if(0u != Capsense_CSHL_API_GENERATE)
            mbx->outbox.baseLine[i] = Capsense_sensorBaseline[i];
            #if (Capsense_SIGNAL_SIZE == Capsense_SIGNAL_SIZE_UINT8)
                mbx->outbox.sensorSignal[i]   = (uint8)Capsense_sensorSignal[i];
            #else
                mbx->outbox.sensorSignal[i]   = (uint16)Capsense_sensorSignal[i];
            #endif  /* (Capsense_SIGNAL_SIZE == Capsense_SIGNAL_SIZE_UINT8) */
        #endif /* (0u != Capsense_CSHL_API_GENERATE) */
    }

    /* Set busy flag */
    mbx->type = Capsense_BUSY_FLAG;
}


#if ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
     (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL))

    /*******************************************************************************
    * Function Name: Capsense_ReadMessage
    ********************************************************************************
    *
    * Summary:
    *  If Capsense_HAVE_MSG is found in the mailbox, the function initializes
    *   the component with parameters which are found in the inbox.
    *   Signal is DONE by overwriting the value in the mailbox size field.
    *  Only available in the manual tuning mode.
    *
    * Parameters:
    *  mbx:  Pointer to Mailbox structure in RAM.
    *
    * Return:
    *  None. Modifies the contents of mbx.
    *
    * Global Variables:
    *  None
    *
    *******************************************************************************/
    void Capsense_ReadMessage(volatile Capsense_MAILBOX *mbx)
    {
        volatile Capsense_INBOX *tmpInbox;

        /* Do we have a message waiting? */
        if ((mbx->size) == Capsense_HAVE_MSG)
        {
            tmpInbox = &(mbx->inbox);

            #if(Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)
                if(tmpInbox->sensorIndex != Capsense_MAX_UINT_8)
                {
                    Capsense_InitCsdParams(tmpInbox);
                }
            #endif /* (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL) */

            #if(0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT)
                if(tmpInbox->sensorIndex == Capsense_MAX_UINT_8)
                {
                    Capsense_InitGesturesParams(tmpInbox);
                }
            #endif /* (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) */

            /* Notify host/tuner that we have consumed message */
            mbx->size = sizeof(Capsense_MAILBOX);

            /* Default settings were changed */
            mbx->outbox.noReadMessage = 0u;

            /* `#START Capsense_MBX_READ_MSG` */

            /* `#END` */

            #ifdef Capsense_READ_MESSAGE_MBX_READ_MSG_CALLBACK
                Capsense_ReadMessage_MBX_READ_MSG_Callback();
            #endif /* Capsense_READ_MESSAGE_MBX_READ_MSG_CALLBACK */
        }
    }
#endif  /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) && (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
            (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)) */


/* [] END OF FILE */
