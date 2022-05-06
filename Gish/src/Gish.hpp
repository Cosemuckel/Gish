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


		ParserResult parserResult = Parser(lexerResult.tokens).parse();
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

		stdcinAllocator.clearAllAllocations();
		GlobalAllocator.clearAllAllocations();

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
		printf("\033[1m\033[36mEnter the path to the file to open\n \033[0m");
		std::string path =  stdcin();
		printf("\033[32mLoading %s\033[0m", path.c_str());
		std::string data;
		bool threadRunning = true;
		bool rReturn = false;
		std::thread thread([=, &data, &threadRunning, &rReturn]() { 		
			try {
				data = stdfin(path.c_str());
			}
			catch (int) {
				printf("\n\033[31mCouldn't open file %s, make sure it exist!\033[0m\n", path.c_str());
				threadRunning = false;
				rReturn = true;
			}
			threadRunning = false;
		});
		printf("\n");
		while (threadRunning) {
			sleep(500 * 1000);
			if (!threadRunning)
				break;
			printf("\r.");
			sleep(500 * 1000);
			if (!threadRunning)
				break;
			printf("\r..");
			sleep(500 * 1000);
			printf("\r...");
			sleep(500 * 1000);
			printf("\r   \r");
		}
		thread.join();
		if (rReturn) {
			system("pause");
			return;
		}

		currentCode = data;
		currentFileName = path;
		printf("\033[32mLoaded %s\n\033[0m", path.c_str());
		printf("\033[32mProccessing %s\n\033[0m", path.c_str());
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		run();
		printf("\033[32mExecution finished in %fs\n\033[0m", std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count());
		printf("Press any key to continue...\n");
		waitInput();
	}

	const char* welcome = "Gish opened, choose what you want to do:\n 1: run file, return to this screen\n 2: run file, delete all data and return to this screen\n 3: run file and then continue in terminal mode\n 4: start terminal mode\n 5: reset all data\n 6: open config\nor type 'exit' to exit program\n\n";

	int main() {
		char c = 0x00;

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
				mainContext.clear();
				continue;
			}
			else if (option == "3") {
				system("cls");
				runFromFile();
				runFromInput();
				continue;
			}
			else if (option == "4") {
				system("cls");
				printf(welcome);
				runFromInput();
				continue;
			}
			else if (option == "5") {
				mainContext.clear();
				Parser::functions.clear();
				continue;
			}
			else if (option == "6") {
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
					printf(" 5: show all defined Symbols\n");
					printf("or type 'exit' to exit config\n\n");

					std::string config = stdcin();

					if (config == "1")
						lexingOutput ^= true;
					else if (config == "2")
						parseingOutput ^= true;
					else if (config == "3")
						interpreterOutput ^= true;
					else if (config == "4")
						lastInterpreterOutput ^= true;
					else if (config == "5") {
						system("cls");
						printf("Config:\n 1: Output lexing output: \033[90m........\033[0m");
						printf(lexingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 2: Output parsing output: \033[90m.......\033[0m");
						printf(parseingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 3: Output interpreter output: \033[90m...\033[0m");
						printf(interpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 4: Output last result output: \033[90m...\033[0m");
						printf(lastInterpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 5: show all defined Symbols\n");
						printf("or type 'exit' to exit config\n\n");

						printf("  -Variables:\n");
						auto it = mainContext.symbolTable.symbols.begin();
						for (size_t i = 0; i < mainContext.symbolTable.symbols.size(); i++) {
							printf("   %s %s: %s", it->second.Name().c_str(), it->first.c_str(), it->second.toString().c_str());
							std::advance(it, 1);
						}
						if (mainContext.symbolTable.symbols.size() == 0)
							printf("   NaN\n");
						printf("  -Functions:\n");
						auto it2 = mainContext.functionTable.symbols.begin();
						for (size_t i = 0; i < mainContext.functionTable.symbols.size(); i++) {
							printf("   %s %s: %s", Value::Name(it2->second.returnType).c_str(), it2->first.c_str(), it2->second.argumentString().c_str());
							std::advance(it2, 1);
						}
						if (mainContext.functionTable.symbols.size() == 0)
							printf("   NaN\n");
						waitInput();
					}

					else if (config == "exit") {
						break;
					}
				}
				continue;
			}
			else if (option == "exit") {
				return 0;
			}
			else {

			}

		}

		return -1;

	}



};