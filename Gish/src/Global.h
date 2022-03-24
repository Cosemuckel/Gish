#pragma once

class Position : public Object {

public:
	int index;
	int column;
	int line;
	static std::string fileName;
	static std::string fileCode;

	Position(int index, int column, int line) {
		this->index = index;
		this->column = column;
		this->line = line;
		this->mClass = Class::Position;
	}
	Position()
		: Position(0, 0, 0) {

	}
	Position(const Position& p)
		: Position(p.index, p.column, p.line) {

	}

	Position advance(char currentChar) {
		this->index++;
		this->column++;
		if (currentChar == '\n') {
			this->line++;
			this->column = 0;
		}
		return *this;
	}

	std::string toString() {
		return std::string("from index" + std::to_string(this->index) + " in collumn " + std::to_string(this->column) + " in line" + std::to_string(this->line) + " in file" + Position::fileCode);
	}

	void clear() {
		this->fileName.~basic_string();
		this->fileCode.~basic_string();
	}
	nullCMP;
	Position nullEQ;
	Position nullCon;
};

std::string Position::fileName = "<stdin>";
std::string Position::fileCode = "";

class Error : public Object {

	std::string errorName;
	std::string details;
	Position posStart;
	Position posEnd;

public:
	Error(std::string errorName, std::string details, Position posStart, Position posEnd)
		: Error() {
		this->errorName = errorName;
		this->details = details;
		this->posStart = posStart;
		this->posEnd = posEnd;
	}
	Error(std::string details, Position posStart, Position posEnd)
		: Error() {
		this->errorName = "";
		this->details = details;
		this->posStart = posStart;
		this->posEnd = posEnd;
	}
	Error() {
		this->mClass = Class::Error;
	}

	std::string stringWithArrow(std::string text, Position posStart, Position posEnd) {
		std::string result;
		std::istringstream iss(text);
		std::string line;
		for (int i = 0; i < posStart.line; i++) {
			std::getline(iss, line);
		}
		result += line;
		result += "\n";
		for (int i = 0; i < posStart.column - 1; i++)
			result += " ";
		for (int i = posStart.column - 1; i < posEnd.column - 1; i++)
			result += "^";
		return result;
	}

	std::string toString() {
		if (this->errorName.empty())
			return std::string("\033[91m") + this->details + ("\033[0m") + "\nFile: " + this->posStart.fileName + ", Line: " + std::to_string(this->posStart.line) + "\n\n" + this->stringWithArrow(Position::fileCode, this->posStart, this->posEnd);
		return std::string("\033[91m") + this->errorName + " : " + this->details + ("\033[0m") + "\nFile: " + this->posStart.fileName + ", Line: " + std::to_string(this->posStart.line).c_str() + "\n\n" + this->stringWithArrow(Position::fileCode, this->posStart, this->posEnd) + ("\033[0m");
	}

	nullCMP;
	Error nullEQ;
	Error nullCon;
};

class IllegalCharError : public Error {
public:
	IllegalCharError(std::string details, Position posStart, Position posEnd)
		: Error("Illegal Character", details, posStart, posEnd) {
		this->mClass = Class::IllegalCharError;
	}
};

class InvalidSyntaxError : public Error {
public:
	InvalidSyntaxError(std::string details, Position posStart, Position posEnd)
		: Error("Invalid Syntax", details, posStart, posEnd) {
		this->mClass = Class::InvalidSyntaxError;
	}
};

class Result : public Object {
public:
	Error error = null;
	Result() {
		this->mClass = Class::Result;
	}
	nullCMP;
	Result nullEQ;
	Result nullCon;
};