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
	ArrayNode(std::vector<Node*> nodes, Token type, Allocator allocationTable, bool passOn);
	ArrayNode(const ArrayNode& node, Allocator allocationTable, bool passOn);

	std::string toString();

	void clear();

};

class ListNode : public BaseNode {
public:
	std::vector<Node*> nodes;

	ListNode() {}
	ListNode(std::vector<Node*> nodes);
	ListNode(std::vector<Node*> nodes, Allocator allocationTable, bool passOn);
	ListNode(const ListNode& node, Allocator allocationTable, bool passOn);
	std::string toString();

	~ListNode() {
		this->nodes.clear();
		this->nodes.~vector();
	}

	void clear();

};

class VarAssignNode : public BaseNode {
public:

	Token varNameToken = null;
	Value::valueType type;
	Node* value;

	VarAssignNode(Token varNameToken, Node* value, Value::valueType type);
	VarAssignNode(const VarAssignNode& node, Allocator allocationTable, bool passOn);
	VarAssignNode(Token varNameToken, Node* value, Value::valueType type, Allocator allocationTable, bool passOn);

	std::string toString();

	void clear();

};

class VarReAssignNode : public BaseNode {
public:

	Token varNameToken = null;
	Node* value;

	VarReAssignNode(Token varNameToken, Node* value);
	VarReAssignNode(const VarReAssignNode& node, Allocator allocationTable, bool passOn);
	VarReAssignNode(Token varNameToken, Node* value, Allocator allocationTable, bool passOn);
	std::string toString();

	void clear();

};
class VarIndexReAssignNode : public BaseNode {
public:
	Token varNameToken = null;
	Node* value = nullptr;
	Node* index = nullptr;


	VarIndexReAssignNode(Token varNameToken, Node* value, Node* index);
	VarIndexReAssignNode(Token varNameToken, Node* value, Node* index, Allocator allocationTable, bool passOn);
	VarIndexReAssignNode(const VarIndexReAssignNode& node, Allocator allocationTable, bool passOn);
	std::string toString();

	void clear();

};


class PropertyNode : public BaseNode {
public:

	Token varNameToken = null;
	int type = 0; //0 => Typeof; 1 => sizeof


	PropertyNode(Token varNameToken, int type) {
		this->varNameToken.clear();
		this->varNameToken = varNameToken;
		this->startPos = varNameToken.startPos;
		this->endPos = varNameToken.endPos;
		this->type = type;
	}
	std::string toString() {
		return (!this->type ? "typeof " : "sizeof ") + this->varNameToken.toString();
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
	int type = 0; // 0 => Index; 1 => Character;

	VarIndexAccessNode(const VarIndexAccessNode& node, Allocator allocationTable, bool passOn);
	VarIndexAccessNode(Token varNameToken, Node* index, int type);
	VarIndexAccessNode(Token varNameToken, Node* index, int type, Allocator allocationTable, bool passOn);
	std::string toString();
	void clear();


};

class UnaryNode : public BaseNode {
public:
	Node* nodeOn = nullptr;
	Token opToken = null;

	UnaryNode(const UnaryNode& node, Allocator allocationTable, bool passOn);
	UnaryNode(Token opToken, Node* nodeOn);
	UnaryNode(Token opToken, Node* nodeOn, Allocator allocationTable, bool passOn);

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

	BinaryNode(const BinaryNode& node, Allocator allocationTable, bool passOn);
	BinaryNode(Token opToken, Node* leftNode, Node* rightNode);
	BinaryNode(Token opToken, Node* leftNode, Node* rightNode, Allocator allocationTable, bool passOn);

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
	IfNode(const IfNode& node, Allocator allocationTable, bool passOn);
	IfNode(Node* contidtion, Node* body, Node* elseStatement);
	IfNode(Node* contidtion, Node* body, Node* elseStatement, Allocator allocationTable, bool passOn);

};

class IterationNode : public BaseNode {
public:

	Node* body;
	Node* iterations;

	std::string toString();
	void clear();
	IterationNode(Node* iterations, Node* body);
	IterationNode(const IterationNode& node, Allocator allocationTable, bool passOn);
	IterationNode(Node* iterations, Node* body, Allocator allocationTable, bool passOn);

};
class TimedIterationNode : public BaseNode {
public:

	Node* body;
	Node* seconds;

