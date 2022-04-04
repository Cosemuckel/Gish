#pragma once
#ifndef _VALUE_TYPES_H_
#define _VALUE_TYPES_H_

class __COSEMUCKEL__GISH__NULL {};
#define null __COSEMUCKEL__GISH__NULL()
#define nullarg __COSEMUCKEL__GISH__NULL __COSEMUCKEL__GISH__NULLARGUMENT
#define nullCMP	bool operator==(nullarg) const { return this->mClass == Class::Null; } bool operator!=(nullarg) const { return this->mClass != Class::Null; } 
#define nullEQ operator=(nullarg) { this->mClass = Class::Null; return *this; }
#define nullCon (nullarg) { this->mClass = Class::Null; }

class Object;
class Value;
class Number;
class Bool;

class Position;
class Result;
class Error;
class IllegalCharError;
class InvalidSyntaxError;

class TokenType;
class Token;
class LexingResult;
class LexerResult;
class Lexer;

class Node;
class NumberNode;
class StringNode;
class BooleanNode;
class ArrayNode;
class ListNode;
class UnaryNode;
class BinaryNode;
class VarAssignNode;
class VarReAssignNode;
class VarAccessNode;
class PropertyNode;
class ParserResult;
class Parser;

enum class Class;

enum class Class {
	Null,
	Object,
	Value,
	Number,
	Bool,
	
	Position,
	Result,
	Error,
	InvalidSyntaxError,
	IllegalCharError,

	TokenType,
	Token,
	LexingResult,
	LexerResult,
	Lexer,
	
	Node,
	NumberNode,
	StringNode,
	BooleanNode,
	ArrayNode,
	ListNode,
	UnaryNode,
	BinaryNode,
	VarAssignNode,
	VarAccessNode,
	VarReAssignNode,
	PropertyNode,
	IfNode,
	IterationNode,
	TimedIterationNode,
	VarIndexNode,
	VarIndexReAssignNode,
	FunctionDefinitionNode,
	FunctionCallNode,
	ReturnNode,
	UndefineNode,
	OutputNode,
	InputNode,
	InterruptionNode,
	ParserResult,
	Parser
};

class Object {
public:
	Class mClass = Class::Object;
	virtual std::string toString() { return ""; }
	Object() {

	}
	bool equals(Object o) {
		return o.mClass == this->mClass;
	}
	bool instanceOf(Class c) {
		return this->mClass == c;
	}
	bool instanceOf(Object o) {
		return this->mClass == o.mClass;
	}
	nullCMP;
	Object nullEQ;
	Object nullCon;
};

class Bool : public Object {
public:
	bool value = false;

	Bool() {
		this->mClass = Class::Bool;
	}
	Bool(bool b) : Bool() {
		this->value = b;
	}

	std::string toString() {
		return value ? "true" : "false";
	}
	nullCMP;
	Bool nullEQ;
	Bool nullCon;
};


class Number : public Object {
public:

	int64_t value = 0;
	bool type = false; //False = long, True = bool

	Number(const Number& number) {
		this->type = number.type;
		for (int i = 0; i < 8; i++)
			memcpy(&this->value, &number.value, 8);
	}
	Number() {
		this->mClass = Class::Number;
	}
	Number(long long l) : Number() {
		this->value = long long(l);
		this->type = false;
	}
	Number(int l) : Number() {
		this->value = long long(l);
		this->type = false;
	}
	Number(double d) : Number() {
		this->value = (int64_t) double(d);
		this->type = true;
	}

	Number plus(Number number) {
		if (!this->type)
			if (!number.type)
				return Number((long long)this->value + (long long)number.value);
			else
				return Number((long long)this->value + (double)number.value);
		else
			if (!number.type)
				return Number((double)this->value + (long long)number.value);
			else
				return Number((double)this->value + (double)number.value);
	}
	Number minus(Number number) {
		if (!this->type)
			if (!number.type)
				return Number((long long)this->value - (long long)number.value);
			else
				return Number((long long)this->value - (double)number.value);
		else
			if (!number.type)
				return Number((double)this->value - (long long)number.value);
			else
				return Number((double)this->value - (double)number.value);
	}
	Number times(Number number) {
		if (!this->type)
			if (!number.type)
				return Number((long long)this->value * (long long)number.value);
			else
				return Number((long long)this->value * (double)number.value);
		else
			if (!number.type)
				return Number((double)this->value * (long long)number.value);
			else
				return Number((double)this->value * (double)number.value);
	}
	Number over(Number number) {
		if (!this->type)
			if (!number.type)
				return Number((long long)this->value / (long long)number.value);
			else
				return Number((long long)this->value / (double)number.value);
		else
			if (!number.type)
				return Number((double)this->value / (long long)number.value);
			else
				return Number((double)this->value / (double)number.value);
	}
	Number powered(Number number) {
		if (!this->type)
			if (!number.type)
				return Number(static_cast<long long>(std::pow((long long)this->value, (long long)number.value)));
			else
				return Number(std::pow((long long)this->value, (double)number.value));
		else
			if (!number.type)
				return Number(std::pow((double)this->value, (long long)number.value));
			else
				return Number(std::pow((double)this->value, (double)number.value));
	}
	Number factorial() {
		if (!this->type) {
			int r = 1;
			int f = (long long)this->value;
			for (int i = 1; i <= f; i++)
				r *= i;
			return Number(r);
		}
		return Number(1);
	}

