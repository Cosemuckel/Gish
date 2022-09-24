
class NodeBase {
public:
	Position* startPosition;
	Position* endPosition;

	int getDepth() {
		return depth;
	}

protected:
	
	int depth = 0;
};

class NodeWrapper;


class ValueNode : public NodeBase {

public:
	ValueNode(Token*);
	Token* getValue();

	std::string toString();

	Value* evaluate();

private:
	Token* valueToken = nullptr;

};

class ArrayNode : public NodeBase {

public:
	ArrayNode(std::vector<NodeWrapper*>*, Token*);
	std::vector<NodeWrapper*>* getNodes();
	Token* getType();

	std::string toString();

	Value* evaluate(Environment*);

private:
	std::vector<NodeWrapper*>* nodes = nullptr;
	Token* typeToken = nullptr;

};

class BinaryNode : public NodeBase {
	
	public:
		BinaryNode(NodeWrapper*, Token*, NodeWrapper*);
		NodeWrapper* getLeft();
		NodeWrapper* getRight();
		
		std::string toString();

		Value* evaluate(Environment*);

		
	private:
		NodeWrapper* left = nullptr;
		Token* operatorToken = nullptr;
		NodeWrapper* right = nullptr;
		
};

class UnaryNode : public NodeBase {
	
	public:
		UnaryNode(Token*, NodeWrapper*);
		NodeWrapper* getRight();
		
		std::string toString();
		
		Value* evaluate(Environment*);
		
	private:
		Token* operatorToken = nullptr;
		NodeWrapper* right = nullptr;
		
};

class ListNode : public NodeBase {
	
	public:
		ListNode(std::vector<NodeWrapper*>*);
		ListNode(Position*, std::vector<NodeWrapper*>*, Position*);
		std::vector<NodeWrapper*>* getNodes();
		
		std::string toString();

		Value* evaluate(Environment*);
		
	private:
		std::vector<NodeWrapper*>* nodes = nullptr;
		
};

class VariableDeclarationNode : public NodeBase {
	
	public:
		VariableDeclarationNode(Token*, Token*, NodeWrapper*);
		Token* getType();
		Token* getName();
		NodeWrapper* getValue();
		
		std::string toString();

		Value* evaluate(Environment*);
		
	private:
		Token* typeToken = nullptr;
		Token* nameToken = nullptr;
		NodeWrapper* value = nullptr;
		
};

class VariableReAssignNode : public NodeBase {
	
	public:
		VariableReAssignNode(Token*, NodeWrapper* );
		Token* getName();
		NodeWrapper* getValue();
		
		std::string toString();

		Value* evaluate(Environment*);
		
	private:
		Token* nameToken = nullptr;
		NodeWrapper* value = nullptr;
		
};

class VariableAccessNode : public NodeBase {
	
	public:
		VariableAccessNode(Token*);
		Token* getName();
		
		std::string toString();

		Value* evaluate(Environment*);
		
	private:
		Token* nameToken = nullptr;
		
};

class FunctionDefinitionNode : public NodeBase {
	
	public:
		FunctionDefinitionNode(Token*, std::vector<NodeWrapper*>*, NodeWrapper*);
		Token* getName();
		std::vector<NodeWrapper*>* getArguments();
		NodeWrapper* getBody();
		
		std::string toString();

		Value* evaluate(Environment*);
		
	private:
		Token* nameToken = nullptr;
		std::vector<NodeWrapper*>* arguments = nullptr;
		NodeWrapper* body = nullptr;
		
};

class FunctionCallNode : public NodeBase {
	
	public:
		FunctionCallNode(Token*, std::vector<NodeWrapper*>*);
		Token* getName();
		std::vector<NodeWrapper*>* getArguments();
		
		std::string toString();

		Value* evaluate(Environment*);
		
	private:
		Token* nameToken = nullptr;
		std::vector<NodeWrapper*>* arguments = nullptr;
		
};

class IfNode : public NodeBase {
	
	public:
		IfNode(NodeWrapper*, NodeWrapper*, NodeWrapper*);
		NodeWrapper* getCondition();
		NodeWrapper* getThenBody();
		NodeWrapper* getElseBody();
		
		std::string toString();
		
		Value* evaluate(Environment*);
		
