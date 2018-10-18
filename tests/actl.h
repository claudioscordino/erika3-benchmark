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

/** \file	actl.h
 *  \brief	Test: ACTL
 *
 *  Activate a low priority task and measure how much time it takes
 *  to return to the caller.
 *
 *  \author	Errico Guidieri
 *  \date	2018
 */
#ifndef __ACTL_H__
#define __ACTL_H__

#include "../hal.h"
#include "../test.h"

static void actl_setup(struct perftest *data)
{
}

static void actl_main(struct perftest *data)
{
	int i;

	for (i = 0; i < PERF_LOOPS; i++)
		ActivateTask(Task4);
}

static void actl_task1(struct perftest *data)
{
}

static void actl_task3(struct perftest *data)
{
}

static void actl_task4(struct perftest *data)
{
	OSEE_TICK_TYPE delta;
	static int i;

	perf_start_measure();

	ActivateTask(Task3);

	delta = perf_read_measure();
	perf_store_sample(data, delta, ++i);
}

static void actl_cleanup(struct perftest *data)
{
	perf_finalize(data);
}

#endif /* __ACTL_H__ */
