#pragma once

class TokenType {
public:
	std::string Class;
	std::string type;

	TokenType(std::string Class, std::string type) {
		this->Class = Class;
		this->type = type;
	}
	TokenType(const TokenType& tokenType) {
		this->Class = tokenType.Class;
		this->type = tokenType.type;
	}

	std::string toString() {
		if (this->type.empty())
			return "\033[90m" + Class + "\033[0m";
		return "\033[90m" + Class + "\033[0m" + ':' + type;
	}

	bool equals(const TokenType& t) {
		return this->Class == t.Class && this->type == t.type;
	}

	void clear() {
		this->Class.~basic_string();
		this->type.~basic_string();
	}

};

#include "Tokens.h"

class Token {

public:

	TokenType* tokenType = nullptr;
	Position* startPos = nullptr;
	Position* endPos = nullptr;
	Value* value = nullptr;

	std::string toString() {
		if (this->value != nullptr && this->value->type != Value::Type::Null)
			return tokenType->toString() + ":\033[36m" + value->toString() + "\033[0m";
		else return tokenType->toString();
	}

	//Copy constructor
	Token(const Token& token) {
		this->tokenType = token.tokenType;
		this->startPos = token.startPos;
		this->endPos = token.endPos;
		this->value = token.value;
	}
	
	Token(TokenType* tokenType, Position* startPos, Position* endPos, Value* value) {
		this->tokenType = tokenType;
		this->startPos = startPos;
		this->endPos = endPos;
		this->value = value;
	}

	Token(TokenType* tokenType, Position* startPos, Value* value) {
		this->tokenType = tokenType;
		this->startPos = startPos;
		this->value = value;
	}

	Token(TokenType* tokenType, Position* startPos) {
		this->tokenType = tokenType;
		this->startPos = startPos;
	}

	bool matches(const TokenType& tokenType) {
		return this->tokenType->equals(tokenType);
	}

	bool matchesWord(const std::string& word) {
		if (!this->matches(TT_WORD))
			return false;
		return this->value->toString() == word;
	}

};

class LexerResult : public Result {

public:

	std::vector<Token> tokens;

	LexerResult(std::vector<Token> tokens, Error* error) {
		this->tokens = std::vector<Token>(tokens);
		this->error = error;
	}

	std::string toString() {
		if (this->error == nullptr) {
			std::string o = "[";
			for (int i = 0; i < tokens.size(); i++) {
				if (i == 0)
					o += " ";
				o += tokens[i].toString();
				if (i < tokens.size() - 1)
					o += ", ";
				else o += " ";
			}
			o += "]";
			return std::string(o);
		}
		else return this->error->toString();
	}

	void clear() {
		tokens.clear();
	}

};

class LexingResult {
public:
	Token* token = nullptr;
	Error* error = nullptr;

	LexingResult() {}

	Token Register(LexingResult result) {
		if (result.error != nullptr)
			this->error = result.error;
		else
			this->token = result.token;
		return *result.token;
	
	}

	LexingResult success(Token* token) {
		this->token = token;
		return *this;
	}

	LexingResult failure(Error* error) {
		this->error = error;
		return *this;
	}
	
};

class Lexer {
public:

	std::string code;
	Position position;
	char currentChar = 1;
	char lastChar = 1;

	Lexer(std::string& code, std::string& name) {
		this->code = code;
		this->position = Position(0, 0, 1, new std::string(code), new std::string(name));
	}

	void advance() {
		this->position.advance(this->currentChar);
		this->lastChar = this->currentChar;
		if (this->position.character - 1 < this->code.size())
			this->currentChar = this->code.at(this->position.character - 1);
		else this->currentChar = 2;
	}