	private:
		NodeWrapper* condition = nullptr;
		NodeWrapper* thenBody = nullptr;
		NodeWrapper* elseBody = nullptr;
	
};

class ForNode : public NodeBase {
	
	public:
		ForNode(NodeWrapper*, NodeWrapper*, int);
		NodeWrapper* getAmount();
		NodeWrapper* getBody();
		int getType();

		std::string toString();

		Value* evaluate(Environment*);
		
	private:
		NodeWrapper* amount = nullptr;
		NodeWrapper* body = nullptr;
		int type = 0; // 0 = iterations, 1 = seconds, 2 = forever
		
};

class AsLongAsNode : public NodeBase {
	
	public:
		AsLongAsNode(NodeWrapper*, NodeWrapper*);
		NodeWrapper* getCondition();
		NodeWrapper* getBody();
		
		std::string toString();
		
		Value* evaluate(Environment*);
		
	private:
		NodeWrapper* condition = nullptr;
		NodeWrapper* body = nullptr;
		
};

class NodeWrapper {
private:
	void* node = nullptr;	
public:

	enum class NodeType {
		ValueNode,
		ArrayNode,
		BinaryNode,
		UnaryNode,
		ListNode,
		VariableDeclarationNode,
		VariableReAssignNode,
		VariableAccessNode,
		FunctionDefinitionNode,
		FunctionCallNode,
		IfNode,
		ForNode,
		AsLongAsNode
		
	} nodeType;
	
	
	NodeWrapper(ValueNode*);
	NodeWrapper(ArrayNode*);
	NodeWrapper(BinaryNode*);
	NodeWrapper(UnaryNode*);
	NodeWrapper(ListNode*);
	NodeWrapper(VariableDeclarationNode*);
	NodeWrapper(VariableReAssignNode*);
	NodeWrapper(VariableAccessNode*);
	NodeWrapper(FunctionDefinitionNode*);
	NodeWrapper(FunctionCallNode*);
	NodeWrapper(IfNode*);
	NodeWrapper(ForNode*);
	NodeWrapper(AsLongAsNode*);

	std::string typeName();
	
	std::string toString();
	
	Position* getStartPosition();
	Position* getEndPosition();

	Value* evaluate(Environment*);

	int getDepth();

};

//--------------------------
//ValueNode Defines
//--------------------------
ValueNode::ValueNode(Token* value) {
	this->startPosition = &value->startPos;
	this->endPosition = &value->endPos;
	this->valueToken = value;
}
Token* ValueNode::getValue() {
	return this->valueToken;
}
std::string ValueNode::toString() {
	return this->valueToken->toString();
}
Value* ValueNode::evaluate() {
	return &this->valueToken->value;
}

//--------------------------
//ArrayNode Defines
//--------------------------
ArrayNode::ArrayNode(std::vector<NodeWrapper*>* nodes, Token* typeToken) {
	this->startPosition = nodes->at(0)->getStartPosition();
	this->endPosition = nodes->at(nodes->size() - 1)->getEndPosition();
	this->nodes = nodes;
	this->typeToken = typeToken;
	int maxDepth = 0;
	for (int i = 0; i < nodes->size(); i++) {
		if (nodes->at(i)->getDepth() > maxDepth) {
			maxDepth = nodes->at(i)->getDepth();
		}
	}
	this->depth = maxDepth + 1;
}
std::vector<NodeWrapper*>* ArrayNode::getNodes() {
	return this->nodes;
}
Token* ArrayNode::getType() {
	return this->typeToken;
}
std::string ArrayNode::toString() {
	return "A[ " + joinPointer(*this->nodes, ", ") + " ]A";
}
Value* ArrayNode::evaluate(Environment* env) {
	Value* value = GlobalAllocator.allocate(Value());
	value->type = this->typeToken->value.type;
	value->a = std::vector<Value>();
	for (NodeWrapper* node : *this->nodes) {
		value->a.push_back(*node->evaluate(env));
	}
	return value;
}

