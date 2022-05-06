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

	InterpretedValue(Value value, Position startPos, Position endPos, Error error)
		:Value(value) {
		this->error = error;
	}
	InterpretedValue(Value value, Position startPos, Position endPos)
		:Value(value) {
		this->error = null;
	}

	std::string toString() {
		return Value::toString();
	}
	std::string toString(bool) {
		return Value::toString(true);
	}

	InterpretedValue addedTo(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.plus(value.cNumber), this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't add a ") + value.Name() + " to a Number", startPos, endPos));
		}
		if (this->type == Value::valueType::String) {
			if (value.type == Value::valueType::String)
				return InterpretedValue(this->cString + value.cString, this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't add a ") + value.Name() + " to a String", startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't add a ") + value.Name() + " to a " + this->Name(), startPos, endPos));
	}
	InterpretedValue subbedBy(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.minus(value.cNumber), this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't subtract a ") + value.Name() + " from a Number", startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't subtract a ") + value.Name() + " from a " + this->Name(), startPos, endPos));
	}
	InterpretedValue multipliedWith(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.times(value.cNumber), this->startPos, value.endPos, null);
			if (value.type == Value::valueType::String)
				if (this->cNumber.type ? (double)this->cNumber.value < 0.0f : (long long)this->cNumber.value < 0ll) return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't mutiple a String with a negative Number"), startPos, endPos));
				else return InterpretedValue(stringTimes(value.cString, this->cNumber), this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't multiply a Number with a ") + value.Name(), startPos, endPos));
		}
		if (this->type == Value::valueType::String) {
			if (value.type == Value::valueType::Number)
				if (value.cNumber.type ? (double)value.cNumber.value < 0.0f : (long long)value.cNumber.value < 0ll) return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't mutiple a String with a negative Number"), startPos, endPos));
				else return InterpretedValue(stringTimes(this->cString, value.cNumber), this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't multiply a String with a ") + value.Name(), startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't multiply a ") + this->Name() + " with a " + value.Name(), startPos, endPos));
	}
	InterpretedValue dividedBy(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				if (value.cNumber.type ? (double)value.cNumber.value == 0.0f : (long long)value.cNumber.value == 0ll) return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't divide by 0"), startPos, endPos));
				else return InterpretedValue(this->cNumber.over(value.cNumber), this->startPos, value.endPos, null);
			if (value.type == Value::valueType::String)
				return InterpretedValue(stringDivis(value.cString, this->cNumber), this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't divide a Number by a ") + value.Name(), startPos, endPos));
		}
		if (this->type == Value::valueType::String) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(stringDivis(this->cString, value.cNumber), this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't divide a String by a ") + value.Name(), startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't divide a ") + this->Name() + " by a " + value.Name(), startPos, endPos));
	}
	InterpretedValue raisedTo(InterpretedValue value) {
		if (this->type == Value::valueType::Number) {
			if (value.type == Value::valueType::Number)
				return InterpretedValue(this->cNumber.powered(value.cNumber), this->startPos, value.endPos, null);
			return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't raise a Number to a "), startPos, endPos));
		}
		return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't raise a ") + this->Name() + " to a " + value.Name(), startPos, endPos));
	}
	InterpretedValue factorial() {
		if (this->type == Value::valueType::Number) {
			if (this->cNumber.type ? ((double)this->cNumber.value < 0 || (double)this->cNumber.value != std::floor((double)this->cNumber.value)) : (long long)this->cNumber.value < 0)
				return InterpretedValue(InterpretedValue(null), this->startPos, this->endPos, RuntimeError(std::string("Can't factorialize a fraction or a negative"), startPos, endPos));
			if (this->cNumber.type ? ((double)this->cNumber.value > 12) : (long long)this->cNumber.value > 12)
				return InterpretedValue(InterpretedValue(null), this->startPos, this->endPos, RuntimeError(std::string("Number is too big"), startPos, endPos));
			return InterpretedValue(this->cNumber.factorial(), this->startPos, this->endPos, null);
		}
		return InterpretedValue(InterpretedValue(null), this->startPos, this->endPos, RuntimeError(std::string("Can't factorialize a ") + this->Name(), startPos, endPos));
	}
	InterpretedValue equalTo(InterpretedValue value, bool invert) {
		if (this->type == Value::valueType::Number && value.type == Value::valueType::Number)
			return InterpretedValue(Bool(this->cNumber.equals(value.cNumber) ^ invert), this->startPos, value.endPos, Error(null));
		else if (this->type == Value::valueType::String && value.type == Value::valueType::String)
			return InterpretedValue(Bool((this->cString == value.cString) ^ invert), this->startPos, value.endPos, Error(null));
		else if (this->type == Value::valueType::Bool && value.type == Value::valueType::Bool)
			return InterpretedValue(Bool((this->cBool.value == value.cBool.value) ^ invert), this->startPos, value.endPos, Error(null));
		else return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't compare a ") + this->Name() + " to a " + value.Name(), startPos, endPos));
		return InterpretedValue(Bool(false), this->startPos, value.endPos, Error(null));
	}
	InterpretedValue greaterThan(InterpretedValue value, bool orEqual, bool invert) {
		if (!orEqual)
			if (this->type == Value::valueType::Number && value.type == Value::valueType::Number)
				return InterpretedValue(Bool(this->cNumber.greaterThan(value.cNumber) ^ invert), this->startPos, value.endPos, Error(null));
			else return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't compare a ") + this->Name() + " to a " + value.Name(), startPos, endPos));
		if (this->type == Value::valueType::Number && value.type == Value::valueType::Number)
			return InterpretedValue(Bool(( this->cNumber.greaterThan(value.cNumber) || this->cNumber.equals(value.cNumber)) ^ invert), this->startPos, value.endPos, Error(null));
		else return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't compare a ") + this->Name() + " to a " + value.Name(), startPos, endPos));
	}
	InterpretedValue smallerThan(InterpretedValue value, bool orEqual, bool invert) {
		if (!orEqual)
			if (this->type == Value::valueType::Number && value.type == Value::valueType::Number)
				return InterpretedValue(Bool(this->cNumber.lessThan(value.cNumber) ^ invert), this->startPos, value.endPos, Error(null));
			else return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't compare a ") + this->Name() + " to a " + value.Name(), startPos, endPos));
		if (this->type == Value::valueType::Number && value.type == Value::valueType::Number)
			return InterpretedValue(Bool((this->cNumber.lessThan(value.cNumber) || this->cNumber.equals(value.cNumber)) ^ invert), this->startPos, value.endPos, Error(null));
		else return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Can't compare a ") + this->Name() + " to a " + value.Name(), startPos, endPos));
	}
	InterpretedValue ored(InterpretedValue value) {
		if (this->type == Value::valueType::Bool && value.type == Value::valueType::Bool)
			return InterpretedValue(Bool(value.cBool.value || this->cBool.value), this->startPos, value.endPos, Error(null));
		else return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Operants must be of type Boolean, but are of type ") + this->Name() + " and " + value.Name(), startPos, endPos));
	}
	InterpretedValue anded(InterpretedValue value) {
		if (this->type == Value::valueType::Bool && value.type == Value::valueType::Bool)
			return InterpretedValue(Bool(value.cBool.value && this->cBool.value), this->startPos, value.endPos, Error(null));
		else return InterpretedValue(InterpretedValue(null), this->startPos, value.endPos, RuntimeError(std::string("Operants must be of type Boolean, but are of type ") + this->Name() + " and " + value.Name(), startPos, endPos));
	}


	nullCMP;
	InterpretedValue nullEQ;
	InterpretedValue nullCon;
};

#define RET_RETA(value)  if (result.m_shouldReturn) return result.success(value);
#define RET_RET  if (result.m_shouldReturn) return result;

class RuntimeResult : public Result {
public:

	InterpretedValue result;
	bool m_shouldReturn = false;
	bool m_break = false;
	bool m_continue = false;

	RuntimeResult(InterpretedValue result, Error error);
	RuntimeResult(InterpretedValue result);
	RuntimeResult() {
		this->result = null;
		this->error = null;
	}


	InterpretedValue Register(RuntimeResult result) {
		if (result.error != null)
			this->error = result.error;
		this->m_shouldReturn = result.m_shouldReturn;
		return result.result;
	}

	RuntimeResult success(InterpretedValue value);

	RuntimeResult shouldReturn() {
		this->m_shouldReturn = true;
		return *this;
	}

	RuntimeResult failure(Error error) {
		this->error = error;
		return *this;
	}


	std::string toString(bool) {
		if (this->error != null)
			return this->error.toString();
		if (this->result.cVector.size() > 0)
			return this->result.cVector[this->result.cVector.size() - 1].toString();
		return "";
	}
	std::string toString(bool, bool) {
		if (this->error != null)
			return this->error.toString();
		if (this->result.cVector.size() > 0)
			return this->result.cVector[this->result.cVector.size() - 1].toString(true);
		return "";
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
class Context;

class Function : public Object {
public:
	std::string Name;
	Node body;
	Value::valueType returnType;
	std::vector<Argument> arguments;
	Interpreter* mInterpreter;
	Allocator mAllocations;

	RuntimeResult execute(std::vector<InterpretedValue> arguments, Context& context);

	Function() {

	}

	Function(const Function& function) 
		:Function(function.Name, function.body, function.arguments, function.returnType, function.mInterpreter) {
	}

	Function(std::string Name, Node body, std::vector<Argument> arguments, Value::valueType returnType, Interpreter* mInterpreter) {
		this->Name = Name;
		this->body = Node(body, mAllocations, true);
		this->arguments = arguments;
		this->mInterpreter = mInterpreter;
		this->returnType = returnType;
	}

	std::string toString() {
		return this->Name + ":" + this->body.toString();
	}

	std::string argumentString() {
		if (this->arguments.size() == 0)
			return "";
		std::string out;
		for (int i = 0; i < this->arguments.size(); i++) {
			out += Value::Name(this->arguments[i].type);
			out += " ";
			out += this->arguments[i].Name;
			if (i != this->arguments.size() - 1)
				out += ", ";
		}
		return out;
	}

	void clear() {
		this->arguments.clear();
		this->mAllocations.clearAllAllocations();
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
		for (auto& symbol : symbols)
			symbol.second.clear();
		this->symbols.clear();
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

	Function* get(std::string key) {
		try {
			return &this->symbols.at(key);
		}
		catch (std::out_of_range) {
			return nullptr;
		}
	}

	void set(std::string name, Function& value) {
		this->symbols.insert_or_assign(name, Function(value));
	}
	void remove(std::string name) {
		this->symbols[name].clear();
		this->symbols.erase(name);
	}

	void clear() {
		for (auto& symbol : symbols)
			symbol.second.clear();
		this->symbols.clear();
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

	RuntimeResult run(Node* node, Context& context) {
		return this->visit(*node, context, false);
	}

	RuntimeResult visit(Node node, Context& context, bool inFunction) {
		switch (node.type)
		{
		case Class::NumberNode: return this->visitNumberNode(*(NumberNode*)node.nodePtr, context, inFunction);
		case Class::BooleanNode: return this->visitBooleanNode(*(BooleanNode*)node.nodePtr, context, inFunction);
		case Class::StringNode: return this->visitStringNode(*(StringNode*)node.nodePtr, context, inFunction);
		case Class::ArrayNode: return this->visitArrayNode(*(ArrayNode*)node.nodePtr, context, inFunction);
		case Class::ListNode: return this->visitListNode(*(ListNode*)node.nodePtr, context, inFunction);
		case Class::PropertyNode: return this->visitPropertyNode(*(PropertyNode*)node.nodePtr, context, inFunction);
		case Class::UnaryNode: return this->visitUnaryNode(*(UnaryNode*)node.nodePtr, context, inFunction);
		case Class::BinaryNode: return this->visitBinaryNode(*(BinaryNode*)node.nodePtr, context, inFunction);
		case Class::VarAccessNode: return this->visitVarAccessNode(*(VarAccessNode*)node.nodePtr, context, inFunction);
		case Class::VarIndexNode: return this->visitVarIndexAccessNode(*(VarIndexAccessNode*)node.nodePtr, context, inFunction);
		case Class::VarAssignNode: return this->visitVarAssignNode(*(VarAssignNode*)node.nodePtr, context, inFunction);
		case Class::VarReAssignNode: return this->visitVarReAssignNode(*(VarReAssignNode*)node.nodePtr, context, inFunction);
		case Class::VarIndexReAssignNode: return this->visitVarIndexReAssignNode(*(VarIndexReAssignNode*)node.nodePtr, context, inFunction);
		case Class::IfNode: return this->visitIfNode(*(IfNode*)node.nodePtr, context, inFunction);
		case Class::IterationNode: return this->visitIterationNode(*(IterationNode*)node.nodePtr, context, inFunction);
		case Class::TimedIterationNode: return this->visitTimedIterationNode(*(TimedIterationNode*)node.nodePtr, context, inFunction);
		case Class::FunctionDefinitionNode: return this->visitFunctionDefinitionNode(*(FunctionDefinitionNode*)node.nodePtr, context, inFunction);
		case Class::FunctionCallNode: return this->visitFunctionCallNode(*(FunctionCallNode*)node.nodePtr, context, inFunction);
		case Class::ReturnNode: return this->visitReturnNode(*(ReturnNode*)node.nodePtr, context, inFunction);
		case Class::UndefineNode: return this->visitUndefineNode(*(UndefineNode*)node.nodePtr, context, inFunction);
		case Class::OutputNode: return this->visitOutputNode(*(OutputNode*)node.nodePtr, context, inFunction);
		case Class::InputNode: return this->visitInputNode(*(InputNode*)node.nodePtr, context, inFunction);
		case Class::InterruptionNode: return this->visitInterruptionNode(*(InterruptionNode*)node.nodePtr, context, inFunction);
		default:
			return RuntimeResult({}, null);
		}

	}

	RuntimeResult visitListNode(ListNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::vector<Value> results;
		for (Node* element : node.nodes) {
			RuntimeResult r = this->visit(*element, context, inFunction);
			InterpretedValue v = InterpretedValue(result.Register(r));
			RET_ERROR;
			RET_RETA(v);
			results.push_back(v);
		}
		InterpretedValue data = InterpretedValue(results, node.startPos, node.endPos);
		return result.success(data);
	}

	RuntimeResult visitNumberNode(NumberNode node, Context& context, bool inFunction) {
		return RuntimeResult().success(InterpretedValue(node.token.value, node.startPos, node.endPos));
	} 

	RuntimeResult visitStringNode(StringNode node, Context& context, bool inFunction) {
		return RuntimeResult().success(InterpretedValue(node.token.value, node.startPos, node.endPos));
	}

	RuntimeResult visitBooleanNode(BooleanNode node, Context& context, bool inFunction) {
		return RuntimeResult().success(InterpretedValue(node.token.value, node.startPos, node.endPos));
	}

	RuntimeResult visitArrayNode(ArrayNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::vector<Value> results;
		for (Node* element : node.nodes) {
			InterpretedValue v = InterpretedValue(result.Register(this->visit(*element, context, inFunction)));
			RET_ERROR;
			RET_RETA(v);
			results.push_back(v);
		}
		return result.success(InterpretedValue(results, node.startPos, node.endPos));
	}

	RuntimeResult visitUnaryNode(UnaryNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue value = result.Register(this->visit(*node.nodeOn, context, inFunction));
		RET_ERROR;
		RET_RET;
		if (node.opToken.matches(TT_MINUS))
			return InterpretedValue(value, node.startPos, node.endPos).multipliedWith(InterpretedValue(Number(-1), node.startPos, node.endPos));
		if (node.opToken.matches(TT_FAC))
			return InterpretedValue(value, node.startPos, node.endPos).factorial();
		return result;
	}

	RuntimeResult visitBinaryNode(BinaryNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue left = result.Register(this->visit(*node.leftNode, context, inFunction));
		RET_ERROR;
		RET_RETA(left);
		InterpretedValue right = result.Register(this->visit(*node.rightNode, context, inFunction));
		RET_ERROR;
		RET_RETA(right);

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
			return RuntimeResult(left.equalTo(right, node.opToken.value.cBool.value));
		if (node.opToken.matches(TT_SMALL))
			return RuntimeResult(left.smallerThan(right, false, node.opToken.value.cBool.value));
		if (node.opToken.matches(TT_GREAT))
			return RuntimeResult(left.greaterThan(right, false, node.opToken.value.cBool.value));
		if (node.opToken.matches(TT_SMALL_EQ))
			return RuntimeResult(left.smallerThan(right, true, node.opToken.value.cBool.value));
		if (node.opToken.matches(TT_GREAT_EQ))
			return RuntimeResult(left.greaterThan(right, true, node.opToken.value.cBool.value));
		if (node.opToken.matches(TT_OR))
			return RuntimeResult(left.ored(right));
		if (node.opToken.matches(TT_AND))
			return RuntimeResult(left.anded(right));
	}

	RuntimeResult visitPropertyNode(PropertyNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue variable = context.symbolTable.get(variableName);
		if (variable == null)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not defined", node.startPos, node.endPos));
		if (!node.type)
			return result.success(InterpretedValue(variable.Name(), node.startPos, node.endPos));
		if (variable.type != Value::valueType::Array && variable.type != Value::valueType::String)
			return result.failure(RuntimeError(std::string("Can't get the size of a '") + variableName + "'", node.startPos, node.endPos));
		return variable.type == Value::valueType::Array ? result.success(InterpretedValue(Number((long long)variable.cVector.size()), node.startPos, node.endPos)) : result.success(InterpretedValue(Number((long long)variable.cString.size()), node.startPos, node.endPos));
	}

	RuntimeResult visitVarAssignNode(VarAssignNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue value = result.Register(this->visit(*node.value, context, inFunction));
		RET_ERROR;
		RET_RETA(value);
		if (context.symbolTable.get(variableName) != null)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is already defined", node.startPos, node.endPos));
		if (node.type != value.type)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is of type " + Value::Name(node.type) + " but value is of type " + value.Name(), node.startPos, node.endPos));
		context.symbolTable.set(variableName, InterpretedValue(value));
		return result.success(value);
	}

	RuntimeResult visitVarReAssignNode(VarReAssignNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue value = result.Register(this->visit(*node.value, context, inFunction));
		RET_ERROR;
		RET_RETA(value);
		if (context.symbolTable.get(variableName) == null)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not defined", node.startPos, node.endPos));
		if (value.type != context.symbolTable.get(variableName).type)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is of type " + context.symbolTable.get(variableName).Name() + " but value is of type " + value.Name(), node.startPos, node.endPos));
		context.symbolTable.set(variableName, value);
		return result.success(value);
	}

	RuntimeResult visitVarIndexReAssignNode(VarIndexReAssignNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		if (context.symbolTable.get(variableName) == null)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not defined", node.startPos, node.endPos));
		if (context.symbolTable.get(variableName).type != Value::valueType::Array)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not of Array type", node.startPos, node.endPos));
		InterpretedValue index = result.Register(this->visit(*node.index, context, inFunction));
		if (index.type != Value::valueType::Number)
			return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is of wrong type", node.startPos, node.endPos));
		if (index.cNumber.type)
			return result.failure(RuntimeError("Can't read index of type double: index '" + index.toString() + "' of '" + variableName + "'", node.startPos, node.endPos));
		if ((long long)index.cNumber.value < 0 || unsigned long long((long long)index.cNumber.value) >= context.symbolTable.get(variableName).cVector.size())
			return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is out of range", node.startPos, node.endPos));
		InterpretedValue value = result.Register(this->visit(*node.value, context, inFunction));
		RET_ERROR;
		RET_RETA(value);
		context.symbolTable.symbols[variableName].cVector[(long long)index.cNumber.value] = InterpretedValue(value);
		return result.success(value);
	}

	RuntimeResult visitVarAccessNode(VarAccessNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue variable = context.symbolTable.get(variableName);
		if (variable == null)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not defined", node.startPos, node.endPos));
		return result.success(variable);
	}

	RuntimeResult visitVarIndexAccessNode(VarIndexAccessNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::string variableName = node.varNameToken.value.cString;
		InterpretedValue variable = context.symbolTable.get(variableName);
		if (variable == null)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not defined", node.startPos, node.endPos));
		if (variable.type != Value::valueType::Array && node.type == 0)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not of Array type", node.startPos, node.endPos));
		if (variable.type != Value::valueType::String && node.type == 1)
			return result.failure(RuntimeError(std::string("Variable '") + variableName + "' is not of String type", node.startPos, node.endPos));
		InterpretedValue index = result.Register(this->visit(*node.index, context, inFunction));
		if (index.type != Value::valueType::Number)
			return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is of wrong type", node.startPos, node.endPos));
		if (index.cNumber.type)
			return result.failure(RuntimeError("Can't read index of type double: index '" + index.toString() + "' of '" + variableName + "'", node.startPos, node.endPos));

		if (node.type == 0) {
			if ((long long)index.cNumber.value < 0 || unsigned long long((long long)index.cNumber.value) >= variable.cVector.size())
				return result.failure(RuntimeError("Can't read index '" + index.toString() + "' of '" + variableName + "' : index is out of range", node.startPos, node.endPos));
			return result.success(InterpretedValue(variable.cVector[(long long)index.cNumber.value], node.startPos, node.endPos));
		}
		if ((long long)index.cNumber.value < 0 || unsigned long long((long long)index.cNumber.value) >= variable.cString.size())
			return result.failure(RuntimeError("Can't read character '" + index.toString() + "' of '" + variableName + "' : index is out of range", node.startPos, node.endPos));
		return result.success(InterpretedValue(std::string("") + variable.cString[(long long)index.cNumber.value], node.startPos, node.endPos));
	}

	RuntimeResult visitIfNode(IfNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue value = result.Register(this->visit(*node.condition, context, inFunction));
		if (value.type == Value::valueType::Bool) {
			bool conditionTrue = value.cBool.value;
			if (!conditionTrue)
				if (node.elseStatement == nullptr)
					return result.success(InterpretedValue());
				else {
					InterpretedValue returnValue = result.Register(this->visit(*node.elseStatement, context, inFunction));
					RET_ERROR;
					RET_RETA(returnValue);
					return result.success(returnValue);
				}
			InterpretedValue returnValue = result.Register(this->visit(*node.body, context, inFunction));
			RET_ERROR;
			RET_RETA(returnValue);
			return result.success(returnValue);
		}
		return result.failure(RuntimeError("If-Condition is not of type Boolean", node.startPos, node.endPos));
	}

	RuntimeResult visitIterationNode(IterationNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue amount = result.Register(this->visit(*node.iterations, context, inFunction));
		if (amount.type == Value::valueType::Number) {
			if (amount.cNumber.type)
				return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
			long long iterations = (long long)amount.cNumber.value;
			InterpretedValue last = InterpretedValue();
			for (int i = 0; i < iterations; i++) {
				if (i == iterations - 1) {
					last = result.Register(this->visit(*node.body, context, inFunction));
					RET_ERROR;
					RET_RETA(last);
				}
				else {
					result.Register(this->visit(*node.body, context, inFunction));
					RET_ERROR;
					RET_RETA(last);
				}
			}
			return last;

		}
		return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
	}

	RuntimeResult visitTimedIterationNode(TimedIterationNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		InterpretedValue amount = result.Register(this->visit(*node.seconds, context, inFunction));
		if (amount.type == Value::valueType::Number) {
			if (amount.cNumber.type)
				return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
			long long seconds = (long long)amount.cNumber.value;
			InterpretedValue last = InterpretedValue();
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			while (true) {
				last = result.Register(this->visit(*node.body, context, inFunction));
				RET_ERROR;
				RET_RETA(last);
				if (std::chrono::steady_clock::now() - start > std::chrono::seconds(seconds))
					break;
			}
			return last;

		}
		return result.failure(RuntimeError("For-Condition is not of type int-Number", node.startPos, node.endPos));
	}

	RuntimeResult visitFunctionDefinitionNode(FunctionDefinitionNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();

		std::string functionName = node.varNameToken.value.cString;

		if (context.functionTable.get(functionName) != nullptr)
			return result.failure(RuntimeError(std::string("Function '") + functionName + "' is already defined", node.startPos, node.endPos));

		std::vector<Argument> functionArguments = node.arguments;

		Function function = Function(functionName, *node.body, functionArguments, node.returnType, this);
		context.functionTable.set(functionName, function);
		return result.success(null);
	}

	RuntimeResult visitFunctionCallNode(FunctionCallNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		std::vector<InterpretedValue> arguments;

		/*printf("  -Functions:\n");
		auto it2 = context.functionTable.symbols.begin();
		for (size_t i = 0; i < context.functionTable.symbols.size(); i++) {
			printf("   %s %s: %s", Value::Name(it2->second.returnType).c_str(), it2->first.c_str(), it2->second.argumentString().c_str());
			std::advance(it2, 1);
		}
		printf("\n");

		std::cout <<( context.functionTable.get(node.varNameToken.value.cString) == nullptr )<< "???\n";*/

		if (context.functionTable.get(node.varNameToken.value.cString) == nullptr)
			return result.failure(RuntimeError(std::string("Function '") + node.varNameToken.value.cString + "' is not defined!", node.startPos, node.endPos));

		for (int i = 0; i < node.argumentsInOrder.size(); i++) {
			arguments.push_back(result.Register(this->visit(*node.argumentsInOrder[i], context, inFunction)));
			RET_ERROR;
			RET_RET;
		}

		RuntimeResult execution = context.functionTable.get(node.varNameToken.value.cString)->execute(arguments, context);
		InterpretedValue returnedValue = result.Register(execution);
		RET_ERROR;
		RET_RETA(returnedValue);
		return result.success(returnedValue);
		//return result.success(InterpretedValue(Number(0), null, null));
	}

	RuntimeResult visitReturnNode(ReturnNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();
		if (!inFunction)
			return result.failure(RuntimeError("Can't return a value outside of a function", node.startPos, node.endPos));
		InterpretedValue returnValue = result.Register(this->visit(*node.value, context, inFunction));
		RET_RETA(returnValue);
		return result.success(returnValue).shouldReturn();
	}
	
	RuntimeResult visitUndefineNode(UndefineNode node, Context& context, bool inFunction) {
		RuntimeResult result = RuntimeResult();

		if (!node.type) {
			if (context.symbolTable.get(node.varNameToken.value.cString) == null)
				return result.failure(RuntimeError(std::string("'") + node.varNameToken.value.cString + "' is not defined", node.startPos, node.endPos));
			context.symbolTable.remove(node.varNameToken.value.cString);
		}
		else {
			if (context.functionTable.get(node.varNameToken.value.cString) == nullptr)
				return result.failure(RuntimeError(std::string("'") + node.varNameToken.value.cString + "' is not defined", node.startPos, node.endPos));
			context.functionTable.remove(node.varNameToken.value.cString);
		}
		return result.success(null);
	}

	RuntimeResult visitOutputNode(OutputNode node, Context& context, bool inFuntion) {
		RuntimeResult result = RuntimeResult();

		if (node.type == 0) {
			if (node.object == nullptr) {
				std::cout << "\n";
				return result.success(InterpretedValue(std::string("newline"), node.startPos, node.endPos));
			}

			InterpretedValue valueToPrint = result.Register(this->visit(*node.object, context, inFuntion));
			RET_ERROR;
			RET_RETA(valueToPrint);

			if (node.printLine)
				std::cout << valueToPrint.toString(true) << "\n";
			else std::cout << valueToPrint.toString(true);
			return result.success(valueToPrint);
		}
		else {

			InterpretedValue commandToExecute = result.Register(this->visit(*node.object, context, inFuntion));

			system(commandToExecute.toString(true).c_str());
			return result.success(commandToExecute);
		}
	}
	
	RuntimeResult visitInputNode(InputNode node, Context& context, bool inFuntion) {
		RuntimeResult result = RuntimeResult();

		std::string string = stdcin();

		return result.success(InterpretedValue(string, node.startPos, node.endPos));
	}
	
	RuntimeResult visitInterruptionNode(InterruptionNode node, Context& context, bool inFuntion) {
		return RuntimeResult().failure(RuntimeError("Can't '" + node.toString() + "' here", node.startPos, node.endPos));
	}

};

RuntimeResult Function::execute(std::vector<InterpretedValue> arguments, Context& context) {
	RuntimeResult result = RuntimeResult();
	Context newContext;
	newContext.symbolTable.parentTable = &context.symbolTable;

	if (arguments.size() == 0 && this->arguments.size() != 0)
		return result.failure(RuntimeError("Too few arguments for function call", Position(), Position()));
	if (arguments.size() < this->arguments.size())
		return result.failure(RuntimeError("Too few arguments for function call", arguments[arguments.size() - 1].startPos, Position(arguments[arguments.size() - 1].endPos).advance(0)));
	if (arguments.size() > this->arguments.size())
		return result.failure(RuntimeError("Too many arguments for function call", arguments[arguments.size() - 1].startPos, Position(arguments[arguments.size() - 1].endPos).advance(0)));
	for (int i = 0; i < arguments.size(); i++) {
		newContext.symbolTable.set(this->arguments[i].Name, arguments[i]);
	}

	InterpretedValue returnedResult = result.Register(mInterpreter->visit(this->body, newContext, true));

	RET_ERROR;
	if (!result.m_shouldReturn && this->returnType != Value::valueType::Void)
		return result.failure(RuntimeError("'" + this->Name + "' must return a value", returnedResult.startPos, returnedResult.endPos));
	RET_ERROR;

	result.m_shouldReturn = false;
	return result.success(returnedResult);
}