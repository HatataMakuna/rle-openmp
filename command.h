#pragma once

#include "option.h"
#include <string>
#include <exception>
#include "common.h"

using std::string;
using std::exception;

class Command final {
	Option option;
	string srcPath;

public:
	Command(const char* argv[]);

	constexpr Option getEncode() const {
		return option;
	}

	string getSrcPath() const;
	void exec() const;
};