//--------------------------
//BinaryNode Defines
//--------------------------
BinaryNode::BinaryNode(NodeWrapper* left, Token* operatorToken, NodeWrapper* right) {
	this->startPosition = left->getStartPosition();
	this->endPosition = right->getEndPosition();
	this->left = left;
	this->operatorToken = operatorToken;
	this->right = right;
	this->depth = max(left->getDepth(), right->getDepth()) + 1;
}
NodeWrapper* BinaryNode::getLeft() {
	return this->left;
}
NodeWrapper* BinaryNode::getRight() {
	return this->right;
}
std::string BinaryNode::toString() {
	return "B( " + this->left->toString() + " " + this->operatorToken->toString() + " " + this->right->toString() + " )B";
}
Value* BinaryNode::evaluate(Environment* env) {
	//Asyncronously evaluate the left and right nodes using std::async, if the depth is greater than 5
	Value leftValue;
	Value rightValue;
	
	if (this->depth > 5) {
		std::future<Value*> leftFuture = std::async(std::launch::async, &NodeWrapper::evaluate, this->left, env);
		std::future<Value*> rightFuture = std::async(std::launch::async, &NodeWrapper::evaluate, this->right, env);
		leftValue = *leftFuture.get();
		rightValue = *rightFuture.get();
	} else {
		leftValue = *this->left->evaluate(env)->setPositions(this->left->getStartPosition(), this->left->getEndPosition());
		rightValue = *this->right->evaluate(env)->setPositions(this->right->getStartPosition(), this->right->getEndPosition());
	}
	
	//Perform the operation
	if (this->operatorToken->matches(TT_PLUS))
		return GlobalAllocator.allocate(Value(leftValue + rightValue));
	else if (this->operatorToken->matches(TT_MINUS))
		return GlobalAllocator.allocate(Value(leftValue - rightValue));
	else if (this->operatorToken->matches(TT_MULT))
		return GlobalAllocator.allocate(Value(leftValue * rightValue));
	else if (this->operatorToken->matches(TT_DIV))
		return GlobalAllocator.allocate(Value(leftValue / rightValue));	
}

//--------------------------
//UnaryNode Defines
//--------------------------
UnaryNode::UnaryNode(Token* operatorToken, NodeWrapper* right) {
	//If the operator is a minus, the start position is the start position of the operatorToken, and the end position the end of the node, otherwise the start position is the start position of the node, and the end position the end of the operatorToken.
	if (operatorToken->matches(TT_MINUS)) {
		this->startPosition = &operatorToken->startPos;
		this->endPosition = right->getEndPosition();
	} else {
		this->startPosition = right->getStartPosition();
		this->endPosition = &operatorToken->endPos;
	}

	this->operatorToken = operatorToken;
	this->right = right;
	this->depth = right->getDepth() + 1;
}
NodeWrapper* UnaryNode::getRight() {
	return this->right;
}
std::string UnaryNode::toString() {
	return "U( " + this->operatorToken->toString() + " " + this->right->toString() + " )U";
}
Value* UnaryNode::evaluate(Environment* env) {
	//Asyncronously evaluate the right node using std::async, if the depth is greater than 5
	Value rightValue = *this->right->evaluate(env)->setPositions(this->startPosition, this->endPosition);
	
	//Perform the operation
	if (this->operatorToken->matches(TT_MINUS))
		return GlobalAllocator.allocate(Value(-rightValue));
	return GlobalAllocator.allocate(Value(rightValue));
}

//--------------------------
//ListNode Defines
//--------------------------
ListNode::ListNode(std::vector<NodeWrapper*>* nodes) {
	this->startPosition = nodes->at(0)->getStartPosition();
	this->endPosition = nodes->back()->getEndPosition();
	this->nodes = nodes;
	int maxDepth = 0;
	for (NodeWrapper* node : *nodes) {
		if (node->getDepth() > maxDepth)
			maxDepth = node->getDepth();
	}
	this->depth = maxDepth + 1;
}
ListNode::ListNode(Position* startPosition, std::vector<NodeWrapper*>* nodes, Position* endPosition) {
	this->startPosition = startPosition;
	this->endPosition = endPosition;
	this->nodes = nodes;
}
std::vector<NodeWrapper*>* ListNode::getNodes() {
	return this->nodes;
}
std::string ListNode::toString() {
	return "L[" + joinPointer(*this->nodes, ", ") + "]L";
}
Value* ListNode::evaluate(Environment* env) {
	Value* value = GlobalAllocator.allocate(Value());
	value->type = Value::Type::Array;
	value->a = std::vector<Value>();
	for (NodeWrapper* node : *this->nodes) {
		value->a.push_back(*node->evaluate(env));
	}
	return value;
}