	std::string toString();
	void clear();
	TimedIterationNode(const TimedIterationNode& node, Allocator allocationTable, bool passOn);
	TimedIterationNode(Node* seconds, Node* body);
	TimedIterationNode(Node* seconds, Node* body, Allocator allocationTable, bool passOn);

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
	FunctionDefinitionNode(const FunctionDefinitionNode& node, Allocator allocationTable, bool passOn);
	FunctionDefinitionNode(std::vector<Argument> arguments, Token varNameToken, Value::valueType returnType, Node* body);
	FunctionDefinitionNode(std::vector<Argument> arguments, Token varNameToken, Value::valueType returnType, Node* body, Allocator allocationTable, bool passOn);

};
class FunctionCallNode : public BaseNode {
public:
	std::vector<Node*> argumentsInOrder;
	Token varNameToken = null;

	FunctionCallNode(const FunctionCallNode& node, Allocator allocationTable, bool passOn);
	FunctionCallNode(std::vector<Node*> argumentsInOrder, Token varNameToken);
	FunctionCallNode(std::vector<Node*> argumentsInOrder, Token varNameToken, Allocator allocationTable, bool passOn);
	std::string toString();
	void clear();

};
class ReturnNode : public BaseNode {
public:
	Node* value;

	std::string toString();
	void clear();
	ReturnNode(Node* value, Allocator allocationTable, bool passOn);
	ReturnNode(Node* value);
	ReturnNode(const ReturnNode& node, Allocator allocationTable, bool passOn);

};
class UndefineNode : public BaseNode {
public:
	bool type; //False = variable, true = function
	Token varNameToken = null;

	std::string toString() {
		return (this->type ? "Undef Func" : "Undef Var ") + this->varNameToken.toString();
	}
	void clear() {};
	UndefineNode(bool type, Token varNameToken)
		: type(type), varNameToken(varNameToken) {
		this->startPos = varNameToken.startPos;
		this->endPos = varNameToken.endPos;
	}

};

class OutputNode : public BaseNode {
public:

	Node* object;
	bool printLine = false;
	short type = 0; // 0 => Print; 1 => Command

	std::string toString();
	void clear();
	OutputNode(Node* object, bool printLine, short type, Allocator allocationTable, bool passOn);
	OutputNode(Node* object, bool printLine, short type);
	OutputNode(const OutputNode& node, Allocator allocationTable, bool passOn);
};

class InputNode : public BaseNode {
public:

	std::string toString() {
		return "ConsoleInput";
	}
	void clear() {}
};

class InterruptionNode : public BaseNode {
public:

	Token type = null;

	InterruptionNode(Token type) {
		this->type = type;
		this->startPos = type.startPos;
		this->endPos = type.endPos;
	}
	std::string toString() {
		return type.matches(TT_KEYWORD_CONTINUE) ? "Continue" : "Break";
	}
	void clear() {}

};

class LoopNode : public BaseNode {
public:

	Node* condition;
	Node* body;

