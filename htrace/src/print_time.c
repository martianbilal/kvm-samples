/*
 * Copyright (c) 2015-2016 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2015-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "defs.h"

#include DEF_MPERS_TYPE(time_t)

#include MPERS_DEFS

SYS_FUNC(time)
{
	if (exiting(tcp)) {
		time_t t;

		if (!umove_or_printaddr(tcp, tcp->u_arg[0], &t)) {
			tprint_indirect_begin();
			PRINT_VAL_D(t);
			tprints_comment(sprinttime(t));
			tprint_indirect_end();
		}

		if (!syserror(tcp)) {
			tcp->auxstr = sprinttime((time_t) tcp->u_rval);

			return RVAL_STR;
		}
	}

	return 0;
}
