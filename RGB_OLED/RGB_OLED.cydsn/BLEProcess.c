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

#include "BLEProcess.h"
#include "lcd_ili9163.h"
#include <stdint.h>
#include <stdlib.h>  

void GeneralEventHandler(uint32 event, void * eventParam)
{
	/* Structure to store data written by Client */
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
	
    /* Array to store data*/
    uint8 BadgeData[BADGE_DATA_LEN];
    uint8 AgeData[AGE_DATA_LEN];
    char IdNumber[3];
    char ColorNumber[3];
	uint8_t i, j;
    uint16_t idValue = 0;
    uint8_t ColorValue = 0;
    
    ArrayFull = 0;
    
    switch(event)
	{
	
        case CYBLE_EVT_STACK_ON:
		/* This event is generated when BLE stack is ON */
        /* BLE stack is on. Start BLE advertisement */
        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
		break;
	
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
		/* This event is generated at GAP disconnection. */
        /* Restart advertisement */
        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
		break;
	
        case CYBLE_EVT_GATTS_WRITE_REQ:
		/* This event is generated when the connected Central */
		/* device sends a Write request. */
		/* The parameter ‘eventParam’ contains the data written */
        /* Extract the Write data sent by Client */
         wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
        
        /* If the attribute handle of the characteristic written to
        * is equal to that of RGB_LED characteristic, then extract
        * the RGB LED data */
        
         if(CYBLE_BADGE_NAME_STRING_CHAR_HANDLE ==
         wrReqParam->handleValPair.attrHandle)
         {

            
            /* Store Badge data in local array */
            for(i = 0; i < 13; i++)
            {
                

                    BadgeData[i] =
                    wrReqParam->handleValPair.value.val[i];

            }
            
            /* Parse string value in two different arrays */
            j = 0;
            while(BadgeData[j] != ' ')
            {
              
                IdNumber[j] = BadgeData[j];
                j++;
                
            }
            if(j==1)
            {
                IdNumber[2] = IdNumber[0];
                IdNumber[1] = '0';
                IdNumber[0] = '0';
            }
            
            if(j==2)
            {
                IdNumber[2] = IdNumber[1]; 
                IdNumber[1] = IdNumber[0];
                IdNumber[0] = '0';
            }
            
            j++;
            
            i = 0;
            while(BadgeData[j] != ' ')
            {
              
                ColorNumber[i] = BadgeData[j];
                j++;
                i++;               
            }
            
            if(i==1)
            {
                ColorNumber[2] = ColorNumber[0];
                ColorNumber[1] = '0';
                ColorNumber[0] = '0';
            }
            
            if(i==2)
            {
                ColorNumber[2] = ColorNumber[1]; 
                ColorNumber[1] = ColorNumber[0];
                ColorNumber[0] = '0';
            }
            
            /* Convert string value to number */
            idValue = atoi(IdNumber);
            ColorValue = atoi(ColorNumber);
            
            ColorData[idValue] = ColorValue; 
            

            /* Update the PrISM component density value to represent color */
            lcd_displayImage(ColorData, 15, 15, 0, 0);
            /* Update the GATT DB for RGB LED read characteristics*/
            UpdateBadgecharacteristic(BadgeData,
                                      sizeof(BadgeData),
                                      CYBLE_BADGE_NAME_STRING_CHAR_HANDLE);
         }
        
        if(CYBLE_BADGE_COLOR_STRING_CHAR_HANDLE ==
         wrReqParam->handleValPair.attrHandle)
         {
            /* Store Badge data in local array */
             for(i = 0; i < 2; i++)
            {
                AgeData[i] =
                wrReqParam->handleValPair.value.val[i];
            }

            /* Update the PrISM component density value to represent color */
             lcd_displayString(0,16,AgeData, BLUE, BLACK);
            /* Update the GATT DB for RGB LED read characteristics*/
            UpdateBadgecharacteristic(AgeData,
                                      sizeof(AgeData),
                                      CYBLE_BADGE_COLOR_STRING_CHAR_HANDLE);
         }
        
        
        /* Send the response to the write request received. */
        CyBle_GattsWriteRsp(cyBle_connHandle);
		break;

	default:
		break;
	}
}

/*******************************************************************************
* Function Name: UpdateBadgeCharacteristic
********************************************************************************
* Summary:
*        Update the GATT DB with new RGB Color received to allow Client to read 
* back updated color values.
*
* Parameters:
*  ledData:		array pointer where color value is stored
*  len: 		length of the array
*  attrHandle:	attribute handle of the characteristic
*
* Return:
*  void
*
*******************************************************************************/
void  UpdateBadgecharacteristic(uint8* BadgeData, uint8 len, uint16 attrHandle)
{
	/* 'rgbHandle' stores RGB control data parameters */
	CYBLE_GATT_HANDLE_VALUE_PAIR_T		BadgeHandle;

	/* Update RGB control handle with new values */
	BadgeHandle.attrHandle = attrHandle;
	BadgeHandle.value.val = BadgeData;
	BadgeHandle.value.len = len;
	
	/* Update the RGB LED attribute value. This will allow 
	* Client device to read the existing color values over 
	* RGB LED characteristic */
	CyBle_GattsWriteAttributeValue(& BadgeHandle, 
									0, 
									&cyBle_connHandle, 
									CYBLE_GATT_DB_PEER_INITIATED);
}

/* [] END OF FILE */
