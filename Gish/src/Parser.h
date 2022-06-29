
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
		NodeWrapper* result = expression();
		if (!this->getCurrentToken()->matches(TT_EOF)) {
			throw InvalidSyntaxError("Expected operator", &this->getCurrentToken()->startPos, &this->getCurrentToken()->endPos);
		}
		return result;
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
		Token* token = getCurrentToken();
		if ( token->matches(TT_INT) || token->matches(TT_DOUBLE) || token->matches(TT_STRING) || token->matches(TT_BOOLEAN) ) {
			this->advance();		
			NodeWrapper valueNode = GlobalAllocator.allocate(ValueNode(token));
			nodes->push_back(valueNode);
			return &nodes->back();
		}
		// Throw an error because no matching token has been found
		throw InvalidSyntaxError("Expected value", &token->startPos, &token->endPos);
	}


	//Binary opeation taking pointers to left and right node creation functions, and a list of tokens to match
	NodeWrapper* binaryOperation(NodeWrapper* (Parser::*leftNodeCreationFunction)(), NodeWrapper* (Parser::*rightNodeCreationFunction)(), std::vector<TokenType*>&& tokensToMatch) {
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