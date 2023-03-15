/*-------------------------------------------------------------------------
 *
 * Calculation of last digit
 *
 * Copyright (c) 2007-2023, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2023 Ilia Evdokimov
 *
 * contrib/my_first_extension/calculate_digit.c
 *
 *-------------------------------------------------------------------------
 */

#include "calculate_digit.h"

uint8_t calculate_sum_id(uint64_t id)
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
