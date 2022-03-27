#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <chrono>

std::vector<void*> allocationsToClear;

#include "ExitCodes.h"
#include "Functions.h"

#include "ValueTypes.h"
#include "Global.h"
#include "Lexer.h"
#include "Commenter.h"
#include "Nodes.h"
#include "Parser.h"
#include "Interpreter.h"

int main() {

	while (1) {
		printf("gish >> ");
		std::string s = stdcin();
		if (s[0] == '$')
			if (s[1] == '$')
				if (s[2] == 'x') {
					system("cls");
					continue;
				}
				else if (s[2] == 'e')
					exit(EXIT_SUCCES_USER_FORCED_FROM_INPUT);
				else if (s[2] == 'f')
					s = stdfin("text.txt");
				else if (s[2] == 'b')
					break;
		if (s.empty())
			continue;

		Position::fileCode = s;
		Position::fileName = "<stdin>";
		LexerResult lexerResult = Lexer(s).lex();
		std::cout << "\n" << lexerResult.toString() << "\n\n";
		if (lexerResult.error != null) {
			lexerResult.clear();
			continue;
		}
		CommenterResult commenterResult = Commenter(lexerResult.tokens).comment();
		std::cout << "\n" << commenterResult.toString() << "\n\n";
		ParserResult parserResult = Parser(commenterResult.tokens).parse();
		std::cout << "\n" << parserResult.toString() << "\n\n";
		if (parserResult.error != null) {
			parserResult.clear();
			continue;
		}
		RuntimeResult runtimeResult = Interpreter().run(&parserResult.node);
		std::cout << "\n" << runtimeResult.toString() << "\n\n";
		if (runtimeResult.error != null) {
			runtimeResult.clear();
			continue;
		}
		lexerResult.clear();
		parserResult.clear();
		runtimeResult.clear();
		freeLast();
		std::cout << Interpreter::context.functionTable.symbols["v"].toString() << "\n";
		// Interpreter::context.functionTable.clear();
	}
	system("pause");
} 