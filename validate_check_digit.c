/*-------------------------------------------------------------------------
 *
 * Checking ID.
 *
 * Copyright (c) 2007-2023, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2023 Ilia Evdokimov
 *
 * contrib/my_first_extension/validate_isr_id.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "fmgr.h"
#include "time.h"

#include "calculate_digit.h"

PG_FUNCTION_INFO_V1(validate_check_digit);

Datum
validate_check_digit(PG_FUNCTION_ARGS)
{
	text  *name = PG_GETARG_TEXT_PP(0);
	uint64_t id = atol(name->vl_dat);

	if (id > 999999999) ///< id must has 9 size length
	{
		return false;
	}

	return calculate_sum_id(id) % 10;
}
