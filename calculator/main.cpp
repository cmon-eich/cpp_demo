#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "calculator.h"
#include "tokenizer.h"
#include "helper.h"


std::vector<const tokenizer::Token*> infixToPostfix(const std::vector<tokenizer::Token*> &tokens) {
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
			operators.pop(); // remove OpeningBracket
		} else if (token->t== tokenizer::Operator) {
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

// Step 2: Evaluate postfix expression
double evaluatePostfix(std::vector<const tokenizer::Token*>& postfix) {
	std::stack<const tokenizer::Token*> values;

	for (const tokenizer::Token *token : postfix) {
		if (token->t == tokenizer::Operator) {
			const tokenizer::Token *b = values.top(); 
			values.pop();
			const tokenizer::Token *a = values.top(); 
			values.pop();
			if (token->str == '+') values.push(new tokenizer::ValueToken(a->getValue() + b->getValue()));
			else if (token->str == '-') values.push(new tokenizer::ValueToken(a->getValue() - b->getValue()));
			else if (token->str == '*') values.push(new tokenizer::ValueToken(a->getValue() * b->getValue()));
			else if (token->str == '/') values.push(new tokenizer::ValueToken(a->getValue() / b->getValue()));
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
	tokenizer::Token* tempToken;
	std::vector<tokenizer::Token*> tokens;

	std::string number = "";
	std::string text = "";
	Token *previous;
	for (int i = 0; i < input.length(); i++) {
		if (std::isdigit(input[i])) {
			number += input[i];
			if (i+1 != input.length()) {
				if (!std::isdigit(input[i+1])) {
					tokenizer::ValueToken *valueToken = new tokenizer::ValueToken(std::stoi(number));
					tokens.push_back(valueToken);
					number = "";
				}
				continue;
			} else {
				tokenizer::ValueToken *valueToken = new tokenizer::ValueToken(std::stoi(number));
				tokens.push_back(valueToken);
			}
		} else if (input[i] == ' ') {
		       continue;
		} else if (helper::isOperator(input[i])) {
			tokenizer::OperatorToken *operatorToken = new tokenizer::OperatorToken(input[i]);
			tokens.push_back(operatorToken);
		} else if (input[i] == '(') {
			tokenizer::OpeningBracketToken *bracketToken = new tokenizer::OpeningBracketToken();
			tokens.push_back(bracketToken);
		} else if (input[i] == ')') {
			tokenizer::ClosingBracketToken *bracketToken = new tokenizer::ClosingBracketToken();
			tokens.push_back(bracketToken);
		}else {
			std::cout << "Encountered unexpected Token: " << input[i] << std::endl;
			return 1;
			continue;
		}
	}
	std::vector<const tokenizer::Token*> postfix = infixToPostfix(tokens);
	double result = evaluatePostfix(postfix);
	std::cout << "Result: " << result << std::endl;
	return 0;
}
