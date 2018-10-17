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

/** \file	perftest.h
 *  \brief	Code to run tests.
 *
 *  \author	Errico Guidieri, Claudio Scordino
 *  \date	2018
 */
#ifndef __PERFTEST_H__
#define __PERFTEST_H__

#include "osperf.h"

#define PERF_LOOPS  5

/*
 * Each test has right to store its own results into a perfdata structure.
 * All values expressed in ns.
 */
struct perfdata {
  	OSEE_TICK_TYPE mean;
  	OSEE_TICK_TYPE max;
  	OSEE_TICK_TYPE min;
  	const char *test_name;
};

/* Common code to update the values of the measures. */
static void perf_store_sample(struct perfdata *data,
  		OSEE_TICK_TYPE sample, uint32_t n)
{
  	data->mean = (((n - 1U) * data->mean) + sample) / n;

  	if(sample > data->max) {
    		data->max = sample;
  	}

  	if((sample < data->min) || (data->min == 0U)) {
    		data->min = sample;
  	}
}

/*
 * Test descriptor:
 * - setup() is called when the execution of the single test begins;
 * - run() is called to execute the test, and is supposed to generate
 *   all the necessary events and to collect stats on them;
 * - cleanup() restores the initial condition.
 */
struct perftest {
  	void (*setup)(struct perfdata *data);
  	void (*main)(struct perfdata *data);
  	void (*task1)(struct perfdata *data);
  	void (*task3)(struct perfdata *data);
  	void (*task4)(struct perfdata *data);
  	void (*cleanup)(struct perfdata *data);
};

static struct perftest alltests[];
static struct perfdata alldata[];
static int perf_test;

#define curdata (&alldata[perf_test])

/* Supported tests. */
#include "tests/act.h"
#include "tests/actl.h"
#include "tests/intdisable.h"
#include "tests/intenable.h"

#define PERF_ENABLE(name)  { 0, 0 },
static struct perfdata alldata[] = {
#include "perflist.h"
};
#undef PERF_ENABLE

#define PERF_ENABLE(name)  { 0, 0, 0, 0, 0, 0 },
static struct perftest alltests[] = {
#include "perflist.h"
	{ 0, 0, 0, 0, 0, 0 }
};
#undef PERF_ENABLE

#define PERF_ENABLE(name) {               \
  	alltests[i].setup = name ## _setup;     \
  	alltests[i].main = name ## _main;       \
  	alltests[i].task1 = name ## _task1;     \
  	alltests[i].task3 = name ## _task3;     \
  	alltests[i].task4 = name ## _task4;     \
  	alltests[i].cleanup = name ## _cleanup; \
  	alldata[i].mean = 0U;                   \
  	alldata[i].max  = 0U;                   \
  	alldata[i].min  = 0U;                   \
  	alldata[i].test_name  = #name;          \
  	i++; }

volatile unsigned alltest_size;

static void perf_init(void)
{
  	int i = 0;

#include "perflist.h"
  	/* just to be sure to end the test suite */
  	alltests[i].main = 0;
  	alltest_size = i;
}
#undef PERF_ENABLE

/* Common code to run the tests, to be used in code.c */
static void perf_run_all(void)
{
  	struct perftest *test;
  	struct perfdata *data;

  	perf_test = 0;
  	test = &alltests[0];

  	do {
    		data = &alldata[perf_test];

    		test->setup(data);
    		test->main(data);
    		test->cleanup(data);

    		test = &alltests[++perf_test];
  	} while (test->main);
}

void perf_finalize(struct perfdata *data);
void perf_finalize(struct perfdata *data)
{
	/*
 	 * Put here any code that must be executed by all tests
	 *  once the test is finished.
	 */
}


static void perf_final_results ( void )
{
	/*
 	 * Put here any code to be executed once the test suite
	 * has finished execution (e.g. to normalize data).
	 */
}


/*
 * NOTE: The two dereferences add a constant offset to the measure: may
 * this be considered too much we should move the measure to the entry of
 * the task itself.
 */
#define perf_run_task(id) do {                  \
  	struct perftest *test = &alltests[perf_test]; \
  	struct perfdata *data = &alldata[perf_test];  \
  	test->task ## id (data);                      \
} while (0)

#endif /* __PERFTEST_H__ */
