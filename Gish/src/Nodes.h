
class NodeBase {
public:
	Position* startPosition;
	Position* endPosition;
	
};

class NodeWrapper;


class ValueNode : public NodeBase {

public:
	ValueNode(Token* value);
	Token* getValue();

	std::string toString();

private:
	Token* valueToken = nullptr;

};

class ArrayNode : public NodeBase {

public:
	ArrayNode(std::vector<NodeWrapper*>* nodes, Token* typeToken);
	std::vector<NodeWrapper*>* getNodes();
	Token* getType();

	std::string toString();

private:
	std::vector<NodeWrapper*>* nodes = nullptr;
	Token* typeToken = nullptr;

};

class BinaryNode : public NodeBase {
	
	public:
		BinaryNode(NodeWrapper* left, Token* operatorToken, NodeWrapper* right);
		NodeWrapper* getLeft();
		NodeWrapper* getRight();
		
		std::string toString();
		
	private:
		NodeWrapper* left = nullptr;
		Token* operatorToken = nullptr;
		NodeWrapper* right = nullptr;
		
};

class UnaryNode : public NodeBase {
	
	public:
		UnaryNode(Token* operatorToken, NodeWrapper* right);
		NodeWrapper* getRight();
		
		std::string toString();
		
	private:
		Token* operatorToken = nullptr;
		NodeWrapper* right = nullptr;
		
};

class ListNode : public NodeBase {
	
	public:
		ListNode(std::vector<NodeWrapper*>* nodes);
		std::vector<NodeWrapper*>* getNodes();
		
		std::string toString();
		
	private:
		std::vector<NodeWrapper*>* nodes = nullptr;
		
};

class VariableDeclarationNode : public NodeBase {
	
	public:
		VariableDeclarationNode(Token* typeToken, Token* nameToken, NodeWrapper* value);
		Token* getType();
		Token* getName();
		NodeWrapper* getValue();
		
		std::string toString();
		
	private:
		Token* typeToken = nullptr;
		Token* nameToken = nullptr;
		NodeWrapper* value = nullptr;
		
};

class VariableReAssignNode : public NodeBase {
	
	public:
		VariableReAssignNode(Token* nameToken, NodeWrapper* value);
		Token* getName();
		NodeWrapper* getValue();
		
		std::string toString();
		
	private:
		Token* nameToken = nullptr;
		NodeWrapper* value = nullptr;
		
};

class VariableAccessNode : public NodeBase {
	
	public:
		VariableAccessNode(Token* nameToken);
		Token* getName();
		
		std::string toString();
		
	private:
		Token* nameToken = nullptr;
		
};

class NodeWrapper {
private:
	void* node = nullptr;	
	enum class NodeType {
		ValueNode,
		ArrayNode,
		BinaryNode,
		UnaryNode,
		ListNode,
		VariableDeclarationNode,
		VariableReAssignNode,
		VariableAccessNode
		
	} nodeType;
	
public:
	
	NodeWrapper(ValueNode* node) {
		this->node = node;
		nodeType = NodeType::ValueNode;
	}
	NodeWrapper(ArrayNode* node) {
		this->node = node;
		nodeType = NodeType::ArrayNode;
	}
	NodeWrapper(BinaryNode* node) {
		this->node = node;
		nodeType = NodeType::BinaryNode;
	}
	NodeWrapper(UnaryNode* node) {
		this->node = node;
		nodeType = NodeType::UnaryNode;
	}
	NodeWrapper(ListNode* node) {
		this->node = node;
		nodeType = NodeType::ListNode;
	}
	NodeWrapper(VariableDeclarationNode* node) {
		this->node = node;
		nodeType = NodeType::VariableDeclarationNode;
	}
	NodeWrapper(VariableReAssignNode* node) {
		this->node = node;
		nodeType = NodeType::VariableReAssignNode;
	}
	NodeWrapper(VariableAccessNode* node) {
		this->node = node;
		nodeType = NodeType::VariableAccessNode;
	}

	std::string typeName() {
		switch (nodeType) {
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
			default: 
				return "UnknownNode '" + std::to_string((int)nodeType) + "'";
		}
	}
	
