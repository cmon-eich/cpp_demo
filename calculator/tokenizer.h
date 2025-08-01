#ifndef tokenizer
#define tokenizer

class Token {
	public:
		Token() {};
		virtual ~Token() {};
		virtual void Print() const = 0;
	private:
};
class OperatorToken : public Token {
	public:
		OperatorToken(char op): op(op) {};
		~OperatorToken() override {};
		void Print() const override;
	private:
		char op;
};
class ValueToken : public Token {
	public:
		ValueToken(int value): value(value) {};
		~ValueToken() override {};
		void Print() const override;
	private:
		int value;
};
class BracketToken : public Token {
	public:
		BracketToken(char bracket);
		~BracketToken() override {};
		void Print() const override;
	private:
		bool isOpening;
};
#endif
