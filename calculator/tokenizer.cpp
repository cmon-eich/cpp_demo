#include <iostream>
#include "tokenizer.h"

void ValueToken::Print() const {
	std::cout << "ValueToken: " << value << std::endl;
}

void OperatorToken::Print() const {
	std::cout << "OperatorToken: " << op << std::endl;
}

void BracketToken::Print() const {
	char bracket = ')';
	if (isOpening) {
		bracket = '(';
	}
	std::cout << "BracketToken: " << bracket << std::endl;
}

BracketToken::BracketToken(char bracket) {
	if (bracket == '(') {
		isOpening = true;
	} else {
		isOpening = false;
	}
}
