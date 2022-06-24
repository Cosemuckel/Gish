#pragma once

class TokenType : public Object {
public:
	std::string aClass;
	std::string type;

	TokenType(std::string aClass, std::string type) {
		this->aClass = aClass;
		this->type = type;
		this->mClass = Class::TokenType;
	}
	TokenType(const TokenType& tokenType) {
		this->aClass = tokenType.aClass;
		this->type = tokenType.type;
	}

	std::string toString() {
		if (this->type.empty())
			return "\033[90m" + aClass + "\033[0m";
		return "\033[90m" + aClass + "\033[0m" + ':' + type;
	}

	bool equals(TokenType t) {
		return this->aClass == t.aClass && this->type == t.type;
	}

	void clear() {
		this->aClass.~basic_string();
		this->type.~basic_string();
	}
	nullCMP;
	TokenType nullEQ;
	TokenType nullCon;
};

#include "Tokens.h"


class LexerResult : public Result {

public:

	std::vector<Token> tokens;

	LexerResult(std::vector<Token> tokens, Error error) {
		this->mClass = Class::LexerResult;
		this->tokens = std::vector<Token>(tokens);
		this->error = error;
	}

	std::string toString() {
		if (this->error == null) {
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
		else return this->error.toString();
	}

	void clear() {
		for (int i = 0; i < tokens.size(); i++)
			tokens[i].clear();
		tokens.~vector();
	}

	nullCMP;
	LexerResult nullEQ;
	LexerResult nullCon;
};


class Lexer : public Object {
public:

	std::string code;
	Position position;
	char currentChar = 1;
	char lastChar = 1;

	Lexer(std::string code) {
		this->code = code;
		this->position = Position(-1, 0, 1);
		this->mClass = Class::Lexer;
	}

	void advance() {
		this->position.advance(this->currentChar);
		this->lastChar = this->currentChar;
		if (this->position.index < this->code.size())
			this->currentChar = this->code.at(this->position.index);
		else this->currentChar = 2;
	}

	LexerResult lex() {
		std::vector<Token> result = std::vector<Token>();
		this->advance();
		Position::fileCode;
		while (this->currentChar != 2) {
			if (LEXING::spaceCharacters.find(this->currentChar) != std::string::npos) {
				this->advance();
			}
			else if (LEXING::digits.find(this->currentChar) != std::string::npos) {
				Token token = Token(this->makeNumberLiteral());
				result.push_back(token);
				token.clear();
			}
			else if (this->currentChar == '"') {
				LexingResult r;
				Token t = r.Register(this->makeStringLiteral());
				if (r.error != null)
					return LexerResult({}, r.error);
				result.push_back(t);
				t.clear();
				r.clear();
			}
			else if (LEXING::letters.find(this->currentChar) != std::string::npos) {
				Token token = Token(this->makeIdentifier());
				result.push_back(token);
				token.clear();
			}			

			else if (this->currentChar == '+') { result.push_back(Token(TT_PLUS, this->position)); this->advance(); }
			else if (this->currentChar == '-') { result.push_back(Token(TT_MINUS, this->position)); this->advance(); }
			else if (this->currentChar == '*') { result.push_back(Token(TT_MULT, this->position)); this->advance(); }
			else if (this->currentChar == '/') { 
				this->advance();
				if (this->currentChar == '/') {
					this->advance();
					while (!(this->currentChar == '\n') || this->currentChar != 2)
						this->advance();
					continue;
				}
				else this->position.index -= 2;
				this->advance();
				result.push_back(Token(TT_DIV, this->position)); this->advance(); 
			}
			else if (this->currentChar == '!') { result.push_back(Token(TT_FAC, this->position)); this->advance(); }
			else if (this->currentChar == '^') { result.push_back(Token(TT_POW, this->position)); this->advance(); }

			else if (this->currentChar == '(') { result.push_back(Token(TT_L_PAREN, this->position)); this->advance(); }
			else if (this->currentChar == ')') { result.push_back(Token(TT_R_PAREN, this->position)); this->advance(); }
			else if (this->currentChar == '[') { result.push_back(Token(TT_L_SQUARE_PAREN, this->position)); this->advance(); }
			else if (this->currentChar == '[') { result.push_back(Token(TT_L_SQUARE_PAREN, this->position)); this->advance(); }
			else if (this->currentChar == ']') { result.push_back(Token(TT_R_SQUARE_PAREN, this->position)); this->advance(); }
			else if (this->currentChar == '{') { result.push_back(Token(TT_L_CURLY_PAREN, this->position)); this->advance(); }
			else if (this->currentChar == '}') { result.push_back(Token(TT_R_CURLY_PAREN, this->position)); this->advance(); }

			else if (this->currentChar == ',') { result.push_back(Token(TT_COMMA, this->position)); this->advance(); }
			else if (this->currentChar == ':') { result.push_back(Token(TT_COLON, this->position)); this->advance(); }
			else if (this->currentChar == ';') { result.push_back(Token(TT_SEMICOLON, this->position)); this->advance(); }

			else {
				return LexerResult({}, IllegalCharError(std::string("'") + this->currentChar + "'", this->position, Position(this->position).advance(this->currentChar)));
			}

		}

		result.push_back(Token(TT_EOF));
		return LexerResult(result, null);

	}