//--------------------------
//VariableDeclarationNode Defines
//--------------------------
VariableDeclarationNode::VariableDeclarationNode(Token* typeToken, Token* nameToken, NodeWrapper* value) {
	this->startPosition = &typeToken->startPos;
	this->endPosition = value->getEndPosition();
	this->typeToken = typeToken;
	this->nameToken = nameToken;
	this->value = value;
	this->depth = value->getDepth();
}
Token* VariableDeclarationNode::getType() {
	return this->typeToken;
}
Token* VariableDeclarationNode::getName() {
	return this->nameToken;
}
NodeWrapper* VariableDeclarationNode::getValue() {
	return this->value;
}
std::string VariableDeclarationNode::toString() {
	return "V( " + this->typeToken->toString() + ": " + this->nameToken->toString() + " <- " + this->value->toString() + " )V";
}
Value* VariableDeclarationNode::evaluate(Environment* env) {
	//Check if the variable already exists in the environment, if so, throw an error
	std::string name = this->nameToken->value.s;
	if (env->hasVariable(name))
		throw RuntimeError("Variable " + name + "already exists", &this->nameToken->startPos, &this->nameToken->endPos);
	//Evaluate the value and add it to the environment
	Value* value = this->value->evaluate(env);
	env->symbolTable->set(name, value);
	return value;
}

//--------------------------
//VariableReAssignNode Defines
//--------------------------
VariableReAssignNode::VariableReAssignNode(Token* nameToken, NodeWrapper* value) {
	this->startPosition = &nameToken->startPos;
	this->endPosition = value->getEndPosition();
	this->nameToken = nameToken;
	this->value = value;
	this->depth = value->getDepth() + 1;
}
Token* VariableReAssignNode::getName() {
	return this->nameToken;
}
NodeWrapper* VariableReAssignNode::getValue() {
	return this->value;
}
std::string VariableReAssignNode::toString() {
	return "V( " + this->nameToken->toString() + " <- " + this->value->toString() + " )V";
}
Value* VariableReAssignNode::evaluate(Environment* env) {
	//Check if the variable exists in the environment, if not, throw an error
	std::string name = this->nameToken->value.s;
	if (!env->hasVariable(name))
		throw RuntimeError("Variable " + name + "does not exist", &this->nameToken->startPos, &this->nameToken->endPos);
	//Evaluate the value and set it in the environment
	Value* value = this->value->evaluate(env);
	env->symbolTable->set(name, value);
	return value;
}

//--------------------------
//VariableAccessNode Defines
//--------------------------
 VariableAccessNode::VariableAccessNode(Token* nameToken) {
	this->startPosition = &nameToken->startPos;
	this->endPosition = &nameToken->endPos;
	this->nameToken = nameToken;
}
Token* VariableAccessNode::getName() {
	return this->nameToken;
}
std::string VariableAccessNode::toString() {
	return "V( " + this->nameToken->toString() + " )V";
}
Value* VariableAccessNode::evaluate(Environment* env) {
	//Check if the variable exists in the environment, if not, throw an error
	std::string name = this->nameToken->value.s;
	if (!env->hasVariable(name))
		throw RuntimeError("Variable " + name + "does not exist", &this->nameToken->startPos, &this->nameToken->endPos);
	//Return the value of the variable
	return env->symbolTable->get(name);
}

//--------------------------
 //FunctionDefinitionNode Defines
 //--------------------------
FunctionDefinitionNode::FunctionDefinitionNode(Token* nameToken, std::vector<NodeWrapper*>* arguments, NodeWrapper* body) {
	this->startPosition = &nameToken->startPos;
	this->endPosition = body->getEndPosition();
	this->nameToken = nameToken;
	this->arguments = arguments;
	this->body = body;
	int maxDepth = 0;
	for (NodeWrapper* argument : *arguments) {
		int depth = argument->getDepth();
		if (depth > maxDepth)
			maxDepth = depth;
	}
	this->depth = max(maxDepth, body->getDepth());
}
Token* FunctionDefinitionNode::getName() {
	return this->nameToken;
}
std::vector<NodeWrapper*>* FunctionDefinitionNode::getArguments() {
	return this->arguments;
}
NodeWrapper* FunctionDefinitionNode::getBody() {
	return this->body;
}
std::string FunctionDefinitionNode::toString() {
	return "F( " + this->nameToken->toString() + " " + joinPointer(*this->arguments, ", ") + " " + this->body->toString() + " )F";
}
Value* FunctionDefinitionNode::evaluate(Environment* env) {
	return nullptr;
}

