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

	ArrayNode(std::vector<Node*> nodes, Token type);
	ArrayNode(ArrayNode* node);

	std::string toString();

	void clear();

};

class ListNode : public BaseNode {
public:
	std::vector<Node*> nodes;

	ListNode() {}
	ListNode(std::vector<Node*> nodes);
	ListNode(std::vector<Node*> nodes, bool b);
	ListNode(ListNode* node);
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

	BinaryNode(const BinaryNode& node)
		:BinaryNode(node.opToken, node.leftNode, node.rightNode) {
	}
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
class FunctionCallNode : public BaseNode {
public:
	std::vector<Node*> argumentsInOrder;
	Token varNameToken = null;

	FunctionCallNode(std::vector<Node*> argumentsInOrder, Token varNameToken);
	std::string toString();
	void clear();

};
class ReturnNode : public BaseNode {
public:
	Node* value;

	std::string toString();
	void clear();
	ReturnNode(Node* value);

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
		case Class::FunctionCallNode: ((FunctionCallNode*)this->nodePtr)->clear(); break;
		case Class::ReturnNode: ((ReturnNode*)this->nodePtr)->clear(); break;
		}
	}

	Node(const Node& node) {
		this->type = node.type;
		this->nodePtr = node.nodePtr;
	}

	Node() {
		this->mClass = Class::Node;
	}

	Node(NumberNode* node) {
		this->nodePtr =node;
		this->type = Class::NumberNode;
	}
	Node(StringNode* node) {
		this->nodePtr = node;
		this->type = Class::StringNode;
	}
	Node(BooleanNode* node) {
		this->nodePtr = node;
		this->type = Class::BooleanNode;
	}
	Node(ArrayNode* node) {
		this->nodePtr = node;
		this->type = Class::ArrayNode;
	}
	Node(ListNode* node) {
		this->nodePtr = node;
		this->type = Class::ListNode;
	}
	Node(TypeNode* node) {
		this->nodePtr = node;
		this->type = Class::TypeNode;
	}
	Node(VarAccessNode* node) {
		this->nodePtr = node;
		this->type = Class::VarAccessNode;
	}
	Node(VarIndexAccessNode* node) {
		this->nodePtr = node;
		this->type = Class::VarIndexNode;
	}
	Node(VarAssignNode* node) {
		this->nodePtr = node;
		this->type = Class::VarAssignNode;
	}
	Node(VarReAssignNode* node) {
		this->nodePtr = node;
		this->type = Class::VarReAssignNode;
	}
	Node(UnaryNode* node) {
		this->nodePtr = node;
		this->type = Class::UnaryNode;
	}
	Node(BinaryNode* node) {
		this->nodePtr = node;
		this->type = Class::BinaryNode;
	}
	Node(IfNode* node) {
		this->nodePtr = node;
		this->type = Class::IfNode;
	}
	Node(VarIndexReAssignNode* node) {
		this->nodePtr = node;
		this->type = Class::VarIndexReAssignNode;
	}
	Node(IterationNode* node) {
		this->nodePtr = node;
		this->type = Class::IterationNode;
	}
	Node(TimedIterationNode* node) {
		this->nodePtr = node;
		this->type = Class::TimedIterationNode;
	}
	Node(FunctionDefinitionNode* node) {
		this->nodePtr = node;
		this->type = Class::FunctionDefinitionNode;
	}
	Node(FunctionCallNode* node) {
		this->nodePtr = node;
		this->type = Class::FunctionCallNode;
	}
	Node(ReturnNode* node) {
		this->nodePtr = node;
		this->type = Class::ReturnNode;
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
		case Class::FunctionCallNode: return ((FunctionCallNode*)this->nodePtr)->toString(); break;
		case Class::ReturnNode: return ((ReturnNode*)this->nodePtr)->toString(); break;
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
	this->nodeOn = nodeOn;
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
	this->opToken.clear();
}

BinaryNode::BinaryNode(Token opToken, Node* leftNode, Node* rightNode) {
	this->opToken.clear();
	this->opToken = opToken;
	this->leftNode = leftNode;
	this->rightNode = rightNode;
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
	}
	this->nodes.~vector();
}
VarIndexAccessNode::VarIndexAccessNode(Token varNameToken, Node* index) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->index = index;
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
	}
}

VarIndexReAssignNode::VarIndexReAssignNode(Token varNameToken, Node* value, Node* index) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = value;
	this->startPos = index->startPos;
	this->endPos = value->endPos;
	this->index = index;
}
std::string VarIndexReAssignNode::toString() {
	return  std::string("index ") + index->toString() + ": (" + this->varNameToken.value.toString() + ":" + this->value->toString() + ")";
}
void VarIndexReAssignNode::clear() {
	this->value->clear();
	this->varNameToken.clear();
	if (this->index != nullptr) {
		this->index->clear();
	}
}