	Token makeIdentifier() {
		std::string string;
		Position startPos = this->position;
		while ((LEXING::lettersDigits + '_').find(this->currentChar) != std::string::npos) {
			string += this->currentChar;
			this->advance();
		}
		TokenType tokenType = null;
			 if (string == "plus")       tokenType = TT_PLUS;
		else if (string == "minus")     tokenType = TT_MINUS;
		else if (string == "times")     tokenType = TT_MULT;
		else if (string == "over")      tokenType = TT_DIV;
		else if (string == "factorial") tokenType = TT_FAC;
		else if (string == "equals")    tokenType = TT_EQUALS;

		else if (string == "true")  return Token(TT_BOOLEAN, Value(Bool(true)), startPos, this->position);
		else if (string == "false") return Token(TT_BOOLEAN, Value(Bool(false)), startPos, this->position);

		else if (string == "add")      tokenType = TT_ADD;
		else if (string == "subtract") tokenType = TT_SUB;
		else if (string == "multiply") tokenType = TT_MULTIPLY;
		else if (string == "divide")   tokenType = TT_DIVIDE;
		else if (string == "square")   tokenType = TT_SQUARE;
		else if (string == "cube")     tokenType = TT_CUBE;

		else if (string == "squared") tokenType = TT_SQUARED;
		else if (string == "cubed")   tokenType = TT_CUBED;
		else if (string == "power")   tokenType = TT_POWER;

		else if (string == "greater") tokenType = TT_GREAT;
		else if (string == "smaller") tokenType = TT_SMALL;
		else if (string == "not")     tokenType = TT_NOT;

		else if (string == "return")  tokenType = TT_KEYWORD_RETURN;

		else if (string == "print")   tokenType = TT_PRINT;
		else if (string == "scan")    tokenType = TT_SCAN;
		else if (string == "execute") tokenType = TT_EXECUTE;
		else if (string == "command") tokenType = TT_COMMAND;

		else if (string == "index")        tokenType = TT_KEYWORD_INDEX;
		else if (string == "character")    tokenType = TT_KEYWORD_CHARACTER;
		else if (string == "sizeof")       tokenType = TT_KEYWORD_SIZEOF;
		else if (string == "iterations")   tokenType = TT_KEYWORD_ITERATIONS;

		else if (string == "newline")   tokenType = TT_KEYWORD_NEWLINE;
		else if (string == "console")   tokenType = TT_KEYWORD_CONSOLE;
		else if (string == "input")     tokenType = TT_KEYWORD_INPUT;

		else if (string == "continue")  tokenType = TT_KEYWORD_CONTINUE;
		else if (string == "break")     tokenType = TT_KEYWORD_BREAK;

		else if (string == "with") tokenType = TT_WITH;
		else if (string == "is")   tokenType = TT_IS;
		else if (string == "than") tokenType = TT_THAN;
		else if (string == "to")   tokenType = TT_TO;
		else if (string == "by")   tokenType = TT_BY;
		else if (string == "of")   tokenType = TT_OF;
		else if (string == "from") tokenType = TT_FROM;
		else if (string == "the")  tokenType = TT_THE;
		else if (string == "type") tokenType = TT_TYPE;
		else if (string == "a")    tokenType = TT_A;
		else if (string == "as")   tokenType = TT_AS;
		else if (string == "long")   tokenType = TT_LONG;

		else if (string == "or")    tokenType = TT_OR;
		else if (string == "and")    tokenType = TT_AND;

		else if (string == "if")      tokenType = TT_KEYWORD_IF;
		else if (string == "else")    tokenType = TT_KEYWORD_ELSE;
		else if (string == "while")   tokenType = TT_KEYWORD_WHILE;
		else if (string == "for")     tokenType = TT_KEYWORD_FOR;
		else if (string == "do")      tokenType = TT_KEYWORD_DO;
		else if (string == "times")   tokenType = TT_KEYWORD_TIMES;
		else if (string == "seconds") tokenType = TT_KEYWORD_SECONDS;

		else if (string == "typeof")   tokenType = TT_KEYWORD_TYPEOF;
		else if (string == "undefine") tokenType = TT_KEYWORD_UNDEFINE;
		else if (string == "variable") tokenType = TT_KEYWORD_VARIABLE;
		else if (string == "function") tokenType = TT_KEYWORD_FUNCTION;

		else if (string == "String")  tokenType = TT_KEYWORD_STRING;
		else if (string == "Number")  tokenType = TT_KEYWORD_NUMBER;
		else if (string == "Array")   tokenType = TT_KEYWORD_ARRAY;
		else if (string == "Boolean") tokenType = TT_KEYWORD_BOOLEAN;
		else if (string == "Void")    tokenType = TT_KEYWORD_VOID;

		else if (string == "set")       tokenType = TT_SET;
		else if (string == "equal")     tokenType = TT_EQUAL;
		else if (string == "make")      tokenType = TT_MAKE;
		else if (string == "reference") tokenType = TT_REFERENCE;
		else if (string == "taking")    tokenType = TT_TAKING;
		else if (string == "nothing")   tokenType = TT_NOTHING;
		
		else tokenType = TT_IDENTIFIER;
		if (tokenType.aClass != TT_IDENTIFIER.aClass)
			return Token(tokenType, Value(null), startPos, this->position);
		return Token(tokenType, Value(string), startPos, this->position);

	}

	LexingResult makeStringLiteral() {
		std::string string;
		Position startPos = this->position;
		this->advance();
		LexingResult result;

		while (this->currentChar != '"') {
			if (this->currentChar == 2)
				return result.failure(InvalidSyntaxError("Unexpected end of string!", this->position, Position(this->position).advance(0)));
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
					return result.failure(IllegalCharError(std::string("Invalid escape Character \033[31m'") + this->currentChar + "'\033[0m", this->position, Position(this->position).advance(0)));
				}
			}
			else string += this->currentChar;
			this->advance();
		}
		this->advance();

		return result.success(Token(TT_STRING, Value(string), startPos, this->position));
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
			return Token(TT_INT, Value(Number(std::stoll(numString))), startPos, this->position);
		return Token(TT_DOUBLE, Value(Number(std::stod(numString))), startPos, this->position);
	}

	nullCMP;
	Lexer nullEQ;
	Lexer nullCon;
};