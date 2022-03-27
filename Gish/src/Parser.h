#pragma once

bool contains(std::vector<TokenType> v, TokenType x) {
	for (int i = 0; i < v.size(); i++)
		if (v[i].equals(x))
			return true;
	return false;
}

class ParserResult : public Result {
public:
	Node node;
	int advanceCount = 0;
	int reverseCount = 0;

	ParserResult() {
		this->node = Node();
		this->error = null;
		this->mClass = Class::ParserResult;
	}

	ParserResult(Node node, Error error) {
		this->node = node;
		this->error = error;
	}

	Node Register(ParserResult result) {
		if (result.error != null)
			this->error = result.error;
		return result.node;
	}

	void regAdvance() {

	}

	ParserResult success(Node node) {
		this->node.clear();
		this->node = node;
		return *this;
	}

	ParserResult failure(Error error) {
		this->error = error;
		return *this;
	}

	std::string toString() {
		if (error != null)
			return error.toString();
		return node.toString();
	}

	~ParserResult() {
		// delete this->node;
	}

	void clear() {
		this->node.clear();
	}

	nullCMP;
	ParserResult nullEQ;
	ParserResult nullCon;
};

class Parser : public Object {

#define RET_ERROR if (result.error != null) return result;
#define REG_ADVANCE this->advance(); result.regAdvance();
public:
	std::vector<Token> tokens;
	int tokenIndex = -1;
	Token lastToken = null;
	Token currentToken = null;

	Parser(std::vector<Token> tokens) {
		this->tokens = tokens;
		this->advance();
	}

	Token advance() {
		this->lastToken = currentToken;
		this->tokenIndex++;
		if (this->tokenIndex < this->tokens.size()) {
			this->currentToken.clear();
			this->currentToken = this->tokens[this->tokenIndex];
		}
		return this->currentToken;
	}

	ParserResult parse() {
		ParserResult result = this->multiStatementExpression(null, TT_EOF);
		if (result.error == null && !this->currentToken.matches(TT_EOF)) {
			return result.failure(InvalidSyntaxError(
				std::string("Expected '+', '-', '*' or '/' at ") + this->currentToken.toString() + " ",
				this->currentToken.startPos, this->currentToken.endPos
			));
		}
		return result;
	}

	ParserResult multiStatementExpression(Token tok, TokenType endOfExpression) {
		ParserResult result = ParserResult();
		std::vector<Node*> statements;
		while (this->currentToken.matches(TT_SEMICOLON)) {
			REG_ADVANCE;
		}
		while (!this->currentToken.matches(endOfExpression)) {
			Node node = result.Register(this->returnableExpression(tok));
			RET_ERROR;
			bool s = false;
			while (this->currentToken.matches(TT_SEMICOLON)) {
				REG_ADVANCE;
				s = true;
			}
			if (!s)
				return result.failure(InvalidSyntaxError(" Missing a ';'" + this->currentToken.toString(), this->currentToken.startPos, this->currentToken.endPos));
			statements.push_back(new Node(node));
		}
		REG_ADVANCE;
		ListNode* l = new ListNode(statements);
		allocationsToClear.push_back(l);
		return result.success(l);
	}

	ParserResult returnableExpression(Token tok) {

		if (this->currentToken.matches(TT_KEYWORD_RETURN)) {
			ParserResult result = ParserResult();
			REG_ADVANCE;
			Node* value = new Node(result.Register(this->expression(tok)));
			RET_ERROR;
			ReturnNode* node = new ReturnNode(value);
			allocationsToClear.push_back(value);
			allocationsToClear.push_back(node);
			return result.success(node);			
		}

		return this->functionalExpression(tok);
	}