	std::string toString();
	void clear();
	LoopNode(const LoopNode& node, Allocator allocationTable, bool passOn);
	LoopNode(Node* contidtion, Node* body);
	LoopNode(Node* contidtion, Node* body, Allocator allocationTable, bool passOn);
};

class Node : public Object {
public:
	void* nodePtr = nullptr;
	Class type = Class::Node;
	Position getStartPos() {
		switch (this->type) {
		case Class::NumberNode: return ((NumberNode*)this->nodePtr)->startPos; break;
		case Class::BooleanNode: return ((BooleanNode*)this->nodePtr)->startPos; break;
		case Class::StringNode: return ((StringNode*)this->nodePtr)->startPos; break;
		case Class::ArrayNode: return  ((ArrayNode*)this->nodePtr)->startPos; break;
		case Class::ListNode: return   ((ListNode*)this->nodePtr)->startPos; break;
		case Class::PropertyNode: return   ((PropertyNode*)this->nodePtr)->startPos; break;
		case Class::VarAccessNode: return   ((VarAccessNode*)this->nodePtr)->startPos; break;
		case Class::VarIndexNode: return ((VarIndexAccessNode*)this->nodePtr)->startPos; break;
		case Class::VarAssignNode: return   ((VarAssignNode*)this->nodePtr)->startPos; break;
		case Class::VarReAssignNode: return ((VarReAssignNode*)this->nodePtr)->startPos; break;
		case Class::VarIndexReAssignNode: return ((VarIndexReAssignNode*)this->nodePtr)->startPos; break;
		case Class::UnaryNode: return  ((UnaryNode*)this->nodePtr)->startPos; break;
		case Class::BinaryNode: return ((BinaryNode*)this->nodePtr)->startPos; break;
		case Class::IfNode: return ((IfNode*)this->nodePtr)->startPos; break;
		case Class::IterationNode: return ((IterationNode*)this->nodePtr)->startPos; break;
		case Class::TimedIterationNode: return ((TimedIterationNode*)this->nodePtr)->startPos; break;
		case Class::FunctionDefinitionNode: return ((FunctionDefinitionNode*)this->nodePtr)->startPos; break;
		case Class::FunctionCallNode: return ((FunctionCallNode*)this->nodePtr)->startPos; break;
		case Class::ReturnNode: return ((ReturnNode*)this->nodePtr)->startPos; break;
		case Class::OutputNode: return ((OutputNode*)this->nodePtr)->startPos; break;
		case Class::InputNode: return ((InputNode*)this->nodePtr)->startPos; break;
		case Class::InterruptionNode: return ((InterruptionNode*)this->nodePtr)->startPos; break;
		case Class::LoopNode: return ((LoopNode*)this->nodePtr)->startPos; break;
		}
		return Position(0, 0, 0);
	};
	Position getEndPos() {
		switch (this->type) {
		case Class::NumberNode: return ((NumberNode*)this->nodePtr)->endPos; break;
		case Class::BooleanNode: return ((BooleanNode*)this->nodePtr)->endPos; break;
		case Class::StringNode: return ((StringNode*)this->nodePtr)->endPos; break;
		case Class::ArrayNode: return  ((ArrayNode*)this->nodePtr)->endPos; break;
		case Class::ListNode: return   ((ListNode*)this->nodePtr)->endPos; break;
		case Class::PropertyNode: return   ((PropertyNode*)this->nodePtr)->endPos; break;
		case Class::VarAccessNode: return   ((VarAccessNode*)this->nodePtr)->endPos; break;
		case Class::VarIndexNode: return ((VarIndexAccessNode*)this->nodePtr)->endPos; break;
		case Class::VarAssignNode: return   ((VarAssignNode*)this->nodePtr)->endPos; break;
		case Class::VarReAssignNode: return ((VarReAssignNode*)this->nodePtr)->endPos; break;
		case Class::VarIndexReAssignNode: return ((VarIndexReAssignNode*)this->nodePtr)->endPos; break;
		case Class::UnaryNode: return  ((UnaryNode*)this->nodePtr)->endPos; break;
		case Class::BinaryNode: return ((BinaryNode*)this->nodePtr)->endPos; break;
		case Class::IfNode: return ((IfNode*)this->nodePtr)->endPos; break;
		case Class::IterationNode: return ((IterationNode*)this->nodePtr)->endPos; break;
		case Class::TimedIterationNode: return ((TimedIterationNode*)this->nodePtr)->endPos; break;
		case Class::FunctionDefinitionNode: return ((FunctionDefinitionNode*)this->nodePtr)->endPos; break;
		case Class::FunctionCallNode: return ((FunctionCallNode*)this->nodePtr)->endPos; break;
		case Class::ReturnNode: return ((ReturnNode*)this->nodePtr)->endPos; break;
		case Class::OutputNode: return ((OutputNode*)this->nodePtr)->endPos; break;
		case Class::InputNode: return ((InputNode*)this->nodePtr)->endPos; break;
		case Class::InterruptionNode: return ((InterruptionNode*)this->nodePtr)->endPos; break;
		case Class::LoopNode: return ((LoopNode*)this->nodePtr)->endPos; break;
		}
		return Position(0, 0, 0);
	};

	void clear() {
		switch (this->type) {
		case Class::NumberNode: ((NumberNode*)this->nodePtr)->clear(); break;
		case Class::BooleanNode:((BooleanNode*)this->nodePtr)->clear(); break;
		case Class::StringNode: ((StringNode*)this->nodePtr)->clear(); break;
		case Class::ArrayNode:  ((ArrayNode*)this->nodePtr)->clear(); break;
		case Class::ListNode:   ((ListNode*)this->nodePtr)->clear(); break;
		case Class::PropertyNode:   ((PropertyNode*)this->nodePtr)->clear(); break;
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
		case Class::OutputNode: ((OutputNode*)this->nodePtr)->clear(); break;
		case Class::InputNode: ((InputNode*)this->nodePtr)->clear(); break;
		case Class::InterruptionNode: ((InterruptionNode*)this->nodePtr)->clear(); break;
		case Class::LoopNode: ((LoopNode*)this->nodePtr)->clear(); break;
		}
	}

