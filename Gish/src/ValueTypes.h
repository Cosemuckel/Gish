
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
typedef ttmath::Int<4> int256;

class Number {
public:
	Number(int256 n) : value(n) {}
	Number() : value(0) {}
	int256 value;
	
	std::string toString() const {
		return value.ToString();
	}	

};

class Position;

class Value {

private:

	Position* startPos = nullptr;
	Position* endPos = nullptr;

public:

	enum class Type {
		Bool,
		Number,
		String,
		Array,
		Void,
		Null
	};

	static std::string typeName(Type const t) {
		switch (t) {
		case Type::Bool: return "Boolean";
		case Type::Number: return "Number";
		case Type::String: return "String";
		case Type::Array: return "Array";
		case Type::Void: return "Void";
		case Type::Null: return "Null";
		}
	}

	std::string typeName() const {
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
	
	Value* setStartPos(Position* pos) {
		startPos = pos;
		return this;
	}
	Value* setEndPos(Position* pos) {
		endPos = pos;
		return this;
	}
	Value* setPositions(Position* start, Position* end) {
		startPos = start;
		endPos = end;
		return this;
	}

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
	
	Value& operator+(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			r.n.value += v.n.value;
			return r;
		}
		if (type == Type::String && v.type == Type::String) {
			r.s += v.s;
			return r;
		}
		if (type == Type::Array && v.type == Type::Array) {
			r.a.insert(r.a.end(), v.a.begin(), v.a.end());
			return r;
		}
		throw RuntimeError("cannot add " + typeName() + " and " + v.typeName(), startPos, v.endPos);
	}

	Value& operator-(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			r.n.value -= v.n.value;
			return r;
		}
		throw RuntimeError("cannot subtract " + v.typeName() + " from " + typeName(), startPos, v.endPos);
	}

	Value& operator-() {
		Value r = *this;
		if (type == Type::Number) {
			r.n.value = -r.n.value;
			return r;
		}
		throw RuntimeError("cannot negate " + typeName(), startPos, endPos);
	}
	
	Value& operator*(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			r.n.value *= v.n.value;
			return r;
		}
		if (type == Type::String && v.type == Type::Number) {
			r.s = "";
			for (int256 i = 0; i < v.n.value; i++) {
				r.s += s;
			}
			return r;
		}
		if (type == Type::Number && v.type == Type::String) {
			r.s = "";
			for (int256 i = 0; i < n.value; i++) {
				r.s += v.s;
			}
			return r;
		}
		throw RuntimeError("cannot multiply " + typeName() + " and " + v.typeName(), startPos, v.endPos);
	}

	Value& operator/(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			// Check for division by zero
			if (v.n.value == 0) {
				throw RuntimeError("division by zero", startPos, v.endPos);
			}
			r.n.value /= v.n.value;
			return r;
		}
		throw RuntimeError("cannot divide " + typeName() + " by " + v.typeName(), startPos, v.endPos);
	}

	Value& operator==(const Value& v) {
		Value r = *this;
		if (type == Type::Bool && v.type == Type::Bool) {
			r.b.value = b.value == v.b.value;
			return r;
		}
		if (type == Type::Number && v.type == Type::Number) {
			r.b.value = n.value == v.n.value;
			return r;
		}
		if (type == Type::String && v.type == Type::String) {
			if (s.size() != v.s.size()) {
				r.b.value = false;
				return r;
			}
			for (int i = 0; i < s.size(); i++) {
				if (s[i] != v.s[i]) {
					r.b.value = false;
					return r;
				}
			}
			r.b.value = true;
			return r;
		}
		if (type == Type::Array && v.type == Type::Array) {
			if (a.size() != v.a.size()) {
				r.b.value = false;
				return r;
			}
			for (int i = 0; i < a.size(); i++) {
				if ((a[i] != v.a[i]).b.value) {
					r.b.value = false;
					return r;
				}
			}
			r.b.value = true;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}
	
	Value& operator!=(const Value& v) {
		Value r = *this;
		if (type == Type::Bool && v.type == Type::Bool) {
			r.b.value = b.value != v.b.value;
			return r;
		}
		if (type == Type::Number && v.type == Type::Number) {
			r.b.value = n.value != v.n.value;
			return r;
		}
		if (type == Type::String && v.type == Type::String) {
			if (s.size() != v.s.size()) {
				r.b.value = true;
				return r;
			}
			for (int i = 0; i < s.size(); i++) {
				if (s[i] != v.s[i]) {
					r.b.value = true;
					return r;
				}
			}
			r.b.value = false;
			return r;
		}
		if (type == Type::Array && v.type == Type::Array) {
			if (a.size() != v.a.size()) {
				r.b.value = true;
				return r;
			}
			for (int i = 0; i < a.size(); i++) {
				if ((a[i] != v.a[i]).b.value) {
					r.b.value = true;
					return r;
				}
			}
			r.b.value = false;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}

	Value& operator<(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			r.b.value = n.value < v.n.value;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}
	
	Value& operator>(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			r.b.value = n.value > v.n.value;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}

	Value& operator<=(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			r.b.value = n.value <= v.n.value;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}

	Value& operator>=(const Value& v) {
		Value r = *this;
		if (type == Type::Number && v.type == Type::Number) {
			r.b.value = n.value >= v.n.value;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}

	Value& operator&&(const Value& v) {
		Value r = *this;
		if (type == Type::Bool && v.type == Type::Bool) {
			r.b.value = b.value && v.b.value;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}

	Value& operator||(const Value& v) {
		Value r = *this;
		if (type == Type::Bool && v.type == Type::Bool) {
			r.b.value = b.value || v.b.value;
			return r;
		}
		throw RuntimeError("cannot compare " + typeName() + " to " + v.typeName(), startPos, v.endPos);
	}

};
