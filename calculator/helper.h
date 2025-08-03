#ifndef helper
#define helper

bool isOperator(char op) {
	if (op == '+' || op == '-' || op == '*' || op == '/') {
		return true;
	}
	return false;
}

#endif