//--------------------------
 //FunctionCallNode Defines
 //--------------------------
FunctionCallNode::FunctionCallNode(Token* nameToken, std::vector<NodeWrapper*>* arguments) {
	this->startPosition = &nameToken->startPos;
	this->endPosition = &nameToken->endPos;
	this->nameToken = nameToken;
	this->arguments = arguments;
	int maxDepth = 0;
	for (NodeWrapper* argument : *arguments) {
		int depth = argument->getDepth();
		if (depth > maxDepth)
			maxDepth = depth;
	}
	this->depth = maxDepth + 1;
}
Token* FunctionCallNode::getName() {
	return this->nameToken;
}
std::vector<NodeWrapper*>* FunctionCallNode::getArguments() {
	return this->arguments;
}
std::string FunctionCallNode::toString() {
	return "F( " + this->nameToken->toString() + " " + joinPointer(*this->arguments, ", ") + " )F";
}
Value* FunctionCallNode::evaluate(Environment* env) {
	return nullptr;
}

//--------------------------
//IfNode Defines
//--------------------------
IfNode::IfNode(NodeWrapper* condition, NodeWrapper* thenBody, NodeWrapper* elseBody) {
	this->startPosition = condition->getStartPosition();
	this->endPosition = elseBody->getEndPosition();
	this->condition = condition;
	this->thenBody = thenBody;
	this->elseBody = elseBody;
	this->depth = max(condition->getDepth(), max(thenBody->getDepth(), elseBody->getDepth())) + 1;
}
NodeWrapper* IfNode::getCondition() {
	return this->condition;
}
NodeWrapper* IfNode::getThenBody() {
	return this->thenBody;
}
NodeWrapper* IfNode::getElseBody() {
	return this->elseBody;
}
std::string IfNode::toString() {
	return "I( if " + this->condition->toString() + " do " + this->thenBody->toString() + " else " + this->elseBody->toString() + " )I";
}
Value* IfNode::evaluate(Environment* env) {
	//Evaluate the condition
	Value* condition = this->condition->evaluate(env);
	//If the condition is not a boolean, throw an error
	if (condition->type != Value::Type::Bool)
		throw RuntimeError("Condition must be a boolean", this->condition->getStartPosition(), this->condition->getEndPosition());
	//If the condition is true, evaluate the then body
	if (condition->b.value)
		return this->thenBody->evaluate(env);
	//Otherwise, evaluate the else body
	return this->elseBody->evaluate(env);
}

//--------------------------
 //ForNode Defines
 //--------------------------
ForNode::ForNode(NodeWrapper* amount, NodeWrapper* body, int type) {
	this->startPosition = amount->getStartPosition();
	this->endPosition = body->getEndPosition();
	this->amount = amount;
	this->body = body;
	this->type = type;
	this->depth = max(amount->getDepth(), body->getDepth()) + 1;
}
NodeWrapper* ForNode::getAmount() {
	return this->amount;
}
NodeWrapper* ForNode::getBody() {
	return this->body;
}
int ForNode::getType() {
	return this->type;
}
std::string ForNode::toString() {
	if (this->type == 2)
		return "F( do " + this->body->toString() + " forever )F";
	return "F( for " + this->amount->toString() + ( this->type ? "secs" : "its" ) + " do " + this->body->toString() + " )F";
}
Value* ForNode::evaluate(Environment* env) {
	//Evaluate the amount
	Value* amount = this->amount->evaluate(env);
	//If the amount is not a number, throw an error
	if (amount->type != Value::Type::Number)
		throw RuntimeError("Amount must be a number", this->amount->getStartPosition(), this->amount->getEndPosition());
	//If the amount is 0, do nothing
	if (amount->n.value == 0)
		return new Value();
	//Otherwise, evaluate the body
	//If the type is forever go into an infinite loop
	if (this->type == 2) {
		while (true)
			this->body->evaluate(env);
	}
	if (this->type == 0) {
		//If the amount is a number, evaluate the body for that many times
		for (int256 i = 0; i < amount->n.value; i++)
			this->body->evaluate(env);
		return new Value();
	}
	//Otherwise, evaluate the body for that many seconds
	auto start = std::chrono::high_resolution_clock::now();
	while (int256(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count()) < amount->n.value)
		this->body->evaluate(env);
	return new Value();
}
	
