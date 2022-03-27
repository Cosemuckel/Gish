#pragma once


class RuntimeError : public Error {
public:

	RuntimeError(std::string details, Position startPos, Position endPos) 
		: Error("RuntimeError", details, startPos, endPos) {}

};

std::string stringTimes(std::string string, Number number) {
	return "";
}
std::string stringDivis(std::string string, Number number) {
	return "";
}

class InterpretedValue;

class InterpretedValue : public Value {
public:

	Position startPos;
	Position endPos;
	Error error = Error(null);

	InterpretedValue() {
	}

	InterpretedValue(const InterpretedValue& interpretedValue)
		:Value(interpretedValue) {
		this->startPos = interpretedValue.startPos;
		this->endPos = interpretedValue.endPos;
		this->error = interpretedValue.error;
	}

	InterpretedValue(Value value, Error error)
		:Value(value) {
		this->error = error;
	}
	InterpretedValue(Value value)
		:Value(value) {
	}

	std::string toString() {
		return Value::toString();
	}

	InterpretedValue addedTo(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.plus(value.cNumber), null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't add a ") + value.Name() + " to a Number", startPos, endPos));
		}
		if (this->type == Value::valueType::String) {
			if (value.type == Value::valueType::String)
				return InterpretedValue(this->cString + value.cString, null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't add a ") + value.Name() + " to a String", startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't add a ") + value.Name() + " to a " + this->Name(), startPos, endPos));
	}
	InterpretedValue subbedBy(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.minus(value.cNumber), null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't subtract a ") + value.Name() + " from a Number", startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't subtract a ") + value.Name() + " from a " + this->Name(), startPos, endPos));
	}
	InterpretedValue multipliedWith(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.times(value.cNumber), null);
			if (value.type == Value::valueType::String)
				if (this->cNumber.type ? (double)this->cNumber.value < 0.0f : (long long)this->cNumber.value < 0ll) return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't mutiple a String with a negative Number"), startPos, endPos));
				else return InterpretedValue(stringTimes(value.cString, this->cNumber), null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't multiply a Number with a ") + value.Name(), startPos, endPos));
		}
		if (this->type == Value::valueType::String) {
			if (value.type == Value::valueType::Number)
				if (value.cNumber.type ? (double)value.cNumber.value < 0.0f : (long long)value.cNumber.value < 0ll) return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't mutiple a String with a negative Number"), startPos, endPos));
				else return InterpretedValue(stringTimes(this->cString, value.cNumber), null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't multiply a String with a ") + value.Name(), startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't multiply a ") + this->Name() + " with a " + value.Name(), startPos, endPos));
	}
	InterpretedValue dividedBy(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				if (value.cNumber.type ? (double)value.cNumber.value == 0.0f : (long long)value.cNumber.value == 0ll) return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't divide by 0"), startPos, endPos));
				else return InterpretedValue(this->cNumber.over(value.cNumber), null);
			if (value.type == Value::valueType::String)
				return InterpretedValue(stringDivis(value.cString, this->cNumber), null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't divide a Number by a ") + value.Name(), startPos, endPos));
		}
		if (this->type == Value::valueType::String) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(stringDivis(this->cString, value.cNumber), null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't divide a String by a ") + value.Name(), startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't divide a ") + this->Name() + " by a " + value.Name(), startPos, endPos));
	}
	InterpretedValue raisedTo(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.powered(value.cNumber), null);
			return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't raise a Number to a "), startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't raise a ") + this->Name() + " to a " + value.Name(), startPos, endPos));
	}
	InterpretedValue factorial() {
		if (this->type == Value::valueType::Number) {
			if ( this->cNumber.type ? ( (double)this->cNumber.value < 0 || (double)this->cNumber.value != std::floor((double)this->cNumber.value)) : (long long)this->cNumber.value < 0)
				return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't factorialize a fraction or a negative"), startPos, endPos));
			if (this->cNumber.type ? ((double)this->cNumber.value > 12) : (long long)this->cNumber.value > 12)
				return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Number is too big"), startPos, endPos));
			return InterpretedValue(this->cNumber.factorial(), null);
		}
		return InterpretedValue(InterpretedValue(null), RuntimeError(std::string("Can't factorialize a ") + this->Name(), startPos, endPos));
	}
	InterpretedValue equalTo(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (this->cNumber.type && value.cNumber.type)
				return InterpretedValue(Bool((double)this->cNumber.value == (double)value.cNumber.value), null);
			if (!this->cNumber.type && !value.cNumber.type)
				return InterpretedValue(Bool((long long)this->cNumber.value == (long long)value.cNumber.value), null);
		}
	}

	nullCMP;
	InterpretedValue nullEQ;
	InterpretedValue nullCon;
};

class RuntimeResult : public Result {
public:

