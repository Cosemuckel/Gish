#pragma once

class CommenterResult {
public:
	std::vector<Token> tokens;

	void clear() {
		for (int i = 0; i < tokens.size(); i++)
			tokens[i].clear();
	}
	CommenterResult(std::vector<Token> tokens) {
		this->tokens = tokens;
	}

	std::string toString() {
		std::string string;
		string += "[ ";
		int i = 0;
		for (Token token : tokens) {
			string += token.toString();
			if (i < tokens.size() - 1)
				string += ", ";
			i++;
		}
		string += " ]";
		return string;
	}

};

class Commenter {
public:

	std::vector<Token> tokens;
	Token currentToken = null;
	long long index = -1;

	Commenter(std::vector<Token> tokens) {
		this->tokens = tokens;
		this->advance();
	}

	void advance() {
		this->index++;
		if (this->index < this->tokens.size())
			this->currentToken = this->tokens[index];
	}

	CommenterResult comment() {

		std::vector<Token> result;

		while (!this->currentToken.matches(TT_EOF)) {
			if (this->currentToken.matches(TT_DIV)) {
				this->advance();
				if (this->currentToken.matches(TT_DIV)) {
					this->advance();
					while (!this->currentToken.matches(TT_NEWLINE) && !this->currentToken.matches(TT_EOF))
						this->advance();
					continue;
				}
				else {
					this->index -= 2;
					this->advance();
				};
			}
			if (this->currentToken.matches(TT_NEWLINE)) {
				this->advance();
				continue;
			}
			result.push_back(this->currentToken);
			this->advance();
		}

		result.push_back(Token(TT_EOF));
		return CommenterResult(result);

	}

};


