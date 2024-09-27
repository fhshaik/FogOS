#ifndef FLOAT_H
#define FLOAT_H

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//definition of float type using a sign val, exponent, and a mantissa. 
//One can think of it as a computer implementation of scientific notation
typedef struct{
	uint8 sign;
	int exponent;
	uint64 mantissa;
} r_float;

//complex float type
typedef struct{
	r_float real;
	r_float complex;
} c_float;


r_float r_float_normalize(r_float a);

r_float r_float_add(r_float a, r_float b);
r_float r_float_sub(r_float a, r_float b);
r_float r_float_mul(r_float a, r_float b);
r_float r_float_div(r_float a, r_float b);

r_float int_to_r_float(uint64 a);

uint64 pow(int base,int exp);

void print_r_float_to_dec(r_float a, int prec);

r_float dec_to_r_float(char* dec);

void print_binary(uint64 num);

void print_uint64(uint64 val);

#endif //FLOAT_H
