
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

  	delta = DemoHAL_TimerGetValue();
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

  	delta = DemoHAL_TimerGetValue();
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
