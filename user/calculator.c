//#include "user/user.h"
#include "user/float.h"

int main(int argc,char* argv[]){
	r_float a = dec_to_r_float(argv[1]);
	r_float b = dec_to_r_float(argv[3]);
	if(*argv[2] == '+'){
		print_r_float_to_dec(r_float_add(a, b), 8);
	}
	if(*argv[2] == '-'){
		print_r_float_to_dec(r_float_sub(a, b), 8);
	}
	if(*argv[2] == '*'){
		print_r_float_to_dec(r_float_mul(a, b), 8);
	}
	if(*argv[2] == '/'){
		print_r_float_to_dec(r_float_div(a, b), 8);
	}

	return 0;
}