//--------------------------
//AsLongAsNode Defines
//--------------------------
AsLongAsNode::AsLongAsNode(NodeWrapper* condition, NodeWrapper* body) {
	this->startPosition = condition->getStartPosition();
	this->endPosition = body->getEndPosition();
	this->condition = condition;
	this->body = body;
	this->depth = max(this->condition->getDepth(), this->body->getDepth()) + 1;
}
NodeWrapper* AsLongAsNode::getCondition() {
	return this->condition;
}
NodeWrapper* AsLongAsNode::getBody() {
	return this->body;
}
std::string AsLongAsNode::toString() {
	return "A( as long as " + this->condition->toString() + " do " + this->body->toString() + " )A";
}
Value* AsLongAsNode::evaluate(Environment* env) {
	//Evaluate the condition
	Value* condition = this->condition->evaluate(env);
	//If the condition is not a boolean, throw an error
	if (condition->type != Value::Type::Bool)
		throw RuntimeError("Condition must be a boolean", this->condition->getStartPosition(), this->condition->getEndPosition());
	//If the condition is true, evaluate the body until the condition is false
	while (condition->b.value) {
		this->body->evaluate(env);
		condition = this->condition->evaluate(env);
	}
	env->inLoop = inLoop;
	return GlobalAllocator.allocate(Value());
}

//--------------------------
//InterruptionNode Defines
//--------------------------
InterruptionNode::InterruptionNode(Token* typeToken) {
	this->startPosition = &typeToken->startPos;
	this->endPosition = &typeToken->endPos;
	this->typeToken = typeToken;
}
std::string InterruptionNode::toString() {
	return this->typeToken->matches(TT_KEYWORD_BREAK) ? "break" : "continue";
}
Token* InterruptionNode::getTypeToken() {
	return this->typeToken;
}
Value* InterruptionNode::evaluate(Environment* env) {
	//If we are not in a loop, throw an error
	if (!env->inLoop)
		throw RuntimeError("Cannot interrupt outside of a loop", this->startPosition, this->endPosition);
	//If we are in a loop, interrupt the loop
	if (this->typeToken->matches(TT_KEYWORD_BREAK))
		env->shouldBreak = true;
	else if (this->typeToken->matches(TT_KEYWORD_CONTINUE))
		env->shouldContinue = true;
	return GlobalAllocator.allocate(Value());
	//Otherwise, return a null value
	return new Value();
}










//--------------------------
//NodeWrapper Defines
//--------------------------
std::string NodeWrapper::typeName() {
	switch (this->nodeType) {
	case NodeType::ValueNode:
		return "ValueNode";
	case NodeType::ArrayNode:
		return "ArrayNode";
	case NodeType::BinaryNode:
		return "BinaryNode";
	case NodeType::UnaryNode:
		return "UnaryNode";
	case NodeType::ListNode:
		return "ListNode";
	case NodeType::VariableDeclarationNode:
		return "VariableDeclarationNode";
	case NodeType::VariableReAssignNode:
		return "VariableReAssignNode";
	case NodeType::VariableAccessNode:
		return "VariableAccessNode";
	case NodeType::FunctionDefinitionNode:
		return "FunctionDefinitionNode";
	case NodeType::FunctionCallNode:
		return "FunctionCallNode";
	case NodeType::IfNode:
		return "IfNode";
	case NodeType::ForNode:
		return "ForNode";
	case NodeType::AsLongAsNode:
		return "AsLongAsNode";
	default:
		return "UnknownNode '" + std::to_string((int)nodeType) + "'";
	}
}
std::string NodeWrapper::toString() {
	switch (this->nodeType) {
	case NodeType::ValueNode:
		return ((ValueNode*)this->node)->toString();
	case NodeType::ArrayNode:
		return ((ArrayNode*)this->node)->toString();
	case NodeType::BinaryNode:
		return ((BinaryNode*)this->node)->toString();
	case NodeType::UnaryNode:
		return ((UnaryNode*)this->node)->toString();
	case NodeType::ListNode:
		return((ListNode*)this->node)->toString();
	case NodeType::VariableDeclarationNode:
		return ((VariableDeclarationNode*)this->node)->toString();
	case NodeType::VariableReAssignNode:
		return ((VariableReAssignNode*)this->node)->toString();
	case NodeType::VariableAccessNode:
		return ((VariableAccessNode*)this->node)->toString();
	case NodeType::FunctionDefinitionNode:
		return ((FunctionDefinitionNode*)this->node)->toString();
	case NodeType::FunctionCallNode:
		return ((FunctionCallNode*)this->node)->toString();
	case NodeType::IfNode:
		return ((IfNode*)this->node)->toString();
	case NodeType::ForNode:
		return ((ForNode*)this->node)->toString();
	case NodeType::AsLongAsNode:
		return ((AsLongAsNode*)this->node)->toString();
	default:
		return "Error getting Node values for type '" + typeName() + "'";
	}
}

