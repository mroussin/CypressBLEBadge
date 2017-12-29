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


#include "lcd_ili9163.h"
#include "project.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SSD1331_CMD                     0
#define SSD1331_DATA                    1



void lcd_Reset(void);
void lcd_writeCommand(uint8_t data);
void lcd_writeData(uint8_t data);
void lcd_writeData16(uint16_t data);
void lcd_SetAddr(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);



/**
 * \brief Resets display using external reset pint
 *
 * This functions toggles the reset pin to perform a hard reset
 *
 * \param none
 */
void lcd_Reset(void)
{
	
	OLED_Reset_Write(1);
	CyDelay(1);
	
	OLED_Reset_Write(0);
	CyDelay(50);
	
	OLED_Reset_Write(1);
	CyDelay(120);
	
}

/**
 * \brief Write data to the display controller
 *
 * This functions sets the pin D/C# before writing to the controller. Different
 * data write function is called based on the selected interface.
 *
 * \param data the data to write
 */
void lcd_writeData(uint8_t data)
{

        OLED_DC_Write(SSD1331_DATA);
        SPI_Master_SpiUartWriteTxData(data);
        
        /* Wait for the end of the transfer. The number of transmitted data
        * elements has to be equal to the number of received data elements.
        */
        while(1 != SPI_Master_SpiUartGetRxBufferSize())
        {
            /* Wait */
        }

        /* Clear dummy bytes from RX buffer */
        SPI_Master_SpiUartClearRxBuffer();
}

/**
 * \brief Write 16-bit data to the display controller
 *
 * This functions sets the pin D/C# before writing to the controller. Different
 * data write function is called based on the selected interface.
 *
 * \param data = the data to write
 */
void lcd_writeData16(uint16_t data)
{
		uint8_t data1, data2;
		
		data1 = data >> 8;
		data2 = data & 0xFF;
		

		OLED_DC_Write(SSD1331_DATA);
		SPI_Master_SpiUartWriteTxData(data1);
        /* Wait for the end of the transfer. The number of transmitted data
        * elements has to be equal to the number of received data elements.
        */
        while(1 != SPI_Master_SpiUartGetRxBufferSize())
        {
            /* Wait */
        }

        /* Clear dummy bytes from RX buffer */    
        SPI_Master_SpiUartClearRxBuffer();
		
        SPI_Master_SpiUartWriteTxData(data2);
		/* Wait for the end of the transfer. The number of transmitted data
        * elements has to be equal to the number of received data elements.
        */
        while(1 != SPI_Master_SpiUartGetRxBufferSize())
        {
            /* Wait */
        }

        /* Clear dummy bytes from RX buffer */
        SPI_Master_SpiUartClearRxBuffer();
}


 /**
 * \brief Writes a command to the display controller
 *
 * This functions pull pin D/C# low before writing to the controller. Different
 * data write function is called based on the selected interface.
 *
 * \param command the command to write
 */
void lcd_writeCommand(uint8_t command)
{

	OLED_DC_Write(SSD1331_CMD);
    SPI_Master_SpiUartWriteTxData(command);
		
    /* Wait for the end of the transfer. The number of transmitted data
    * elements has to be equal to the number of received data elements.
    */
    while(1 != SPI_Master_SpiUartGetRxBufferSize())
    {
        /* Wait */
    }

    /* Clear dummy bytes from RX buffer */
    SPI_Master_SpiUartClearRxBuffer();    
    
    
}

/**
 * \brief Set area to write data to the display controller
 *
 * This functions sets the area in pixels to write data to
 * 
 *
 * \param X1 = Start Column, X2= End Column, Y1 = Start Row
 *  Y2 = End Row
 */
