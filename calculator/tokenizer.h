#ifndef tokenizer
#define tokenizer

enum TokenType {
	Operator,
	Value,
	OpeningBracket,
	ClosingBracket
};

class Token {
	public:
		Token(TokenType t): t(t), str('0') {};
		Token(TokenType t, int precedence): t(t), str('0'), precedence(precedence) {};
		Token(TokenType t, char str): t(t), str(str) {};
		virtual ~Token() {};
		TokenType t;
		int precedence;
		char str;
		virtual void Print() const = 0;
		void setNext(Token *nextToken);
		virtual int getValue() const {
			return 0;
		};
	private:
		Token *next;
};
class ValueToken : public Token {
	public:
		ValueToken(double value): Token(Value, 3), value(value) {};
		~ValueToken() override {};
		void Print() const override;
		virtual int getValue() const override;
	private:
		double value;
};

class NonValueToken : public Token {
	public:
		NonValueToken(TokenType t): Token(t) {};
		NonValueToken(TokenType t, char str): Token(t, str) {};
		virtual ~NonValueToken() {};
		virtual void Print() const = 0;
};
class OperatorToken : public NonValueToken {
	public:
		OperatorToken(char op);
		~OperatorToken() override {};
		void Print() const override;
};

class BracketToken : public NonValueToken {
	public:
		BracketToken(TokenType t);
		virtual ~BracketToken() override {};
		virtual void Print() const = 0;
};

class OpeningBracketToken : public BracketToken {
	public:
		OpeningBracketToken();
		~OpeningBracketToken() override {};
		void Print() const override;
};
class ClosingBracketToken : public BracketToken {
	public:
		ClosingBracketToken();
		~ClosingBracketToken() override {};
		void Print() const override;
};
#endif
