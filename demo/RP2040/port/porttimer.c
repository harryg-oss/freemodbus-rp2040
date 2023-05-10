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
#include "hardware/irq.h"
#include "pico/time.h"

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Static functions ---------------------------------*/
static int64_t __isr prvvTIMERExpiredISR(alarm_id_t id, void *user_data);

/* ----------------------- Static variables ---------------------------------*/
static alarm_id_t alarm_num = -1; /* 0 - 3 */
static int alarm_timeout    = 50;


/* ----------------------- Start implementation -----------------------------*/


BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    alarm_timeout = usTim1Timerout50us;
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    if (alarm_num != -1)
    {
        cancel_alarm(alarm_num);
    }

    alarm_num = add_alarm_in_ms(alarm_timeout / 20, prvvTIMERExpiredISR, NULL, true);

}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
    if (alarm_num != -1)
    {
        cancel_alarm(alarm_num);
        alarm_num = -1;
    }
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static int64_t __isr prvvTIMERExpiredISR(alarm_id_t id, void *user_data)
{
    ( void )pxMBPortCBTimerExpired(  );
}