void lcd_SetAddr(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
	lcd_writeCommand(LCD_SET_COL_ADDRESS);
	lcd_writeData(0x00);
	lcd_writeData(X1);
	lcd_writeData(0x00);
	lcd_writeData(X2);
	
	lcd_writeCommand(LCD_SET_PAGE_ADDRESS);
	lcd_writeData(0x00);
	lcd_writeData(Y1);  // Value was 32+Y1
	lcd_writeData(0x00);
	lcd_writeData(Y2);  // Value was 32+Y2

	lcd_writeCommand(LCD_WRITE_MEM_START);

}

void lcd_drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	lcd_SetAddr(x, y, x, y);

	lcd_writeCommand(LCD_WRITE_MEM_START);
	lcd_writeData(color >> 8);
	lcd_writeData(color & 0xFF);
}

/**
 * \brief Draw a line using two points on lcd
 *
 * This function takes two points and draws a line
 * between those two points.
 *
 * \param chXpos0 = Start X, chYpos0 = Start Y
 * \param chXpos1 = End X, chYpos1 = End Y, Color = RGB Color
 *
 */
void draw_line(uint8_t chXpos0, uint8_t chYpos0, uint8_t chXpos1, uint8_t chYpos1, uint16_t Color) 
{
	int x = chXpos1 - chXpos0;
    int y = chYpos1 - chYpos0;
    int dx = abs(x), sx = chXpos0 < chXpos1 ? 1 : -1;
    int dy = -abs(y), sy = chYpos0 < chYpos1 ? 1 : -1;
    int err = dx + dy, e2;

	if (chXpos0 >= MAX_HORIZ || chYpos0 >= MAX_VERT || chXpos1 >= MAX_HORIZ || chYpos1 >= MAX_VERT) {
		return;
	}
    
    for (;;){
        lcd_drawPixel(chXpos0, chYpos0 , Color);
        e2 = 2 * err;
        if (e2 >= dy) {     
            if (chXpos0 == chXpos1) break;
            err += dy; chXpos0 += sx;
        }
        if (e2 <= dx) {
            if (chYpos0 == chYpos1) break;
            err += dx; chYpos0 += sy;
        }
    }
}

/**
 * \brief Set background color
 *
 * This function sets the background to defined color
 * 
 *
 * \param Color = Color background is set to
 */
void lcd_setScreen(uint16_t Color)
{
	volatile uint16_t XCnt, YCnt;

	lcd_SetAddr(0, 0, MAX_HORIZ-1, MAX_VERT-1);

	for(XCnt = 0; XCnt<MAX_HORIZ; XCnt++)
	{
		for(YCnt = 0; YCnt<MAX_VERT; YCnt++)
		{
			lcd_writeData16(Color);
		}
	}
}

void lcd_setArea(uint16_t Color, uint8_t sizeX, uint8_t sizeY, uint8_t startX, uint8_t startY)
{
	volatile uint16_t XCnt, YCnt;
    
    if(sizeX > 127 || sizeY > 127 || startX > 127 || startY > 127)
        return;
    
	lcd_SetAddr(startX, startY, sizeX-1, sizeY-1);

	for(XCnt = 0; XCnt<sizeX; XCnt++)
	{
		for(YCnt = 0; YCnt<sizeY; YCnt++)
		{
			lcd_writeData16(Color);
		}
	}
}


/**
 * \brief Display image
 *
 * This function sets the image on the display
 * 
 *
 * \param Image - Pointer
 */
void lcd_displayImage(const uint8_t *Image, uint8_t sizeX, uint8_t sizeY, uint8_t startX, uint8_t startY)
{
    
    uint32_t size = 2 * sizeX * sizeY;
    
	lcd_SetAddr(startX, startY, startX+sizeX-1, startY+sizeY-1); //lcd_SetAddr(0, 0, MAX_HORIZ-1, MAX_VERT-1)
    lcd_writeCommand(LCD_WRITE_MEM_START);

    OLED_DC_Write(SSD1331_DATA);
    
    /* Start transfer */
    SPI_Master_SpiUartPutArray(Image, size);
    
    //for(uint32_t i = 0; i < size; i++)
    //{
        /* Start transfer */
    //    SPI_Master_SpiUartWriteTxData(Image[i]);

    //}
    
    /* Wait for the end of the transfer. The number of transmitted data
    * elements has to be equal to the number of received data elements.
    */
    while(8 != SPI_Master_SpiUartGetRxBufferSize())
    {
    }

    /* Clear dummy bytes from RX & TX buffer */
    SPI_Master_SpiUartClearRxBuffer();
    SPI_Master_SpiUartClearTxBuffer();
}


