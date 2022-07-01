
#pragma once

// Return if an element is in a vector
template <typename T>
bool isInVector(const T& element, const std::vector<T>& vector) {
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

class Parser {
private:
	std::vector<Token>* tokens;
	std::vector<NodeWrapper>* nodes;
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

	Parser(std::vector<Token>* tokens, std::vector<NodeWrapper>* nodes) {
		this->tokens = tokens;
		this->nodes = nodes;
		this->advance();
	}

	Token* advance() {
		this->currenToken++;
		if (this->currenToken < tokens->size()) {
			return &tokens->at(this->currenToken);
		}
	}

	NodeWrapper* parse() {
		NodeWrapper* result = comparaisonExpression();
		if (!this->getCurrentToken()->matches(TT_EOF)) {
			throw InvalidSyntaxError("Expected operator instead of " + this->getCurrentToken()->toString(), &this->getCurrentToken()->startPos, &this->getCurrentToken()->endPos);
		}
		return result;
	}

	NodeWrapper* comparaisonExpression() {
		NodeWrapper* left = this->expression();
		Token* token = this->getCurrentToken();
		bool negated = false;
		if (token->matchesWord("is")) {
			token = this->advance();
			if (token->matchesWord("not")) {
				negated = true;
				token = this->advance();
			}
			if (!token->matches(TT_GREAT) && !token->matches(TT_EQUAL) && !token->matches(TT_SMALL))
				throw InvalidSyntaxError("Expected 'greater', 'smaller' or 'equal'" , &token->startPos, &token->endPos);
			this->advance();
			if (token->matches(TT_GREAT) || token->matches(TT_SMALL)) {
				if (this->getCurrentToken()->matchesWord("or")) {
					this->advance();
					if (!this->getCurrentToken()->matches(TT_EQUAL))
						throw InvalidSyntaxError("Expected 'equal'", &token->startPos, &token->endPos);
					Position endPos = this->getCurrentToken()->endPos;
					this->advance();
					if (!this->getCurrentToken()->matchesWord("to"))
						throw InvalidSyntaxError("Expected 'to'", &token->startPos, &token->endPos);
					this->advance();
					NodeWrapper* right = this->expression();
					NodeWrapper binaryNode = new BinaryNode(left, -Token(token->matches(TT_GREAT) ? &TT_GREAT_EQ : &TT_SMALL_EQ, token->startPos, endPos, +Value(Bool(negated))), right);
					nodes->push_back(binaryNode);
					return &nodes->back();
				}
				if (!this->getCurrentToken()->matchesWord("than"))
					throw InvalidSyntaxError("Expected 'to'", &token->startPos, &token->endPos);
				this->advance();
				NodeWrapper* right = this->expression();
				NodeWrapper binaryNode = new BinaryNode(left, -Token(token->tokenType, token->startPos, token->endPos, +Value(Bool(negated))), right);
				nodes->push_back(binaryNode);
				return &nodes->back();
			}
			if (!this->getCurrentToken()->matchesWord("to"))
				throw InvalidSyntaxError("Expected 'to'", &token->startPos, &token->endPos);
			this->advance();
			NodeWrapper* right = this->expression();
			NodeWrapper binaryNode = new BinaryNode(left, -Token(token->tokenType, token->startPos, token->endPos, +Value(Bool(negated))), right);
			nodes->push_back(binaryNode);
			return &nodes->back();
		}
		return left;
	}

	NodeWrapper* expression() {
		return binaryOperation(&Parser::term, &Parser::term, { &TT_PLUS, &TT_MINUS });
	}

	NodeWrapper* term() {
		Token* token = getCurrentToken();
		if (token->matches(TT_PLUS) || token->matches(TT_MINUS)) {
			advance();
			NodeWrapper* node = term();
			NodeWrapper unaryNode = GlobalAllocator.allocate(UnaryNode(token, node));
			nodes->push_back(unaryNode);
			return &nodes->back();
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
			NodeWrapper unaryNode = GlobalAllocator.allocate(UnaryNode(token, node));
			nodes->push_back(unaryNode);
			return &nodes->back();
		}
		return node;
	}

	NodeWrapper* nucleus() {
		Token* token = getCurrentToken();
		if (token->matches(TT_INT) || token->matches(TT_DOUBLE) || token->matches(TT_STRING) || token->matches(TT_BOOLEAN)) {
			this->advance();
			NodeWrapper valueNode = GlobalAllocator.allocate(ValueNode(token));
			nodes->push_back(valueNode);
			return &nodes->back();
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
			NodeWrapper binaryNode = GlobalAllocator.allocate(BinaryNode(leftNode, operatorToken, rightNode));

			//Add binary node to nodes
			nodes->push_back(binaryNode);

			//Set left node to binary node by pointing to the binary node in the vector of nodes
			leftNode = &nodes->at(nodes->size() - 1);
		}

		return leftNode;
	}

};
