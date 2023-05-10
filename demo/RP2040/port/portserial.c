/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"

#include "port.h"
#include "pinmap.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Static functions ---------------------------------*/
static void prvvUARTxISR( void );
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Static variables ---------------------------------*/
static uart_inst_t*     uart            = NULL;
static volatile bool    is_rx           = TRUE;
static volatile bool    is_tx           = FALSE;

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

#if MB_RE_GPIO != MB_DE_GPIO
    int re_level = xRxEnable ? MB_RE_GPIO_ACTIVE : MB_RE_GPIO_INACTIVE;
    gpio_put( MB_RE_GPIO, re_level );
#endif
    uart_set_irq_enables( uart, xRxEnable, xTxEnable );
    is_rx = xRxEnable;

    int de_level = xTxEnable ? MB_DE_GPIO_ACTIVE : MB_DE_GPIO_INACTIVE;
    gpio_put( MB_DE_GPIO, de_level );
    is_tx = xTxEnable;
}


void xMBPortSerialPoll(void)
{
    if ( is_tx && uart_is_writable( uart ) )
    {
        pxMBFrameCBTransmitterEmpty(  );
    }
}


BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    switch (ucPORT)
    {
        case 0:
            uart = uart0;
            break;
        case 1:
            uart = uart1;
            break;
        default:
            return FALSE;
    }
    uart_parity_t parity;
    switch (eParity)
    {
        case MB_PAR_NONE:
            parity = UART_PARITY_NONE;
            break;
        case MB_PAR_ODD:
            parity = UART_PARITY_ODD;
            break;
        case MB_PAR_EVEN:
            parity = UART_PARITY_EVEN;
            break;
        default:
            return FALSE;
    }
    uart_set_baudrate( uart, ulBaudRate );
    uart_set_format( uart, ucDataBits, 1, parity );

#if MB_RE_GPIO != MB_DE_GPIO
    gpio_init(MB_RE_GPIO);
    gpio_set_dir(MB_RE_GPIO, TRUE);
#endif
    gpio_init(MB_DE_GPIO);
    gpio_set_dir(MB_DE_GPIO, TRUE);

    int uart_irq = uart == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(uart_irq, prvvUARTxISR);
    irq_set_enabled(uart_irq, true);
    uart_set_irq_enables( uart, TRUE, FALSE );
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    uart_putc_raw( uart, ucByte );
    uart_tx_wait_blocking(uart);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    *pucByte = uart_getc( uart );
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

static void __isr prvvUARTxISR( void )
{
    if ( is_rx )
    {
        prvvUARTRxISR(  );
    }
    if ( is_tx )
    {
        prvvUARTTxReadyISR(  );
    }
}
