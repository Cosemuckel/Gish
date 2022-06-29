#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <chrono>
#include <thread>
#include <conio.h>

#include "ttmath.h"

namespace windows {
#include <Windows.h>
}

void sleep(unsigned long long int duration) {
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	while (true) {
		if (std::chrono::steady_clock::now() - start > std::chrono::microseconds(duration))
			break;
	}
}

template <typename T>
std::string join(std::vector<T>& v, const char* delimiter) {
	std::string result = "";
	for (unsigned int i = 0; i < v.size(); i++) {
		if (i <= v.size() - 2)
			result += v[i].toString() + delimiter;
		else
			result += v[i].toString();
	}
	return result;
}

template <typename T>
std::string joinPointer(std::vector<T>& v, const char* delimiter) {
	std::string result = "";
	for (unsigned int i = 0; i < v.size(); i++) {
		if (i <= v.size() - 2)
			result += v[i]->toString() + delimiter;
		else
			result += v[i]->toString();
	}
	return result;
}

#include "ExitCodes.h"
#include "Allocator.h"
#include "Functions.h"

#include "ValueTypes.h"
#include "Global.h"
#include "Lexer.h"
#include "Nodes.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Gish.hpp"

#include "PostDefines.h"

int main(int argc, char** argv) {

	return GishClient::main();
}