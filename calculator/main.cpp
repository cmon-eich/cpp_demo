#include <iostream>
#include <string>
#include <vector>
#include "calculator.h"
#include "tokenizer.h"
#include "helper.h"

int main() {
	std::string input;
	std::getline(std::cin, input);
	std::cout << "Ihre Eingabe: " << input << std::endl;
	tokenizer::Token* tempToken;
	std::vector<tokenizer::Token*> tokens;

	std::string number = "";
	std::string text = "";
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
		} else if (helper::isBracket(input[i])) {
			tokenizer::BracketToken *bracketToken = new tokenizer::BracketToken(input[i]);
			tokens.push_back(bracketToken);
		} else {
			std::eout << "Encountered unexpected Token: " << input[i] << std::endl;
			return 1
			continue;
		}
	}
	for (auto t : tokens) {
		t->Print();
	}
	return 0;
}
