/***************************************************************************//**
* \file SPI_Master_SPI_UART.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_Master_PVT.h"
#include "SPI_Master_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(SPI_Master_INTERNAL_RX_SW_BUFFER_CONST)
    /* Start index to put data into the software receive buffer.*/
    volatile uint32 SPI_Master_rxBufferHead;
    /* Start index to get data from the software receive buffer.*/
    volatile uint32 SPI_Master_rxBufferTail;
    /**
    * \addtogroup group_globals
    * \{
    */
    /** Sets when internal software receive buffer overflow
    *  was occurred.
    */
    volatile uint8  SPI_Master_rxBufferOverflow;
    /** \} globals */
#endif /* (SPI_Master_INTERNAL_RX_SW_BUFFER_CONST) */

#if(SPI_Master_INTERNAL_TX_SW_BUFFER_CONST)
    /* Start index to put data into the software transmit buffer.*/
    volatile uint32 SPI_Master_txBufferHead;
    /* Start index to get data from the software transmit buffer.*/
    volatile uint32 SPI_Master_txBufferTail;
#endif /* (SPI_Master_INTERNAL_TX_SW_BUFFER_CONST) */

#if(SPI_Master_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 SPI_Master_rxBufferInternal[SPI_Master_INTERNAL_RX_BUFFER_SIZE];
#endif /* (SPI_Master_INTERNAL_RX_SW_BUFFER) */

#if(SPI_Master_INTERNAL_TX_SW_BUFFER)
    volatile uint8 SPI_Master_txBufferInternal[SPI_Master_TX_BUFFER_SIZE];
#endif /* (SPI_Master_INTERNAL_TX_SW_BUFFER) */


