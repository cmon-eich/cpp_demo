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
		explicit Token(const TokenType t): t(t), str('0') {};
		Token(const TokenType t, const int precedence): t(t), precedence(precedence), str('0') {};
		Token(const TokenType t, const char str): t(t), str(str) {};
		Token(const TokenType t, const char str, const int precedence): t(t), precedence(precedence), str(str) {};
		virtual ~Token() = default;
		TokenType t;
		int precedence{};
		char str;
		virtual void Print() const = 0;
		void setNext(Token *nextToken);
		void setPrecedence(int p) {
			precedence = p;
		}
		[[nodiscard]] virtual double getValue() const {
			return 0;
		};
	private:
		Token *next{};
};
class ValueToken final : public Token {
	public:
		explicit ValueToken(const double value): Token(Value, 0), value(value) {};
		~ValueToken() override = default;
		void Print() const override;
		[[nodiscard]] double getValue() const override;
	private:
		double value;
};

class NonValueToken : public Token {
	public:
		explicit NonValueToken(const TokenType t): Token(t) {};
		NonValueToken(const TokenType t, const char str): Token(t, str) {};
		~NonValueToken() override = default;
		void Print() const override = 0;
};
class OperatorToken final : public NonValueToken {
	public:
		explicit OperatorToken(char op);
		~OperatorToken() override = default;
		void Print() const override;
};

class BracketToken : public NonValueToken {
	public:
		explicit BracketToken(TokenType t);
		~BracketToken() override = default;
		void Print() const override = 0;
};

class OpeningBracketToken final : public BracketToken {
	public:
		OpeningBracketToken();
		~OpeningBracketToken() override = default;
		void Print() const override;
};
class ClosingBracketToken final : public BracketToken {
	public:
		ClosingBracketToken();
		~ClosingBracketToken() override = default;
		void Print() const override;
};
#endif
