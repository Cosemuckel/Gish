
class Bool {
public:
	Bool(bool b) : value(b) {}
	Bool () : value(false) {}
	bool value;

	std::string toString() const {
		return value ? "true" : "false";
	}
};

typedef ttmath::Big<32, 95> float128;
typedef ttmath::Int<256> int256;

class Number {
public:
	Number(int256 n) : value(n) {}
	Number() : value(0) {}
	int256 value;
	
	std::string toString() const {
		return value.ToString();
	}

};

class Value {

public:

	enum class Type {
		Bool,
		Number,
		String,
		Array,
		Void,
		Null
	};

	static std::string typeName(Type t) {
		switch (t) {
		case Type::Bool: return "bool";
		case Type::Number: return "number";
		case Type::String: return "string";
		case Type::Array: return "array";
		case Type::Void: return "void";
		case Type::Null: return "null";
		}
	}

	std::string typeName() {
		return typeName(type);
	}

	Type type;
	
	// union {
		Bool b;
		Number n;
		std::string s;
		std::vector<Value> a;
	// };

	Value(Bool b) : type(Type::Bool), b(b) {}
	Value(Number n) : type(Type::Number), n(n) {}
	Value(std::string s) : type(Type::String), s(s) {}
	Value(std::vector<Value> a) : type(Type::Array), a(a) {}
	Value() : type(Type::Null) {}
	Value(const Value& v) {
		std::cout << "Created: ";
		type = v.type;
		switch (type) {
		case Type::Bool: b = v.b; break;
		case Type::Number: n = v.n;  break;
		case Type::String: s = v.s;  break;
		case Type::Array: a = v.a;  break;
		case Type::Void: break;
		case Type::Null: break;
		}
	}
	
	std::string toString() {
		switch (this->type) {
		case Type::Bool: return this->b.toString();
		case Type::Number: return this->n.toString();
		case Type::String: return '"' + this->s + '"';
		case Type::Array: return "[" + join(a, ", ") + "]";
		case Type::Void: return "void";
		case Type::Null: return "null";
		}
	}

	~Value() {}
	
	Value& operator=(const Value& v) {
		type = v.type;
		switch (type) {
		case Type::Bool: b = v.b; break;
		case Type::Number: n = v.n; break;
		case Type::String: s = v.s; break;
		case Type::Array: a = v.a; break;
		case Type::Void: break;
		case Type::Null: break;
		}
		return *this;
	}

};