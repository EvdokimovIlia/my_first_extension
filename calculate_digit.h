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

#include <stdint.h>

uint8_t calculate_sum_id(uint64_t id);