void lcd_putChar8x16(uint8_t x, uint8_t y, char c, uint16_t fColor, uint16_t bColor)
{     
	unsigned int i, j;

	lcd_SetAddr(x, y, x + 8 - 1, y + 16 - 1);
	lcd_writeCommand(LCD_WRITE_MEM_START);

	for(i = 0; i < 16; i++)      
	{         
		unsigned char m = Font8x16[(c - 0x20) * 16 + i];
        for(j = 0; j < 8; j++) 
        {
             
        	if((m & 0x80) == 0x80)    
            {
        		lcd_writeData16(fColor);
            }
            else 
            {
            	lcd_writeData16(bColor);
            }             
        	m <<= 1;         
        }     
	}
}

/**
 * \brief Display Character
 *
 * This function displays the 16x28 character on the display
 * 
 *
 * \param x - X-Position on LCD (0-127)
 *        y - Y-Position on LCD (0-127)
 *        c - Character to display
 *        fColor - Color of character
 *        bColor - Color of background
 */
void lcd_putChar16x28(uint8_t x, uint8_t y, char c, uint16_t fColor, uint16_t bColor)
{     
	unsigned int i, j;

	lcd_SetAddr(x, y, x + 16 - 1, y + 28 - 1);
	lcd_writeCommand(LCD_WRITE_MEM_START);

	for(i = 0; i < 56; i++)      
	{         
		unsigned char m = Font16x28[(c - 0x20) * 56 + i];
        for(j = 0; j < 8; j++) 
        {
             
        	if((m & 0x01) == 0x01) //if((m & 0x80) == 0x80)   
            {
        		lcd_writeData16(fColor);
            }
            else 
            {
            	lcd_writeData16(bColor);
            }             
        	m >>= 1;         
        }
     
	}
}


/**
  * @brief Displays one character at the specified position    
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  chSize: 
  * @param  chMode
  * @retval 
**/
/*
void display_char(uint8_t chXpos, uint8_t chYpos, uint8_t chChr, uint8_t chSize, uint16_t hwColor)
{      	
	uint8_t i, j; 
    uint8_t chTemp = 0;
	uint8_t chYpos0 = chYpos;

	if (chXpos >= MAX_HORIZ || chYpos >= MAX_VERT) {
		return;
	}
					   
    for (i = 0; i < chSize; i ++) {   
		if (FONT_1206 == chSize) {
			chTemp = c_chFont1206[chChr - 0x20][i];  
		} else if (FONT_1608 == chSize) { 
			chTemp = c_chFont1608[chChr - 0x20][i];
		}
		
        for (j = 0; j < 8; j ++) {
    		if (chTemp & 0x80) {
				lcd_drawPixel(chXpos, chYpos, hwColor);
    		}			
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == chSize) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}  	 
    } 
}
*/

/**
  * @brief  Displays a string on the screen
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  pchString: Pointer to a string to display on the screen 
  *         
  * @retval  None
**/

/*
void display_string(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchString, uint8_t chSize, uint16_t hwColor)
{
	if (chXpos >= MAX_HORIZ || chYpos >= MAX_VERT) {
		return;
	}
	
    while (*pchString != '\0') {       
        if (chXpos > (MAX_HORIZ - chSize / 2)) {
			chXpos = 0;
			chYpos += chSize;
			if (chYpos > (MAX_VERT - chSize)) {
				chYpos = chXpos = 0;
				lcd_setScreen(0x00);
			}
		}
		
        display_char(chXpos, chYpos, *pchString, chSize, hwColor);
        chXpos += chSize / 2;
        pchString ++;
    } 
}
*/

