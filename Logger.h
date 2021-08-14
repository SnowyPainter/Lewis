#pragma once

#include <iostream>

class Logger {
private:
	std::string section = "";
public:
	Logger(std::string context_section) {
		section = context_section;
	}
	void Log(std::string detail) {
		std::cout << "[ " << section << " ] " << detail << std::endl;
		return;
	}
	void Error(std::string detail) {
		std::cout << "[ " << section << " ] Error : " << detail << std::endl;
		return;
	}
};