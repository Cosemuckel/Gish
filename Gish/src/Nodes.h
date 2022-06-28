
class NodeBase {
public:
	Position* startPosition;
	Position* endPosition;
	
};

class NodeWrapper;


class ValueNode : public NodeBase {
	
	public:
		ValueNode(Position* startPosition, Position* endPosition, Token* value);
		Token* getValue();
		
		std::string toString();

	private:
		Token* valueToken = nullptr;
		
};

class BinaryNode : public NodeBase {
	
	public:
		BinaryNode(Position* startPosition, Position* endPosition, NodeWrapper* left, Token* operatorToken, NodeWrapper* right);
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
		UnaryNode(Position* startPosition, Position* endPosition, Token* operatorToken, NodeWrapper* right);
		NodeWrapper* getRight();
		
		std::string toString();
		
	private:
		Token* operatorToken = nullptr;
		NodeWrapper* right = nullptr;
		
};

class ListNode : public NodeBase {
	
	public:
		ListNode(Position* startPosition, Position* endPosition, std::vector<NodeWrapper*>* nodes);
		std::vector<NodeWrapper*>* getNodes();
		
		std::string toString();
		
	private:
		std::vector<NodeWrapper*>* nodes = nullptr;
		
};

class NodeWrapper {
private:
	void* node = nullptr;	
	enum class NodeType {
		ValueNode,
		BinaryNode,
		UnaryNode,
		ListNode
		
	} nodeType;
	
public:
	
	NodeWrapper(ValueNode* node) {
		this->node = node;
		nodeType = NodeType::ValueNode;
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

	std::string toString() {
		switch (this->nodeType) {
			case NodeType::ValueNode:
				return ((ValueNode*)this->node)->toString();
				break;
			case NodeType::BinaryNode:
				return ((BinaryNode*)this->node)->toString();
				break;
			case NodeType::UnaryNode:
				return ((UnaryNode*)this->node)->toString();
				break;
			case NodeType::ListNode:
				return ((ListNode*)this->node)->toString();
				break;
			default:
				return "";
				break;
		}
	}


};

//--------------------------
//ValueNode Defines
//--------------------------
ValueNode::ValueNode(Position* startPosition, Position* endPosition, Token* value) {
	this->startPosition = startPosition;
	this->endPosition = endPosition;
	this->valueToken = value;
}
Token* ValueNode::getValue() {
	return this->valueToken;
}
std::string ValueNode::toString() {
	return this->valueToken->toString();
}

//--------------------------
//BinaryNode Defines
//--------------------------
BinaryNode::BinaryNode(Position* startPosition, Position* endPosition, NodeWrapper* left, Token* operatorToken, NodeWrapper* right) {
	this->startPosition = startPosition;
	this->endPosition = endPosition;
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
	return "( " + this->left->toString() + " " + this->operatorToken->toString() + " " + this->right->toString() + " )";
}

//--------------------------
//UnaryNode Defines
//--------------------------
UnaryNode::UnaryNode(Position* startPosition, Position* endPosition, Token* operatorToken, NodeWrapper* right) {
	this->startPosition = startPosition;
	this->endPosition = endPosition;
	this->operatorToken = operatorToken;
	this->right = right;
}
NodeWrapper* UnaryNode::getRight() {
	return this->right;
}
std::string UnaryNode::toString() {
	return "( " + this->operatorToken->toString() + " " + this->right->toString() + " )";
}

//--------------------------
//ListNode Defines
//--------------------------
ListNode::ListNode(Position* startPosition, Position* endPosition, std::vector<NodeWrapper*>* nodes) {
	this->startPosition = startPosition;
	this->endPosition = endPosition;
	this->nodes = nodes;
}
std::vector<NodeWrapper*>* ListNode::getNodes() {
	return this->nodes;
}
std::string ListNode::toString() {
	return "[ " + join(*this->nodes, ", ") + " ]";
}
