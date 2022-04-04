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

namespace windows {
#include <Windows.h>
}

#include "ExitCodes.h"
#include "Functions.h"

#include "ValueTypes.h"
#include "Global.h"
#include "Lexer.h"
#include "Commenter.h"
#include "Nodes.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Gish.hpp"

int main() {

	GishClient::main();

	system("pause");
} 