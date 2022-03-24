#pragma once
class BaseNode : public Object {
public:
	Position startPos = Position();
	Position endPos = Position();
};

class NumberNode : public BaseNode {
public:
	Token token = null;
	NumberNode(Token token) {
		this->token.clear();
		this->token = token;
		this->startPos = token.startPos;
		this->endPos = token.endPos;
		this->mClass = Class::NumberNode;
	}
	NumberNode(NumberNode* node) {
		this->token.clear();
		this->token = node->token;
		this->startPos = node->token.startPos;
		this->endPos = node->token.endPos;
		this->mClass = Class::NumberNode;
	}

	std::string toString() {
		if (token != null)
			return token.toString();
		return "Null";
	}

	void clear() {
		this->token.clear();
	}

	nullCMP;
	NumberNode nullEQ;
	NumberNode nullCon;
};
class BooleanNode : public BaseNode {
public:
	Token token = null;
	BooleanNode(Token token) {
		this->token.clear();
		this->token = token;
		this->startPos = token.startPos;
		this->endPos = token.endPos;
		this->mClass = Class::BooleanNode;
	}
	BooleanNode(BooleanNode* node) {
		this->token.clear();
		this->token = Token(node->token);
		this->startPos = node->token.startPos;
		this->endPos = node->token.endPos;
		this->mClass = Class::BooleanNode;
	}


	std::string toString() {
		return token.toString();
	}

	void clear() {
		std::cout << "HEY";
		this->token.clear();
	}

	nullCMP;
	BooleanNode nullEQ;
	BooleanNode nullCon;
};
class StringNode : public BaseNode {
public:
	Token token = null;
	StringNode(Token token) {
		this->token.clear();
		this->token = token;
		this->startPos = token.startPos;
		this->endPos = token.endPos;
		this->mClass = Class::StringNode;
	}
	StringNode(StringNode* node) {
		this->token.clear();
		this->token = Token(node->token);
		this->startPos = node->token.startPos;
		this->endPos = node->token.endPos;
		this->mClass = Class::StringNode;
	}

	std::string toString() {
		return token.toString();
	}

	void clear() {
		this->token.clear();
	}

	nullCMP;
	StringNode nullEQ;
	StringNode nullCon;
};
class ArrayNode : public BaseNode {
public:
	std::vector<Node*> nodes;
	Token type = null;

	ArrayNode(std::vector<Node*> nodes, Token type) {
		this->type.clear();
		this->nodes = nodes;
		this->type = type;
	}
	ArrayNode(ArrayNode* node) {
		this->type.clear();
		this->type = node->type;
		this->startPos = node->type.startPos;
		this->endPos = node->type.endPos;
		this->nodes = node->nodes;
		this->mClass = Class::ArrayNode;
	}

	std::string toString();

	void clear();

};

class ListNode : public BaseNode {
public:
	std::vector<Node*> nodes;

	ListNode(std::vector<Node*> nodes) {
		this->nodes = nodes;
	}
	ListNode(ListNode* node) {
		this->startPos = node->startPos;
		this->endPos = node->endPos;
		this->nodes = node->nodes;
		this->mClass = Class::ListNode;
	}

	std::string toString();

	void clear();

};

class VarAssignNode : public BaseNode {
public:

	Token varNameToken = null;
	Value::valueType type;
	Node* value;

	VarAssignNode(Token varNameToken, Node* value, Value::valueType type);

	std::string toString();

	void clear();

};

class VarReAssignNode : public BaseNode {
public:

	Token varNameToken = null;
	Node* value;

	VarReAssignNode(Token varNameToken, Node* value);
	std::string toString();

	void clear();

};
class VarIndexReAssignNode : public BaseNode {
public:
	Token varNameToken = null;
	Node* value = nullptr;
	Node* index = nullptr;


	VarIndexReAssignNode(Token varNameToken, Node* value, Node* index);
	std::string toString();

	void clear();

};


class TypeNode : public BaseNode {
public:

	Token varNameToken = null;

	TypeNode(Token varNameToken) {
		this->varNameToken.clear();
		this->varNameToken = varNameToken;
		this->startPos = varNameToken.startPos;
		this->endPos = varNameToken.endPos;
	}
	std::string toString() {
		return this->varNameToken.value.toString();
	}

	void clear() {
		this->varNameToken.clear();
	}

};

class VarAccessNode : public BaseNode {
public:
	Token varNameToken = null;

	VarAccessNode(Token varNameToken) {
		this->varNameToken.clear();
		this->varNameToken = varNameToken;
		this->startPos = varNameToken.startPos;
		this->endPos = varNameToken.endPos;
	}
	std::string toString() {
		return this->varNameToken.value.toString();
	}

