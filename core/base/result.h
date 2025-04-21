#pragma once

#include <iostream>

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
	V unwrap() {
		if (m_status == ResultStatus::ERROR) {
			std::cerr << "[UNWRAP PANIC]: Tried unwrapping an errored result" << std::endl;
			exit(1);
		}
		return m_value;
	}

	// Tries to get the error else crashes if value is found
	E unwrap_err() {
		if (m_status == ResultStatus::OK) {
			std::cerr << "[UNWRAP PANIC]: The result is not an error" << std::endl;
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