#if(SPI_Master_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: SPI_Master_SpiUartReadRxData
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * \return
    *  Next data element from the receive buffer. 
    *  The amount of data bits to be received depends on RX data bits selection 
    *  (the data bit counting starts from LSB of return value).
    *
    * \globalvars
    *  SPI_Master_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  SPI_Master_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 SPI_Master_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

    #if (SPI_Master_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (SPI_Master_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (SPI_Master_CHECK_RX_SW_BUFFER)
        {
            if (SPI_Master_rxBufferHead != SPI_Master_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (SPI_Master_rxBufferTail + 1u);

                if (SPI_Master_INTERNAL_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = SPI_Master_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                SPI_Master_rxBufferTail = locTail;

                #if (SPI_Master_CHECK_UART_RTS_CONTROL_FLOW)
                {
                    /* Check if RX Not Empty is disabled in the interrupt */
                    if (0u == (SPI_Master_INTR_RX_MASK_REG & SPI_Master_INTR_RX_NOT_EMPTY))
                    {
                        /* Enable RX Not Empty interrupt source to continue
                        * receiving data into software buffer.
                        */
                        SPI_Master_INTR_RX_MASK_REG |= SPI_Master_INTR_RX_NOT_EMPTY;
                    }
                }
                #endif

            }
        }
        #else
        {
            /* Read data from RX FIFO */
            rxData = SPI_Master_RX_FIFO_RD_REG;
        }
        #endif

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: SPI_Master_SpiUartGetRxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * \return
    *  Number of received data elements.
    *
    * \globalvars
    *  SPI_Master_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  SPI_Master_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 SPI_Master_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
    #if (SPI_Master_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (SPI_Master_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (SPI_Master_CHECK_RX_SW_BUFFER)
        {
            locHead = SPI_Master_rxBufferHead;

            if(locHead >= SPI_Master_rxBufferTail)
            {
                size = (locHead - SPI_Master_rxBufferTail);
            }
            else
            {
                size = (locHead + (SPI_Master_INTERNAL_RX_BUFFER_SIZE - SPI_Master_rxBufferTail));
            }
        }
        #else
        {
            size = SPI_Master_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: SPI_Master_SpiUartClearRxBuffer
    ****************************************************************************//**
    *
    *  Clears the receive buffer and RX FIFO.
    *
    * \globalvars
    *  SPI_Master_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  SPI_Master_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    void SPI_Master_SpiUartClearRxBuffer(void)
    {
        #if (SPI_Master_CHECK_RX_SW_BUFFER)
        {
            /* Lock from component interruption */
            SPI_Master_DisableInt();

            /* Flush RX software buffer */
            SPI_Master_rxBufferHead = SPI_Master_rxBufferTail;
            SPI_Master_rxBufferOverflow = 0u;

            SPI_Master_CLEAR_RX_FIFO;
            SPI_Master_ClearRxInterruptSource(SPI_Master_INTR_RX_ALL);

            #if (SPI_Master_CHECK_UART_RTS_CONTROL_FLOW)
            {
                /* Enable RX Not Empty interrupt source to continue receiving
                * data into software buffer.
                */
                SPI_Master_INTR_RX_MASK_REG |= SPI_Master_INTR_RX_NOT_EMPTY;
            }
            #endif
            
            /* Release lock */
            SPI_Master_EnableInt();
        }
        #else
        {
            SPI_Master_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (SPI_Master_RX_DIRECTION) */


#if(SPI_Master_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: SPI_Master_SpiUartWriteTxData
    ****************************************************************************//**
    *
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *   The amount of data bits to be transmitted depends on TX data bits selection 
    *   (the data bit counting starts from LSB of txDataByte).
    *
    * \globalvars
    *  SPI_Master_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SPI_Master_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void SPI_Master_SpiUartWriteTxData(uint32 txData)
    {
    #if (SPI_Master_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (SPI_Master_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (SPI_Master_CHECK_TX_SW_BUFFER)
        {
            /* Put data directly into the TX FIFO */
            if ((SPI_Master_txBufferHead == SPI_Master_txBufferTail) &&
                (SPI_Master_SPI_UART_FIFO_SIZE != SPI_Master_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                SPI_Master_TX_FIFO_WR_REG = txData;
            }
            /* Put data into TX software buffer */
            else
            {
                /* Head index to put data */
                locHead = (SPI_Master_txBufferHead + 1u);

                /* Adjust TX software buffer index */
                if (SPI_Master_TX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                /* Wait for space in TX software buffer */
                while (locHead == SPI_Master_txBufferTail)
                {
                }

                /* TX software buffer has at least one room */

                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                SPI_Master_ClearTxInterruptSource(SPI_Master_INTR_TX_NOT_FULL);

                SPI_Master_PutWordInTxBuffer(locHead, txData);

                SPI_Master_txBufferHead = locHead;

                /* Check if TX Not Full is disabled in interrupt */
                if (0u == (SPI_Master_INTR_TX_MASK_REG & SPI_Master_INTR_TX_NOT_FULL))
                {
                    /* Enable TX Not Full interrupt source to transmit from software buffer */
                    SPI_Master_INTR_TX_MASK_REG |= (uint32) SPI_Master_INTR_TX_NOT_FULL;
                }
            }
        }
        #else
        {
            /* Wait until TX FIFO has space to put data element */
            while (SPI_Master_SPI_UART_FIFO_SIZE == SPI_Master_GET_TX_FIFO_ENTRIES)
            {
            }

            SPI_Master_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: SPI_Master_SpiUartPutArray
    ****************************************************************************//**
    *
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * \param wrBuf: pointer to an array of data to be placed in transmit buffer. 
    *  The width of the data to be transmitted depends on TX data width selection 
    *  (the data bit counting starts from LSB for each array element).
    * \param count: number of data elements to be placed in the transmit buffer.
    *
    * \globalvars
    *  SPI_Master_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SPI_Master_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void SPI_Master_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for (i=0u; i < count; i++)
        {
            SPI_Master_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: SPI_Master_SpiUartGetTxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of elements currently in the transmit buffer.
    *   - TX software buffer is disabled: returns the number of used entries in
    *     TX FIFO.
    *   - TX software buffer is enabled: returns the number of elements currently
    *     used in the transmit buffer. This number does not include used entries in
    *     the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *     not full.
    *
    * \return
    *  Number of data elements ready to transmit.
    *
    * \globalvars
    *  SPI_Master_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SPI_Master_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    uint32 SPI_Master_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
    #if (SPI_Master_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (SPI_Master_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (SPI_Master_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = SPI_Master_txBufferTail;

            if (SPI_Master_txBufferHead >= locTail)
            {
                size = (SPI_Master_txBufferHead - locTail);
            }
            else
            {
                size = (SPI_Master_txBufferHead + (SPI_Master_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = SPI_Master_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: SPI_Master_SpiUartClearTxBuffer
    ****************************************************************************//**
    *
    *  Clears the transmit buffer and TX FIFO.
    *
    * \globalvars
    *  SPI_Master_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SPI_Master_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void SPI_Master_SpiUartClearTxBuffer(void)
    {
        #if (SPI_Master_CHECK_TX_SW_BUFFER)
        {
            /* Lock from component interruption */
            SPI_Master_DisableInt();

            /* Flush TX software buffer */
            SPI_Master_txBufferHead = SPI_Master_txBufferTail;

            SPI_Master_INTR_TX_MASK_REG &= (uint32) ~SPI_Master_INTR_TX_NOT_FULL;
            SPI_Master_CLEAR_TX_FIFO;
            SPI_Master_ClearTxInterruptSource(SPI_Master_INTR_TX_ALL);

            /* Release lock */
            SPI_Master_EnableInt();
        }
        #else
        {
            SPI_Master_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (SPI_Master_TX_DIRECTION) */


/*******************************************************************************
* Function Name: SPI_Master_SpiUartDisableIntRx
****************************************************************************//**
*
*  Disables the RX interrupt sources.
*
*  \return
*   Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 SPI_Master_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = SPI_Master_GetRxInterruptMode();

    SPI_Master_SetRxInterruptMode(SPI_Master_NO_INTR_SOURCES);

    return (intSource);
}


/*******************************************************************************
* Function Name: SPI_Master_SpiUartDisableIntTx
****************************************************************************//**
*
*  Disables TX interrupt sources.
*
*  \return
*   Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 SPI_Master_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = SPI_Master_GetTxInterruptMode();

    SPI_Master_SetTxInterruptMode(SPI_Master_NO_INTR_SOURCES);

    return (intSourceMask);
}


#if(SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: SPI_Master_PutWordInRxBuffer
    ****************************************************************************//**
    *
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param index:      index to store data byte/word in the RX buffer.
    *  \param rxDataByte: byte/word to store.
    *
    *******************************************************************************/
    void SPI_Master_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if (SPI_Master_ONE_BYTE_WIDTH == SPI_Master_rxDataBits)
        {
            SPI_Master_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            SPI_Master_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            SPI_Master_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: SPI_Master_GetWordFromRxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \return
    *   Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 SPI_Master_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if (SPI_Master_ONE_BYTE_WIDTH == SPI_Master_rxDataBits)
        {
            value = SPI_Master_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) SPI_Master_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)SPI_Master_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }


    /*******************************************************************************
    * Function Name: SPI_Master_PutWordInTxBuffer
    ****************************************************************************//**
    *
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx:        index to store data byte/word in the TX buffer.
    *  \param txDataByte: byte/word to store.
    *
    *******************************************************************************/
    void SPI_Master_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if (SPI_Master_ONE_BYTE_WIDTH == SPI_Master_txDataBits)
        {
            SPI_Master_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            SPI_Master_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            SPI_Master_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: SPI_Master_GetWordFromTxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx: index to get data byte/word from the TX buffer.
    *
    *  \return
    *   Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 SPI_Master_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if (SPI_Master_ONE_BYTE_WIDTH == SPI_Master_txDataBits)
        {
            value = (uint32) SPI_Master_txBuffer[idx];
        }
        else
        {
            value  = (uint32) SPI_Master_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) SPI_Master_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }

#endif /* (SPI_Master_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
