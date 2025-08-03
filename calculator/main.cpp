#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "tokenizer.h"
#include "helper.h"


std::vector<const tokenizer::Token*> infixToPostfix(const std::vector<const tokenizer::Token*> &tokens) {
	std::vector<const tokenizer::Token*> output;
	std::stack<const tokenizer::Token*> operators;
	for (const tokenizer::Token *token : tokens) {
		if (token->t == tokenizer::Value) {
			output.push_back(token);
		} else if (token->t == tokenizer::OpeningBracket) {
			operators.push(token);
		} else if (token->t == tokenizer::ClosingBracket) {
			while (!operators.empty() && operators.top()->t != tokenizer::OpeningBracket) {
				output.push_back(operators.top());
				operators.pop();
			}
			if (!operators.empty()) {
				operators.pop(); // remove OpeningBracket
			}
		} else {
			while (!operators.empty() && operators.top()->precedence >= token->precedence) {
				output.push_back(operators.top());
				operators.pop();
			}
			operators.push(token);
		}
	}
	while (!operators.empty()) {
		output.push_back(operators.top());
		operators.pop();
	}
	return output;
}

double evaluatePostfix(const std::vector<const tokenizer::Token*>& postfix) {
	std::stack<const tokenizer::Token*> values;

	for (const tokenizer::Token *token : postfix) {
		if (token->t == tokenizer::Operator) {
			if (values.size() < 2) {
			    throw std::runtime_error("Invalid expression: not enough operands");
			}
			const tokenizer::Token *b = values.top();
			values.pop();
			const tokenizer::Token *a = values.top();
			values.pop();
			if (token->str == '+') values.push(new tokenizer::ValueToken(a->getValue() + b->getValue()));
			else if (token->str == '-') values.push(new tokenizer::ValueToken(a->getValue() - b->getValue()));
			else if (token->str == '*') values.push(new tokenizer::ValueToken(a->getValue() * b->getValue()));
			else if (token->str == '/') values.push(new tokenizer::ValueToken(a->getValue() / b->getValue()));
			else std::cout << "encountered unexpected token!" << std::endl;
		} else {
		    values.push(token);
		}
	}
	return values.top()->getValue();
}

int main() {
	std::string input;
	std::getline(std::cin, input);
	std::cout << "Ihre Eingabe: " << input << std::endl;
	std::vector<const tokenizer::Token*> tokens;
	std::string number;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == ' ') {
			continue;
		}
		if (std::isdigit(input[i])) {
			number += input[i];
			if (i+1 != input.length()) {
				if (!std::isdigit(input[i+1])) {
					tokens.push_back(new tokenizer::ValueToken(std::stod(number)));
					number = "";
				}
			} else {
				tokens.push_back(new tokenizer::ValueToken(std::stod(number)));
			}
		} else if (helper::isOperator(input[i])) {
			tokens.push_back(new tokenizer::OperatorToken(input[i]));
		} else if (input[i] == '(') {
			tokens.push_back(new tokenizer::OpeningBracketToken());
		} else if (input[i] == ')') {
			tokens.push_back(new tokenizer::ClosingBracketToken());
		} else {
			std::cout << "Encountered unexpected Token: " << input[i] << std::endl;
			return 1;
		}
	}
	const std::vector<const tokenizer::Token*> postfix = infixToPostfix(tokens);
	const double result = evaluatePostfix(postfix);
	std::cout << "Result: " << result << std::endl;
	return 0;
}