	InterpretedValue result;

	RuntimeResult(InterpretedValue result, Error error);
	RuntimeResult(InterpretedValue result);
	RuntimeResult() {
		this->result = null;
		this->error = null;
	}


	InterpretedValue Register(RuntimeResult result) {
		if (result.error != null)
			this->error = result.error;
		return result.result;
	}

	RuntimeResult success(InterpretedValue value);

	RuntimeResult failure(Error error) {
		this->error = error;
		return *this;
	}



	void clear();
	std::string toString();

	nullCMP;
	RuntimeResult nullEQ;
	RuntimeResult nullCon;
};

void RuntimeResult::clear() {
	result.clear();
}
RuntimeResult RuntimeResult::success(InterpretedValue value) {
	this->result.clear();
	this->result = InterpretedValue(value);
	return *this;
}
RuntimeResult::RuntimeResult(InterpretedValue result, Error error) {
	this->result = InterpretedValue(result);
	this->error = error;
}
RuntimeResult::RuntimeResult(InterpretedValue result) {
	this->result = InterpretedValue(result);
	this->error = result.error;
}
std::string RuntimeResult::toString() {
	if (this->error != null)
		return this->error.toString();
	return this->result.toString();
}

class Interpreter;

class Function : public Object {
public:
	std::string Name;
	ListNode body;
	std::vector<Argument> arguments;
	Interpreter* mInterpreter;

	RuntimeResult execute(std::vector<InterpretedValue> arguments);

	Function() {

	}

	Function(const Function& function) {
		this->Name = function.Name;
		this->body.clear();
		this->body = ListNode(function.body);
		this->arguments.clear();
		this->arguments = function.arguments;
		this->mInterpreter = function.mInterpreter;
	}

	Function(std::string Name, Node body, std::vector<Argument> arguments, Interpreter* mInterpreter) {
		this->Name = Name;
		this->body = ListNode(((ListNode*)body.nodePtr)->nodes, true);
		this->arguments = arguments;
		this->mInterpreter = mInterpreter;
	}

	std::string toString() {
		return this->Name + ":" + this->body.toString();
	}

	void clear() {
		this->arguments.clear();
	}
	
	nullCMP;
	Function nullEQ;
	Function nullCon;

};


class SymbolTable : public Object {
public:
	SymbolTable* parentTable = nullptr;
	std::map<std::string, InterpretedValue> symbols;

	SymbolTable() {

	}

	InterpretedValue get(std::string key) {
		InterpretedValue value;
		try {
			value = this->symbols.at(key);
		}
		catch (std::out_of_range) {
			if (this->parentTable != nullptr)
				return this->parentTable->get(key);
			return InterpretedValue(null);
		}
		return value;
	}

	void set(std::string name, InterpretedValue value) {
		this->symbols.insert_or_assign(name, value);
	}
	void remove(std::string name) {
		this->symbols.erase(name);
	}

	void clear() {

	}

	nullCMP;
	SymbolTable nullEQ;
	SymbolTable nullCon;
};

class FunctionTable {
public:
	std::map<std::string, Function> symbols;

	FunctionTable() {

	}

	Function get(std::string key) {
		Function value;
		try {
			value = this->symbols.at(key);
		}
		catch (std::out_of_range) {
			value = null;
		}
		return value;
	}

	void set(std::string name, Function value) {
		this->symbols.insert_or_assign(name, Function(value));
	}
	void remove(std::string name) {
		this->symbols.erase(name);
	}

	void clear() {
		for (auto& symbol : symbols)
			symbol.second.clear();
	}
};

class Context : public Object {
public:

	Context() {

	}

	SymbolTable symbolTable = SymbolTable();
	FunctionTable functionTable = FunctionTable();

