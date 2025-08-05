#ifndef helper
#define helper

bool isOperator(const char op) {
	if (op == '+' || op == '-' || op == '*' || op == '/') {
		return true;
	}
	return false;
}

#endif
