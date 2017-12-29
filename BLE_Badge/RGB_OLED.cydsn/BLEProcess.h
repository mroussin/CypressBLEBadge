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
#if !defined (BLEPROCESS_H)
#define BLEPROCESS_H

#include <project.h>

/********************************** Macors ******************************************/
/* Badge String data length*/
#define BADGE_DATA_LEN				13
#define AGE_DATA_LEN				2
#define COLOR_VALUE                 7
/******************************************************************************************/

uint8 ColorData[450];  
uint8_t ArrayFull;
    
/******************************* Function Declaration *************************************/
void GeneralEventHandler(uint32 , void*);
void UpdateBadgecharacteristic(uint8* , uint8, uint16);
/********************************************************************************************/
#endif
/* [] END OF FILE */
