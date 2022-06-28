
#pragma once

//Holds a Node pointer
class ParserResult : public Result {
	
public:

	ParserResult(NodeWrapper* node, Error* error) {
		this->node = node;
		this->error = error;
	}
	
	ParserResult() {
		this->node = nullptr;
		this->error = nullptr;
	}

	NodeWrapper* Register(ParserResult& result) {
		if (result.error != nullptr) {
			this->error = result.error;
			return nullptr;
		}
		this->node = result.node;
		return this->node;
	}

	NodeWrapper* success(NodeWrapper* node) {
		this->node = node;
		return node;
	}

	NodeWrapper* failure(Error* error) {
		this->error = error;
	}
	
	std::string toString() {
		if (this->error != nullptr)
			return this->error->toString();
		return this->node->toString();
	}
	
private:

	NodeWrapper* node;
	
};

class Parser {
private:
	std::vector<Token>* tokens;
	int tokenIndex = -1;
	int lastToken = -1; //Pointing to element in tokens
	int currenToken = -1; //Pointing to element in tokens
	
public:

	
	
};