	void clear() {
		this->varNameToken.clear();
	}
};

class VarIndexAccessNode : public BaseNode {
public:
	Token varNameToken = null;
	Node* index = nullptr;

	VarIndexAccessNode(Token varNameToken, Node* index);
	std::string toString();
	void clear();


};

class UnaryNode : public BaseNode {
public:
	Node* nodeOn;
	Token opToken = null;

	UnaryNode(Token opToken, Node* nodeOn);

	std::string toString();

	void clear();

	nullCMP;
	UnaryNode nullEQ;
	UnaryNode nullCon;
};

class BinaryNode : public BaseNode {
public:
	Node* leftNode;
	Token opToken = null;
	Node* rightNode;

	BinaryNode(Token opToken, Node* leftNode, Node* rightNode);

	std::string toString();

	void clear();

	nullCMP;
	BinaryNode nullEQ;
	BinaryNode nullCon;
};

class IfNode : public BaseNode {
public:

	Node* condition;
	Node* body;
	Node* elseStatement = nullptr;

	std::string toString();
	void clear();
	IfNode(Node* contidtion, Node* body, Node* elseStatement);

};

class IterationNode : public BaseNode {
public:

	Node* body;
	Node* iterations;

	std::string toString();
	void clear();
	IterationNode(Node* iterations, Node* body);

};
class TimedIterationNode : public BaseNode {
public:

	Node* body;
	Node* seconds;

	std::string toString();
	void clear();
	TimedIterationNode(Node* seconds, Node* body);

};
struct Argument {
	Value::valueType type;
	std::string Name;
	Argument(Value::valueType type, std::string Name) {
		this->type = type;
		this->Name = Name;
	}
};
class FunctionDefinitionNode : public BaseNode {
public:

	std::vector<Argument> arguments;
	Token varNameToken = null;
	Value::valueType returnType;
	Node* body = nullptr;

	std::string toString();
	void clear();
	FunctionDefinitionNode(std::vector<Argument> arguments, Token varNameToken, Value::valueType returnType, Node* body);

};


class Node : public Object {
public:
	void* nodePtr = nullptr;
	Position startPos = Position();
	Position endPos = Position();
	Class type = Class::Node;

