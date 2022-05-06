
#include <fstream>

Allocator stdcinAllocator;

const char* stdcin() {
	int Character = EOF;
	char* string = (char*)malloc(1);
	uint32_t i = 0;
	uint32_t size = 1;
	while ((Character = getchar()) != '\n' && Character != EOF) {
		char* buf = (char*)malloc(size);
		for (unsigned int j = 0; j < size; j++)
			buf[i] = string[i];
		string = (char*)realloc(string, size + 1);
		if (string == 0)
			exit(CONSOLE_INPUT_REALLOC_FAILURE);
		for (unsigned int j = 0; j < size; j++)
			string[i] = buf[i];
		string[i++] = Character;
		size++;
		free(buf);
	}
	string[i] = '\0';
	stdcinAllocator.registerAllocation(string);
	return string;

}

const char* stdfin(const char* fileName) {
	std::ifstream ifs(fileName);
	if (ifs.fail())
		throw 0;
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	char* result = (char*)malloc(content.length() + 1);
	memcpy((void*)result, content.c_str(), content.length());
	result[content.length()] = '\0';
	stdcinAllocator.registerAllocation(result);
	return result;
}

void stdfout(const char* fileName, const char* data) {
	std::ofstream ofs(fileName, std::ios::binary);
	int s = 0;
	while (data[s] != '\0')
		s++;
	ofs << data;
	ofs.close();
}

void waitInput() {
	char Character = EOF;
	while ((Character = getchar()) != '\n' && Character != EOF);
}