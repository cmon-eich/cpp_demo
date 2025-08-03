#include <iostream>
#include "tokenizer.h"

void Token::setNext(Token *nextToken) {
	next = nextToken;
}

void ValueToken::Print() const {
	std::cout << "ValueToken: " << value << std::endl;
}

double ValueToken::getValue() const {
	return value;
}

void OperatorToken::Print() const {
	std::cout << "OperatorToken: " << str << std::endl;
}

OperatorToken::OperatorToken(const char op): NonValueToken(Operator, op) {
	precedence = 1;
	if (op == '*' || op == '/') {
		precedence = 2;
	}
}

void OpeningBracketToken::Print() const {
	std::cout << "OpeningBracketToken: (" << std::endl;
}

void ClosingBracketToken::Print() const {
	std::cout << "ClosingBracketToken: )" << std::endl;
}

BracketToken::BracketToken(const TokenType t): NonValueToken(t) {
	precedence = 0;
}
OpeningBracketToken::OpeningBracketToken(): BracketToken(OpeningBracket) {}
ClosingBracketToken::ClosingBracketToken(): BracketToken(ClosingBracket) {}

