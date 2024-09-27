//float.c
#include "user/float.h"
#include "user/user.h"

//normalizes the float type so it maintains max precision and doesnt overflow
r_float r_float_normalize(r_float a){
	while(a.mantissa >= (((uint64)1) << 63)){
		a.mantissa >>= 1;
		a.exponent++;
	}	

	while(a.mantissa < (((uint64)1) << 62) && a.mantissa != 0){
		a.mantissa <<= 1;
		a.exponent -= 1;
	}
	a.exponent = (int) a.exponent;
	return a;
}

// adds two floats
r_float r_float_add(r_float a, r_float b){
	while(a.exponent > b.exponent){
		b.mantissa >>= 1;
		b.exponent++;
	}

	while(b.exponent > a.exponent){
		a.mantissa >>= 1;
		a.exponent++;
	}

	r_float result;
	result.exponent = a.exponent;
	if(a.sign == b.sign){
		result.mantissa = a.mantissa + b.mantissa;
		result.sign = a.sign;
	}
	else{
		if(a.mantissa > b.mantissa){
			result.mantissa = a.mantissa - b.mantissa;
			result.sign = a.sign;	
		}
		else{
			result.mantissa = b.mantissa - a.mantissa;
			result.sign = b.sign;
		}
	}
	return r_float_normalize(result);
}

//subtract two floats
r_float r_float_sub(r_float a, r_float b){
	b.sign ^= 1;
	return r_float_add(a, b);
}

//multiplies two floats
r_float r_float_mul(r_float a, r_float b){
	r_float result;

	result.sign = a.sign ^ b.sign;

	a.mantissa >>= 31;
	b.mantissa >>= 31;
	
	result.mantissa = (a.mantissa * b.mantissa);

	result.exponent = a.exponent + b.exponent; 

	return r_float_normalize(result);
}

//divides a float by another
r_float r_float_div(r_float a, r_float b){

	r_float result;
	if(b.mantissa == 0){
		result.sign = 0;
		result.mantissa = 0;
		result.exponent = 0;
		return result;
	}

	result.sign = a.sign ^ b.sign;
	b.mantissa >>= 31;
	result.mantissa = a.mantissa / b.mantissa;

	result.exponent = a.exponent - b.exponent + 31;
	return r_float_normalize(result);
}

//normalizes mantissa
uint64 mantissa_normalize(uint64 a){
	while(a >= (((uint64)1) << 63)){
		a >>= 1;
	}	
	while(a < (((uint64)1) << 62) && a != 0){
		a <<= 1;
	}
	return a;	
}

//takes an unsigned 64 bit int and turns it into a float
r_float int_to_r_float(uint64 a){
	r_float result;
	if(a < 0){
		result.sign = 1;
		a = -a;
	}
	else{
		result.sign = 0;
	}

	if(a == 0){
		result.mantissa = 0;
		result.exponent = 0;
		return result;
	}
	
	result.mantissa = (uint64) a;
	result.exponent = 0;
	
	
	while(result.mantissa < (((uint64)1)<<62)){
		result.mantissa <<= 1;
		result.exponent--;
	}
	result.exponent += 62;

	return r_float_normalize(result);
}

//helper function for finding the power of a number
uint64 pow(int base, int exp){
	int i = 0;
	uint64 result = 1;
	while(i <= exp){
		result *= (uint64) base;
		i++;
	}
	return result;
}

//takes a float and a precision and prints float on terminal
void print_r_float_to_dec(r_float a, int prec){

	uint64 first_part = ((a.exponent >= 0) ? (a.mantissa >> (62 - a.exponent)) : 0);

	uint64 scale_factor = pow(10, prec);
	int i = 0;

	uint64 second_part = (uint64) ((a.exponent >= 0) ? a.mantissa << a.exponent : a.mantissa >> (-a.exponent));

	uint64 second_result = 0;

	while (i < 60){
		if ((int)((second_part >> (62 - i - 1))&1) == 1){
			second_result += (uint64)(scale_factor >> (i+1));
		}
		
		i++;
	}
	uint64 temp = second_result;
	int j = 0;
	while(temp != 0){
		temp /= 10;
		j++;
	}
	
	j = prec - j + 1;
	i = 0;

	char zero_buff[64];
	while(j > 0){

		*(zero_buff+i) = '0';
		j--;
		i++;
	}
	*(zero_buff+i) = '\0';

	second_result = (((second_result + 50)/100) * 10);
	while((second_result%10) == 0){
		second_result /= 10;
	}
	if (a.sign){
		printf("-");
	}
	print_uint64(first_part);
	printf(".%s", zero_buff);

	print_uint64(second_result);
	printf("\n");

	return;
}

//takes a decimal string and turns it into a float
r_float dec_to_r_float(char* dec){

	int i = 0;
	uint64 first_part = 0;

	while(*(dec+i) != '.'){

		first_part *= 10;
		first_part += (*(dec+i) - '0');
		i++;
	}
	r_float a = int_to_r_float(first_part);

	i++;
	int j = -1;
	uint64 second_part = 0;
	while(*(dec+i) != '\0'){
		second_part *= 10;
		second_part += (*(dec+i) - '0');
		i++;
		j++;
	}

	r_float b = int_to_r_float(second_part);
	r_float c = int_to_r_float(pow(10, j));

	b = r_float_div(b, c);

	return r_float_add(a, b);
}

//helper function for printing a binary string from a uint64
void print_binary(uint64 num){
	for(int i = 63; i >= 0; i--){
		printf("%d", (int)((num>>i)&1));
		
	}
	printf("\n");
	return;
}

//helper function to print a uint64
void print_uint64(uint64 val){
	if (val == 0){
		printf("%d", 0);
		return;
	}
	char buffer[32];
	int index = 0;
	while (val > 0){
		buffer[index++] = (((char)(val % 10)) + '0');
		val /= 10;
	}
	while(index > 0){
		index -= 1;
		printf("%c", buffer[index]);
	}
	return;
}

