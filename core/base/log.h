#pragma once
#include <stdio.h>

#define panic(x, ...) \
	do {\
		if (!(x)) {\
			fprintf(stderr, "\033[31m[ASSERTION]: %s:%d:\033[0m ", __FILE__, __LINE__);\
			fprintf(stderr, " " __VA_ARGS__);\
			fprintf(stderr, "\n");\
			exit(1);\
		}\
	} while(0)

#define LOG_NORMAL 0
#define LOG_ERROR  91
#define LOG_SUCESS 92
#define LOG_WARN   93
#define LOG_INFO   95

#define log_typed(type, ...)         \
	({                                 \
		printf("\033[%dm", type);        \
		printf(__VA_ARGS__);             \
		printf("\033[%dm", LOG_NORMAL);  \
	})

#define log_sucess(...)                   \
	({                                      \
		log_typed(LOG_SUCESS, "[SUCESS]: ");  \
		log_typed(LOG_SUCESS, __VA_ARGS__);   \
	})

#define log_warn(...)                     \
	({                                      \
		log_typed(LOG_WARN,   "[WARN]:   ");  \
		log_typed(LOG_WARN, __VA_ARGS__);     \
	})

#define log_error(...)                    \
	({                                      \
		log_typed(LOG_ERROR,  "[ERROR]:  ");  \
		log_typed(LOG_ERROR, __VA_ARGS__);    \
	})

#define log_info(...)                     \
	({                                      \
		log_typed(LOG_INFO,   "[INFO]:   ");  \
		log_typed(LOG_INFO, __VA_ARGS__);     \
	})