Position* NodeWrapper::getStartPosition() {
	switch (this->nodeType) {
	case NodeType::ValueNode:
		return ((ValueNode*)this->node)->startPosition;
	case NodeType::ArrayNode:
		return ((ArrayNode*)this->node)->startPosition;
	case NodeType::BinaryNode:
		return ((BinaryNode*)this->node)->startPosition;
	case NodeType::UnaryNode:
		return ((UnaryNode*)this->node)->startPosition;
	case NodeType::ListNode:
		return ((ListNode*)this->node)->startPosition;
	case NodeType::VariableDeclarationNode:
		return ((VariableDeclarationNode*)this->node)->startPosition;
	case NodeType::VariableReAssignNode:
		return ((VariableReAssignNode*)this->node)->startPosition;
	case NodeType::VariableAccessNode:
		return ((VariableAccessNode*)this->node)->startPosition;
	case NodeType::FunctionDefinitionNode:
		return ((FunctionDefinitionNode*)this->node)->startPosition;
	case NodeType::FunctionCallNode:
		return ((FunctionCallNode*)this->node)->startPosition;
	case NodeType::IfNode:
		return ((IfNode*)this->node)->startPosition;
	case NodeType::ForNode:
		return ((ForNode*)this->node)->startPosition;
	case NodeType::AsLongAsNode:
		return ((AsLongAsNode*)this->node)->startPosition;
	default:
		return nullptr;
	}
}

Position* NodeWrapper::getEndPosition() {
	switch (this->nodeType) {
	case NodeType::ValueNode:
		return ((ValueNode*)this->node)->endPosition;
	case NodeType::ArrayNode:
		return ((ArrayNode*)this->node)->endPosition;
	case NodeType::BinaryNode:
		return ((BinaryNode*)this->node)->endPosition;
	case NodeType::UnaryNode:
		return ((UnaryNode*)this->node)->endPosition;
	case NodeType::ListNode:
		return ((ListNode*)this->node)->endPosition;
	case NodeType::VariableDeclarationNode:
		return ((VariableDeclarationNode*)this->node)->endPosition;
	case NodeType::VariableReAssignNode:
		return ((VariableReAssignNode*)this->node)->endPosition;
	case NodeType::VariableAccessNode:
		return ((VariableAccessNode*)this->node)->endPosition;
	case NodeType::FunctionDefinitionNode:
		return ((FunctionDefinitionNode*)this->node)->endPosition;
	case NodeType::FunctionCallNode:
		return ((FunctionCallNode*)this->node)->endPosition;
	case NodeType::IfNode:
		return ((IfNode*)this->node)->endPosition;
	case NodeType::ForNode:
		return ((ForNode*)this->node)->endPosition;
	case NodeType::AsLongAsNode:
		return ((AsLongAsNode*)this->node)->endPosition;
	default:
		return nullptr;
	}
}