VarAssignNode::VarAssignNode(Token varNameToken, Node* value, Value::valueType type) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = value;
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
}

VarReAssignNode::VarReAssignNode(Token varNameToken, Node* value) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = value;
	this->startPos = value->startPos;
	this->endPos = value->endPos;
}
std::string VarReAssignNode::toString() {
	return this->varNameToken.value.toString() + ":" + this->value->toString();
}
void VarReAssignNode::clear() {
	this->value->clear();
	this->varNameToken.clear();
}

IfNode::IfNode(Node* condition, Node* body, Node* elseStatement) {
	this->condition = condition;
	this->body = body;
	if (elseStatement != nullptr)
		this->elseStatement = elseStatement;
}
void IfNode::clear() {
	this->condition->clear();
	this->body->clear();
	if (this->elseStatement != nullptr) {
		this->elseStatement->clear();

	}
}
std::string IfNode::toString() {
	return std::string("if ") + this->condition->toString() + " do " + this->body->toString();
}

IterationNode::IterationNode(Node* iterations, Node* body) {
	this->iterations = iterations;
	this->body = body;
}
void IterationNode::clear() {
	this->iterations->clear();
	this->body->clear();
}
std::string IterationNode::toString() {
	return std::string("do ") + this->body->toString() + " for " + this->iterations->toString() + " iterations";
}
TimedIterationNode::TimedIterationNode(Node* seconds, Node* body) {
	this->seconds = seconds;
	this->body = body;
}
void TimedIterationNode::clear() {
	this->seconds->clear();
	this->body->clear();
}
std::string TimedIterationNode::toString() {
	return std::string("do ") + this->body->toString() + " for " + this->seconds->toString() + " seconds";
}

FunctionDefinitionNode::FunctionDefinitionNode(std::vector<Argument> arguments, Token varNameToken, Value::valueType returnType, Node* body) {
	this->arguments = arguments;
	this->varNameToken = varNameToken;
	this->returnType = returnType;
	this->body = body;
}

void FunctionDefinitionNode::clear() {
	this->varNameToken.clear();
	this->body->clear();
	this->arguments.clear();
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

FunctionCallNode::FunctionCallNode(std::vector<Node*> argumentsInOrder, Token varNameToken) {
	this->argumentsInOrder = argumentsInOrder;
	this->varNameToken = varNameToken;;
}
void FunctionCallNode::clear() {
	for (int i = 0; i < this->argumentsInOrder.size(); i++) {
		this->argumentsInOrder[i]->clear();
	}
	this->argumentsInOrder.clear();
	this->varNameToken.clear();
}
std::string FunctionCallNode::toString() {
	std::string args = "[ ";
	long long l = 0;
	for (Node* argument : this->argumentsInOrder) {
		args += argument->toString();
		l++;
		if (l < this->argumentsInOrder.size())
			args += ", ";
	}
	args += " ]";
	return "(" + varNameToken.toString() + ":" + args + ")";
}

ListNode::ListNode(std::vector<Node*> nodes) {
	this->nodes.clear();
	for (int i = 0; i < nodes.size(); i++) {
		this->nodes.push_back(nodes[i]);
	}
	this->mClass = Class::ListNode;
}
ListNode::ListNode(std::vector<Node*> nodes, bool b) {
	this->nodes.clear();
	for (int i = 0; i < nodes.size(); i++) {
		this->nodes.push_back(nodes[i]);
	}
	this->mClass = Class::ListNode;
}
ListNode::ListNode(ListNode* node) {
	this->startPos = node->startPos;
	this->endPos = node->endPos;
	for (int i = 0; i < this->nodes.size(); i++) {
		this->nodes[i]->clear();
	}
	this->nodes.clear();
	for (int i = 0; i < node->nodes.size(); i++) {
		this->nodes.push_back(node->nodes[i]);
	}
	this->mClass = Class::ListNode;
}

ArrayNode::ArrayNode(std::vector<Node*> nodes, Token type) {
	this->type.clear();
	for (int i = 0; i < nodes.size(); i++)
		this->nodes.push_back(nodes[i]);
	this->type = type;
	this->mClass = Class::ArrayNode;
}
ArrayNode::ArrayNode(ArrayNode* node) {
	this->type.clear();
	this->type = node->type;
	this->startPos = node->type.startPos;
	this->endPos = node->type.endPos;
	for (int i = 0; i < this->nodes.size(); i++) {
		this->nodes[i]->clear();
	}
	this->nodes.clear();
	for (int i = 0; i < node->nodes.size(); i++)
		this->nodes.push_back(node->nodes[i]);
	this->mClass = Class::ArrayNode;
}

ReturnNode::ReturnNode(Node* value) {
	this->value = value;
}
void ReturnNode::clear() {
	this->value->clear();
}
std::string ReturnNode::toString() {
	return "return " + this->value->toString();
}