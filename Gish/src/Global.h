
class Position {
public:
	
	int character;
	int column;
	int line;

	std::string* fileCode;
	std::string* fileName;

	//Starting to count at 1, to mach the non programmative way of counting
	Position(int index, int column, int line, std::string* fileCode, std::string* fileName)
		: character(index), column(column), line(line), fileCode(fileCode), fileName(fileName) {}

	void inherit(const Position& position) {
		this->character = position.character;
		this->column = position.column;
		this->line = position.line;
		this->fileCode = position.fileCode;
		this->fileName = position.fileName;
	}

	Position()
		: Position(0, 0, 0, 0, 0) {}

	Position(const Position& position)
		: Position() {
		this->inherit(position);
	}

	Position* advance(char currentChar) {
		this->character++;
		this->column++;
		if (currentChar == '\n') {
			this->line++;
			this->column = 0;
		}
		return this;
	}

	Position advanced(char currentChar) {
		Position p(*this);
		p.advance(currentChar);
		return p;	
	}

	Position* copy() {
		return GlobalAllocator.allocate(Position(*this));
	}

	std::string toString() {
		return std::string("(") + std::to_string(this->character) + "," + std::to_string(this->column) + "," + std::to_string(this->line) + ")";
	}

};

class Error {
	

	std::string errorName;
	std::string errorMessage;
	Position* positionStart;
	Position* positionEnd;

public:
	
	Error(std::string errorName, std::string errorMessage, Position* positionStart, Position* positionEnd)
		: errorName(errorName), errorMessage(errorMessage), positionStart(positionStart), positionEnd(positionEnd) {}

	Error(std::string errorName, std::string errorMessage, Position* positionStart)
		: Error(errorName, errorMessage, positionStart, positionStart) {}

	Error() 
		: Error("", "", 0, 0) {}

	std::string stringWithArrow(std::string* text, Position* positionStart, Position* positionEnd) {
		std::string result = "";
		std::istringstream iss(*text);
		std::string line;
		for (int i = 0; i < positionStart->line; i++) {
			std::getline(iss, line);
		}
		result += line + "\n";
		for (int i = 0; i < positionStart->column - 1; i++) {
			result += " ";
		}
		for (int i = positionStart->character - 1; i < positionEnd->character - 1; i++) {
			result += "^";
		}
		result += "\n";
		return result;
	}

	std::string toString() {
		if (this->errorName.empty())
			return std::string("\033[91m") + this->errorMessage + ("\033[0m") + "\nFile: " + *this->positionStart->fileName + ", Line: " + std::to_string(this->positionStart->line) + "\n\n" + this->stringWithArrow(this->positionStart->fileCode, this->positionStart, this->positionEnd);
		return std::string("\033[91m") + this->errorName + " : " + this->errorMessage + ("\033[0m") + "\nFile: " + *this->positionStart->fileName + ", Line: " + std::to_string(this->positionStart->line).c_str() + "\n\n" + this->stringWithArrow(this->positionStart->fileCode, this->positionStart, this->positionEnd) + ("\033[0m");
	}

};

class IllegalCharError : public Error {
	public:
	IllegalCharError(std::string details, Position* positionStart, Position* positionEnd)
		: Error("Illegal character", details, positionStart, positionEnd) {}
};

class InvalidSyntaxError : public Error {
	public:
	InvalidSyntaxError(std::string details, Position* positionStart, Position* positionEnd)
		: Error("Invalid syntax", details, positionStart, positionEnd) {}
};

class RuntimeError : public Error {
	public:
	RuntimeError(std::string details, Position* positionStart, Position* positionEnd)
		: Error("Runtime error", details, positionStart, positionEnd) {}
};

class Result {
	
public:

	Error* error;
	
	Result() : error(0) {}	

};