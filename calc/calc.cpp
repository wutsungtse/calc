// calc.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。

/*
Steps
	1. struct Token
	2. vector<Token> tokenize(string expr){}
	3. infix -> postfix (using stack + queue)
	4. evaluate postfix expression (using stack)
*/

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>

using namespace std;

enum class TokenType {
	Number, Plus, Minus, Multiply, Divide, LParen, RParen
};

struct Token {
	TokenType type;
	std::string lexeme;
	double value{};
	int precedence{};
	bool rightAssoc{};
	int line{}, col{};
};

vector<Token> tokenize(string expr)
{
	vector<Token> tokens;
	for (int i=0; i<expr.size(); i++) {
		Token tok;
		if (expr[i] == ' ') {
			continue;
		}
		else if (expr[i] == '+')
		{
			tok.type = TokenType::Plus;
			tok.lexeme = '+';
			tok.precedence = 1;
		}
		else if (expr[i] == '*')
		{
			tok.type = TokenType::Multiply;
			tok.lexeme = "*";
			tok.precedence = 2;
		}
		else if (expr[i] == '/')
		{
			tok.type = TokenType::Divide;
			tok.lexeme = "/";
			tok.precedence = 2;
		}
		else if (expr[i] == '(')
		{
			tok.type = TokenType::LParen;
			tok.lexeme = "(";
		}
		else if (expr[i] == ')')
		{
			tok.type = TokenType::RParen;
			tok.lexeme = ")";
		}
		else if (expr[i] == '-')
		{
			tok.type = TokenType::Minus;
			tok.lexeme = "-";
			tok.precedence = 1;
		}
		else if (isdigit(expr[i]))
		{
			string num;
			int j = i;
			while (j < expr.size() && isdigit(expr[j]))
			{
				num += expr[j];
				j++;
			}
			i = j-1;
			tok.type = TokenType::Number;
			tok.lexeme = num;
			tok.value = stoi(num);
		}
		tokens.push_back(tok);
	}
	return tokens;
}

queue<Token> infixToPostfix(vector<Token> tokens)
{
	stack<Token> myStack;
	queue<Token> myQueue;

	for(int i=0; i<tokens.size(); i++)
	{
		Token tok = tokens[i];

		if (tok.type == TokenType::Number)
		{
			myQueue.push(tok);
		}
		else if (tok.type == TokenType::LParen)
		{
			myStack.push(tok);
		}
		else if (tok.type == TokenType::RParen)
		{
			tok = myStack.top();
			myStack.pop();
			while (tok.type != TokenType::LParen)
			{
				myQueue.push(tok);
				tok = myStack.top();
				myStack.pop();
			}
		}
		else
		{
			if (myStack.empty() || tok.precedence > myStack.top().precedence)
			{
				myStack.push(tok);
			}
			else
			{
				while (!myStack.empty() && tok.precedence <= myStack.top().precedence)
				{
					myQueue.push(myStack.top());
					myStack.pop();
				}
				myStack.push(tok);
			}
		}
	}
	while (!myStack.empty())
	{
		myQueue.push(myStack.top());
		myStack.pop();
	}
	return myQueue;
}

double evaluatePostfix(queue<Token> postfix_tokens)
{
	stack<Token> myStack;

	while (!postfix_tokens.empty())
	{
		Token token = postfix_tokens.front();
		postfix_tokens.pop();

		if (token.type == TokenType::Number)
		{
			myStack.push(token);
		}
		else
		{
			if (myStack.size() < 2) {
				cerr << "Error: Not enough operands in stack for operation.\n";
				return 0.0;
			}
			Token right = myStack.top();
			myStack.pop();
			Token left = myStack.top();
			myStack.pop();
			double result = 0.0;
			if (token.type == TokenType::Plus)
			{
				result = left.value + right.value;
			}
			else if (token.type == TokenType::Minus)
			{
				result = left.value - right.value;
			}
			else if (token.type == TokenType::Multiply)
			{
				result = left.value * right.value;
			}
			else if (token.type == TokenType::Divide)
			{
				if (right.value == 0) {
					cerr << "Error: Division by zero.\n";
					return 0.0;
				}
				result = left.value / right.value;
			}
			Token res_token;
			res_token.type = TokenType::Number;
			res_token.value = result;
			myStack.push(res_token);
		}
	}
	return myStack.top().value;
}

	int main()
	{
		while (true) {
			string expr;
			cout << "Enter Expression: \n";
			cin >> expr;

			vector<Token> tokens = tokenize(expr);
			queue<Token> postfix_tokens = infixToPostfix(tokens);

			cout << "\nInfix Expression:\n";

			for (int i = 0; i < tokens.size(); i++) {
				Token t = tokens[i];
				cout << " " << t.lexeme;
			}

			cout << "\n\nPostfix Expression:\n";

			queue<Token> p_tokens = postfix_tokens;

			while (!postfix_tokens.empty()) {
				Token t = postfix_tokens.front();
				cout << " " << t.lexeme;
				postfix_tokens.pop();
			}

			double result = evaluatePostfix(p_tokens);
			cout << "\n\nResult: " << result << "\n";
			cout << "\n\n";
		}
		return 0;
	}