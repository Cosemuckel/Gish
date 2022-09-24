#pragma once

class SymbolTable {

public:


	Value* get(std::string name) {
		if (symbols.count(name))
			return symbols[name];
		return nullptr;
	}

	void set(std::string name, Value* value) {
		this->symbols.insert_or_assign(name, value);
	}

	void remove(std::string name) {
		this->symbols.erase(name);
	}



private:

	std::unordered_map<std::string, Value*> symbols;
	SymbolTable* parent = nullptr;


};
class Environment {

public:

	SymbolTable* symbolTable;
	
	bool hasVariable(std::string name) {
		return symbolTable->get(name) != nullptr;
	}

};