	Node(const Node& node, Allocator allocationTable, bool passOn) {
		this->type = node.type;
		if (passOn) {
			switch (this->type) {
			case Class::NumberNode: this->nodePtr = new NumberNode(*(NumberNode*)node.nodePtr); break;
			case Class::StringNode: this->nodePtr = new StringNode(*(StringNode*)node.nodePtr); break;
			case Class::BooleanNode: this->nodePtr = new BooleanNode(*(BooleanNode*)node.nodePtr); break;
			case Class::ArrayNode: this->nodePtr = new ArrayNode(*(ArrayNode*)node.nodePtr, allocationTable, true); break;
			case Class::ListNode: this->nodePtr = new ListNode(*(ListNode*)node.nodePtr, allocationTable, true); break;
			case Class::PropertyNode: this->nodePtr = new PropertyNode(*(PropertyNode*)node.nodePtr); break;
			case Class::VarAccessNode: this->nodePtr = new VarAccessNode(*(VarAccessNode*)node.nodePtr); break;
			case Class::VarIndexNode: this->nodePtr = new VarIndexAccessNode(*(VarIndexAccessNode*)node.nodePtr, allocationTable, true); break;
			case Class::VarAssignNode: this->nodePtr = new VarAssignNode(*(VarAssignNode*)node.nodePtr, allocationTable, true); break;
			case Class::VarReAssignNode: this->nodePtr = new VarReAssignNode(*(VarReAssignNode*)node.nodePtr, allocationTable, true); break;
			case Class::VarIndexReAssignNode: this->nodePtr = new VarIndexReAssignNode(*(VarIndexReAssignNode*)node.nodePtr, allocationTable, true); break;
			case Class::UnaryNode: this->nodePtr = new UnaryNode(*(UnaryNode*)node.nodePtr, allocationTable, true); break;
			case Class::BinaryNode: this->nodePtr = new BinaryNode(*(BinaryNode*)node.nodePtr, allocationTable, true); break;
			case Class::IfNode: this->nodePtr = new IfNode(*(IfNode*)node.nodePtr, allocationTable, true); break;
			case Class::LoopNode: this->nodePtr = new LoopNode(*(LoopNode*)node.nodePtr, allocationTable, true); break;
			case Class::IterationNode: this->nodePtr = new IterationNode(*(IterationNode*)node.nodePtr, allocationTable, true); break;
			case Class::TimedIterationNode: this->nodePtr = new TimedIterationNode(*(TimedIterationNode*)node.nodePtr, allocationTable, true); break;
			case Class::FunctionDefinitionNode: this->nodePtr = new FunctionDefinitionNode(*(FunctionDefinitionNode*)node.nodePtr, allocationTable, true); break;
			case Class::FunctionCallNode: this->nodePtr = new FunctionCallNode(*(FunctionCallNode*)node.nodePtr, allocationTable, true); break;
			case Class::ReturnNode: this->nodePtr = new ReturnNode(*(ReturnNode*)node.nodePtr, allocationTable, true); break;
			case Class::UndefineNode: this->nodePtr = new UndefineNode(*(UndefineNode*)node.nodePtr); break;
			case Class::OutputNode: this->nodePtr = new OutputNode(*(OutputNode*)node.nodePtr); break;
			case Class::InputNode: this->nodePtr = new InputNode(*(InputNode*)node.nodePtr); break;
			case Class::InterruptionNode: this->nodePtr = new InterruptionNode(*(InterruptionNode*)node.nodePtr); break;
			}
		}
		else {
			switch (this->type) {
			case Class::NumberNode: this->nodePtr = new NumberNode(*(NumberNode*)node.nodePtr); break;
			case Class::StringNode: this->nodePtr = new StringNode(*(StringNode*)node.nodePtr); break;
			case Class::BooleanNode: this->nodePtr = new BooleanNode(*(BooleanNode*)node.nodePtr); break;
			case Class::ArrayNode: this->nodePtr = new ArrayNode(*(ArrayNode*)node.nodePtr); break;
			case Class::ListNode: this->nodePtr = new ListNode(*(ListNode*)node.nodePtr); break;
			case Class::PropertyNode: this->nodePtr = new PropertyNode(*(PropertyNode*)node.nodePtr); break;
			case Class::VarAccessNode: this->nodePtr = new VarAccessNode(*(VarAccessNode*)node.nodePtr); break;
			case Class::VarIndexNode: this->nodePtr = new VarIndexAccessNode(*(VarIndexAccessNode*)node.nodePtr); break;
			case Class::VarAssignNode: this->nodePtr = new VarAssignNode(*(VarAssignNode*)node.nodePtr); break;
			case Class::VarReAssignNode: this->nodePtr = new VarReAssignNode(*(VarReAssignNode*)node.nodePtr); break;
			case Class::VarIndexReAssignNode: this->nodePtr = new VarIndexReAssignNode(*(VarIndexReAssignNode*)node.nodePtr); break;
			case Class::UnaryNode: this->nodePtr = new UnaryNode(*(UnaryNode*)node.nodePtr); break;
			case Class::BinaryNode: this->nodePtr = new BinaryNode(*(BinaryNode*)node.nodePtr); break;
			case Class::IfNode: this->nodePtr = new IfNode(*(IfNode*)node.nodePtr); break;
			case Class::IterationNode: this->nodePtr = new IterationNode(*(IterationNode*)node.nodePtr); break;
			case Class::TimedIterationNode: this->nodePtr = new TimedIterationNode(*(TimedIterationNode*)node.nodePtr); break;
			case Class::FunctionDefinitionNode: this->nodePtr = new FunctionDefinitionNode(*(FunctionDefinitionNode*)node.nodePtr); break;
			case Class::FunctionCallNode: this->nodePtr = new FunctionCallNode(*(FunctionCallNode*)node.nodePtr); break;
			case Class::ReturnNode: this->nodePtr = new ReturnNode(*(ReturnNode*)node.nodePtr); break;
			case Class::UndefineNode: this->nodePtr = new UndefineNode(*(UndefineNode*)node.nodePtr); break;
			case Class::OutputNode: this->nodePtr = new OutputNode(*(OutputNode*)node.nodePtr); break;
			case Class::InputNode: this->nodePtr = new InputNode(*(InputNode*)node.nodePtr); break;
			case Class::InterruptionNode: this->nodePtr = new InterruptionNode(*(InterruptionNode*)node.nodePtr); break;
			case Class::LoopNode: this->nodePtr = new LoopNode(*(LoopNode*)node.nodePtr); break;
			}
		}
		allocationTable.registerAllocation(this->nodePtr);
	}