	void clear() {
		this->symbolTable.clear();
		this->functionTable.clear();
	}

	nullCMP;
	Context nullEQ;
	Context nullCon;
};

class Interpreter : public Object {
public:

	static Context context;

	RuntimeResult run(Node* node) {
		return this->visit(*node, context);
	}

	RuntimeResult visit(Node node, Context& context) {
		switch (node.type)
		{
		case Class::NumberNode: return this->visitNumberNode(*(NumberNode*)node.nodePtr, context);
		case Class::BooleanNode: return this->visitBooleanNode(*(BooleanNode*)node.nodePtr, context);
		case Class::StringNode: return this->visitStringNode(*(StringNode*)node.nodePtr, context);
		case Class::ArrayNode: return this->visitArrayNode(*(ArrayNode*)node.nodePtr, context);
		case Class::ListNode: return this->visitListNode(*(ListNode*)node.nodePtr, context);
		case Class::TypeNode: return this->visitTypeNode(*(TypeNode*)node.nodePtr, context);
		case Class::UnaryNode: return this->visitUnaryNode(*(UnaryNode*)node.nodePtr, context);
		case Class::BinaryNode: return this->visitBinaryNode(*(BinaryNode*)node.nodePtr, context);
		case Class::VarAccessNode: return this->visitVarAccessNode(*(VarAccessNode*)node.nodePtr, context);
		case Class::VarIndexNode: return this->visitVarIndexAccessNode(*(VarIndexAccessNode*)node.nodePtr, context);
		case Class::VarAssignNode: return this->visitVarAssignNode(*(VarAssignNode*)node.nodePtr, context);
		case Class::VarReAssignNode: return this->visitVarReAssignNode(*(VarReAssignNode*)node.nodePtr, context);
		case Class::VarIndexReAssignNode: return this->visitVarIndexReAssignNode(*(VarIndexReAssignNode*)node.nodePtr, context);
		case Class::IfNode: return this->visitIfNode(*(IfNode*)node.nodePtr, context);
		case Class::IterationNode: return this->visitIterationNode(*(IterationNode*)node.nodePtr, context);
		case Class::TimedIterationNode: return this->visitTimedIterationNode(*(TimedIterationNode*)node.nodePtr, context);
		case Class::FunctionDefinitionNode: return this->visitFunctionDefinitionNode(*(FunctionDefinitionNode*)node.nodePtr, context);
		case Class::FunctionCallNode: return this->visitFunctionCallNode(*(FunctionCallNode*)node.nodePtr, context);
		default:
			return RuntimeResult({}, null);
		}

	}

	RuntimeResult visitListNode(ListNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::vector<Value> results;
		for (Node* element : node.nodes) {
			InterpretedValue v = InterpretedValue(result.Register(this->visit(*element, context)));
			RET_ERROR;
			results.push_back(v);
		}
		return result.success(InterpretedValue(results));
	}

	RuntimeResult visitNumberNode(NumberNode node, Context& context) {
		return RuntimeResult().success(InterpretedValue(node.token.value));
	}

	RuntimeResult visitStringNode(StringNode node, Context& context) {
		return RuntimeResult().success(InterpretedValue(node.token.value));
	}

	RuntimeResult visitBooleanNode(BooleanNode node, Context& context) {
		return RuntimeResult().success(InterpretedValue(node.token.value));
	}

	RuntimeResult visitArrayNode(ArrayNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::vector<Value> results;
		for (Node* element : node.nodes) {
			InterpretedValue v = InterpretedValue(result.Register(this->visit(*element, context)));
			RET_ERROR;
			results.push_back(v);
		}
		return result.success(InterpretedValue((results)));
	}

	RuntimeResult visitUnaryNode(UnaryNode node, Context& context) {
		RuntimeResult result = this->visit(*node.nodeOn, context);
		RET_ERROR;
		if (node.opToken.matches(TT_MINUS))
			return InterpretedValue(Number(result.result.cNumber)).multipliedWith(Value(Number(-1)));
		if (node.opToken.matches(TT_FAC))
			return InterpretedValue(Number(result.result.cNumber)).factorial();
		return result;
	}

