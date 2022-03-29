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

	bool disableOutput = false;

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
				else if (s[2] == 'd') {
					disableOutput = disableOutput ? false : true;
					continue;
				}
				else if (s[2] == 'b')
					break;
		if (s.empty())
			continue;


		Position::fileCode = s;
		Position::fileName = "<stdin>";

		printf("\n");

		LexerResult lexerResult = Lexer(s).lex();
		if (!disableOutput || lexerResult.error != null)
			std::cout << "\n" << lexerResult.toString() << "\n\n";
		if (lexerResult.error != null) {
			lexerResult.clear();
			continue;
		}

		CommenterResult commenterResult = Commenter(lexerResult.tokens).comment();
		if (!disableOutput)
			std::cout << "\n" << commenterResult.toString() << "\n\n";

		ParserResult parserResult = Parser(commenterResult.tokens).parse();
		if (!disableOutput || parserResult.error != null)
			std::cout << "\n" << parserResult.toString() << "\n\n";
		if (parserResult.error != null) {
			parserResult.clear();
			continue;
		}

		RuntimeResult runtimeResult = Interpreter().run(&parserResult.node);
		if(!disableOutput || runtimeResult.error != null)
			std::cout << "\n" << runtimeResult.toString(true) << "\n\n";;
		if (runtimeResult.error != null) {
			runtimeResult.clear();
			continue;
		}

		if (disableOutput)
			printf("\n\n");

		lexerResult.clear();
		parserResult.clear();
		runtimeResult.clear();
		freeLast();
		for (int i = 0; i < allocationsToClear.size(); i++) {
			delete allocationsToClear[i];
		}
		allocationsToClear.clear();
		// Interpreter::context.functionTable.clear();
	}
	system("pause");
} 