	LexerResult lex() {
		std::vector<Token> result = std::vector<Token>();
		this->advance();
		while (this->currentChar != 2) {
			if (LEXING::spaceCharacters.find(this->currentChar) != std::string::npos) {
				this->advance();
			}
			else if (LEXING::digits.find(this->currentChar) != std::string::npos) {
				result.push_back(this->makeNumberLiteral());
			}
			else if (this->currentChar == '"') {
				LexingResult r;
				Token t = r.Register(this->makeStringLiteral());
				if (r.error != nullptr)
					return LexerResult({}, r.error);
				result.push_back(t);
			}
			else if (LEXING::letters.find(this->currentChar) != std::string::npos) {
				Token token = Token(this->makeIdentifier());
				result.push_back(token);
			}			

			else if (this->currentChar == '+') { result.push_back(Token(&TT_PLUS, this->position.copy())); this->advance(); }
			else if (this->currentChar == '-') { result.push_back(Token(&TT_MINUS, this->position.copy())); this->advance(); }
			else if (this->currentChar == '*') { result.push_back(Token(&TT_MULT, this->position.copy())); this->advance(); }
			else if (this->currentChar == '/') { 
				this->advance();
				if (this->currentChar == '/') {
					this->advance();
					while (!(this->currentChar == '\n') || this->currentChar != 2)
						this->advance();
					continue;
				}
				else this->position.character -= 2;
				this->advance();
				result.push_back(Token(&TT_DIV, this->position.copy())); this->advance(); 
			}
			else if (this->currentChar == '!') { result.push_back(Token(&TT_FAC, this->position.copy())); this->advance(); }
			else if (this->currentChar == '^') { result.push_back(Token(&TT_POW, this->position.copy())); this->advance(); }

			else if (this->currentChar == '(') { result.push_back(Token(&TT_L_PAREN, this->position.copy())); this->advance(); }
			else if (this->currentChar == ')') { result.push_back(Token(&TT_R_PAREN, this->position.copy())); this->advance(); }
			else if (this->currentChar == '[') { result.push_back(Token(&TT_L_SQUARE_PAREN, this->position.copy())); this->advance(); }
			else if (this->currentChar == '[') { result.push_back(Token(&TT_L_SQUARE_PAREN, this->position.copy())); this->advance(); }
			else if (this->currentChar == ']') { result.push_back(Token(&TT_R_SQUARE_PAREN, this->position.copy())); this->advance(); }
			else if (this->currentChar == '{') { result.push_back(Token(&TT_L_CURLY_PAREN, this->position.copy())); this->advance(); }
			else if (this->currentChar == '}') { result.push_back(Token(&TT_R_CURLY_PAREN, this->position.copy())); this->advance(); }

			else if (this->currentChar == ',') { result.push_back(Token(&TT_COMMA, this->position.copy())); this->advance(); }
			else if (this->currentChar == ':') { result.push_back(Token(&TT_COLON, this->position.copy())); this->advance(); }
			else if (this->currentChar == ';') { result.push_back(Token(&TT_SEMICOLON, this->position.copy())); this->advance(); }

			else {
				return LexerResult({}, new IllegalCharError(std::string("'") + this->currentChar + "'", position.copy(), position.copy()->advance(this->currentChar)));
			}

		}

		result.push_back(Token(&TT_EOF, this->position.copy()));
		return LexerResult(result, nullptr);

	}