	RuntimeResult visitBinaryNode(BinaryNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue left = result.Register(this->visit(*node.leftNode, context));
		RET_ERROR;
		InterpretedValue right = result.Register(this->visit(*node.rightNode, context));

		if (node.opToken.matches(TT_PLUS))
			return RuntimeResult(left.addedTo(right));
		if (node.opToken.matches(TT_MINUS))
			return RuntimeResult(left.subbedBy(right));
		if (node.opToken.matches(TT_MULT))
			return RuntimeResult(left.multipliedWith(right));
		if (node.opToken.matches(TT_DIV))
			return RuntimeResult(left.dividedBy(right));
		if (node.opToken.matches(TT_POW))
			return RuntimeResult(left.raisedTo(right));
		if (node.opToken.matches(TT_EQUAL))
			return RuntimeResult(left.equalTo(right));
	}

	RuntimeResult visitTypeNode(TypeNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue variable = context.symbolTable.get(variableName);
		if (variable == null)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is not defined", node.startPos, node.endPos));
		return result.success(InterpretedValue(variable.Name()));
	}

	RuntimeResult visitVarAssignNode(VarAssignNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue value = result.Register(this->visit(*node.value, context));
		RET_ERROR;
		if (context.symbolTable.get(variableName) != null)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is already defined", node.startPos, node.endPos));
		if (node.type != value.type)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is of type " + Value::Name(node.type) + " but value is of type " + value.Name(), node.startPos, node.endPos));
		context.symbolTable.set(variableName, InterpretedValue(value));
		return result.success(value);
	}

	RuntimeResult visitVarReAssignNode(VarReAssignNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue value = result.Register(this->visit(*node.value, context));
		RET_ERROR;
		if (context.symbolTable.get(variableName) == null)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is not defined", node.startPos, node.endPos));
		if (value.type != context.symbolTable.get(variableName).type)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is of type " + context.symbolTable.get(variableName).Name() + " but value is of type " + value.Name(), node.startPos, node.endPos));
		context.symbolTable.set(variableName, value);
		return result.success(value);
	}

	RuntimeResult visitVarIndexReAssignNode(VarIndexReAssignNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		if (context.symbolTable.get(variableName) == null)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is not defined", node.startPos, node.endPos));
		if (context.symbolTable.get(variableName).type != Value::valueType::Array)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is not of Array type", node.startPos, node.endPos));
		InterpretedValue index = result.Register(this->visit(*node.index, context));
		if (index.type != Value::valueType::Number)
			return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is of wrong type", node.startPos, node.endPos));
		if (index.cNumber.type)
			return result.failure(RuntimeError("Can't read index of type double: index '" + index.toString() + "' of '" + variableName + "'", node.startPos, node.endPos));
		if ((long long)index.cNumber.value < 0 || (long long)index.cNumber.value >= context.symbolTable.get(variableName).cVector.size())
			return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is out of range", node.startPos, node.endPos));
		InterpretedValue value = result.Register(this->visit(*node.value, context));
		RET_ERROR;
		context.symbolTable.symbols[variableName].cVector[(long long)index.cNumber.value] = InterpretedValue(value);
		return result.success(value);
	}

	RuntimeResult visitVarAccessNode(VarAccessNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue variable = context.symbolTable.get(variableName);
		if (variable == null)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is not defined", node.startPos, node.endPos));
		return result.success(variable);
	}

	RuntimeResult visitVarIndexAccessNode(VarIndexAccessNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue variable = context.symbolTable.get(variableName);
		if (variable == null)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is not defined", node.startPos, node.endPos));
		if (variable.type != Value::valueType::Array)
			return result.failure(RuntimeError(std::string("'") + variableName + "' is not of Array type", node.startPos, node.endPos));
		InterpretedValue index = result.Register(this->visit(*node.index, context));
		if (index.type != Value::valueType::Number)
			return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is of wrong type", node.startPos, node.endPos));
		if (index.cNumber.type)
			return result.failure(RuntimeError("Can't read index of type double: index '" + index.toString() + "' of '" + variableName + "'", node.startPos, node.endPos));
		if ((long long)index.cNumber.value < 0 || (long long)index.cNumber.value >= variable.cVector.size())
			return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is out of range", node.startPos, node.endPos));
		return result.success(InterpretedValue(variable.cVector[(long long)index.cNumber.value]));
	}

