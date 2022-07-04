
#pragma once

// Return if an element is in a vector
template <typename T>
bool isInVector(const T& element, const std::vector<T>& vector) {
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

class Parser {
private:
	std::vector<Token>* tokens;
	int currenToken = -1; //Pointing to element in tokens, the same as tokenIndex

public:

	Token* getCurrentToken() {
		return &tokens->at(currenToken);
	}
	Token* getLastToken() {
		return &tokens->at(currenToken - 1);
	}
	Token* getNextToken() {
		return &tokens->at(currenToken + 1);
	}


public:

	Parser(std::vector<Token>* tokens) {
		this->tokens = tokens;
		this->advance();
	}

	Token* advance() {
		this->currenToken++;
		if (this->currenToken < tokens->size()) {y<
			return &tokens->at(this->currenToken);
		}
	}

	NodeWrapper* parse() {
		NodeWrapper* result = expression();
		if (!this->getCurrentToken()->matches(TT_EOF)) {
			throw InvalidSyntaxError("Expected operator instead of " + this->getCurrentToken()->toString(), &this->getCurrentToken()->startPos, &this->getCurrentToken()->endPos);
		}
		return result;
	}

	NodeWrapper* expression() {
	 	return binaryOperation(&Parser::comparaisonExpression, &Parser::comparaisonExpression, { &TT_AND, &TT_OR });
	}	

	NodeWrapper* comparaisonExpression() {
		NodeWrapper* left = this->normalExpression();
		Token* token = this->getCurrentToken();
		bool negated = false;
		// Print the token, and if it equals "is"
		std::cout << token->toString() << " " << token->matchesWord("") << std::endl;
		if (token->matchesWord("is")) {
			token = this->advance();
			if (token->matchesWord("not")) {
				negated = true;
				token = this->advance();
			}
			if (!token->matches(TT_GREAT) && !token->matches(TT_EQUAL) && !token->matches(TT_SMALL))
				throw InvalidSyntaxError("Expected 'greater', 'smaller' or 'equal'", &token->startPos, &token->endPos);
			this->advance();
			if (token->matches(TT_GREAT) || token->matches(TT_SMALL)) {
				if (this->getCurrentToken()->matches(TT_OR)) {
					this->advance();
					if (!this->getCurrentToken()->matches(TT_EQUAL))
						throw InvalidSyntaxError("Expected 'equal'", &this->getCurrentToken()->startPos, &this->getCurrentToken()->endPos);
					Position endPos = this->getCurrentToken()->endPos;
					this->advance();
					if (!this->getCurrentToken()->matchesWord("to"))
						throw InvalidSyntaxError("Expected 'to'", &this->getCurrentToken()->startPos, &this->getCurrentToken()->endPos);
					this->advance();
					NodeWrapper* right = this->normalExpression();
					NodeWrapper* binaryNode = GlobalAllocator.allocate(NodeWrapper(GlobalAllocator.allocate(BinaryNode(left, GlobalAllocator.allocate(Token(token->matches(TT_GREAT) ? &TT_GREAT_EQ : &TT_SMALL_EQ, token->startPos, endPos, Value(Bool(negated)))), right))));
					return binaryNode;
				}
				if (!this->getCurrentToken()->matchesWord("than"))
					throw InvalidSyntaxError("Expected 'than'", &this->getCurrentToken()->startPos, &this->getCurrentToken()->endPos);
				this->advance();
				NodeWrapper* right = this->normalExpression();
				NodeWrapper* binaryNode = GlobalAllocator.allocate(NodeWrapper(GlobalAllocator.allocate(BinaryNode(left, GlobalAllocator.allocate(Token(token->tokenType, token->startPos, token->endPos, Value(Bool(negated)))), right))));
				return binaryNode;
			}
			if (!this->getCurrentToken()->matchesWord("to"))
				throw InvalidSyntaxError("Expected 'to'", &this->getCurrentToken()->startPos, &this->getCurrentToken()->endPos);
			this->advance();
			NodeWrapper* right = this->normalExpression();
			NodeWrapper* binaryNode = GlobalAllocator.allocate(NodeWrapper(GlobalAllocator.allocate(BinaryNode(left, GlobalAllocator.allocate(Token(token->tokenType, token->startPos, token->endPos, Value(Bool(negated)))), right))));
			return binaryNode;
		}
		return left;
	}

	NodeWrapper* normalExpression() {
		return binaryOperation(&Parser::term, &Parser::term, { &TT_PLUS, &TT_MINUS });
	}

	NodeWrapper* term() {
		Token* token = getCurrentToken();
		if (token->matches(TT_PLUS) || token->matches(TT_MINUS)) {
			advance();
			NodeWrapper* node = term();
			NodeWrapper* unaryNode = GlobalAllocator.allocate(NodeWrapper(GlobalAllocator.allocate(UnaryNode(token, node))));
			return unaryNode;
		}
		return binaryOperation(&Parser::factor, &Parser::factor, { &TT_MULT, &TT_DIV });
	}

	NodeWrapper* factor() {
		return binaryOperation(&Parser::atom, &Parser::atom, { &TT_POW });
	}

	NodeWrapper* atom() {
		NodeWrapper* node = nucleus();
		Token* token = this->getCurrentToken();
		if (token->matches(TT_FAC)) {
			NodeWrapper* unaryNode = GlobalAllocator.allocate(NodeWrapper(GlobalAllocator.allocate(UnaryNode(token, node))));
			return unaryNode;
		}
		return node;
	}

	NodeWrapper* nucleus() {
		Token* token = getCurrentToken();
		if (token->matches(TT_INT) || token->matches(TT_DOUBLE) || token->matches(TT_STRING) || token->matches(TT_BOOLEAN)) {
			this->advance();
			NodeWrapper* valueNode = GlobalAllocator.allocate(NodeWrapper(GlobalAllocator.allocate(ValueNode(token))));
			return valueNode;
		}
		if (token->matches(TT_L_PAREN)) {
			this->advance();
			NodeWrapper* node = expression();
			if (!getCurrentToken()->matches(TT_R_PAREN))
				throw InvalidSyntaxError("Expected ')' ", &getCurrentToken()->startPos, &getCurrentToken()->endPos);
			this->advance();
			return node;
		}
		// Throw an error because no matching token has been found
		throw InvalidSyntaxError("Expected value", &token->startPos, &token->endPos);
	}

	//Binary opeation taking pointers to left and right node creation functions, and a list of tokens to match
	NodeWrapper* binaryOperation(NodeWrapper* (Parser::* leftNodeCreationFunction)(), NodeWrapper* (Parser::* rightNodeCreationFunction)(), std::vector<TokenType*>&& tokensToMatch) {
		NodeWrapper* leftNode = (this->*leftNodeCreationFunction)(); //Points to element in the vector of nodes

		//Continue to binary chain until no more tokens to match
		while (isInVector(getCurrentToken()->tokenType, tokensToMatch)) {
			Token* operatorToken = getCurrentToken(); //Points to element in the vector of tokens

			//Go to next token
			advance();

			//Create right node
			NodeWrapper* rightNode = (this->*rightNodeCreationFunction)(); //Points to element in the vector of nodes

			//Create binary node
			NodeWrapper* binaryNode = GlobalAllocator.allocate(NodeWrapper(GlobalAllocator.allocate(BinaryNode(leftNode, operatorToken, rightNode))));

			//Set left node to binary node by pointing to the binary node in the vector of nodes
			leftNode = binaryNode;
		}

		return leftNode;
	}

};