	void clear() {
		switch (this->type) {
		case Class::NumberNode: ((NumberNode*)this->nodePtr)->clear(); break;
		case Class::BooleanNode:((BooleanNode*)this->nodePtr)->clear(); break;
		case Class::StringNode: ((StringNode*)this->nodePtr)->clear(); break;
		case Class::ArrayNode:  ((ArrayNode*)this->nodePtr)->clear(); break;
		case Class::ListNode:   ((ListNode*)this->nodePtr)->clear(); break;
		case Class::TypeNode:   ((TypeNode*)this->nodePtr)->clear(); break;
		case Class::VarAccessNode:   ((VarAccessNode*)this->nodePtr)->clear(); break;
		case Class::VarIndexNode: ((VarIndexAccessNode*)this->nodePtr)->clear(); break;
		case Class::VarAssignNode:   ((VarAssignNode*)this->nodePtr)->clear(); break;
		case Class::VarReAssignNode: ((VarReAssignNode*)this->nodePtr)->clear(); break;
		case Class::VarIndexReAssignNode: ((VarIndexReAssignNode*)this->nodePtr)->clear(); break;
		case Class::UnaryNode:  ((UnaryNode*)this->nodePtr)->clear(); break;
		case Class::BinaryNode: ((BinaryNode*)this->nodePtr)->clear(); break;
		case Class::IfNode: ((IfNode*)this->nodePtr)->clear(); break;
		case Class::IterationNode: ((IterationNode*)this->nodePtr)->clear(); break;
		case Class::TimedIterationNode: ((TimedIterationNode*)this->nodePtr)->clear(); break;
		case Class::FunctionDefinitionNode: ((FunctionDefinitionNode*)this->nodePtr)->clear(); break;
		}
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
	}
	Node() {
		this->mClass = Class::Node;
	}
	Node(NumberNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new NumberNode(&node);
		this->type = Class::NumberNode;
	}
	Node(StringNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new StringNode(node);
		this->type = Class::StringNode;
	}
	Node(BooleanNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new BooleanNode(node);
		this->type = Class::BooleanNode;
	}
	Node(ArrayNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new ArrayNode(node);
		this->type = Class::ArrayNode;
	}
	Node(ListNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new ListNode(node);
		this->type = Class::ListNode;
	}
	Node(TypeNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new TypeNode(node);
		this->type = Class::TypeNode;
	}
	Node(VarAccessNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new VarAccessNode(node);
		this->type = Class::VarAccessNode;
	}
	Node(VarIndexAccessNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new VarIndexAccessNode(node);
		this->type = Class::VarIndexNode;
	}
	Node(VarAssignNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new VarAssignNode(node);
		this->type = Class::VarAssignNode;
	}
	Node(VarReAssignNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new VarReAssignNode(node);
		this->type = Class::VarReAssignNode;
	}
	Node(UnaryNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new UnaryNode(node);
		this->type = Class::UnaryNode;
	}
	Node(BinaryNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new BinaryNode(node);
		this->type = Class::BinaryNode;
	}
	Node(IfNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new IfNode(node);
		this->type = Class::IfNode;
	}
	Node(VarIndexReAssignNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new VarIndexReAssignNode(node);
		this->type = Class::VarIndexReAssignNode;
	}
	Node(IterationNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new IterationNode(node);
		this->type = Class::IterationNode;
	}
	Node(TimedIterationNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new TimedIterationNode(node);
		this->type = Class::TimedIterationNode;
	}
	Node(FunctionDefinitionNode node) {
		if (this->nodePtr != nullptr)
			delete this->nodePtr;
		this->nodePtr = new FunctionDefinitionNode(node);
		this->type = Class::FunctionDefinitionNode;
	}
	std::string toString() {
		switch (this->type) {
		case Class::NumberNode: return ((NumberNode*)this->nodePtr)->toString(); break;
		case Class::StringNode: return ((StringNode*)this->nodePtr)->toString(); break;
		case Class::BooleanNode: return ((BooleanNode*)this->nodePtr)->toString(); break;
		case Class::UnaryNode: return ((UnaryNode*)this->nodePtr)->toString(); break;
		case Class::BinaryNode: return ((BinaryNode*)this->nodePtr)->toString(); break;
		case Class::VarAssignNode: return ((VarAssignNode*)this->nodePtr)->toString(); break;
		case Class::VarReAssignNode: return ((VarReAssignNode*)this->nodePtr)->toString(); break;
		case Class::VarAccessNode: return ((VarAccessNode*)this->nodePtr)->toString(); break;
		case Class::VarIndexNode: return ((VarIndexAccessNode*)this->nodePtr)->toString(); break;
		case Class::VarIndexReAssignNode: return ((VarIndexReAssignNode*)this->nodePtr)->toString(); break;
		case Class::ArrayNode: return ((ArrayNode*)this->nodePtr)->toString(); break;
		case Class::ListNode: return ((ListNode*)this->nodePtr)->toString(); break;
		case Class::TypeNode: return ((TypeNode*)this->nodePtr)->toString(); break;
		case Class::IfNode: return ((IfNode*)this->nodePtr)->toString(); break;
		case Class::IterationNode: return ((IterationNode*)this->nodePtr)->toString(); break;
		case Class::TimedIterationNode: return ((TimedIterationNode*)this->nodePtr)->toString(); break;
		case Class::FunctionDefinitionNode: return ((FunctionDefinitionNode*)this->nodePtr)->toString(); break;
		default:
			return "Null";
		}
	}

	nullCMP;
	Node nullEQ;
	Node nullCon;
};

UnaryNode::UnaryNode(Token opToken, Node* nodeOn) {
	this->opToken.clear();
	this->opToken = opToken;
	this->nodeOn = new Node(*nodeOn);
	if (opToken.matches(TT_FAC)) {
		this->startPos = nodeOn->startPos;
		this->endPos = opToken.endPos;
	}
	else {
		this->startPos = opToken.startPos;
		this->endPos = nodeOn->endPos;
	}
	this->mClass = Class::UnaryNode;
}
std::string UnaryNode::toString() {
	return std::string("(") + this->opToken.toString() + ", " + this->nodeOn->toString() + ")";
}
void UnaryNode::clear() {
	this->nodeOn->clear();
	delete this->nodeOn;
	this->opToken.clear();
}

BinaryNode::BinaryNode(Token opToken, Node* leftNode, Node* rightNode) {
	this->opToken.clear();
	this->opToken = opToken;
	this->leftNode = new Node(*leftNode);
	this->rightNode = new Node(*rightNode);
	this->startPos = this->leftNode->startPos;
	this->endPos = this->rightNode->endPos;
}
std::string BinaryNode::toString() {
	return std::string("(") + this->leftNode->toString() + ", " + this->opToken.toString() + ", " + this->rightNode->toString() + ")";
}
void BinaryNode::clear() {
	this->leftNode->clear();
	this->rightNode->clear();
	this->opToken.clear();
	delete this->leftNode;
	delete this->rightNode;
}