	std::string toString() {
		switch (this->nodeType) {
			case NodeType::ValueNode:
				return ((ValueNode*)this->node)->toString();
				break;
			case NodeType::ArrayNode:
				return ((ArrayNode*)this->node)->toString();
				break;
			case NodeType::BinaryNode:
				return ((BinaryNode*)this->node)->toString();
				break;
			case NodeType::UnaryNode:
				return ((UnaryNode*)this->node)->toString();
				break;
			case NodeType::ListNode:
				return((ListNode*)this->node)->toString();
				break;
			case NodeType::VariableDeclarationNode:
				return ((VariableDeclarationNode*)this->node)->toString();
				break;
			case NodeType::VariableReAssignNode:
				return ((VariableReAssignNode*)this->node)->toString();
				break;
			case NodeType::VariableAccessNode:
				return ((VariableAccessNode*)this->node)->toString();
				break;
			default:
				return "Error getting Node values for type '" + typeName() + "'";
				break;
		}
	}

	Position* getStartPosition() {
		switch (this->nodeType) {
			case NodeType::ValueNode:
				return ((ValueNode*)this->node)->startPosition;
				break;
			case NodeType::ArrayNode:
				return ((ArrayNode*)this->node)->startPosition;
				break;
			case NodeType::BinaryNode:
				return ((BinaryNode*)this->node)->startPosition;
				break;
			case NodeType::UnaryNode:
				return ((UnaryNode*)this->node)->startPosition;
				break;
			case NodeType::ListNode:
				return ((ListNode*)this->node)->startPosition;
				break;
			case NodeType::VariableDeclarationNode:
				return ((VariableDeclarationNode*)this->node)->startPosition;
				break;
			case NodeType::VariableReAssignNode:
				return ((VariableReAssignNode*)this->node)->startPosition;
				break;
			case NodeType::VariableAccessNode:
				return ((VariableAccessNode*)this->node)->startPosition;
				break;
			default:
				return nullptr;
				break;
		}
	}

	Position* getEndPosition() {
		switch (this->nodeType) {
			case NodeType::ValueNode:
				return ((ValueNode*)this->node)->endPosition;
				break;
			case NodeType::ArrayNode:
				return ((ArrayNode*)this->node)->endPosition;
				break;
			case NodeType::BinaryNode:
				return ((BinaryNode*)this->node)->endPosition;
				break;
			case NodeType::UnaryNode:
				return ((UnaryNode*)this->node)->endPosition;
				break;
			case NodeType::ListNode:
				return ((ListNode*)this->node)->endPosition;
				break;
			case NodeType::VariableDeclarationNode:
				return ((VariableDeclarationNode*)this->node)->endPosition;
				break;
			case NodeType::VariableReAssignNode:
				return ((VariableReAssignNode*)this->node)->endPosition;
				break;
			case NodeType::VariableAccessNode:
				return ((VariableAccessNode*)this->node)->endPosition;
				break;
			default:
				return nullptr;
				break;
		}
	}

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

//--------------------------
//ArrayNode Defines
//--------------------------
ArrayNode::ArrayNode(std::vector<NodeWrapper*>* nodes, Token* typeToken) {
	this->startPosition = nodes->at(0)->getStartPosition();
	this->endPosition = nodes->at(nodes->size() - 1)->getEndPosition();
	this->nodes = nodes;
	this->typeToken = typeToken;
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

//--------------------------
//BinaryNode Defines
//--------------------------
BinaryNode::BinaryNode(NodeWrapper* left, Token* operatorToken, NodeWrapper* right) {
	this->startPosition = left->getStartPosition();
	this->endPosition = right->getEndPosition();
	this->left = left;
	this->operatorToken = operatorToken;
	this->right = right;
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
}
NodeWrapper* UnaryNode::getRight() {
	return this->right;
}
std::string UnaryNode::toString() {
	return "U( " + this->operatorToken->toString() + " " + this->right->toString() + " )U";
}

//--------------------------
//ListNode Defines
//--------------------------
ListNode::ListNode(std::vector<NodeWrapper*>* nodes) {
	this->startPosition = nodes->at(0)->getStartPosition();
	this->endPosition = nodes->at(nodes->size() - 1)->getEndPosition();
	this->nodes = nodes;
}
std::vector<NodeWrapper*>* ListNode::getNodes() {
	return this->nodes;
}
std::string ListNode::toString() {
	return "L[" + joinPointer(*this->nodes, ", ") + "]L";
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

//--------------------------
//VariableReAssignNode Defines
//--------------------------
VariableReAssignNode::VariableReAssignNode(Token* nameToken, NodeWrapper* value) {
	this->startPosition = &nameToken->startPos;
	this->endPosition = value->getEndPosition();
	this->nameToken = nameToken;
	this->value = value;
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