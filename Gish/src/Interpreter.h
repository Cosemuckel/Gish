

class Interpreter {

public:

	Value* run(NodeWrapper* node, Environment* env) {
		this->environment = env;
		return node->evaluate(this->environment);
	}

private:
	
	Environment* environment;

};