void lcd_displayString(uint8_t x, uint8_t y, uint8_t* pcString, uint16_t fColor, uint16_t bColor)
{
	unsigned char ucl = 0;

	while(*pcString)
	{
        //lcd_putChar8x16(x + ucl * 8, y, *pcString, fColor, bColor);
		lcd_putChar16x28(x + ucl * 16, y, *pcString, fColor, bColor);  
		pcString++; ucl++;
	}
}


void lcd_init(void)
{
	lcd_Reset();
	
	//lcd_writeCommand(LCD_SOFT_RESET);
	lcd_writeCommand(LCD_EXIT_SLEEP_MODE);
	CyDelay(100);
	
	lcd_writeCommand(LCD_SET_GAMMA_CURVE);
	lcd_writeData(0x04); // Set Default Gamma
	
	lcd_writeData(LCD_ENTER_NORM_MODE);
	
	lcd_writeCommand(LCD_POWER_CONTROL1);
	lcd_writeData(0x0A); // VRH = 10:  GVDD = 4.3
	lcd_writeData(0x02); // VC = 2: VCI1 = 2.65
	
	lcd_writeCommand(LCD_POWER_CONTROL2);
	lcd_writeData(0x02); // BT = 0: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 4xVCI1, VGL = -3xVCI1

	//lcd_writeCommand(LCD_POWER_CONTROL3);
	//lcd_writeData(0x00); // Least power in current amplifier
	//lcd_writeData(0x07); // Booster Circ #1 =BCLK/8, Booster Circ #2&3 =BCLK/64
	
	//lcd_writeCommand(LCD_POWER_CONTROL4);
	//lcd_writeData(0x00); // Least power in current amplifier
	//lcd_writeData(0x07); // Booster Circ #1 =BCLK/8, Booster Circ #2&3 =BCLK/64
	
	lcd_writeCommand(LCD_VCOM_CONTROL1);
	lcd_writeData(0x50); // 
	lcd_writeData(0x99); // 
		
	lcd_writeCommand(LCD_VCOM_OFFSET_CONTROL);
	lcd_writeData(0x00); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML
	
	lcd_writeCommand(LCD_EXIT_IDLE_MODE);
	
    /* 16 bits per pixel R5 G6 B5 */
	lcd_writeCommand(LCD_SET_PIXEL_FMT);
	lcd_writeData(0x05); 
	
	lcd_writeCommand(LCD_FRAME_RATE_CTRL1);
	lcd_writeData(0x08); // DIVA = 8
	lcd_writeData(0x02); // VPA = 9  Was value = 0x08
		                                                                                 
	lcd_writeCommand(LCD_SET_COL_ADDRESS);
	lcd_writeData(0x00); // XSH
	lcd_writeData(0x00); // XSL
	lcd_writeData(0x00); // XEH
	lcd_writeData(0x7f); // XEL 128 pixels x
	
	lcd_writeCommand(LCD_SET_PAGE_ADDRESS);
	lcd_writeData(0x00);
	lcd_writeData(0x00);
	lcd_writeData(0x00);
	lcd_writeData(0x7f); // 128 pixels y
	
	//lcd_writeCommand(LCD_DISPLAY_INVERSION);
	//lcd_writeData(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)
	
	// Select display orientation
	lcd_writeCommand(LCD_SET_ADDRESS_MODE);
	lcd_writeData(0x00); //

	// Set the display to on
	lcd_writeCommand(LCD_SET_DISP_ON);
	
	lcd_writeCommand(LCD_EXIT_INVERT_MODE);
	
	lcd_writeCommand(LCD_WRITE_MEM_START);
	
}


/* [] END OF FILE */
