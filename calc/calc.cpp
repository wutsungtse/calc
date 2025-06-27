// calc.cpp : 此檔案包含 "main" 函式。程式會於該處開始執行及結束執行。

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
#include <algorithm>

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
		if (expr[i] == ' ') continue;
		if (expr[i] == '+')
		{
			Token tok;
			tok.type = TokenType::Plus; tok.lexeme = "+"; tok.precedence = 1;
			tokens.push_back(tok);
		}
		else if (expr[i] == '*')
		{
			Token tok;
			tok.type = TokenType::Multiply; tok.lexeme = "*"; tok.precedence = 2;
			tokens.push_back(tok);
		}
		else if (expr[i] == '/')
		{
			Token tok; tok.type = TokenType::Divide; tok.lexeme = "/"; tok.precedence = 2;
			tokens.push_back(tok);
		}
		else if (expr[i] == '(')
		{
			Token tok;
			tok.type = TokenType::LParen; tok.lexeme = "(";
			tokens.push_back(tok);
		}
		else if (expr[i] == ')')
		{
			Token tok;
			tok.type = TokenType::RParen; tok.lexeme = ")";
			tokens.push_back(tok);
		}
		else if (expr[i] == '-')
		{
			Token tok;
			if (tokens.empty() || ((tokens.back().type != TokenType::Number) && tokens.back().type != TokenType::RParen))
			{ 
				Token new_token;
				new_token.type = TokenType::Number; new_token.lexeme = "-1"; new_token.value = -1;
				tokens.push_back(new_token);

				tok.type = TokenType::Multiply; tok.lexeme = "*"; tok.precedence = 2;
			}
			else {
				tok.type = TokenType::Minus; tok.lexeme = "-"; tok.precedence = 1;
			}
			tokens.push_back(tok);
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
			Token tok;
			tok.type = TokenType::Number; tok.lexeme = num; tok.value = stoi(num);
			tokens.push_back(tok);
		}
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
			getline(cin, expr);

			vector<Token> tokens = tokenize(expr);

			vector<Token> new_tokens = tokens;
			queue<Token> postfix_tokens = infixToPostfix(new_tokens);

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