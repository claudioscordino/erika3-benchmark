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

/** \file	test.h
 *  \brief	Header for code to run tests.
 *
 *  \author	Errico Guidieri, Claudio Scordino
 *  \date	2018
 */
#ifndef __PERFTEST_H__
#define __PERFTEST_H__

#include <inmate.h>
#include "ee.h"

#define PERF_LOOPS  5

static inline void perf_start_measure( void );

static inline OSEE_TICK_TYPE perf_read_measure( void );

/*
 * Each test has right to store its own results into a perftest structure.
 * All values expressed in ns.
 * Test descriptor:
 * - setup() is called when the execution of the single test begins;
 * - run() is called to execute the test, and is supposed to generate
 *   all the necessary events and to collect stats on them;
 * - cleanup() restores the initial condition.
 */
struct perftest {
  	const char *test_name;
  	void (*setup)(struct perftest *data);
  	void (*main)(struct perftest *data);
  	void (*task1)(struct perftest *data);
  	void (*task3)(struct perftest *data);
  	void (*task4)(struct perftest *data);
  	void (*cleanup)(struct perftest *data);
  	OSEE_TICK_TYPE mean;
  	OSEE_TICK_TYPE max;
  	OSEE_TICK_TYPE min;
};

static struct perftest alltests[];
static int perf_test;

#define curdata (&alltests[perf_test])

/* Common code to update the values of the measures. */
static void perf_store_sample(struct perftest *data,
  		OSEE_TICK_TYPE sample, uint32_t n);


static void perf_init(void);

/* Common code to run the tests, to be used in code.c */
static void perf_run_all(void);

void perf_finalize(struct perftest *data);
static void perf_final_results ( void );


/*
 * NOTE: The two dereferences add a constant offset to the measure: may
 * this be considered too much we should move the measure to the entry of
 * the task itself.
 */
#define perf_run_task(id) do {                  \
  	struct perftest *test = &alltests[perf_test]; \
  	test->task ## id (test);                      \
} while (0)

#endif /* __PERFTEST_H__ */
