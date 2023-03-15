/*-------------------------------------------------------------------------
 *
 * ID generate function
 *
 * Copyright (c) 2007-2023, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2023 Ilia Evdokimov
 *
 * contrib/my_first_extension/generate_check_digit.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "fmgr.h"
#include "time.h"

#include "calculate_digit.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(generate_isr_id);

Datum
generate_isr_id(PG_FUNCTION_ARGS)
{
	text  *name = PG_GETARG_TEXT_PP(0);
	uint64_t id = atol( name->vl_dat );
	uint8_t check_digit = 0;

	if(id > 99999999) ///< id must has 8 size length
	{
		return -1;
	}

	check_digit = calculate_sum_id( id );
	id = id * 10 + check_digit;

	return id;
}