	bool equals(Number number) {
		return this->type ? number.type ? (double)this->value == (double)number.value : (double)this->value == (long long)number.value : number.type ? (long long)this->value == (double)number.value : (long long)this->value == (long long)number.value;
	}
	bool greaterThan(Number number) {
		return this->type ? number.type ? (double)this->value > (double)number.value : (double)this->value > (long long)number.value : number.type ? (long long)this->value > (double)number.value : (long long)this->value > (long long)number.value;
	}
	bool lessThan(Number number) {
		return this->type ? number.type ? (double)this->value < (double)number.value : (double)this->value < (long long)number.value : number.type ? (long long)this->value < (double)number.value : (long long)this->value < (long long)number.value;
	}


	std::string toString() {
		if (*this == null)
			return "Null";
		if (this->type)
			return std::to_string((double)this->value);
		return std::to_string((long long)this->value);
	}

	void clear() {
		
	}
	nullCMP;
	Number nullEQ;
	Number nullCon;
};

class Value : public Object {
public:

	enum class valueType {
		Number,
		String,
		Bool,
		Array,
		Null,
		Void
	};

	valueType type = valueType::Null;
	Bool cBool;
	Number cNumber;
	std::vector<Value> cVector;
	std::string cString = "";

	static std::string Name(Value::valueType type) {
		switch (type)
		{
		case valueType::Number: return "Number";
		case valueType::String: return "String";
		case valueType::Bool: return "Bool";
		case valueType::Array: return "Array";
		case valueType::Null: return "Null";
		default:
			return "Null";
		}
	}

	std::string Name() {
		switch (this->type)
		{
		case valueType::Number: return "Number";
		case valueType::String: return "String";
		case valueType::Bool: return "Bool";
		case valueType::Array: return "Array";
		case valueType::Null: return "Null";
		default:
			return "Null";
		}
	}

	Value(const Value& value) {
		this->type = value.type;
		switch (this->type)
		{
		case valueType::Bool: this->cBool = value.cBool; break;
		case valueType::Number: this->cNumber = value.cNumber; break;
		case valueType::String: this->cString = value.cString; break;
		case valueType::Array: { 
			this->cVector.clear();
			for (int í = 0; í < value.cVector.size(); í++)
				this->cVector.push_back(Value(value.cVector[í]));
			break; }
		default:
			break;
		}
	}
	Value(Number number) {
		this->cNumber = number;
		this->type = valueType::Number;
		this->mClass = Class::Value;
	}
	Value(std::string string) {
		this->cString = string;
		this->type = valueType::String;
		this->mClass = Class::Value;
	}
	Value(Bool boolean) {
		this->cBool = boolean;
		this->type = valueType::Bool;
		this->mClass = Class::Value;
	}
	Value(std::vector<Value> vector) {
		this->cVector = vector;
		this->type = valueType::Array;
		this->mClass = Class::Value;
	}
	Value() {
		this->mClass = Class::Value;
		this->type = valueType::Null;
	}

	void clear() {
		this->cString.~basic_string();
		for (int i = 0; i < this->cVector.size(); i++) {
			this->cVector[i].clear();
		}
		this->cVector.clear();
		this->cVector.~vector();
	}

	std::string toString() {
		switch (this->type)
		{
		case valueType::Bool: return this->cBool.toString(); break;
		case valueType::Number:return this->cNumber.toString(); break;
		case valueType::String: return '"' + this->cString + '"'; break;
		case valueType::Array: { std::string s = "[ "; int i = 0; for (Value v : this->cVector) { s += v.toString(); if (i < this->cVector.size() - 1) s += ", "; i++; } return s + " ]"; break;
		}
		case valueType::Null: return "Null";
		default:
			break;
		}
	}

	std::string toString(bool) {
		switch (this->type)
		{
		case valueType::Bool: return this->cBool.toString(); break;
		case valueType::Number:return this->cNumber.toString(); break;
		case valueType::String: return this->cString; break;
		case valueType::Array: { std::string s = "[ "; int i = 0; for (Value v : this->cVector) { s += v.toString(); if (i < this->cVector.size() - 1) s += ", "; i++; } return s + " ]"; break;
		}
		case valueType::Null: return "Null";
		default:
			break;
		}
	}

	nullCMP;
	Value nullEQ;
	Value nullCon;
};

#endif // !_VALUE_TYPES_H_