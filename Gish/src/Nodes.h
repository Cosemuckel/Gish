
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

	Position* getStartPosition() {
		switch (this->nodeType) {
			case NodeType::ValueNode:
				return ((ValueNode*)this->node)->startPosition;
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
			case NodeType::BinaryNode:
				return ((BinaryNode*)this->node)->endPosition;
				break;
			case NodeType::UnaryNode:
				return ((UnaryNode*)this->node)->endPosition;
				break;
			case NodeType::ListNode:
				return ((ListNode*)this->node)->endPosition;
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
	return "[ " + joinPointer(*this->nodes, ", ") + " ]";
}


