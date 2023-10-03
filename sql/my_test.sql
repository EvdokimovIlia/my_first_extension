CREATE EXTENSION my_first_extension;

SELECT generate_check_digit('327');
SELECT generate_check_digit('816');
SELECT generate_check_digit('-16');
SELECT generate_check_digit('0');
SELECT generate_check_digit('Hi');

SELECT validate_check_digit('3283');
SELECT validate_check_digit('123');
SELECT validate_check_digit('0');
