#pragma once

namespace GishClient {

	//The name of the currently executed file, and the executing code
	std::string currentCode;
	std::string currentFileName;

	//Should the outputs of the proccesses should be printed to the stdout(Read docs: config)
	bool lexingOutput = true;
	bool parsingOutput = true;
	bool interpreterOutput = true;
	bool lastInterpreterOutput = false;

//D	std::vector<Value> valueList;
//D	std::vector<Token> tokenList;
//D	std::vector<NodeWrapper> wrappedNodeList;

	//Process the currently loaded code
	bool execute() {

		//Tokenizes the code
		std::vector <Token> tokenList;
		try {
			tokenList = Lexer(&currentCode, &currentFileName).lex();
		} catch (Error e) {
			std::cout << e.toString() << std::endl;
			return false;
		}

		//Print the output created by the lexer
		if (lexingOutput)
			std::cout << "\n" << ( "[ " + join(tokenList, ", ") + " ]" ) << "\n";

		NodeWrapper* parserResult;
		//Parse the tokens
		try {
			parserResult = Parser(&tokenList).parse();
		} catch (Error e) {
			std::cout << e.toString() << std::endl;
			return false;
		}
		
		//Print the output created by the parser
		if (parsingOutput)
			std::cout << "\n" << ( parserResult->toString() ) << "\n";

		/* INTERPRETER

		//Evaluate the node tree using the interpreter
		Value* interpreterResult;
		try {
			interpreterResult = Interpreter().run(parserResult);
		} catch (Error e) {
			std::cout << e.toString() << std::endl;
			return false;
		}

		//Print the output created by the interpreter
		if (interpreterOutput)
			std::cout << "\n" << ( interpreterResult->toString() ) << "\n";

		*/

		//Compile the code
		Compiler compiler;
		compiler.compile(parserResult, currentFileName);	
		
		return true;
	}

	bool run() {

		//Process the currently loaded code
		execute();

		//Clear buffers from stdinput
		stdcinAllocator.clearAllAllocations();
		//Clear all allocated nodes, etc.
		GlobalAllocator.clearAllAllocations();

		return true;

	}

	//Runs code from prompt, 1 line at a time
	bool runFromInput() {

		while (1) {
			printf("\n\033[1mgish \033[32m>>\033[0m ");
			//Get the user input
			std::string s = stdcin();
			//Exit, if the user typed exit
			if (s == "exit")
				return true;
			//Clear the screen buffer, if the user typed clear
			if (s == "clear") {
				system("cls");
				continue;
			}
			//Do not process emty code
			if (s.empty())
				continue;

			//Load the input into the code, and set filename to stdin
			currentCode = s;
			currentFileName = "stdin";
			//Process the code
			if (!run())
				return false;
		}
		return true;
	}

	//Processes a file
	bool runFromFile() {
		printf("\033[1m\033[36mEnter the path to the file to open\n \033[0m");
		//Get the file path
		std::string path =  stdcin();
		printf("\033[32mLoading %s\033[0m", path.c_str());
		std::string data;
		bool threadRunning = true;
		bool rReturn = false;
		//Creates thread loading the file
		std::thread thread([=, &data, &threadRunning, &rReturn]() {
			try {
				//Try to load the code from the file
				data = stdfin(path.c_str());
			}
			catch (int) {
				//In the case that the file was not found
				printf("\n\033[31mCouldn't open file %s, make sure it exist!\033[0m\n", path.c_str());
				threadRunning = false;
				rReturn = true;
			}
			threadRunning = false;
		});
		printf("\n");
		//As long the file is beeing loaded, there will be a small animation, to show the user, that the program has not stalled
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
			return false;
		}

		//Load the file into code, and set the filename
		currentCode = data;
		currentFileName = path;
		printf("\033[32mLoaded %s\n\033[0m", path.c_str());
		printf("\033[32mProccessing %s\n\033[0m", path.c_str());
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		//Procces the code
		run();
		printf("\033[32mExecution finished in %fs\n\033[0m", std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count());
		printf("Press any key to continue...\n");
		//Wait for user to terminate
		waitInput();
		