	RuntimeResult visitIfNode(IfNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue value = result.Register(this->visit(*node.condition, context));
		if (value.type == Value::valueType::Bool) {
			bool conditionTrue = value.cBool.value;
			if (!conditionTrue)
				if (node.elseStatement == nullptr)
					return result.success(InterpretedValue());
				else {
					InterpretedValue returnValue = result.Register(this->visit(*node.elseStatement, context));
					RET_ERROR;
					return result.success(returnValue);
				}
			InterpretedValue returnValue = result.Register(this->visit(*node.body, context));
			RET_ERROR;
			return result.success(returnValue);
		}
		return result.failure(RuntimeError("If-Condition is not of type Boolean", node.startPos, node.endPos));
	}

	RuntimeResult visitIterationNode(IterationNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue amount = result.Register(this->visit(*node.iterations, context));
		if (amount.type == Value::valueType::Number) {
			if (amount.cNumber.type)
				return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
			long long iterations = (long long)amount.cNumber.value;
			InterpretedValue last = InterpretedValue();
			for (int i = 0; i < iterations; i++) {
				if (i == iterations - 1) {
					last = result.Register(this->visit(*node.body, context));
					RET_ERROR;
				}
				else {
					result.Register(this->visit(*node.body, context));
					RET_ERROR;
				}
			}
			return last;

		}
		return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
	}

	RuntimeResult visitTimedIterationNode(TimedIterationNode node, Context& context) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue amount = result.Register(this->visit(*node.seconds, context));
		if (amount.type == Value::valueType::Number) {
			if (amount.cNumber.type)
				return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
			long long seconds = (long long)amount.cNumber.value;
			InterpretedValue last = InterpretedValue();
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			while (true) {
				last = result.Register(this->visit(*node.body, context));
				RET_ERROR;
				if (std::chrono::steady_clock::now() - start > std::chrono::seconds(seconds))
					break;
			}
			return last;

		}
		return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
	}

	RuntimeResult visitFunctionDefinitionNode(FunctionDefinitionNode node, Context& context) {
		RuntimeResult result = RuntimeResult();

		std::string functionName = node.varNameToken.value.cString;
		Node functionBody = Node(*node.body);
		std::vector<Argument> functionArguments = node.arguments;

		Function function = Function(functionName, functionBody, functionArguments, this);
		context.functionTable.set(functionName, function);
		return result.success(null);
	}

	RuntimeResult visitFunctionCallNode(FunctionCallNode node, Context& context) {
		RuntimeResult result = RuntimeResult();		
		std::vector<InterpretedValue> arguments;

		for (int i = 0; i < node.argumentsInOrder.size(); i++) {
			arguments.push_back(result.Register(this->visit(*node.argumentsInOrder[i], context)));
			RET_ERROR;
		}

		InterpretedValue returnedValue = result.Register(context.functionTable.get(node.varNameToken.value.cString).execute(arguments));
		RET_ERROR;
		return result.success(returnedValue);
	}

};

Context Interpreter::context = Context();

RuntimeResult Function::execute(std::vector<InterpretedValue> arguments) {
	RuntimeResult result = RuntimeResult();
	Context context = mInterpreter->context;
	Context newContext;
	newContext.symbolTable.parentTable = &context.symbolTable;
	if (arguments.size() == 0 && this->arguments.size() != 0)
		return result.failure(RuntimeError("Too few arguments for function call", Position(), Position()));

	std::cout << arguments.size() << " : " << this->arguments.size() << "\n";

	if (arguments.size() < this->arguments.size())
		return result.failure(RuntimeError("Too few arguments for function call", arguments[arguments.size() - 1].startPos, Position(arguments[arguments.size() - 1].endPos).advance(0)));
	if (arguments.size() > this->arguments.size())
		return result.failure(RuntimeError("Too many arguments for function call", arguments[arguments.size() - 1].startPos, Position(arguments[arguments.size() - 1].endPos).advance(0)));
	for (int i = 0; i < arguments.size(); i++) {
		newContext.symbolTable.set(this->arguments[i].Name, arguments[i]);
	}

	std::cout << this->body.toString() << "<<\n";
	InterpretedValue returnedResult = result.Register(mInterpreter->visit(this->body, newContext));
	RET_ERROR;

	return result.success(returnedResult);
}