/*******************************************************************************
* File Name: Capsense_TunerHelper.c
* Version 2.60
*
* Description:
*  This file provides the source code of the Tuner helper APIs for the CapSense CSD
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

#include "Capsense_TunerHelper.h"
#include "cyapicallbacks.h"

#if (Capsense_TUNER_API_GENERATE)
    #include "SCB.h"
    #include "SCB_EZI2C.h"
    volatile Capsense_MAILBOXES Capsense_mailboxesComm;
#endif  /* (Capsense_TUNER_API_GENERATE) */

/* `#START Capsense_TunerHelper_HEADER` */

/* `#END` */

/*******************************************************************************
* Function Name: Capsense_TunerStart
********************************************************************************
*
* Summary:
*  Initializes the CapSense CSD component and EzI2C communication component to use
*  a mailbox data structure for communication with the Tuner GUI.
*  Start the scanning, after initialization is complete.
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
*******************************************************************************/
void Capsense_TunerStart(void)
{
    #if (0u != Capsense_TUNER_API_GENERATE)

        /* Init mbx and quick check */
        Capsense_InitMailbox(&Capsense_mailboxesComm.csdMailbox);
        Capsense_mailboxesComm.numMailBoxes = Capsense_DEFAULT_MAILBOXES_NUMBER;

        /* Start CapSense and baselines */
        Capsense_Start();

        /* Initialize baselines */
        #if(0u != Capsense_CSHL_API_GENERATE)
            Capsense_InitializeEnabledBaselines();
        #endif /* (0u != Capsense_CSHL_API_GENERATE) */

        /* Start EzI2C, clears buf pointers */
        SCB_Start();

        /* Setup EzI2C buffers */
        SCB_EzI2CSetBuffer1(sizeof(Capsense_mailboxesComm), sizeof(Capsense_mailboxesComm),
                                                   (volatile uint8 *) &Capsense_mailboxesComm);

        /* Starts scan all enabled sensors */
        Capsense_ScanEnabledWidgets();

    #endif  /* (0u != Capsense_TUNER_API_GENERATE) */
}


/*******************************************************************************
* Function Name: Capsense_TunerComm
********************************************************************************
*
* Summary:
*  This function is blocking. It waits till scanning loop is completed and applies
*  the new parameters from the Tuner GUI if available (the manual tuning mode only). Updates
*  the enabled baselines and state of widgets. Waits while the Tuner GUI reports that
*  the mailbox content  could be modified. Then loads the report data into the outbox
*  and sets a busy flag. Starts a new scanning loop.
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
*******************************************************************************/
void Capsense_TunerComm(void)
{
    #if (0u != Capsense_TUNER_API_GENERATE)
        if (0u == Capsense_IsBusy())
        {
            /* Apply new settings */
            #if((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) &&\
                (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
                (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL))

                Capsense_ReadMessage(&Capsense_mailboxesComm.csdMailbox);

            #endif  /* ((Capsense_TUNING_METHOD != Capsense__TUNING_NONE) &&\
                        (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
                        (Capsense_TUNING_METHOD == Capsense__TUNING_MANUAL)) */

            #if(0u != Capsense_CSHL_API_GENERATE)
                /* Update all baselines and process all widgets */
                Capsense_UpdateEnabledBaselines();
                Capsense_ProcessAllWidgets(&Capsense_mailboxesComm.csdMailbox.outbox);
            #endif /* (0u != Capsense_CSHL_API_GENERATE) */

            Capsense_PostMessage(&Capsense_mailboxesComm.csdMailbox);

            /* Enable EZI2C interrupts, after scan complete */
            SCB_EnableInt();

            while((Capsense_mailboxesComm.csdMailbox.type != Capsense_TYPE_ID) ||
                  (0u != (SCB_EzI2CGetActivity() & SCB_EZI2C_STATUS_BUSY))){}

            /* Disable EZI2C interrupts, while scanning */
            SCB_DisableInt();

            /* Start scan all sensors */
            Capsense_ScanEnabledWidgets();
        }
    #endif /* (0u != Capsense_TUNER_API_GENERATE) */
}