	Token makeIdentifier() {
		std::string string;
		Position startPos = this->position;
		while ((LEXING::lettersDigits + '_').find(this->currentChar) != std::string::npos) {
			string += this->currentChar;
			this->advance();
		}
		TokenType* tokenType = nullptr;
			 if (string == "plus")      tokenType = &TT_PLUS;
		else if (string == "minus")     tokenType = &TT_MINUS;
		else if (string == "times")     tokenType = &TT_MULT;
		else if (string == "over")      tokenType = &TT_DIV;
		else if (string == "factorial") tokenType = &TT_FAC;
		else if (string == "equals")    tokenType = &TT_EQUALS;

		else if (string == "true")  return Token(&TT_BOOLEAN, startPos.copy(), this->position.copy(), new Value(Bool(true)));
		else if (string == "false") return Token(&TT_BOOLEAN, startPos.copy(), this->position.copy(), new Value(Bool(false)));

		else if (string == "add")      tokenType = &TT_ADD;
		else if (string == "subtract") tokenType = &TT_SUB;
		else if (string == "multiply") tokenType = &TT_MULTIPLY;
		else if (string == "divide")   tokenType = &TT_DIVIDE;
		else if (string == "square")   tokenType = &TT_SQUARE;
		else if (string == "cube")     tokenType = &TT_CUBE;

		else if (string == "squared") tokenType = &TT_SQUARED;
		else if (string == "cubed")   tokenType = &TT_CUBED;
		else if (string == "power")   tokenType = &TT_POWER;

		else if (string == "greater") tokenType = &TT_GREAT;
		else if (string == "smaller") tokenType = &TT_SMALL;
		else if (string == "not")     tokenType = &TT_NOT;

		else if (string == "return")  tokenType = &TT_KEYWORD_RETURN;

		else if (string == "print")   tokenType = &TT_PRINT;
		else if (string == "scan")    tokenType = &TT_SCAN;
		else if (string == "execute") tokenType = &TT_EXECUTE;
		else if (string == "command") tokenType = &TT_COMMAND;

		else if (string == "index")        tokenType = &TT_KEYWORD_INDEX;
		else if (string == "character")    tokenType = &TT_KEYWORD_CHARACTER;
		else if (string == "sizeof")       tokenType = &TT_KEYWORD_SIZEOF;
		else if (string == "iterations")   tokenType = &TT_KEYWORD_ITERATIONS;

		else if (string == "newline")   tokenType = &TT_KEYWORD_NEWLINE;
		else if (string == "console")   tokenType = &TT_KEYWORD_CONSOLE;
		else if (string == "input")     tokenType = &TT_KEYWORD_INPUT;

		else if (string == "continue")  tokenType = &TT_KEYWORD_CONTINUE;
		else if (string == "break")     tokenType = &TT_KEYWORD_BREAK;

		else if (string == "or")    tokenType = &TT_OR;
		else if (string == "and")   tokenType = &TT_AND;

		else if (string == "if")      tokenType = &TT_KEYWORD_IF;
		else if (string == "else")    tokenType = &TT_KEYWORD_ELSE;
		else if (string == "while")   tokenType = &TT_KEYWORD_WHILE;
		else if (string == "for")     tokenType = &TT_KEYWORD_FOR;
		else if (string == "do")      tokenType = &TT_KEYWORD_DO;
		else if (string == "times")   tokenType = &TT_KEYWORD_TIMES;
		else if (string == "seconds") tokenType = &TT_KEYWORD_SECONDS;

		else if (string == "typeof")   tokenType = &TT_KEYWORD_TYPEOF;
		else if (string == "undefine") tokenType = &TT_KEYWORD_UNDEFINE;
		else if (string == "variable") tokenType = &TT_KEYWORD_VARIABLE;
		else if (string == "function") tokenType = &TT_KEYWORD_FUNCTION;

		else if (string == "String")  tokenType = &TT_KEYWORD_STRING;
		else if (string == "Number")  tokenType = &TT_KEYWORD_NUMBER;
		else if (string == "Array")   tokenType = &TT_KEYWORD_ARRAY;
		else if (string == "Boolean") tokenType = &TT_KEYWORD_BOOLEAN;
		else if (string == "Void")    tokenType = &TT_KEYWORD_VOID;

		else if (string == "set")       tokenType = &TT_SET;
		else if (string == "equal")     tokenType = &TT_EQUAL;
		else if (string == "make")      tokenType = &TT_MAKE;
		else if (string == "reference") tokenType = &TT_REFERENCE;
		else if (string == "taking")    tokenType = &TT_TAKING;
		else if (string == "nothing")   tokenType = &TT_NOTHING;
		
		else tokenType = &TT_WORD;
		if (tokenType->Class != TT_WORD.Class)
			return Token(tokenType, startPos.copy(), this->position.copy(), new Value());
		return Token(tokenType, startPos.copy(), this->position.copy(), new Value(string));

	}

	LexingResult makeStringLiteral() {
		std::string string;
		Position startPos = this->position;
		this->advance();
		LexingResult result;

		while (this->currentChar != '"') {
			if (this->currentChar == 2)
				return result.failure(new InvalidSyntaxError("Unexpected end of string!", this->position.copy(), this->position.copy()->advance(0)));
			if (this->currentChar == '\\') {
				this->advance();
				switch (this->currentChar) {
				case '"': string += ('"'); break;
				case 'n': string += ('\n'); break;
				case '\\': string += ('\\'); break;
				case 'r': string += ('\r'); break;
				case 't': string += ('\t'); break;
				case 'b': string += ('\b'); break;
				case 'f': string += ('\f'); break;
				case '0': string += ('\0'); break;
				case '\'': string += ('\''); break;
				default: 
					return result.failure(new IllegalCharError(std::string("Invalid escape Character \033[31m'") + this->currentChar + "'\033[0m", this->position.copy(), position.copy()->advance(0)));
				}
			}
			else string += this->currentChar;
			this->advance();
		}
		this->advance();

		return result.success(new Token(&TT_STRING, startPos.copy(), this->position.copy(), new Value(string)));
	}

	Token makeNumberLiteral() {
		std::string numString;
		Position startPos = this->position;
		short dotCount = 0;

		while (this->currentChar != 2 && (LEXING::digits + '.').find(this->currentChar) != std::string::npos) {

			if (this->currentChar == '.') {
				if (dotCount == 1)
					break;
				dotCount++;
				numString += ('.');
			}
			else numString += this->currentChar;
			this->advance();
		}

		if (!dotCount)
			return Token(&TT_INT, startPos.copy(), this->position.copy(), new Value(Number(std::stoll(numString))));
		// return Token(&TT_DOUBLE, startPos.copy(), this->position.copy(), new Value(Number(std::stod(numString))));
	}

};