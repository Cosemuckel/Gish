#pragma once

namespace GishClient {
	
	std::string currentCode;
	std::string currentFileName;

	bool lexingOutput = false;
	bool parseingOutput = false;
	bool interpreterOutput = false;
	bool lastInterpreterOutput = false;

	Context mainContext = Context();

	void execute() {
		Position::fileCode = currentCode;
		Position::fileName = currentFileName;

		LexerResult lexerResult = Lexer(currentCode).lex();

		if (lexingOutput || lexerResult.error != null)
			std::cout << "\n" << lexerResult.toString() << "\n";
		if (lexerResult.error != null) {
			lexerResult.clear();
			return;
		}

		CommenterResult commenterResult = Commenter(lexerResult.tokens).comment();
		if (lexingOutput)
			std::cout << "\n" << commenterResult.toString() << "\n";

		ParserResult parserResult = Parser(commenterResult.tokens).parse();
		if (parseingOutput || parserResult.error != null)
			std::cout << "\n" << parserResult.toString() << "\n";
		if (parserResult.error != null) {
			parserResult.clear();
			return;
		}

		RuntimeResult runtimeResult = Interpreter().run(&parserResult.node, mainContext);
		if (interpreterOutput || runtimeResult.error != null)
			std::cout << "\n" << runtimeResult.toString() << "\n";;
		if (runtimeResult.error != null) {
			runtimeResult.clear();
			return;
		}

		if (lastInterpreterOutput)
			std::cout << "\n" << runtimeResult.toString(true) << "\n";;

		lexerResult.clear();
		parserResult.clear();
		runtimeResult.clear();
	
	}
	
	void run() {

		execute();

		freeLast();
		GloabalAllocator.clearAllAllocations();

	}

	void runFromInput() {

		while (1) {
			printf("\n\033[1mgish \033[32m>>\033[0m ");
			std::string s = stdcin();
			if (s == "exit")
				return;
			if (s == "clear") {
				system("cls");
				continue;
			}
			if (s.empty())
				continue;

			currentCode = s;
			currentFileName = "<stdin>";
			run();
		}

	}

	void runFromFile() {
		printf("Path to the file to open\n");
		std::string path = stdcin();
		printf("Loading %s\n", path.c_str());
		std::string data;
		try {
			data = stdfin(path.c_str());
		}
		catch (int) {
			printf("Couldn't open file %d, make sure it exist!", path.c_str());
			return;
		}
		currentCode = data;
		currentFileName = path;
		run();
	}

	const char* welcome = "Gish opened, choose what you want to do:\n 1: run file, return to this screen\n 2: run file and then continue in terminal mode\n 3: start terminal mode\n 4: reset all data\n 5: open config\nor type 'exit' to exit program\n\n";

	void main() {

		while (true) {
			system("cls");
			printf(welcome);
			std::string option = stdcin();

			if (option == "1") {
				system("cls");
				runFromFile();
				continue;
			}
			else if (option == "2") {
				system("cls");
				runFromFile();
				runFromInput();
				continue;
			}
			else if (option == "3") {
				system("cls");
				printf(welcome);
				runFromInput();
				continue;
			}
			else if (option == "4") {
				mainContext.clear();
			}
			else if (option == "5") {
				while (1) {
					system("cls");
					printf("Config:\n 1: Output lexing output: \033[90m........\033[0m");
					printf(lexingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
					printf(" 2: Output parsing output: \033[90m.......\033[0m");
					printf(parseingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
					printf(" 3: Output interpreter output: \033[90m...\033[0m");
					printf(interpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
					printf(" 4: Output last result output: \033[90m...\033[0m");
					printf(lastInterpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");

					printf("or type 'exit' to exit config\n\n");

					std::string config = stdcin();

					if (config == "1")
						lexingOutput = !lexingOutput;
					if (config == "2")
						parseingOutput = !parseingOutput;
					if (config == "3")
						interpreterOutput = !interpreterOutput;
					if (config == "4")
						lastInterpreterOutput = !lastInterpreterOutput;

					else if (config == "exit")
						break;
				}
			}
			else if (option == "exit") {
				exit(0);
			}
			else {

			}

		}

	}



};