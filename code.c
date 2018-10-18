/* ###*B*###
 * Erika Enterprise, version 3
 * 
 * Copyright (C) 2017 - 2018 Evidence s.r.l.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License, version 2, for more details.
 * 
 * You should have received a copy of the GNU General Public License,
 * version 2, along with this program; if not, see
 * < www.gnu.org/licenses/old-licenses/gpl-2.0.html >.
 * 
 * This program is distributed to you subject to the following
 * clarifications and special exceptions to the GNU General Public
 * License, version 2.
 * 
 * THIRD PARTIES' MATERIALS
 * 
 * Certain materials included in this library are provided by third
 * parties under licenses other than the GNU General Public License. You
 * may only use, copy, link to, modify and redistribute this library
 * following the terms of license indicated below for third parties'
 * materials.
 * 
 * In case you make modified versions of this library which still include
 * said third parties' materials, you are obligated to grant this special
 * exception.
 * 
 * The complete list of Third party materials allowed with ERIKA
 * Enterprise version 3, together with the terms and conditions of each
 * license, is present in the file THIRDPARTY.TXT in the root of the
 * project.
 * ###*E*### */

/** \file	code.c
 *  \brief	Tasks for benchmark.
 *
 *  \author	Errico Guidieri
 *  \date	2018
 */

/*********************************
 * Generic OS Benchmark - TESTS
 **********************************/

#include "perftest.h"
#include "hal.h"

/* I put this one here just to be sure that debugger is gonna see symbol */
volatile uint8_t end_flag;

ISR2(isr2entry_isr2)
{
  	static int i;
  	OSEE_TICK_TYPE delta;

  	delta = perf_read_measure();
  	perf_store_sample(curdata, delta, ++i);
}

ISR2(istentry_isr2)
{
  	ActivateTask(Task3);
  	perf_start_measure();
}

ISR2(istexit_isr2)
{
  	ActivateTask(Task3);
}

ISR1(isrentry_isr1)
{
  	static int i;
  	OSEE_TICK_TYPE delta;

  	delta = perf_read_measure();
  	perf_store_sample(curdata, delta, ++i);
}

ISR1(isrexit_isr1)
{
  	perf_start_measure();
}


TASK(Task1)
{
  	perf_run_task(1);
  	TerminateTask();
}

TASK(Task3)
{
  	perf_run_task(3);
  	TerminateTask();
}

TASK(Task4)
{
  	perf_run_task(4);
  	TerminateTask();
}

TASK(MainTask)
{
	DemoHAL_ISRInit();
	DemoHAL_TimerInit(1U);
  	perf_init();
  	perf_run_all();
  	perf_final_results();
  	end_flag = 1;
}


int main( void )
{
	// Temporary fix for ISR1 in x86
	set_interrupt_handler(0x24, isrentry_isr1);
	set_interrupt_handler(0x25, isrexit_isr1);

  	StartOS(OSDEFAULTAPPMODE);
  	return 0;
}
