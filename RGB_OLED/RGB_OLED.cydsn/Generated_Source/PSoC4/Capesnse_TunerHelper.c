/*******************************************************************************
* File Name: Capesnse_TunerHelper.c
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

#include "Capesnse_TunerHelper.h"
#include "cyapicallbacks.h"

#if (Capesnse_TUNER_API_GENERATE)
    #include "SCB.h"
    #include "SCB_EZI2C.h"
    volatile Capesnse_MAILBOXES Capesnse_mailboxesComm;
#endif  /* (Capesnse_TUNER_API_GENERATE) */

/* `#START Capesnse_TunerHelper_HEADER` */

/* `#END` */

/*******************************************************************************
* Function Name: Capesnse_TunerStart
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
void Capesnse_TunerStart(void)
{
    #if (0u != Capesnse_TUNER_API_GENERATE)

        /* Init mbx and quick check */
        Capesnse_InitMailbox(&Capesnse_mailboxesComm.csdMailbox);
        Capesnse_mailboxesComm.numMailBoxes = Capesnse_DEFAULT_MAILBOXES_NUMBER;

        /* Start CapSense and baselines */
        Capesnse_Start();

        /* Initialize baselines */
        #if(0u != Capesnse_CSHL_API_GENERATE)
            Capesnse_InitializeEnabledBaselines();
        #endif /* (0u != Capesnse_CSHL_API_GENERATE) */

        /* Start EzI2C, clears buf pointers */
        SCB_Start();

        /* Setup EzI2C buffers */
        SCB_EzI2CSetBuffer1(sizeof(Capesnse_mailboxesComm), sizeof(Capesnse_mailboxesComm),
                                                   (volatile uint8 *) &Capesnse_mailboxesComm);

        /* Starts scan all enabled sensors */
        Capesnse_ScanEnabledWidgets();

    #endif  /* (0u != Capesnse_TUNER_API_GENERATE) */
}


/*******************************************************************************
* Function Name: Capesnse_TunerComm
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
void Capesnse_TunerComm(void)
{
    #if (0u != Capesnse_TUNER_API_GENERATE)
        if (0u == Capesnse_IsBusy())
        {
            /* Apply new settings */
            #if((Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) &&\
                (0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
                (Capesnse_TUNING_METHOD == Capesnse__TUNING_MANUAL))

                Capesnse_ReadMessage(&Capesnse_mailboxesComm.csdMailbox);

            #endif  /* ((Capesnse_TUNING_METHOD != Capesnse__TUNING_NONE) &&\
                        (0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT) ||\
                        (Capesnse_TUNING_METHOD == Capesnse__TUNING_MANUAL)) */

            #if(0u != Capesnse_CSHL_API_GENERATE)
                /* Update all baselines and process all widgets */
                Capesnse_UpdateEnabledBaselines();
                Capesnse_ProcessAllWidgets(&Capesnse_mailboxesComm.csdMailbox.outbox);
            #endif /* (0u != Capesnse_CSHL_API_GENERATE) */

            Capesnse_PostMessage(&Capesnse_mailboxesComm.csdMailbox);

            /* Enable EZI2C interrupts, after scan complete */
            SCB_EnableInt();

            while((Capesnse_mailboxesComm.csdMailbox.type != Capesnse_TYPE_ID) ||
                  (0u != (SCB_EzI2CGetActivity() & SCB_EZI2C_STATUS_BUSY))){}

            /* Disable EZI2C interrupts, while scanning */
            SCB_DisableInt();

            /* Start scan all sensors */
            Capesnse_ScanEnabledWidgets();
        }
    #endif /* (0u != Capesnse_TUNER_API_GENERATE) */
}


