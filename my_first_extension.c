/*-------------------------------------------------------------------------
 *
 * ID generate function using
 *
 * Copyright (c) 2007-2023, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2023 Ilia Evdokimov
 *
 * contrib/my_first_extension/my_first_extension.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "fmgr.h"
#include "time.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(generate_isr_id);
PG_FUNCTION_INFO_V1(validate_check_digit);

static uint8_t calculate_sum_id(uint64_t id)
{
	uint8_t checkDigit = 2;
	uint8_t sum = 0;

	while(id)
	{
		uint8_t tmp = (id % 10) * checkDigit;
		if(tmp > 9)
		{
			sum += tmp / 10 + tmp % 10;
		}
		else
		{
			sum += tmp;
		}

		id /= 10;
		checkDigit = (2 == checkDigit) ? 1 : 2;
	}

	return sum;
}

static bool is_id_valid(uint8_t id)
{
	uint8_t sum = calculate_sum_id(id);
	return sum % 10;
}

Datum
generate_isr_id(PG_FUNCTION_ARGS)
{
	text  *name = PG_GETARG_TEXT_PP(0);

	srand(time(NULL));

	uint64_t id = rand() % 100000000;

	uint8_t check_digit = calculate_sum_id( id );

	id = id * 10 + check_digit;

	return id;
}

Datum
validate_check_digit(PG_FUNCTION_ARGS)
{
	text  *name = PG_GETARG_TEXT_PP(0);

	srand(time(NULL));

	uint64_t id = rand() % 1000000000;

	return is_id_valid( id );
	
}
