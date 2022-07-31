#pragma once

class SymbolTable {

public:


	Value* get(std::string name) {
		if (symbols.count(name))
			return symbols[name];
		if (parent != nullptr)
			return parent->get(name);
		return nullptr;
	}

	void set(std::string name, Value* value) {
		this->symbols.insert_or_assign(name, new Value(*value));
	}

	void remove(std::string name) {
		this->symbols.erase(name);
	}


	std::unordered_map<std::string, Value*> symbols;


private:
	SymbolTable* parent = nullptr;


};
class Environment {

public:

	Environment() {
		this->symbolTable = new SymbolTable();
	}

	SymbolTable* symbolTable;
	
	bool shouldReturn = false;
	bool shouldBreak = false;
	bool shouldContinue = false;

	bool inLoop = false;
	bool inFunction = false;
	
	bool hasVariable(std::string name) {
		return symbolTable->get(name) != nullptr;
	}

};
