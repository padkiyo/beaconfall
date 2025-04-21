#pragma once
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <source_location>

namespace log {
	inline void log_internal(
		const char* level,
		const char* fmt,
		va_list args,
		const std::source_location location = std::source_location::current()
	) {
		std::fprintf(stderr, "[%s] %s:%d: ", level, location.file_name(), location.line());
		std::vfprintf(stderr, fmt, args);
		std::fprintf(stderr, "\n");
	}

	inline void info(const char* fmt, const std::source_location loc = std::source_location::current(), ...) {
		va_list args;
		va_start(args, loc);
		log_internal("INFO", fmt, args, loc);
		va_end(args);
	}

	inline void success(const char* fmt, const std::source_location loc = std::source_location::current(), ...) {
		va_list args;
		va_start(args, loc);
		log_internal("SUCCESS", fmt, args, loc);
		va_end(args);
	}

	inline void error(const char* fmt, const std::source_location loc = std::source_location::current(), ...) {
		va_list args;
		va_start(args, loc);
		log_internal("ERROR", fmt, args, loc);
		va_end(args);
	}

	inline void panic(const char* fmt, const std::source_location loc = std::source_location::current(), ...) {
		va_list args;
		va_start(args, loc);
		log_internal("PANIC", fmt, args, loc);
		va_end(args);
		std::exit(EXIT_FAILURE);
	}
}
