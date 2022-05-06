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

int main() {

	return GishClient::main();

} 