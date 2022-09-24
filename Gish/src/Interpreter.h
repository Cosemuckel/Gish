

class Interpreter {

public:

	Value* run(NodeWrapper* node) {
		return node->evaluate(this->environment);
	}

private:
	
	Environment* environment;

};