	Node(const Node& node) {
		this->type = node.type;
		this->nodePtr = node.nodePtr;
	}

	Node() {
		this->mClass = Class::Node;
	}

	Node(NumberNode* node) {
		this->nodePtr = node;
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
	Node(PropertyNode* node) {
		this->nodePtr = node;
		this->type = Class::PropertyNode;
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
	Node(UndefineNode* node) {
		this->nodePtr = node;
		this->type = Class::UndefineNode;
	}
	Node(OutputNode* node) {
		this->nodePtr = node;
		this->type = Class::OutputNode;
	}
	Node(InputNode* node) {
		this->nodePtr = node;
		this->type = Class::InputNode;
	}
	Node(InterruptionNode* node) {
		this->nodePtr = node;
		this->type = Class::InterruptionNode;
	}
	Node(LoopNode* node) {
		this->nodePtr = node;
		this->type = Class::LoopNode;
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
		case Class::PropertyNode: return ((PropertyNode*)this->nodePtr)->toString(); break;
		case Class::IfNode: return ((IfNode*)this->nodePtr)->toString(); break;
		case Class::IterationNode: return ((IterationNode*)this->nodePtr)->toString(); break;
		case Class::TimedIterationNode: return ((TimedIterationNode*)this->nodePtr)->toString(); break;
		case Class::FunctionDefinitionNode: return ((FunctionDefinitionNode*)this->nodePtr)->toString(); break;
		case Class::FunctionCallNode: return ((FunctionCallNode*)this->nodePtr)->toString(); break;
		case Class::ReturnNode: return ((ReturnNode*)this->nodePtr)->toString(); break;
		case Class::UndefineNode: return ((UndefineNode*)this->nodePtr)->toString(); break;
		case Class::OutputNode: return ((OutputNode*)this->nodePtr)->toString(); break;
		case Class::InputNode: return ((InputNode*)this->nodePtr)->toString(); break;
		case Class::InterruptionNode: return ((InterruptionNode*)this->nodePtr)->toString(); break;
		case Class::LoopNode: return ((LoopNode*)this->nodePtr)->toString(); break;
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
		this->startPos = nodeOn->getStartPos();
		this->endPos = opToken.endPos;
	}
	else {
		this->startPos = opToken.startPos;
		this->endPos = nodeOn->getEndPos();
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
UnaryNode::UnaryNode(const UnaryNode& node, Allocator allocationTable, bool passOn) {
	this->nodeOn = new Node(*node.nodeOn, allocationTable, passOn);
	allocationTable.registerAllocation(this->nodeOn);
	this->opToken = node.opToken;
}

BinaryNode::BinaryNode(Token opToken, Node* leftNode, Node* rightNode) {
	this->opToken.clear();
	this->opToken = opToken;
	this->leftNode = leftNode;
	this->rightNode = rightNode;
	this->startPos = this->leftNode->getStartPos();
	this->endPos = this->rightNode->getEndPos();
}
BinaryNode::BinaryNode(Token opToken, Node* leftNode, Node* rightNode, Allocator allocationTable, bool passOn) {
	this->opToken.clear();
	this->opToken = opToken;
	this->leftNode = new Node(*leftNode, allocationTable, passOn);
	this->rightNode = new Node(*rightNode, allocationTable, passOn);
	allocationTable.registerAllocation(this->leftNode);
	allocationTable.registerAllocation(this->rightNode);
	this->startPos = this->leftNode->getStartPos();
	this->endPos = this->rightNode->getEndPos();
}
BinaryNode::BinaryNode(const BinaryNode& node, Allocator allocationTable, bool passOn) {
	this->opToken.clear();
	this->opToken = node.opToken;
	this->leftNode = new Node(*node.leftNode, allocationTable, passOn);
	this->rightNode = new Node(*node.rightNode, allocationTable, passOn);
	allocationTable.registerAllocation(this->leftNode);
	allocationTable.registerAllocation(this->rightNode);
	this->startPos = this->leftNode->getStartPos();
	this->endPos = this->rightNode->getEndPos();
}
std::string BinaryNode::toString() {
	return std::string("(") + this->leftNode->toString() + ", " + this->opToken.toString() + ", " + this->rightNode->toString() + ")";
}
void BinaryNode::clear() {
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
VarIndexAccessNode::VarIndexAccessNode(Token varNameToken, Node* index, int type) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->index = index;
	this->startPos = index->getStartPos();
	this->endPos = varNameToken.endPos;
	this->type = type;
}
VarIndexAccessNode::VarIndexAccessNode(Token varNameToken, Node* index, int type, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->index = new Node(*index, allocationTable, passOn);
	allocationTable.registerAllocation(this->index);
	this->startPos = index->getStartPos();
	this->endPos = varNameToken.endPos;
	this->type = type;
}
VarIndexAccessNode::VarIndexAccessNode(const VarIndexAccessNode& node, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = node.varNameToken;
	this->index = new Node(*node.index, allocationTable, passOn);
	allocationTable.registerAllocation(this->index);
	this->startPos = node.index->getStartPos();
	this->endPos = node.varNameToken.endPos;
	this->type = node.type;
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
	this->startPos = index->getStartPos();
	this->endPos = value->getEndPos();
	this->index = index;
}
VarIndexReAssignNode::VarIndexReAssignNode(Token varNameToken, Node* value, Node* index, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = new Node(*value, allocationTable, passOn);
	this->startPos = index->getStartPos();
	this->endPos = value->getEndPos();
	this->index = new Node(*index, allocationTable, passOn);
	allocationTable.registerAllocation(this->index);
	allocationTable.registerAllocation(this->value);
}
VarIndexReAssignNode::VarIndexReAssignNode(const VarIndexReAssignNode& node, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = node.varNameToken;
	this->value = new Node(*node.value, allocationTable, passOn);
	this->startPos = node.index->getStartPos();
	this->endPos = node.value->getEndPos();
	this->index = new Node(*node.index, allocationTable, passOn);
	allocationTable.registerAllocation(this->index);
	allocationTable.registerAllocation(this->value);
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
	this->startPos = value->getStartPos();
	this->endPos = value->getEndPos();
	this->type = type;
}
VarAssignNode::VarAssignNode(Token varNameToken, Node* value, Value::valueType type, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = new Node(*value, allocationTable, passOn);
	allocationTable.registerAllocation(this->value);
	this->startPos = value->getStartPos();
	this->endPos = value->getEndPos();
	this->type = type;
}
VarAssignNode::VarAssignNode(const VarAssignNode& node, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = node.varNameToken;
	this->value = new Node(*node.value, allocationTable, passOn);
	allocationTable.registerAllocation(this->value);
	this->startPos = node.value->getStartPos();
	this->endPos = node.value->getEndPos();
	this->type = node.type;
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
	this->startPos = value->getStartPos();
	this->endPos = value->getEndPos();
}
VarReAssignNode::VarReAssignNode(Token varNameToken, Node* value, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = varNameToken;
	this->value = new Node(*value, allocationTable, passOn);
	allocationTable.registerAllocation(this->value);
	this->startPos = value->getStartPos();
	this->endPos = value->getEndPos();
}
VarReAssignNode::VarReAssignNode(const VarReAssignNode& node, Allocator allocationTable, bool passOn) {
	this->varNameToken.clear();
	this->varNameToken = node.varNameToken;
	this->value = new Node(*node.value, allocationTable, passOn);
	allocationTable.registerAllocation(this->value);
	this->startPos = value->getStartPos();
	this->endPos = value->getEndPos();
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
IfNode::IfNode(Node* condition, Node* body, Node* elseStatement, Allocator allocationTable, bool passOn) {
	this->condition = new Node(*condition, allocationTable, passOn);
	this->body = new Node(*body, allocationTable, passOn);
	allocationTable.registerAllocation(this->condition);
	allocationTable.registerAllocation(this->body);
	if (elseStatement != nullptr) {
		this->elseStatement = new Node(*elseStatement, allocationTable, passOn);
		allocationTable.registerAllocation(this->elseStatement);
	}
}
IfNode::IfNode(const IfNode& node, Allocator allocationTable, bool passOn) {
	this->condition = new Node(*node.condition, allocationTable, passOn);
	this->body = new Node(*node.body, allocationTable, passOn);
	allocationTable.registerAllocation(this->condition);
	allocationTable.registerAllocation(this->body);
	if (node.elseStatement != nullptr) {
		this->elseStatement = new Node(*node.elseStatement, allocationTable, passOn);
		allocationTable.registerAllocation(this->elseStatement);
	}
}
void IfNode::clear() {
	this->condition->clear();
	this->body->clear();
	if (this->elseStatement != nullptr) {
		this->elseStatement->clear();
	}
}
std::string IfNode::toString() {
	if (this->elseStatement == nullptr)
		return std::string("if ") + this->condition->toString() + " do " + this->body->toString();
	return std::string("if ") + this->condition->toString() + " do " + this->body->toString() + " ( else " + this->elseStatement->toString() + ")";
}

IterationNode::IterationNode(Node* iterations, Node* body) {
	this->iterations = iterations;
	this->body = body;
}
IterationNode::IterationNode(Node* iterations, Node* body, Allocator allocationTable, bool passOn) {
	this->iterations = new Node(*iterations, allocationTable, passOn);
	this->body = new Node(*body, allocationTable, passOn);
	allocationTable.registerAllocation(this->iterations);
	allocationTable.registerAllocation(this->body);
}
IterationNode::IterationNode(const IterationNode& node, Allocator allocationTable, bool passOn) {
	this->iterations = new Node(*node.iterations, allocationTable, passOn);
	this->body = new Node(*node.body, allocationTable, passOn);
	allocationTable.registerAllocation(this->iterations);
	allocationTable.registerAllocation(this->body);
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
TimedIterationNode::TimedIterationNode(Node* seconds, Node* body, Allocator allocationTable, bool passOn) {
	this->seconds = new Node(*seconds, allocationTable, passOn);
	this->body = new Node(*body, allocationTable, passOn);
	allocationTable.registerAllocation(this->seconds);
	allocationTable.registerAllocation(this->body);
}
TimedIterationNode::TimedIterationNode(const TimedIterationNode& node, Allocator allocationTable, bool passOn) {
	this->seconds = new Node(*node.seconds, allocationTable, passOn);
	this->body = new Node(*node.body, allocationTable, passOn);
	allocationTable.registerAllocation(this->seconds);
	allocationTable.registerAllocation(this->body);
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
FunctionDefinitionNode::FunctionDefinitionNode(std::vector<Argument> arguments, Token varNameToken, Value::valueType returnType, Node* body, Allocator allocationTable, bool passOn) {
	this->arguments = arguments;
	this->varNameToken = varNameToken;
	this->returnType = returnType;
	this->body = new Node(*body, allocationTable, passOn);
	allocationTable.registerAllocation(this->body);
}
FunctionDefinitionNode::FunctionDefinitionNode(const FunctionDefinitionNode& node, Allocator allocationTable, bool passOn) {
	this->arguments = node.arguments;
	this->varNameToken = node.varNameToken;
	this->returnType = node.returnType;
	this->body = new Node(*node.body, allocationTable, passOn);
	allocationTable.registerAllocation(this->body);
}

void FunctionDefinitionNode::clear() {
	this->varNameToken.clear();
	this->body->clear();
	this->arguments.clear();
}
std::string FunctionDefinitionNode::toString() {
	std::string args = "[ ";
	unsigned long long l = 0;
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
FunctionCallNode::FunctionCallNode(const FunctionCallNode& node, Allocator allocationTable, bool passOn) {
	for (int i = 0; i < node.argumentsInOrder.size(); i++) {
		this->argumentsInOrder.push_back(new Node(*node.argumentsInOrder[i], allocationTable, passOn));
		allocationTable.registerAllocation(this->argumentsInOrder[i]);
	}
	this->varNameToken = node.varNameToken;
}
FunctionCallNode::FunctionCallNode(std::vector<Node*> argumentsInOrder, Token varNameToken, Allocator allocationTable, bool passOn) {
	for (int i = 0; i < argumentsInOrder.size(); i++) {
		this->argumentsInOrder.push_back(new Node(*argumentsInOrder[i], allocationTable, passOn));
		allocationTable.registerAllocation(this->argumentsInOrder[i]);
	}
	this->varNameToken = varNameToken;
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
	unsigned long long l = 0;
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
ListNode::ListNode(std::vector<Node*> nodes, Allocator allocationTable, bool passOn) {
	this->nodes.clear();
	for (int i = 0; i < nodes.size(); i++) {
		this->nodes.push_back(new Node(*nodes[i], allocationTable, passOn));
		allocationTable.registerAllocation(this->nodes[i]);
	}
	this->mClass = Class::ListNode;
}
ListNode::ListNode(const ListNode& node, Allocator allocationTable, bool passOn) {
	this->startPos = node.startPos;
	this->endPos = node.endPos;
	for (int i = 0; i < this->nodes.size(); i++) {
		this->nodes[i]->clear();
	}
	this->nodes.clear();
	for (int i = 0; i < node.nodes.size(); i++) {
		this->nodes.push_back(new Node(*node.nodes[i], allocationTable, passOn));
		allocationTable.registerAllocation(this->nodes[i]);
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

ArrayNode::ArrayNode(std::vector<Node*> nodes, Token type, Allocator allocationTable, bool passOn) {
	this->type.clear();
	for (int i = 0; i < nodes.size(); i++) {
		if (!passOn)
			this->nodes.push_back(new Node(*nodes[i]));
		else
			this->nodes.push_back(new Node(*nodes[i], allocationTable, passOn));
		allocationTable.registerAllocation(this->nodes[i]);
	}
	this->type = type;
	this->mClass = Class::ArrayNode;
}
ArrayNode::ArrayNode(const ArrayNode& node, Allocator allocationTable, bool passOn) {
	this->type.clear();
	this->type = node.type;
	this->startPos = node.type.startPos;
	this->endPos = node.type.endPos;
	for (int i = 0; i < this->nodes.size(); i++) {
		this->nodes[i]->clear();
	}
	this->nodes.clear();
	for (int i = 0; i < node.nodes.size(); i++) {
		this->nodes.push_back(new Node(*node.nodes[i], allocationTable, passOn));
		allocationTable.registerAllocation(this->nodes[i]);
	}
	this->mClass = Class::ArrayNode;
}

ReturnNode::ReturnNode(Node* value) {
	this->value = value;
	this->startPos = value->getStartPos();
	this->endPos = value->getEndPos();
	this->mClass = Class::ReturnNode;
}
ReturnNode::ReturnNode(Node* value, Allocator allocationTable, bool passOn) {
	this->value = new Node(*value, allocationTable, passOn);
	allocationTable.registerAllocation(this->value);
	this->mClass = Class::ReturnNode;
	this->startPos = value->getStartPos();
	this->endPos = value->getEndPos();
}
ReturnNode::ReturnNode(const ReturnNode& node, Allocator allocationTable, bool passOn) {
	this->value = new Node(*node.value, allocationTable, passOn);
	allocationTable.registerAllocation(this->value);
	this->mClass = Class::ReturnNode;
	this->startPos = node.startPos;
	this->endPos = node.endPos;

}
void ReturnNode::clear() {
	this->value->clear();
}
std::string ReturnNode::toString() {
	return "return " + this->value->toString();
}

OutputNode::OutputNode(Node* object, bool printLine, short type) {
	this->object = object;
	this->printLine = printLine;
	this->mClass = Class::ReturnNode;
	this->type = type;
}
OutputNode::OutputNode(Node* value, bool printLine, short type, Allocator allocationTable, bool passOn) {
	this->object = new Node(*value, allocationTable, passOn);
	this->printLine = printLine;
	this->type = type;
	allocationTable.registerAllocation(this->object);
	this->mClass = Class::ReturnNode;
}
OutputNode::OutputNode(const OutputNode& node, Allocator allocationTable, bool passOn) {
	this->object = new Node(*node.object, allocationTable, passOn);
	allocationTable.registerAllocation(this->object);
	this->mClass = Class::ReturnNode;
	this->printLine = node.printLine;
	this->type = node.type;

}
void OutputNode::clear() {
	if (this->object != nullptr)
		this->object->clear();
}
std::string OutputNode::toString() {
	if (this->object == nullptr)
		return "print a newline";
	return (this->type == 0 ? "print " : "consoleCMD ") + this->object->toString();
}

LoopNode::LoopNode(Node* condition, Node* body) {
	this->condition = condition;
	this->body = body;
}
LoopNode::LoopNode(Node* condition, Node* body, Allocator allocationTable, bool passOn) {
	this->condition = new Node(*condition, allocationTable, passOn);
	allocationTable.registerAllocation(this->condition);
	this->body = new Node(*body, allocationTable, passOn);
	allocationTable.registerAllocation(this->body);
}
LoopNode::LoopNode(const LoopNode& node, Allocator allocationTable, bool passOn) {
	this->condition = new Node(*node.condition, allocationTable, passOn);
	allocationTable.registerAllocation(this->condition);
	this->body = new Node(*node.body, allocationTable, passOn);
	allocationTable.registerAllocation(this->body);
}
void LoopNode::clear() {
	this->condition->clear();
	this->body->clear();
}
std::string LoopNode::toString() {
	return std::string("as long as ") + this->condition->toString() + " do " + this->body->toString();
}