	ParserResult functionalExpression(Token tok) {
		ParserResult result = ParserResult();
		Node* body;
		if (this->currentToken.matches(TT_KEYWORD_DO)) {
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_L_CURLY_PAREN)) {
				body = new Node(result.Register(this->returnableExpression(tok)));
			}
			else {
				REG_ADVANCE;
				body = new Node(result.Register(this->multiStatementExpression(tok, TT_R_CURLY_PAREN)));
			}
			allocationsToClear.push_back(body);
			RET_ERROR;
			if (this->currentToken.matches(TT_KEYWORD_IF)) {
				REG_ADVANCE;
				Node* condition = new Node(result.Register(this->expression(tok)));
				allocationsToClear.push_back(condition);
				RET_ERROR;
				if (!this->currentToken.matches(TT_COMMA)) {
					IfNode* node = new IfNode(condition, body, nullptr);
					allocationsToClear.push_back(node);
					return result.success(node);
				}
				REG_ADVANCE;
				RET_ERROR;
				if (this->currentToken.matches(TT_KEYWORD_ELSE)) {
					REG_ADVANCE;
					Node* elseStatement;
					if (!this->currentToken.matches(TT_L_CURLY_PAREN)) {
						if (!this->currentToken.matches(TT_KEYWORD_DO))
							return result.failure(InvalidSyntaxError("Expected 'do'", this->currentToken.startPos, this->currentToken.endPos));
						elseStatement = new Node(result.Register(this->returnableExpression(tok)));
						allocationsToClear.push_back(elseStatement);
					}
					else {
						REG_ADVANCE; 
						if (!this->currentToken.matches(TT_KEYWORD_DO))
							return result.failure(InvalidSyntaxError("Expected 'do'", this->currentToken.startPos, this->currentToken.endPos));
						elseStatement = new Node(result.Register(this->multiStatementExpression(tok, TT_R_CURLY_PAREN)));
					}
					IfNode* node = new IfNode(condition, body, elseStatement);
					allocationsToClear.push_back(node);
					return result.success(node);
				}
				IfNode* node = new IfNode(condition, body, nullptr);
				allocationsToClear.push_back(node);
				return result.success(node);
			}			
			if (this->currentToken.matches(TT_KEYWORD_FOR)) {
				REG_ADVANCE;
				Node* amount = new Node(result.Register(this->expression(tok)));
				allocationsToClear.push_back(amount);
				RET_ERROR;
				if (this->currentToken.matches(TT_KEYWORD_ITERATIONS)) {
					REG_ADVANCE;
					IterationNode* node = new IterationNode(amount, body);
					allocationsToClear.push_back(node);
					return result.success(node);
				}
				if (this->currentToken.matches(TT_KEYWORD_SECONDS)) {
					REG_ADVANCE;
					TimedIterationNode* node = new TimedIterationNode(amount, body);
					allocationsToClear.push_back(node);
					return result.success(node);
				}
				return result.failure(InvalidSyntaxError("Expected 'iterations' or 'seconds'", this->currentToken.startPos, this->currentToken.endPos));
			}