std::string ArrayNode::toString() {
	std::string string;
	for (Node* node : nodes)
		string += node->toString();
	return string;
}
void ArrayNode::clear() {
	this->type.clear();
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->clear();
		delete nodes[i];
	}
	this->nodes.~vector();
}
std::string ListNode::toString() {
	std::string string;
	string += "[ ";
	int i = 0;
	for (Node* node : nodes) {
		string += node->toString();
		if (i < nodes.size() - 1)
			string += ", ";
		i++;
	}
	string += " ]";
	return string;
}
void ListNode::clear() {
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->clear();
		delete nodes[i];
	}
	this->nodes.~vector();
}
VarIndexAccessNode::VarIndexAccessNode(Token varNameToken, Node* index) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->index = new Node(*index);
	this->startPos = index->startPos;
	this->endPos = varNameToken.endPos;
}
std::string VarIndexAccessNode::toString() {
	return std::string("index ( ") + index->toString() + " of " + varNameToken.toString() + " )";
}
void VarIndexAccessNode::clear() {
	this->varNameToken.clear();
	if (this->index != nullptr) {
		this->index->clear();
		delete this->index;
	}
}

VarIndexReAssignNode::VarIndexReAssignNode(Token varNameToken, Node* value, Node* index) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = new Node(*value);
	this->startPos = index->startPos;
	this->endPos = value->endPos;
	this->index = new Node(*index);
}
std::string VarIndexReAssignNode::toString() {
	return  std::string("index ") + index->toString() + ": (" + this->varNameToken.value.toString() + ":" + this->value->toString() + ")";
}
void VarIndexReAssignNode::clear() {
	this->value->clear();
	this->varNameToken.clear();
	delete this->value;
	if (this->index != nullptr) {
		this->index->clear();
		delete this->index;
	}
}

VarAssignNode::VarAssignNode(Token varNameToken, Node* value, Value::valueType type) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = new Node(*value);
	this->startPos = value->startPos;
	this->endPos = value->endPos;
	this->type = type;
}
std::string VarAssignNode::toString() {
	return "(" + this->varNameToken.value.toString() + ":" + this->value->toString() + ")";
}
void VarAssignNode::clear() {
	this->value->clear();
	this->varNameToken.clear();
	delete this->value;
}

VarReAssignNode::VarReAssignNode(Token varNameToken, Node* value) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = new Node(*value);
	this->startPos = value->startPos;
	this->endPos = value->endPos;
}
std::string VarReAssignNode::toString() {
	return this->varNameToken.value.toString() + ":" + this->value->toString();
}
void VarReAssignNode::clear() {
	this->value->clear();
	this->varNameToken.clear();
	delete this->value;
}

IfNode::IfNode(Node* condition, Node* body, Node* elseStatement) {
	this->condition = new Node(*condition);
	this->body = new Node(*body);
	if (elseStatement != nullptr)
		this->elseStatement = new Node(*elseStatement);
}
void IfNode::clear() {
	this->condition->clear();
	delete this->condition;
	this->body->clear();
	delete this->body;
	if (this->elseStatement != nullptr) {
		this->elseStatement->clear();
		delete this->elseStatement;
	}
}
std::string IfNode::toString() {
	return std::string("if ") + this->condition->toString() + " do " + this->body->toString();
}

IterationNode::IterationNode(Node* iterations, Node* body) {
	this->iterations = new Node(*iterations);
	this->body = new Node(*body);
}
void IterationNode::clear() {
	this->iterations->clear();
	delete this->iterations;
	this->body->clear();
	delete this->body;
}
std::string IterationNode::toString() {
	return std::string("do ") + this->body->toString() + " for " + this->iterations->toString() + " iterations";
}
TimedIterationNode::TimedIterationNode(Node* seconds, Node* body) {
	this->seconds = new Node(*seconds);
	this->body = new Node(*body);
}
void TimedIterationNode::clear() {
	this->seconds->clear();
	delete this->seconds;
	this->body->clear();
	delete this->body;
}
std::string TimedIterationNode::toString() {
	return std::string("do ") + this->body->toString() + " for " + this->seconds->toString() + " seconds";
}

FunctionDefinitionNode::FunctionDefinitionNode(std::vector<Argument> arguments, Token varNameToken, Value::valueType returnType, Node* body) {
	this->arguments = arguments;
	this->varNameToken = varNameToken;
	this->returnType = returnType;
	this->body = new Node(*body);
}
void FunctionDefinitionNode::clear() {
	this->varNameToken.clear();
	this->body->clear();
	delete this->body;
}
std::string FunctionDefinitionNode::toString() {
	std::string args = "[ ";
	long long l = 0;
	for (Argument argument : this->arguments) {
		args += Value::Name(argument.type);
		l++;
		if (l < this->arguments.size())
			args += ", ";
	}
	args += " ]";
	return "(" + varNameToken.toString() + ":" + args + ") :" + this->body->toString();
}