/*
--- Day 12: JSAbacusFramework.io ---

Santa's Accounting-Elves need help balancing the books after a recent order. Unfortunately, their accounting software uses a peculiar storage format. That's where you come in.

They have a JSON document which contains a variety of things: arrays ([1,2,3]), objects ({"a":1, "b":2}), numbers, and strings. Your first job is to simply find all of the numbers throughout the document and add them together.

For example:

    [1,2,3] and {"a":2,"b":4} both have a sum of 6.
    [[[3]]] and {"a":{"b":4},"c":-1} both have a sum of 3.
    {"a":[-1,1]} and [-1,{"a":1}] both have a sum of 0.
    [] and {} both have a sum of 0.

You will not encounter any strings containing numbers.

What is the sum of all numbers in the document?
*/
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

typedef struct Token {
public:
	typedef enum Kind {
		Number,
		String,
		Other,
		END
	} Kind;

	char other;
	std::string str;
	long num;
	Kind kind;

	/* constructors */
	Token(long N) : num(N), kind(Number) {}
	Token(std::string s) : str(s), kind(String) {}
	Token(char c) : other(c), kind(Other) {}
	Token(size_t end) : num(end), kind(END) {}

	/* Copy */
	Token &operator=(const Token &Other)
	{
		if (this != &Other)
		{
			this->kind = Other.kind;
			this->num = Other.num;
			this->str = Other.str;
			this->other = Other.other;
		}
		return (*this);
	}

	Token(const Token &Other)
	{
		if (this == &Other)
			return ;
		this->kind = Other.kind;
		this->num = Other.num;
		this->str = Other.str;
		this->other = Other.other;
	}
} Token;
Token Next(std::string data);

class Lexer {
private:
	std::vector<Token> tokens;
	size_t cursor;
public:
	Lexer(std::string data): cursor(0)
	{
		Token current = Next(data);
		while (current.kind != Token::END)
		{
			this->tokens.push_back(current);
			current = Next(data);
		}
		this->tokens.push_back(current);
		std::cout << "Lexer: Got " << this->tokens.size() << " tokens from data\n";
	}
	Token peek()
	{
		return (this->tokens[this->cursor]);
	}
	void advance()
	{
		if (this->cursor < this->tokens.size()) this->cursor++;
	}
	~Lexer()
	{}
};

typedef struct object {
public:
	/* Types that the parser implements */
	typedef enum Kind {
		Table,
		Number,
		String,
		Array,
		Neither
	} Kind;
	Kind kind;

	/* The underlying types that can be recursively parsed */
	typedef struct table_s {
		std::string Key;
		object *Value;
	} table_t;

	std::vector<object*> array;
	table_t table;
	std::string string;
	long number;
	object() : kind(Neither) {};
	~object() {
		/* recursively delete all the dangling data.. */
		/* TODO: Delete the Arrays and Tables?? */
		switch (this->kind)
		{
			case Neither: {} break;
			case String: {} break;
			case Table: {
				if (this->table.Value) delete this->table.Value;
			} break;
			case Array: {
				for (size_t x = 0; x < this->array.size(); ++x) delete this->array[x];
			} break;
			case Number: {} break;
			default:{} break;
		}
	};
} object;


Token Next(std::string data)
{
	static size_t cursor;
	std::string token;

	for (;cursor < data.size() && isspace(data[cursor]);cursor++);
	if (cursor == data.size()) return (Token(cursor));
	if (isdigit(data[cursor]) || data[cursor] == '-')
	{
		token.push_back(data[cursor++]);
		while (cursor < data.size() && isdigit(data[cursor]))
			token.push_back(data[cursor++]);
		return (Token(std::atol(token.c_str())));
	} else {
		if (data[cursor] == '\"') {
			token.push_back(data[cursor++]);
			while (cursor < data.size() && data[cursor] != '\"')
				token.push_back(data[cursor++]);
			token.push_back(data[cursor++]);
		} else if (isalpha(data[cursor])) {
			token.push_back(data[cursor++]);
			while (cursor < data.size() && isalnum(data[cursor]))
				token.push_back(data[cursor++]);
		} else {
			return (Token(data[cursor++]));
		}
		return (Token(token));
	}
	return (Token(data[cursor++]));
}

object *Parse_Object(Lexer &lexer)
{
	/* Top level function to parse an object while , is the next token*/
	/* can I parse also arrays???*/
	object *obj;

	obj = new object();
	Token next = lexer.peek();
	assert(next.kind == Token::Other
		&& next.other == '{'
		&& "Found an object but does not start with {!!");
	lexer.advance();
	do {
		next = lexer.peek();
		switch ()
		{
		}
	} while (next.kind != Token::END);
	return(obj);
}

int main(int ac, char **av)
{
	size_t n = 0;
	ssize_t nread = 1;
	char *Line = NULL;  
	object *obj;
	if (ac == 1)
		return 0;
	FILE *fp = fopen(av[1], "r");

	if (!fp) return (1);
	nread = getline(&Line, &n, fp);
	if (nread == -1) return (1);
	Lexer lex(Line);
	obj = Parse_Object(lex);
	std::cout << std::hex << "Ptr: "  << obj;
	delete obj;
	free(Line);
	fclose(fp);
	reurn (0);
}
