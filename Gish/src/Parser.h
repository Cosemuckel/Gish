
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
	}

	Token* advance() {
		this->currenToken++;
		return &tokens->at(currenToken);
	}

	void Parse() {
		
	}
		
	NodeWrapper* expression() {
		//Return a binary operation between two factors
		return binaryOperation(term, term, { &TT_PLUS, &TT_MINUS });
	}
	
	NodeWrapper* term() {
		Token* token = getCurrentToken();
		if (token->matches(TT_PLUS) || token->matches(TT_MINUS)) {
			advance();
			NodeWrapper* node = term();			
			NodeWrapper unaryNode = new UnaryNode(token, node);
			nodes->push_back(unaryNode);
			return &nodes->back();			
		}
		//Return a binary operation between two factors
		return binaryOperation(factor, factor, { &TT_MULT, &TT_DIV });
	}
	

	NodeWrapper* factor() {
		Token* token = getCurrentToken();
		if ( token->matches(TT_INT) || token->matches(TT_DOUBLE) || token->matches(TT_STRING) || token->matches(TT_BOOLEAN) ) {
			this->advance();		
			NodeWrapper valueNode = new ValueNode(token);			
			nodes->push_back(valueNode);
			return &nodes->back();
		}
	}


	//Binary opeation taking pointers to left and right node creation functions, and a list of tokens to match
	NodeWrapper* binaryOperation(NodeWrapper* (*leftNodeCreationFunction)(), NodeWrapper* (*rightNodeCreationFunction)(), std::vector<TokenType*>&& tokensToMatch) {
		NodeWrapper* leftNode = leftNodeCreationFunction(); //Points to element in the vector of nodes

		//Continue to binary chain until no more tokens to match		
		while (isInVector(getCurrentToken()->tokenType, tokensToMatch)) {
			Token* operatorToken = getCurrentToken(); //Points to element in the vector of tokens
			
			//Create right node
			NodeWrapper* rightNode = rightNodeCreationFunction(); //Points to element in the vector of nodes
			
			//Create binary node
			NodeWrapper binaryNode = new BinaryNode(leftNode, operatorToken, rightNode);
		
			//Add binary node to nodes
			nodes->push_back(binaryNode);
			
			//Advance to next token
			advance();
			
			//Set left node to binary node by pointing to the binary node in the vector of nodes
			leftNode = &nodes->at(nodes->size() - 1);
		}

		return leftNode;
	}	

};