Value* NodeWrapper::evaluate(Environment* environment) {
	switch (this->nodeType) {
	case NodeType::ValueNode:
		return ((ValueNode*)this->node)->evaluate();
	case NodeType::ArrayNode:
		return ((ArrayNode*)this->node)->evaluate(environment);
	case NodeType::BinaryNode:
		return ((BinaryNode*)this->node)->evaluate(environment);
	case NodeType::UnaryNode:
		return ((UnaryNode*)this->node)->evaluate(environment);
	case NodeType::ListNode:
		return ((ListNode*)this->node)->evaluate(environment);
	case NodeType::VariableDeclarationNode:
		return ((VariableDeclarationNode*)this->node)->evaluate(environment);
	case NodeType::VariableReAssignNode:
		return ((VariableReAssignNode*)this->node)->evaluate(environment);
	case NodeType::VariableAccessNode:
		return ((VariableAccessNode*)this->node)->evaluate(environment);
	case NodeType::FunctionDefinitionNode:
		return ((FunctionDefinitionNode*)this->node)->evaluate(environment);
	case NodeType::FunctionCallNode:
		return ((FunctionCallNode*)this->node)->evaluate(environment);
	case NodeType::IfNode:
		return ((IfNode*)this->node)->evaluate(environment);
	case NodeType::ForNode:
		return ((ForNode*)this->node)->evaluate(environment);
	case NodeType::AsLongAsNode:
		return ((AsLongAsNode*)this->node)->evaluate(environment);
	default:
		return nullptr;
	}
}

int NodeWrapper::getDepth() {
	switch (this->nodeType) {
	case NodeType::ValueNode:
		return 0;
	case NodeType::ArrayNode:
		return 1 + ((ArrayNode*)this->node)->getDepth();
	case NodeType::BinaryNode:
		return 1 + ((BinaryNode*)this->node)->getDepth();
	case NodeType::UnaryNode:
		return 1 + ((UnaryNode*)this->node)->getDepth();
	case NodeType::ListNode:
		return 1 + ((ListNode*)this->node)->getDepth();
	case NodeType::VariableDeclarationNode:
		return 1 + ((VariableDeclarationNode*)this->node)->getDepth();
	case NodeType::VariableReAssignNode:
		return 1 + ((VariableReAssignNode*)this->node)->getDepth();
	case NodeType::VariableAccessNode:
		return 1 + ((VariableAccessNode*)this->node)->getDepth();
	case NodeType::FunctionDefinitionNode:
		return 1 + ((FunctionDefinitionNode*)this->node)->getDepth();
	case NodeType::FunctionCallNode:
		return 1 + ((FunctionCallNode*)this->node)->getDepth();
	case NodeType::IfNode:
		return 1 + ((IfNode*)this->node)->getDepth();
	case NodeType::ForNode:
		return 1 + ((ForNode*)this->node)->getDepth();
	case NodeType::AsLongAsNode:
		return 1 + ((AsLongAsNode*)this->node)->getDepth();
	default:
		return 0;
	}
}

NodeWrapper::NodeWrapper(ValueNode* node) {
	this->node = node;
	nodeType = NodeType::ValueNode;
}
NodeWrapper::NodeWrapper(ArrayNode* node) {
	this->node = node;
	nodeType = NodeType::ArrayNode;
}
NodeWrapper::NodeWrapper(BinaryNode* node) {
	this->node = node;
	nodeType = NodeType::BinaryNode;
}
NodeWrapper::NodeWrapper(UnaryNode* node) {
	this->node = node;
	nodeType = NodeType::UnaryNode;
}
NodeWrapper::NodeWrapper(ListNode* node) {
	this->node = node;
	nodeType = NodeType::ListNode;
}
NodeWrapper::NodeWrapper(VariableDeclarationNode* node) {
	this->node = node;
	nodeType = NodeType::VariableDeclarationNode;
}
NodeWrapper::NodeWrapper(VariableReAssignNode* node) {
	this->node = node;
	nodeType = NodeType::VariableReAssignNode;
}
NodeWrapper::NodeWrapper(VariableAccessNode* node) {
	this->node = node;
	nodeType = NodeType::VariableAccessNode;
}
NodeWrapper::NodeWrapper(FunctionDefinitionNode* node) {
	this->node = node;
	nodeType = NodeType::FunctionDefinitionNode;
}
NodeWrapper::NodeWrapper(FunctionCallNode* node) {
	this->node = node;
	nodeType = NodeType::FunctionCallNode;
}
NodeWrapper::NodeWrapper(IfNode* node) {
	this->node = node;
	nodeType = NodeType::IfNode;
}
NodeWrapper::NodeWrapper(ForNode* node) {
	this->node = node;
	nodeType = NodeType::ForNode;
}
NodeWrapper::NodeWrapper(AsLongAsNode* node) {
	this->node = node;
	nodeType = NodeType::AsLongAsNode;
}