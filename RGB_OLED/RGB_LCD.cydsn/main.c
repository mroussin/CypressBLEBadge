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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    SPI_Master_Start();

    lcd_init();
    
    lcd_setScreen(BLACK);
    CyDelay(1000);
    lcd_setScreen(RED);
    CyDelay(1000);    
    lcd_setScreen(GREEN);
    CyDelay(1000);
    lcd_setScreen(BLUE);
    CyDelay(1000);
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
