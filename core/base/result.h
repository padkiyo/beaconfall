#pragma once

#include <iostream>
#include <source_location>

enum class ResultStatus {
	OK,
	ERROR,
};


// V = Type of value
// E = Type of error
template<typename V, typename E>
class Result {
public:
	Result(V value): m_value(value), m_status(ResultStatus::OK)
		{}
	Result(E error): m_error(error), m_status(ResultStatus::ERROR)
		{}

	// Tries to get the value else crashes if error is found
	V unwrap(const std::source_location& loc = std::source_location::current()) {
		if (m_status == ResultStatus::ERROR) {
			std::cerr
				<< "\033[31m[UNWRAP PANIC]: "
				<< loc.file_name() << ":" << loc.line() << ": "
				<< "Tried unwrapping an errored result\033[0m"
				<< std::endl;
			std::cerr
				<< "\033[31m[UNWRAP PANIC]: "
				<< loc.file_name() << ":" << loc.line() << ": "
				<< m_error << "\033[0m"
				<< std::endl;
			exit(1);
		}
		return m_value;
	}

	// Tries to get the error else crashes if value is found
	E unwrap_err(const std::source_location& loc = std::source_location::current()) {
		if (m_status == ResultStatus::OK) {
			std::cerr
				<< "\033[31m[UNWRAP PANIC]: "
				<< loc.file_name() << ":" << loc.line() << ": "
				<< "The result is not an error\033[0m"
				<< std::endl;
			exit(1);
		}
		return m_error;
	}

	bool is_ok() {
		return m_status == ResultStatus::OK;
	}

	bool is_err() {
		return m_status == ResultStatus::ERROR;
	}

private:
	V m_value;
	E m_error;
	ResultStatus m_status;
};
