#pragma once
#include "common.h"

#define GLC(x)\
	(\
		gl_clear_error(),\
		x\
	);\
	gl_get_error(#x, __FILE__, __LINE__);\

void gl_clear_error();
void gl_get_error(const char* function, const char* file, int line);