			return result.success(*body);
		}

		return this->varChangingExpression(tok);
	}

	ParserResult varChangingExpression(Token tok) {
		ParserResult result = ParserResult();
		if (this->currentToken.matches(TT_SET)) {
			REG_ADVANCE;

			if (this->currentToken.matches(TT_KEYWORD_INDEX)) {
				REG_ADVANCE;
				Node* index = new Node(result.Register(this->expression(tok)));
				allocationsToClear.push_back(index);
				RET_ERROR;
				if (!this->currentToken.matches(TT_OF))
					return result.failure(InvalidSyntaxError("Expected 'of'", this->currentToken.startPos, this->currentToken.endPos));
				REG_ADVANCE;
				if (!this->currentToken.matches(TT_IDENTIFIER))
					return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos));
				Token variableName = this->currentToken;
				REG_ADVANCE;
				if (!this->currentToken.matches(TT_EQUAL))
					return result.failure(InvalidSyntaxError("Expected 'equal'", this->currentToken.startPos, this->currentToken.endPos));
				REG_ADVANCE;
				if (!this->currentToken.matches(TT_TO))
					return result.failure(InvalidSyntaxError("Expected 'to'", this->currentToken.startPos, this->currentToken.endPos));
				REG_ADVANCE;
				Node* expression = new Node(result.Register(this->expression(tok)));
				allocationsToClear.push_back(expression);
				RET_ERROR;
				VarIndexReAssignNode* node = new VarIndexReAssignNode(variableName, expression, index);
				allocationsToClear.push_back(node);
				return result.success(node);
			}

			if (!this->currentToken.matches(TT_IDENTIFIER))
				return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos)); 
			Token variableName = this->currentToken;
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_EQUAL))
				return result.failure(InvalidSyntaxError("Expected 'equal'", this->currentToken.startPos, this->currentToken.endPos));
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_TO))
				return result.failure(InvalidSyntaxError("Expected 'to'", this->currentToken.startPos, this->currentToken.endPos));
			REG_ADVANCE;
			Node* expression = new Node(result.Register(this->expression(tok)));
			allocationsToClear.push_back(expression);
			RET_ERROR;
			VarReAssignNode* node = new VarReAssignNode(variableName, expression);
			allocationsToClear.push_back(node);
			return result.success(node);
		}
		if (contains({ TT_SQUARE, TT_CUBE }, this->currentToken.type)) {
			Token token = this->currentToken;
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_IDENTIFIER))
				return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos));
			Token variableName = this->currentToken;
			REG_ADVANCE;
			if (token.matches(TT_SQUARE)) {
				VarAccessNode* _n = new VarAccessNode(variableName);
				NumberNode* _nm = new NumberNode(Token(TT_INT, Value(Number(2))));
				Node* n = new Node(_n);
				Node* nm = new Node(_nm);
				BinaryNode* b = new BinaryNode(Token(TT_POW), n, nm);
				allocationsToClear.push_back(_n);
				allocationsToClear.push_back(_nm);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(nm);
				allocationsToClear.push_back(b);
				return result.success(b);
			}
			if (token.matches(TT_CUBE)) {
				VarAccessNode* _n = new VarAccessNode(variableName);
				NumberNode* _nm = new NumberNode(Token(TT_INT, Value(Number(3))));
				Node* n = new Node(_n);
				Node* nm = new Node(_nm);
				BinaryNode* b = new BinaryNode(Token(TT_POW), n, nm);
				allocationsToClear.push_back(_n);
				allocationsToClear.push_back(_nm);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(nm);
				allocationsToClear.push_back(b);
				return result.success(b);
			}
		}
		if (contains({ TT_ADD, TT_SUB, TT_MULTIPLY, TT_DIVIDE }, this->currentToken.type)) {
			Token token = this->currentToken;
			REG_ADVANCE;
			Node* node = new Node(result.Register(this->expression(tok)));
			allocationsToClear.push_back(node);
			RET_ERROR;
			if (token.matches(TT_ADD))
				if (!this->currentToken.matches(TT_TO))
					return result.failure(InvalidSyntaxError("Expected 'to'", this->currentToken.startPos, this->currentToken.endPos));
				else;
			else if (token.matches(TT_SUB))
				if (!this->currentToken.matches(TT_FROM))
					return result.failure(InvalidSyntaxError("Expected 'from'", this->currentToken.startPos, this->currentToken.endPos));
				else;
			else if (token.matches(TT_MULTIPLY) || token.matches(TT_DIVIDE))
				if (!this->currentToken.matches(TT_BY))
					return result.failure(InvalidSyntaxError("Expected 'by'", this->currentToken.startPos, this->currentToken.endPos));
				else;
			else if (token.matches(TT_MULTIPLY) || token.matches(TT_DIVIDE))
				if (!this->currentToken.matches(TT_BY))
					return result.failure(InvalidSyntaxError("Expected 'by'", this->currentToken.startPos, this->currentToken.endPos));
				else;
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_IDENTIFIER))
				return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos));
			Token variableName = this->currentToken;
			REG_ADVANCE;
			if (token.matches(TT_ADD)) {
				VarAccessNode* _n = new VarAccessNode(variableName);
				Node* n = new Node(_n);
				BinaryNode* b = new BinaryNode(Token(TT_PLUS), n, node);
				Node* bn = new Node(b);
				allocationsToClear.push_back(_n);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(b);
				allocationsToClear.push_back(bn);
				VarReAssignNode* node = new VarReAssignNode(variableName, bn);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			if (token.matches(TT_SUB)) {
				VarAccessNode* _n = new VarAccessNode(variableName);
				Node* n = new Node(_n);
				BinaryNode* b = new BinaryNode(Token(TT_MINUS), n, node);
				Node* bn = new Node(b);
				allocationsToClear.push_back(_n);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(b);
				allocationsToClear.push_back(bn);
				VarReAssignNode* node = new VarReAssignNode(variableName, bn);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			if (token.matches(TT_MULTIPLY)) {
				VarAccessNode* _n = new VarAccessNode(variableName);
				Node* n = new Node(_n);
				BinaryNode* b = new BinaryNode(Token(TT_MULT), n, node);
				Node* bn = new Node(b);
				allocationsToClear.push_back(_n);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(b);
				allocationsToClear.push_back(bn);
				VarReAssignNode* node = new VarReAssignNode(variableName, bn);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			if (token.matches(TT_DIVIDE)) {
				VarAccessNode* _n = new VarAccessNode(variableName);
				Node* n = new Node(_n);
				BinaryNode* b = new BinaryNode(Token(TT_DIV), n, node);
				Node* bn = new Node(b);
				allocationsToClear.push_back(_n);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(b);
				allocationsToClear.push_back(bn);
				VarReAssignNode* node = new VarReAssignNode(variableName, bn);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
		}
		return this->varIntroducingExpression(tok);
	}

	ParserResult varIntroducingExpression(Token tok) {
		ParserResult result = ParserResult();
		if (contains({ TT_KEYWORD_STRING, TT_KEYWORD_BOOLEAN, TT_KEYWORD_NUMBER, TT_KEYWORD_VOID }, this->currentToken.type)) {
			Value::valueType type = this->currentToken.matches(TT_KEYWORD_STRING) ? Value::valueType::String : this->currentToken.matches(TT_KEYWORD_BOOLEAN) ? Value::valueType::Bool : this->currentToken.matches(TT_KEYWORD_VOID) ? Value::valueType::Void : Value::valueType::Number;
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_IDENTIFIER))
				return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos));
			Token variableName = this->currentToken;
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_EQUALS) && !this->currentToken.matches(TT_TAKING))
				return result.failure(InvalidSyntaxError("Expected 'equals' or 'taking'", this->currentToken.startPos, this->currentToken.endPos));
			if (this->currentToken.matches(TT_EQUALS)) {
				if (type == Value::valueType::Void)
					return result.failure(InvalidSyntaxError("Can't create Void-variable", this->currentToken.startPos, this->currentToken.endPos));
				REG_ADVANCE;
				Node* expression = new Node(result.Register(this->varIntroducingExpression(tok)));
				allocationsToClear.push_back(expression);
				RET_ERROR;
				VarAssignNode* n = new VarAssignNode(variableName, expression, type);
				allocationsToClear.push_back(n);
				return result.success(n);
			}
			REG_ADVANCE;
			if (this->currentToken.matches(TT_NOTHING)) {
				REG_ADVANCE;
				if (!this->currentToken.matches(TT_COLON))
					return result.failure(InvalidSyntaxError("Expected ':'", this->currentToken.startPos, this->currentToken.endPos));
				REG_ADVANCE;
				if (!this->currentToken.matches(TT_L_CURLY_PAREN))
					return result.failure(InvalidSyntaxError("Expected '{'", this->currentToken.startPos, this->currentToken.endPos));
				REG_ADVANCE;
				Node* body = new Node(result.Register(this->multiStatementExpression(tok, TT_R_CURLY_PAREN)));
				allocationsToClear.push_back(body);
				FunctionDefinitionNode* node = new FunctionDefinitionNode({}, variableName, type, body);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			else {
				std::vector<Argument> arguments;
				bool looped = false;
				while (!this->currentToken.matches(TT_COLON)) {
					if (looped && !this->currentToken.matches(TT_COMMA))
						return result.failure(InvalidSyntaxError("Expected ','", this->currentToken.startPos, this->currentToken.endPos));
					if (looped)
						REG_ADVANCE;
					if (!contains({ TT_KEYWORD_STRING, TT_KEYWORD_BOOLEAN, TT_KEYWORD_NUMBER, TT_KEYWORD_ARRAY }, this->currentToken.type))
						return result.failure(InvalidSyntaxError("Expected typename", this->currentToken.startPos, this->currentToken.endPos));
					Value::valueType type = this->currentToken.matches(TT_KEYWORD_STRING) ? Value::valueType::String : this->currentToken.matches(TT_KEYWORD_BOOLEAN) ? Value::valueType::Bool : this->currentToken.matches(TT_KEYWORD_ARRAY) ? Value::valueType::Array : Value::valueType::Number;
					REG_ADVANCE;
					if (!this->currentToken.matches(TT_IDENTIFIER))
						return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos));
					Token variableName = this->currentToken;
					REG_ADVANCE;
					arguments.push_back(Argument(type, variableName.value.cString));
					looped = true;
				}
				REG_ADVANCE;
				if (!this->currentToken.matches(TT_L_CURLY_PAREN))
					return result.failure(InvalidSyntaxError("Expected '{'", this->currentToken.startPos, this->currentToken.endPos));
				REG_ADVANCE;
				Node* body = new Node(result.Register(this->multiStatementExpression(tok, TT_R_CURLY_PAREN)));
				allocationsToClear.push_back(body);
				FunctionDefinitionNode* node = new FunctionDefinitionNode(arguments, variableName, type, body);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			return result.failure(InvalidSyntaxError("Expected 'nothing'", this->currentToken.startPos, this->currentToken.endPos));
		}
		else if (this->currentToken.matches(TT_KEYWORD_ARRAY)) {
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_OF))
				return result.failure(InvalidSyntaxError("Expected 'of'", this->currentToken.startPos, this->currentToken.endPos));
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_TYPE))
				return result.failure(InvalidSyntaxError("Expected 'type'", this->currentToken.startPos, this->currentToken.endPos));
			REG_ADVANCE;
			if (! contains({ TT_KEYWORD_STRING, TT_KEYWORD_BOOLEAN, TT_KEYWORD_NUMBER, TT_KEYWORD_ARRAY }, this->currentToken.type))
				return result.failure(InvalidSyntaxError("Expected typename", this->currentToken.startPos, this->currentToken.endPos));
			if (this->currentToken.matches(TT_KEYWORD_ARRAY))			
				return result.failure(InvalidSyntaxError("Arrays of Arrays are not yet supported!", this->currentToken.startPos, this->currentToken.endPos));
			REG_ADVANCE;
			Token variableName = this->currentToken;
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_EQUALS))
				return result.failure(InvalidSyntaxError("Expected 'equals'", this->currentToken.startPos, this->currentToken.endPos));
			REG_ADVANCE;
			Node* expression = new Node(result.Register(this->varIntroducingExpression(tok)));
			allocationsToClear.push_back(expression);
			RET_ERROR;
			VarAssignNode* node = new VarAssignNode(variableName, expression, Value::valueType::Array);
			allocationsToClear.push_back(node);
			return result.success(node);
		}
		return this->expression(tok);
	}

	ParserResult compareExpression(Token tok) {
		ParserResult result = ParserResult();
		Token opToken = null;
		Node* left = new Node(result.Register(this->normalExpression(tok)));
		allocationsToClear.push_back(left);
		RET_ERROR;
		bool NOT = false;
		if (this->currentToken.matches(TT_IS)) {
			this->advance();
			result.regAdvance();
			if (this->currentToken.matches(TT_NOT)) {
				this->advance();
				result.regAdvance();
				NOT = true;
			}
			if (this->currentToken.matches(TT_EQUAL)) {
				opToken = Token(TT_EQUAL, Value(Bool(NOT)));
				this->advance();
				result.regAdvance();
				if (!this->currentToken.matches(TT_TO))
					return result.failure(InvalidSyntaxError("Expexted 'to'", this->currentToken.startPos, this->currentToken.endPos));
				this->advance();
				result.regAdvance();
				Node* right = new Node(result.Register(this->normalExpression(tok)));
				allocationsToClear.push_back(right);
				RET_ERROR;
				BinaryNode* node = new BinaryNode(opToken, left, right);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			if (this->currentToken.matches(TT_GREAT) || this->currentToken.matches(TT_SMALL)) {
				opToken = Token(TT_EQUAL, Value(Bool(NOT)));
				this->advance();
				result.regAdvance();
				if (!this->currentToken.matches(TT_THAN))
					return result.failure(InvalidSyntaxError("Expexted 'than'", this->currentToken.startPos, this->currentToken.endPos));
				this->advance();
				result.regAdvance();
				Node* right = new Node(result.Register(this->normalExpression(tok)));
				allocationsToClear.push_back(right);
				RET_ERROR;
				BinaryNode* node = new BinaryNode(opToken, left, right);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			return result.failure(InvalidSyntaxError("Expected ('not' followed by) 'equal to' or 'greater', 'smaller', 'greaterEqual' or 'smallerEqual' followed by 'than'", this->currentToken.startPos, this->currentToken.endPos));
		}
		return result.success(*left);
	}

	ParserResult normalExpression(Token tok) {
		ParserResult result = ParserResult();
		Node node = result.Register(this->binaryOperation("term", { TT_PLUS, TT_MINUS }, false, tok));
		RET_ERROR;
		return result.success(node);
	}

	ParserResult expression(Token tok) {
		ParserResult result = ParserResult();
		Node node = result.Register(this->binaryOperation("compareExpression", { TT_AND, TT_OR }, false, tok));
		RET_ERROR;
		return result.success(node);
	}

	ParserResult term(Token tok) {
		ParserResult result = ParserResult();
		Token token = this->currentToken;
		if (token.matches(TT_PLUS) || token.matches(TT_MINUS)) {
			this->advance();
			result.regAdvance();
			Node* binOp = new Node(result.Register(this->term(tok)));
			allocationsToClear.push_back(binOp);
			RET_ERROR;
			if (token.matches(TT_PLUS))
				return result.success(*binOp);
			UnaryNode* node = new UnaryNode(token, binOp);
			allocationsToClear.push_back(node);
			return result.success(node);
		}
		return this->binaryOperation("factor", { TT_MULT, TT_DIV }, false, tok);
	}

	ParserResult factor(Token tok) {
		return this->binaryOperation("atom", { TT_POW }, false, tok);
	}

	ParserResult atom(Token tok) {
		ParserResult result = ParserResult();
		Node* left = new Node(result.Register(this->nucleus(tok)));
		allocationsToClear.push_back(left);
		RET_ERROR;
		while (this->currentToken.matches(TT_SQUARED) || this->currentToken.matches(TT_CUBED)) {
			if (this->currentToken.matches(TT_SQUARED)) {
				this->advance();
				result.regAdvance();
				NumberNode* nn = new NumberNode(Token(TT_INT, Value(Number(2)), this->lastToken.startPos, this->currentToken.endPos));
				Node* n = new Node(nn);
				BinaryNode* bn = new BinaryNode(Token(TT_POW), left, n);
				allocationsToClear.push_back(nn);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(nn);
				left = new Node(bn);
				allocationsToClear.push_back(left);
			}
			if (this->currentToken.matches(TT_CUBED)) {
				this->advance();
				result.regAdvance();
				NumberNode* nn = new NumberNode(Token(TT_INT, Value(Number(3)), this->lastToken.startPos, this->currentToken.endPos));
				Node* n = new Node(nn);
				BinaryNode* bn = new BinaryNode(Token(TT_POW), left, n);
				allocationsToClear.push_back(nn);
				allocationsToClear.push_back(n);
				allocationsToClear.push_back(nn);
				left = new Node(bn);
				allocationsToClear.push_back(left);
			}
		}
		return result.success(*left);
	}

	ParserResult nucleus(Token tok) {
		ParserResult result = ParserResult();
		Node* node = new Node(result.Register(this->nucleon(tok)));
		allocationsToClear.push_back(node);
		RET_ERROR;
		while (this->currentToken.matches(TT_FAC)) {
			REG_ADVANCE; 
			UnaryNode* bn = new UnaryNode(Token(TT_FAC), node);
			node = new Node(bn);
			allocationsToClear.push_back(bn);
			allocationsToClear.push_back(node);
		}
		return result.success(*node);
	}

	ParserResult nucleon(Token tok) {
		ParserResult result = ParserResult();
		Token token = this->currentToken;
		if (token.matches(TT_IDENTIFIER)) {
			REG_ADVANCE;
			if (this->currentToken.matches(TT_L_PAREN)) {
				REG_ADVANCE;
				std::vector<Node*> arguments;
				bool looped = false;
				while (!this->currentToken.matches(TT_R_PAREN)) {
					if (!this->currentToken.matches(TT_COMMA) && looped)
						return result.failure(InvalidSyntaxError("Expected ','", this->currentToken.startPos, this->currentToken.endPos));
					if (looped)
						REG_ADVANCE;
					arguments.push_back(new Node(result.Register(this->expression(tok))));
					RET_ERROR;
					looped = true;
				}
				REG_ADVANCE; 
				FunctionCallNode* node = new FunctionCallNode(arguments, token);
				allocationsToClear.push_back(node);
				return result.success(node);
			}
			VarAccessNode* node = new VarAccessNode(token);
			allocationsToClear.push_back(node);
			return result.success(node);
		}
		if (token.matches(TT_KEYWORD_TYPEOF)) {
			this->advance();
			result.regAdvance();
			if (!this->currentToken.matches(TT_IDENTIFIER))
				return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos));
			token = this->currentToken;
			this->advance();
			result.regAdvance();
			TypeNode* node = new TypeNode(token);
			allocationsToClear.push_back(node);
			return result.success(node);
		}
		if (token.matches(TT_INT) || token.matches(TT_DOUBLE)) {
			this->advance();
			result.regAdvance();
			NumberNode* nn = new NumberNode(token);
			Node* node = new Node(nn);
			allocationsToClear.push_back(node);
			allocationsToClear.push_back(nn);
			return result.success(*node);
		}
		if (token.matches(TT_STRING)) {
			this->advance();
			result.regAdvance();
			StringNode* sn = new StringNode(token);
			Node* node = new Node(sn);
			allocationsToClear.push_back(node);
			allocationsToClear.push_back(sn);
			return result.success(*node);
		}
		if (token.matches(TT_BOOLEAN)) {
			this->advance();
			result.regAdvance();
			BooleanNode* bn = new BooleanNode(token);
			Node* node = new Node(bn);
			allocationsToClear.push_back(node);
			allocationsToClear.push_back(bn);
			return result.success(*node);
		}
		if (token.matches(TT_L_SQUARE_PAREN)) {
			std::vector<Node*> nodes;
			REG_ADVANCE;
			while (!this->currentToken.matches(TT_R_SQUARE_PAREN)) {
				Node node = result.Register(this->expression(token));
				RET_ERROR;
				nodes.push_back(new Node(node));
				if (!this->currentToken.matches(TT_R_SQUARE_PAREN) && !this->currentToken.matches(TT_COMMA))
					return result.failure(InvalidSyntaxError("Expected 'comma'", this->currentToken.startPos, this->currentToken.endPos));
				if (this->currentToken.matches(TT_COMMA)) {
					this->advance();
					result.regAdvance();
				}
			}
			this->advance();
			result.regAdvance();
			ArrayNode* an = new ArrayNode(nodes, token);
			allocationsToClear.push_back(an);
			return result.success(an);
		}
		if (token.matches(TT_L_PAREN)) {
			this->advance();
			result.regAdvance();
			Node node = result.Register(this->varIntroducingExpression(tok));
			RET_ERROR;
			if (this->currentToken.matches(TT_R_PAREN)) {
				this->advance();
				result.regAdvance();
				return result.success(node);
			}
			std::cout << this->currentToken.toString();
			return result.failure(InvalidSyntaxError("Expected ')'", this->currentToken.startPos, this->currentToken.endPos));

		}
		if (token.matches(TT_KEYWORD_INDEX)) {
			REG_ADVANCE;
			Node* index = new Node(result.Register(this->expression(tok)));
			RET_ERROR;
			if (!this->currentToken.matches(TT_OF))
				return result.failure(InvalidSyntaxError("Expected 'of'", this->currentToken.startPos, this->currentToken.endPos));
			REG_ADVANCE;
			if (!this->currentToken.matches(TT_IDENTIFIER))
				return result.failure(InvalidSyntaxError("Expected identifier", this->currentToken.startPos, this->currentToken.endPos));
			Token variableName = this->currentToken;
			REG_ADVANCE;
			VarIndexAccessNode* node = new VarIndexAccessNode(variableName, index);
			allocationsToClear.push_back(node);
			return result.success(node);

		}
		return result.failure(InvalidSyntaxError("Expected int, float, string, boolean, identifier, '+', '-' or '('", this->currentToken.startPos, this->currentToken.endPos));
	}

	ParserResult binaryOperation(std::string function, std::vector<TokenType> operators, std::string function2, bool required, Token tok) {
		Node* left = new Node();
		allocationsToClear.push_back(left);
		ParserResult result = ParserResult();
		if (function == "compareExpression") left = new Node(result.Register(this->compareExpression(tok)));
		if (function == "normalExpression") left = new Node(result.Register(this->normalExpression(tok)));
		if (function == "expression") left = new Node(result.Register(this->expression(tok)));
		if (function == "term") left = new Node(result.Register(this->term(tok)));
		if (function == "factor") left = new Node(result.Register(this->factor(tok)));
		if (function == "atom") left = new Node(result.Register(this->atom(tok)));
		if (function == "nucleus") left = new Node(result.Register(this->nucleus(tok)));
		allocationsToClear.push_back(left);
		RET_ERROR;

		int adv = 0;
		while (contains(operators, this->currentToken.type)) {
			Token opToken = this->currentToken;
			this->advance();
			result.regAdvance();
			adv++;
			Node* right = new Node();
			allocationsToClear.push_back(right);
			if (function2 == "compareExpression") right = new Node(result.Register(this->compareExpression(tok)));
			if (function2 == "normalExpression") right = new Node(result.Register(this->normalExpression(tok)));
			if (function2 == "expression") right = new Node(result.Register(this->expression(tok)));
			if (function2 == "term") right = new Node(result.Register(this->term(tok)));
			if (function2 == "factor") right = new Node(result.Register(this->factor(tok)));
			if (function2 == "atom") right = new Node(result.Register(this->atom(tok)));
			if (function2 == "nucleus") right = new Node(result.Register(this->nucleus(tok)));
			allocationsToClear.push_back(right);
			RET_ERROR;
			BinaryNode* bn = new BinaryNode(opToken, left, right);
			left = new Node(bn);
			allocationsToClear.push_back(left);
			allocationsToClear.push_back(bn);
		}
		if (adv == 0 && required);
		return result.success(*left);
	}

	
	ParserResult binaryOperation(std::string function, std::vector<TokenType> operators, bool required, Token tok) {
		return this->binaryOperation(function, operators, function, required, tok);
	}

	~Parser() {
		this->tokens.clear();
	}
};