#if ((0u != Capesnse_TUNER_API_GENERATE) && (0u != Capesnse_CSHL_API_GENERATE))
    /*******************************************************************************
    * Function Name: Capesnse_ProcessAllWidgets
    ********************************************************************************
    *
    * Summary:
    *  Calls the required functions to update all the widgets' state:
    *   - Capesnse_GetCentroidPos() - calls only if linear sliders are
    *     available.
    *   - Capesnse_GetRadialCentroidPos() - calls only if the  radial slider is
    *     available.
    *   - Capesnse_GetTouchCentroidPos() - calls only if the  touchpad slider
    *     available.
    *   - Capesnse_CheckIsAnyWidgetActive();
    *  The results of operations are copied to OUTBOX.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  Capesnse_OUTBOX outbox - the structure which is used as an output
    *  buffer to report data to the Tuner GUI.
    *  Update fields:
    *    - position[];
    *    - OnMask[];
    *
    * Global Variables:
    *  None
    *
    *******************************************************************************/
    void Capesnse_ProcessAllWidgets(volatile Capesnse_OUTBOX *outbox)
    {
        uint8  i = 0u;

        #if (0u != Capesnse_TOTAL_MATRIX_BUTTONS_COUNT)
            uint8 mbPositionBuf[Capesnse_MB_POSITION_BUFFER_SIZE];
        #endif /* (0u != Capesnse_TOTAL_MATRIX_BUTTONS_COUNT) */

        #if (0u != Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT)
            uint16 centroidPosBuf[Capesnse_CENTROID_POSITION_BUFFER_SIZE];
        #endif  /* (0u != Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT) */

        #if(0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT)
            Capesnse_POSITION_STRUCT trackpadPosBuf[Capesnse_CENTROID_POSITION_BUFFER_SIZE];
        #endif  /* (0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT) */

        #if ( (0u != Capesnse_TOTAL_RADIAL_SLIDERS_COUNT) || (0u != Capesnse_TOTAL_TOUCH_PADS_COUNT) || \
              (0u != Capesnse_TOTAL_MATRIX_BUTTONS_COUNT))
            uint8 widgetCnt;
        #endif  /* ((0u != Capesnse_TOTAL_RADIAL_SLIDERS_COUNT) || (0u != Capesnse_TOTAL_TOUCH_PADS_COUNT)) ||
                *   (0u != Capesnse_TOTAL_MATRIX_BUTTONS_COUNT)
                */

        /* Calculate widget with centroids */
        #if (0u != Capesnse_TOTAL_LINEAR_SLIDERS_COUNT)
            for(; i < Capesnse_TOTAL_LINEAR_SLIDERS_COUNT; i++)
            {
                outbox->centroidPosition[i] = (uint16)Capesnse_GetCentroidPos((uint32)i);
            }
        #endif /* (0u != Capesnse_TOTAL_LINEAR_SLIDERS_COUNT) */

        #if (0u != Capesnse_TOTAL_RADIAL_SLIDERS_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + Capesnse_TOTAL_RADIAL_SLIDERS_COUNT); i++)
            {
                outbox->centroidPosition[i] = (uint16)Capesnse_GetRadialCentroidPos((uint32)i);
            }
        #endif /* (0u != Capesnse_TOTAL_RADIAL_SLIDERS_COUNT) */

        #if (0u != Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + (Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT * 2u)); i=i+2u)
            {
                if(Capesnse_GetTouchCentroidPos((uint32)i, centroidPosBuf) == 0u)
                {
                    outbox->centroidPosition[i + Capesnse_FIRST_FINGER_X_INDEX] = Capesnse_MAX_UINT_16;
                    outbox->centroidPosition[i + Capesnse_FIRST_FINGER_Y_INDEX] = Capesnse_MAX_UINT_16;
                }
                else
                {
                    outbox->centroidPosition[i + Capesnse_FIRST_FINGER_X_INDEX] =
                    (uint16) centroidPosBuf[Capesnse_FIRST_FINGER_X_INDEX];

                    outbox->centroidPosition[i + Capesnse_FIRST_FINGER_Y_INDEX] =
                    (uint16) centroidPosBuf[Capesnse_FIRST_FINGER_Y_INDEX];
                }
            }
        #endif /* (0u != Capesnse_TOTAL_TOUCH_PADS_BASIC_COUNT) */

        #if (0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT)
            widgetCnt = i;
            for(; i < (widgetCnt + (Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT * 4u)); i=i+4u)
            {
                outbox->fingersNum[Capesnse_DEFAULT_TRACKPAD_INDEX] =
                (uint8)Capesnse_GetDoubleTouchCentroidPos(trackpadPosBuf);

                outbox->gestureID[Capesnse_DEFAULT_TRACKPAD_INDEX] =
                (uint8)Capesnse_DecodeAllGestures((uint32)outbox->fingersNum[Capesnse_DEFAULT_TRACKPAD_INDEX], trackpadPosBuf);

                #if((0u != Capesnse_ST_SCROLL_GEST_ENABLED) || (0u != Capesnse_DT_SCROLL_GEST_ENABLED))
                    /*
                    Report Scroll count only for scroll and Inertial Scroll gestures.
                    For all other gestures report scroll count as zero to tuner.
                     */
                    if (((outbox->gestureID[Capesnse_DEFAULT_TRACKPAD_INDEX] & Capesnse_GESTURE_TYPE_MASK) == Capesnse_NON_INERTIAL_SCROLL) ||
                        ((outbox->gestureID[Capesnse_DEFAULT_TRACKPAD_INDEX] & Capesnse_GESTURE_TYPE_MASK) == Capesnse_INERTIAL_SCROLL))
                    {
                        outbox->scrollCnt = Capesnse_GetScrollCnt();
                    }
                    else
                    {
                        outbox->scrollCnt = 0u;
                    }
                #endif /* ((0u != Capesnse_ST_SCROLL_GEST_ENABLED) || (0u != Capesnse_DT_SCROLL_GEST_ENABLED)) */

                outbox->centroidPosition[i + Capesnse_FIRST_FINGER_X_INDEX] = Capesnse_MAX_UINT_16;
                outbox->centroidPosition[i + Capesnse_FIRST_FINGER_Y_INDEX] = Capesnse_MAX_UINT_16;
                outbox->centroidPosition[i + Capesnse_SECOND_FINGER_X_INDEX] = Capesnse_MAX_UINT_16;
                outbox->centroidPosition[i + Capesnse_SECOND_FINGER_Y_INDEX] = Capesnse_MAX_UINT_16;

                if(0u != outbox->fingersNum[Capesnse_DEFAULT_TRACKPAD_INDEX])
                {
                    outbox->centroidPosition[i + Capesnse_FIRST_FINGER_X_INDEX] =
                    (uint16) trackpadPosBuf[Capesnse_FIRST_FINGER_INDEX].x;

                    outbox->centroidPosition[i + Capesnse_FIRST_FINGER_Y_INDEX] =
                    (uint16) trackpadPosBuf[Capesnse_FIRST_FINGER_INDEX].y;

                    if(outbox->fingersNum[Capesnse_DEFAULT_TRACKPAD_INDEX] > 1u)
                    {
                        outbox->centroidPosition[i + Capesnse_SECOND_FINGER_X_INDEX] =
                        (uint16) trackpadPosBuf[Capesnse_SECOND_FINGER_INDEX].x;

                        outbox->centroidPosition[i + Capesnse_SECOND_FINGER_Y_INDEX] =
                        (uint16) trackpadPosBuf[Capesnse_SECOND_FINGER_INDEX].y;
                    }
                }
            }
        #endif /* (0u != Capesnse_TOTAL_TRACKPAD_GESTURES_COUNT) */

        #if (0u != Capesnse_TOTAL_MATRIX_BUTTONS_COUNT)
            i += Capesnse_TOTAL_BUTTONS_COUNT;
            widgetCnt = 0u;
            for(; widgetCnt < (Capesnse_TOTAL_MATRIX_BUTTONS_COUNT * 2u); widgetCnt += 2u)
            {
                if(Capesnse_GetMatrixButtonPos((uint32)i, mbPositionBuf) == 0u)
                {
                    outbox->mbPosition[widgetCnt] = Capesnse_MAX_UINT_8;
                    outbox->mbPosition[widgetCnt+1u] = Capesnse_MAX_UINT_8;
                }
                else
                {
                    outbox->mbPosition[widgetCnt] = mbPositionBuf[0u];
                    outbox->mbPosition[widgetCnt+1u] = mbPositionBuf[1u];
                }
                i += 2u;
            }
        #endif /* (0u != Capesnse_TOTAL_MATRIX_BUTTONS_COUNT) */

        #if(0u != Capesnse_CSHL_API_GENERATE)
            /* Update On/Off State */
            (void)Capesnse_CheckIsAnyWidgetActive();

            /* Copy OnMask */
            for(i=0u; i < Capesnse_TOTAL_SENSOR_MASK_COUNT; i++)
            {
                outbox->onMask[i]  = Capesnse_sensorOnMask[i];
            }
        #endif /* (0u != Capesnse_CSHL_API_GENERATE) */

        /* `#START Capesnse_ProcessAllWidgets_Debug` */

        /* `#END` */

        #ifdef Capesnse_PROCESS_ALL_WIDGETS_CALLBACK
            Capesnse_ProcessAllWidgets_Callback();
        #endif /* Capesnse_PROCESS_ALL_WIDGETS_CALLBACK */
    }


#endif /* ((0u != Capesnse_TUNER_API_GENERATE) && (0u != Capesnse_CSHL_API_GENERATE)) */


/* [] END OF FILE */