#if ((0u != Capsense_TUNER_API_GENERATE) && (0u != Capsense_CSHL_API_GENERATE))
    /*******************************************************************************
    * Function Name: Capsense_ProcessAllWidgets
    ********************************************************************************
    *
    * Summary:
    *  Calls the required functions to update all the widgets' state:
    *   - Capsense_GetCentroidPos() - calls only if linear sliders are
    *     available.
    *   - Capsense_GetRadialCentroidPos() - calls only if the  radial slider is
    *     available.
    *   - Capsense_GetTouchCentroidPos() - calls only if the  touchpad slider
    *     available.
    *   - Capsense_CheckIsAnyWidgetActive();
    *  The results of operations are copied to OUTBOX.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Capsense_OUTBOX outbox - the structure which is used as an output
    *  buffer to report data to the Tuner GUI.
    *  Update fields:
    *    - position[];
    *    - OnMask[];
    *
    * Global Variables:
    *  None
    *
    *******************************************************************************/
    void Capsense_ProcessAllWidgets(volatile Capsense_OUTBOX *outbox)
    {
        uint8  i = 0u;

        #if (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT)
            uint8 mbPositionBuf[Capsense_MB_POSITION_BUFFER_SIZE];
        #endif /* (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT) */

        #if (0u != Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT)
            uint16 centroidPosBuf[Capsense_CENTROID_POSITION_BUFFER_SIZE];
        #endif  /* (0u != Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT) */

        #if(0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT)
            Capsense_POSITION_STRUCT trackpadPosBuf[Capsense_CENTROID_POSITION_BUFFER_SIZE];
        #endif  /* (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) */

        #if ( (0u != Capsense_TOTAL_RADIAL_SLIDERS_COUNT) || (0u != Capsense_TOTAL_TOUCH_PADS_COUNT) || \
              (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT))
            uint8 widgetCnt;
        #endif  /* ((0u != Capsense_TOTAL_RADIAL_SLIDERS_COUNT) || (0u != Capsense_TOTAL_TOUCH_PADS_COUNT)) ||
                *   (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT)
                */

        /* Calculate widget with centroids */
        #if (0u != Capsense_TOTAL_LINEAR_SLIDERS_COUNT)
            for(; i < Capsense_TOTAL_LINEAR_SLIDERS_COUNT; i++)
            {
                outbox->centroidPosition[i] = (uint16)Capsense_GetCentroidPos((uint32)i);
            }
        #endif /* (0u != Capsense_TOTAL_LINEAR_SLIDERS_COUNT) */

        #if (0u != Capsense_TOTAL_RADIAL_SLIDERS_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + Capsense_TOTAL_RADIAL_SLIDERS_COUNT); i++)
            {
                outbox->centroidPosition[i] = (uint16)Capsense_GetRadialCentroidPos((uint32)i);
            }
        #endif /* (0u != Capsense_TOTAL_RADIAL_SLIDERS_COUNT) */

        #if (0u != Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + (Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u)); i=i+2u)
            {
                if(Capsense_GetTouchCentroidPos((uint32)i, centroidPosBuf) == 0u)
                {
                    outbox->centroidPosition[i + Capsense_FIRST_FINGER_X_INDEX] = Capsense_MAX_UINT_16;
                    outbox->centroidPosition[i + Capsense_FIRST_FINGER_Y_INDEX] = Capsense_MAX_UINT_16;
                }
                else
                {
                    outbox->centroidPosition[i + Capsense_FIRST_FINGER_X_INDEX] =
                    (uint16) centroidPosBuf[Capsense_FIRST_FINGER_X_INDEX];

                    outbox->centroidPosition[i + Capsense_FIRST_FINGER_Y_INDEX] =
                    (uint16) centroidPosBuf[Capsense_FIRST_FINGER_Y_INDEX];
                }
            }
        #endif /* (0u != Capsense_TOTAL_TOUCH_PADS_BASIC_COUNT) */

        #if (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + (Capsense_TOTAL_TRACKPAD_GESTURES_COUNT * 4u)); i=i+4u)
            {
                outbox->fingersNum[Capsense_DEFAULT_TRACKPAD_INDEX] =
                (uint8)Capsense_GetDoubleTouchCentroidPos(trackpadPosBuf);

                outbox->gestureID[Capsense_DEFAULT_TRACKPAD_INDEX] =
                (uint8)Capsense_DecodeAllGestures((uint32)outbox->fingersNum[Capsense_DEFAULT_TRACKPAD_INDEX], trackpadPosBuf);

                #if((0u != Capsense_ST_SCROLL_GEST_ENABLED) || (0u != Capsense_DT_SCROLL_GEST_ENABLED))
                    /*
                    Report Scroll count only for scroll and Inertial Scroll gestures.
                    For all other gestures report scroll count as zero to tuner.
                     */
                    if (((outbox->gestureID[Capsense_DEFAULT_TRACKPAD_INDEX] & Capsense_GESTURE_TYPE_MASK) == Capsense_NON_INERTIAL_SCROLL) ||
                        ((outbox->gestureID[Capsense_DEFAULT_TRACKPAD_INDEX] & Capsense_GESTURE_TYPE_MASK) == Capsense_INERTIAL_SCROLL))
                    {
                        outbox->scrollCnt = Capsense_GetScrollCnt();
                    }
                    else
                    {
                        outbox->scrollCnt = 0u;
                    }
                #endif /* ((0u != Capsense_ST_SCROLL_GEST_ENABLED) || (0u != Capsense_DT_SCROLL_GEST_ENABLED)) */

                outbox->centroidPosition[i + Capsense_FIRST_FINGER_X_INDEX] = Capsense_MAX_UINT_16;
                outbox->centroidPosition[i + Capsense_FIRST_FINGER_Y_INDEX] = Capsense_MAX_UINT_16;
                outbox->centroidPosition[i + Capsense_SECOND_FINGER_X_INDEX] = Capsense_MAX_UINT_16;
                outbox->centroidPosition[i + Capsense_SECOND_FINGER_Y_INDEX] = Capsense_MAX_UINT_16;

                if(0u != outbox->fingersNum[Capsense_DEFAULT_TRACKPAD_INDEX])
                {
                    outbox->centroidPosition[i + Capsense_FIRST_FINGER_X_INDEX] =
                    (uint16) trackpadPosBuf[Capsense_FIRST_FINGER_INDEX].x;

                    outbox->centroidPosition[i + Capsense_FIRST_FINGER_Y_INDEX] =
                    (uint16) trackpadPosBuf[Capsense_FIRST_FINGER_INDEX].y;

                    if(outbox->fingersNum[Capsense_DEFAULT_TRACKPAD_INDEX] > 1u)
                    {
                        outbox->centroidPosition[i + Capsense_SECOND_FINGER_X_INDEX] =
                        (uint16) trackpadPosBuf[Capsense_SECOND_FINGER_INDEX].x;

                        outbox->centroidPosition[i + Capsense_SECOND_FINGER_Y_INDEX] =
                        (uint16) trackpadPosBuf[Capsense_SECOND_FINGER_INDEX].y;
                    }
                }
            }
        #endif /* (0u != Capsense_TOTAL_TRACKPAD_GESTURES_COUNT) */

        #if (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT)
            i += Capsense_TOTAL_BUTTONS_COUNT;
            widgetCnt = 0u;
            for(; widgetCnt < (Capsense_TOTAL_MATRIX_BUTTONS_COUNT * 2u); widgetCnt += 2u)
            {
                if(Capsense_GetMatrixButtonPos((uint32)i, mbPositionBuf) == 0u)
                {
                    outbox->mbPosition[widgetCnt] = Capsense_MAX_UINT_8;
                    outbox->mbPosition[widgetCnt+1u] = Capsense_MAX_UINT_8;
                }
                else
                {
                    outbox->mbPosition[widgetCnt] = mbPositionBuf[0u];
                    outbox->mbPosition[widgetCnt+1u] = mbPositionBuf[1u];
                }
                i += 2u;
            }
        #endif /* (0u != Capsense_TOTAL_MATRIX_BUTTONS_COUNT) */

        #if(0u != Capsense_CSHL_API_GENERATE)
            /* Update On/Off State */
            (void)Capsense_CheckIsAnyWidgetActive();

            /* Copy OnMask */
            for(i=0u; i < Capsense_TOTAL_SENSOR_MASK_COUNT; i++)
            {
                outbox->onMask[i]  = Capsense_sensorOnMask[i];
            }
        #endif /* (0u != Capsense_CSHL_API_GENERATE) */

        /* `#START Capsense_ProcessAllWidgets_Debug` */

        /* `#END` */

        #ifdef Capsense_PROCESS_ALL_WIDGETS_CALLBACK
            Capsense_ProcessAllWidgets_Callback();
        #endif /* Capsense_PROCESS_ALL_WIDGETS_CALLBACK */
    }


#endif /* ((0u != Capsense_TUNER_API_GENERATE) && (0u != Capsense_CSHL_API_GENERATE)) */


/* [] END OF FILE */
