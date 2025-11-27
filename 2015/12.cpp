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

#include <iomanip>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

typedef struct Token {
public:
	typedef enum Kind {
		NoN = 0x0,
		Number,
		String,
		Other,
		END
	} Kind;

	char other;
	std::string str;
	long num;
	Kind kind;

	std::string as_str() {
		std::string final;
		std::string strs[] = {
		"NoN",
		"Number",
		"String",
		"Other",
		"END"
		};
		assert(this->kind <= END && "Invalid kind!");
		final += strs[this->kind] + "\t";
		switch (this->kind)
		{
			case NoN:
			case END: {} break;
			case Number: {
				std::ostringstream convert;
				convert << this->num;
				final += convert.str();
			} break;
			case String: {
				final += this->str;
			} break;
			case Other: {
				final += this->other;
			} break;
		}
		return (final);
	}
	/* constructors */
	Token() : kind(NoN) {};
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
	Token consume()
	{
		Token t = (this->tokens[this->cursor]);
		if (this->cursor < this->tokens.size()) this->cursor++;
		return (t);
	}
	void advance()
	{
		if (this->cursor < this->tokens.size()) this->cursor++;
	}
	~Lexer()
	{}
	inline void expect(Token::Kind kind, char val)
	{
		// std::cout << this->peek().as_str() << "\n";
		assert(this->peek().kind == kind
			&& (!val || val == this->peek().other)
			&& "Expected : but found something else?");
	}
	bool skip_match(Token::Kind kind, char val)
	{
		this->expect(kind, val);
		this->advance();
		return (true);
	}
};

void todo(const std::string desc)
{
	std::cout << "TODO@" << desc << "\n";
	assert(0);
}

typedef struct object {
public:
	static int sum;
	static int noredsum;
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
	std::vector<table_t> tables;
	table_t table;
	std::string string;
	long number;
	object() : kind(Neither) {};

	void print() {
		/* log */
		switch (this->kind)
		{
			case object::Number: {
				std::cout << this->number;
			} break;
			case object::String: {
				std::cout << this->string;
			} break;
			case object::Array: {
				// todo("object::print::Array");
				std::cout << "[ ";
				for (size_t i = 0; i < this->array.size(); ++i)
				{
					this->array[i]->print();
					if (i != this->array.size() - 1)
						std::cout  << ", ";
				}
				std::cout << " ]";
			} break;
			case object::Table: {
				std::cout << "{";
				for (size_t i = 0; i < this->tables.size(); ++i)
				{
					std::cout << this->tables[i].Key << ": ";
					this->tables[i].Value->print();
					if (i != this->tables.size() - 1) std::cout  << ", ";
				}
				std::cout << "}";
			} break;
			case object::Neither: {
				// return;
				todo("object::print::Neither");
			} break;
		}
	}
	int sumall()
	{
		/* log */
		int sum = 0;
		switch (this->kind)
		{
			case object::Number: {
				sum += this->number;
			} break;
			case object::String: {
			} break;
			case object::Array: {
				// todo("object::print::Array");
				for (size_t i = 0; i < this->array.size(); ++i)
					sum+=this->array[i]->sumall();
			} break;
			case object::Table: {
				for (size_t i = 0; i < this->tables.size(); ++i)
					sum+=this->tables[i].Value->sumall();
			} break;
			case object::Neither: {
				// return;
				todo("object::print::Neither");
			} break;
		}
		return (sum);
	}
	int sumnoread()
	{
		/* log */
		int sum = 0;
		// static bool red = false;
		switch (this->kind)
		{
			case object::Number: {
				sum += this->number;
			} break;
			case object::String: {
				// if (this->string == "red")

			} break;
			case object::Array: {
				for (size_t i = 0; i < this->array.size(); ++i) sum+=this->array[i]->sumnoread();
			} break;
			case object::Table: {
				int accu = 0;
				for (size_t i = 0; i < this->tables.size(); ++i)
				{
					accu+=this->tables[i].Value->sumnoread();
					if (this->tables[i].Value->kind == object::String
						&& this->tables[i].Value->string == "\"red\"")
					{
						accu = 0;
						break;
					}
				}
				sum+=accu;
			} break;
			case object::Neither: {
				// return;
				todo("object::print::Neither");
			} break;
		}
		return (sum);
	}
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

int object::sum = 0;
int object::noredsum = 0;
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
	object *o;
	o = new object();

	for (Token t = lexer.peek(); t.kind != Token::END;lexer.advance(), t = lexer.peek())
	{
		switch (t.kind)
		{
		/* premitives first */
			case Token::Number: {
				o->kind = object::Number;
				o->number = t.num;
				lexer.advance();
				return (o);
			} break;
			case Token::String: {

				o->kind = object::String;
				o->string = t.str;
				lexer.advance();
				return (o);
			} break;
		/* Complex types last */
			case Token::Other: {
				if (t.other == '{') 
				/* Object */
				{
					// 1) Jump {
					lexer.advance();
					// 2) Expect key(string)
					do {
						lexer.expect(Token::String, 0);
						o->kind = object::Table;
						o->table.Key = lexer.consume().str; // Key token is gon
						lexer.skip_match(Token::Other, ':');
						o->table.Value = Parse_Object(lexer);
						o->tables.push_back(o->table);
						if (lexer.peek().kind != Token::Other
							|| (lexer.peek().other != ','
							&& lexer.peek().other != '}'))
						{
							std::cout << lexer.peek().as_str() << "\n";
							assert(0 && "WTF!");
						}
						if (lexer.peek().other == '}')
							break;
					} while (lexer.skip_match(Token::Other, ','));
					lexer.skip_match(Token::Other, '}');
					return (o);
				} else if (t.other == '[') 
				/* Array */
				{
					// 1) Jump [
					o->kind = object::Array;
					lexer.advance();
					do {
						object *next = Parse_Object(lexer);
						assert(next);
						o->array.push_back(next);
						/* Expect , for continuation and ] for blockage??? */
						if (lexer.peek().kind != Token::Other || (lexer.peek().other != ',' && lexer.peek().other != ']'))
						{
							std::cout << lexer.peek().as_str() << "\n";
							assert(0 && "WTF!");
						}
						if (lexer.peek().other == ']')
						{
							lexer.advance();
							break;
						}
					} while (lexer.skip_match(Token::Other, ','));
					return (o);
				} else {
					std::cout << t.as_str() << std::endl;
					assert(0 && "This symbol is not expected!!");
				}
			} break;
		/* END */
			case Token::END: {} break;
			case Token::NoN: {
				assert(0 && "Unreachable code was reached???");
			} break;
		}
	}
	return (o);
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
	// std::cout << std::hex << "Ptr: "  << obj;
	// obj->print();
	std::cout << obj->sumall() << "\n";
	std::cout << obj->sumnoread() << "\n";
	delete obj;
	free(Line);
	fclose(fp);
	return (0);
}
