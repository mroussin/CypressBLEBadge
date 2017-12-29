/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "lcd_ili9163.h"
//#include "oled_ssd1331.h"
#include "BLEProcess.h"
#include "fonts.h"
#include "images.h"
#include <stdio.h>

#define FALSE   0
#define TRUE    1

volatile uint8_t DisplayOn = 0;
volatile uint8_t lastScan = 0;
volatile uint8_t pos = 0;
int8_t clockPositionX[12] = {0, 25, 43, 50, 43, 25, 0, -25, -43, -50, -43, -25};
int8_t clockPositionY[12] = {50, 43, 25, 0, -25, -43, -50, -43, -25, 0, 25, 43};

    
CY_ISR(T1_InterruptHandler)
{
     /* Check interrupt source and clear Inerrupt */
    if (Timer_1_INTR_MASK_CC_MATCH == Timer_1_GetInterruptSourceMasked())
    {
        Timer_1_ClearInterrupt(Timer_1_INTR_MASK_CC_MATCH);
        //DisplayOn = 0;

    }
    
}

void ScanCapSense(void)
{
	Capsense_UpdateEnabledBaselines();

	/* Start scanning all enabled sensors */
	Capsense_ScanEnabledWidgets();

	/* Wait for scanning to complete */
	while(Capsense_IsBusy());
}

uint8 ButtonPressed(void)
{
uint8 ActualState;									// For debug purposes
static uint8 PressedReturned	= FALSE;			// Not signaled
	ActualState = Capsense_CheckIsWidgetActive(Capsense_BUTTON0__BTN);
	if(!ActualState) return PressedReturned = FALSE;					// Button not pressed
	// We are here when button is (still) pressed
	if(PressedReturned) return FALSE;									// We already told that
	return PressedReturned = TRUE;										// Signal button pressed
}

void GetTime(void)
{
    RTC_1_DATE_TIME DateTime;
    
    uint8_t timeBuffer[16u];
    uint8_t dateBuffer[16u];
    uint8_t daysBuffer[7][9] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    uint32_t rtcTime, rtcDate, rtcDay;
    /* Load variables for time that was captured */
    RTC_1_GetDateAndTime(&DateTime);
    rtcTime = DateTime.time;
    rtcDate = DateTime.date;
    rtcDay = DateTime.dayOfWeek;
   
    /* Load array buffers wtih time and date */
    sprintf((char*)timeBuffer, "%02lu:%02lu", RTC_1_GetHours(rtcTime), RTC_1_GetMinutes(rtcTime));
    sprintf((char*)dateBuffer, "%02lu/%02lu/%02lu", RTC_1_GetMonth(rtcDate), RTC_1_GetDay(rtcDate), RTC_1_GetYear(rtcDate)%1000);       
    
    lcd_displayString(32,0,timeBuffer, WHITE, BLACK);     
    lcd_displayString(0,28,dateBuffer, WHITE, BLACK);  
    lcd_displayString(0,56,daysBuffer[rtcDay-1], WHITE, BLACK);  
    
}



int main(void)
{
    
    uint8_t strName[] = "Ellie";
       
    /* Start SPI_Master component operation */
    SPI_Master_Start();
    //PWM_Speaker_Start();
    
    /* Start RTC (Real Time Clock) component operation */
    RTC_1_Start();

    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Start BLE stack and register the event callback function. */
    CyBle_Start(GeneralEventHandler);
    
    /* Enable the Interrupt component connected to interrupt */
    T1_CC_ISR_StartEx(T1_InterruptHandler);
    Timer_1_Start();
    
    /* Initialize and start Capsense */
    Capsense_EnableWidget(Capsense_BUTTON0__BTN);
    Capsense_Start();
    Capsense_InitializeAllBaselines();
    Capsense_ScanEnabledWidgets();

        
    Capsense_TunerStart();
    
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /* Initialize lcd - must call configure_spi_master() beforehand*/
	lcd_init();
	lcd_setScreen(BLACK);


    
    for(;;)
    {
        /* Place your application code here. */
        /* Process BLE Events. This generates events in the callback function */
        CyBle_ProcessEvents();
        
        /* Process scanning results and communicates with Tuner GUI */
        //Capsense_TunerComm();
        
        /* Scan Capsense Buttons */
        ScanCapSense(); 
            
        if(ButtonPressed())
        {
            DisplayOn++;
            if(DisplayOn > 2)
            {
                DisplayOn = 0;
            }
            lcd_setScreen(BLACK);
            
            //Timer_1_WriteCounter(0);
        }


        switch(DisplayOn)
        {
            case 0:
            if(lastScan != 0)
            {
                lcd_displayString(0,0,strName, WHITE, BLACK); 
                lcd_displayImage(Jeep_Logo, 128, 72, 0, 28);
            }
            break;
            
            case 1:
             if(lastScan != 1)
            {
                GetTime();
            }
            break;
            
            case 2:
            if(lastScan != 2)
            {
                lcd_displayImage(imgClock, 128, 128, 0, 0);
                
                draw_line(64,70,64+clockPositionX[pos],70-clockPositionY[pos],BLACK);
                pos++;
                if(pos > 11)
                {
                    pos = 0;
                }
            }
            break;
            
            default:
            break;
        }
        
        lastScan = DisplayOn;

        
        if(DisplayOn == 0)
        {
            /* Switch to Sleep Mode */
            //lcd_sleep();
            //CySysPmDeepSleep();
        }
        
      
        /************
        for(uint16_t i = 0; i < 1000; i+=10)
        {
            PWM_Speaker_WriteCompare(i);
            CyDelay(100);
        }
        **********/
        
    }
}

/* [] END OF FILE */
