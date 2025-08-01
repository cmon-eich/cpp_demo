#include <iostream>
#include "tokenizer.h"

void Token::setNext(Token *nextToken) {
	next = nextToken;
	return;
}

void ValueToken::Print() const {
	std::cout << "ValueToken: " << value << std::endl;
}

int ValueToken::getValue() const {
	return value;
}

void OperatorToken::Print() const {
	std::cout << "OperatorToken: " << str << std::endl;
}

OperatorToken::OperatorToken(char op): NonValueToken(Operator, op) {
	precedence = 2;
	if (op == '*' || op == '/') {
		precedence = 1;
	}
}

void OpeningBracketToken::Print() const {
	std::cout << "OpeningBracketToken: (" << std::endl;
}

void ClosingBracketToken::Print() const {
	std::cout << "ClosingBracketToken: )" << std::endl;
}

BracketToken::BracketToken(TokenType t): NonValueToken(t) {
	precedence = 1;
}
OpeningBracketToken::OpeningBracketToken(): BracketToken(OpeningBracket) {}
ClosingBracketToken::ClosingBracketToken(): BracketToken(ClosingBracket) {}