		return true;

	}

	const char* welcome = "Gish opened, choose what you want to do:\n 1: run file, return to this screen\n 2: run file, delete all data and return to this screen\n 3: run file and then continue in terminal mode\n 4: start terminal mode\n 5: reset all data\n 6: open config\nor type 'exit' to exit program\n\n";

	int main() {
		char c = 0x00;

		while (true) {
			system("cls");
			//Print the menu, and get the selection
			printf(welcome);
			std::string option = stdcin();

			//Runs from file, keeping data for further code executions
			if (option == "1") {
				system("cls");
				runFromFile();
				continue;
			}
			//Runs from file, deleting all data
			else if (option == "2") {
				system("cls");
				runFromFile();
//R				mainContext.clear();
				continue;
			}
			//Runs from file, keeping data for further code executions, directly starts prompt mode
			else if (option == "3") {
				system("cls");
				runFromFile();
				runFromInput();
				continue;
			}
			//Starts prompt mode
			else if (option == "4") {
				system("cls");
				printf(welcome);
				runFromInput();
				continue;
			}
			//Clears all data
			else if (option == "5") {
//R				mainContext.clear();
//R				// Parser::functions.clear();
				continue;
			}
			//Opens the configurations
			else if (option == "6") {
				while (1) {
					system("cls");
					printf("Config:\n 1: Output lexing output: \033[90m........\033[0m");
					printf(lexingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
					printf(" 2: Output parsing output: \033[90m.......\033[0m");
					printf(parsingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
					printf(" 3: Output interpreter output: \033[90m...\033[0m");
					printf(interpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
					printf(" 4: Output last result output: \033[90m...\033[0m");
					printf(lastInterpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
					printf(" 5: show all defined Symbols\n");
					printf("or type 'exit' to exit config\n\n");

					//Config selection
					std::string config = stdcin();

					//Change the output settings
					if (config == "1")
						lexingOutput ^= true;
					else if (config == "2")
						parsingOutput ^= true;
					else if (config == "3")
						interpreterOutput ^= true;
					else if (config == "4")
						lastInterpreterOutput ^= true;
					//List defined symbols
					else if (config == "5") {
						//Clear input number
						system("cls");
						printf("Config:\n 1: Output lexing output: \033[90m........\033[0m");
						printf(lexingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 2: Output parsing output: \033[90m.......\033[0m");
						printf(parsingOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 3: Output interpreter output: \033[90m...\033[0m");
						printf(interpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 4: Output last result output: \033[90m...\033[0m");
						printf(lastInterpreterOutput ? "\033[32m true \033[0m\n" : "\033[31m false \033[0m\n");
						printf(" 5: show all defined Symbols\n");
						printf("or type 'exit' to exit config\n\n");

						//List all Variables
						printf("  -Variables:\n");
//R						auto it = mainContext.symbolTable.symbols.begin();
/*						for (size_t i = 0; i < mainContext.symbolTable.symbols.size(); i++) {
							printf("   %s %s: %s", it->second.Name().c_str(), it->first.c_str(), it->second.toString().c_str());
							std::advance(it, 1);
						}
						//Print NaN to show there are no defined Variables
						if (!mainContext.symbolTable.symbols.size())
							printf("   NaN\n");

						//List all functions/methods
						printf("  -Functions:\n");
						auto it2 = mainContext.functionTable.symbols.begin();
						for (size_t i = 0; i < mainContext.functionTable.symbols.size(); i++) {
							printf("   %s %s: %s", Value::typeName(it2->second.returnType).c_str(), it2->first.c_str(), it2->second.argumentString().c_str());
							std::advance(it2, 1);
						}
						//Print NaN to show there are no defined Methods
						if (mainContext.functionTable.symbols.size() == 0)
							printf("   NaN\n");
*/
						//Wait for user to exit
						waitInput();
					}

					//Exit the config
					else if (config == "exit") {
						break;
					}
				}
				continue;
			}
			//Exit gish
			else if (option == "exit") {
				return 0;
			}

		}

		return -1;

	}



};
