
#include <fstream>

char* last = nullptr;

void freeLast() {
	if (last != nullptr)
		free(last);
}

const char* stdcin() {
	int Character = EOF;
	char* string = (char*)malloc(1);
	uint32_t i = 0;
	uint32_t size = 1;
	while ((Character = getchar()) != '\n' && Character != EOF) {
		char* buf = (char*)malloc(size);
		for (int j = 0; j < size; j++)
			buf[i] = string[i];
		string = (char*)realloc(string, size + 1);
		if (string == 0)
			exit(CONSOLE_INPUT_REALLOC_FAILURE);
		for (int j = 0; j < size; j++)
			string[i] = buf[i];
		string[i++] = Character;
		size++;
		free(buf);
	}
	string[i] = '\0';
	last = string;
	return string;

}

const char* stdfin(const char* fileName) {
	std::ifstream ifs(fileName);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	char* result = (char*)malloc(content.length() + 1);
	memcpy((void*)result, content.c_str(), content.length());
	result[content.length()] = '\0';
	return result;
}

void waitInput() {
	char Character = EOF;
	while ((Character = getchar()) != '\n' && Character != EOF);
}