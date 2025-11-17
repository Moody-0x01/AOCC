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
// #include <algorithm>
// #include <string>
// #include <utility>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>

// Part two..
// WIP
typedef struct Token {
public:
	typedef enum Kind {
		Number,
		String,
		Other,
		END
	} Kind;
	std::string str;
	long num;
	Kind kind;
	Token(long N) : num(N), kind(Number) {}
	Token(std::string s) : str(s), kind(String) {}
	Token() : kind(Other) {}
	Token(size_t end) : num(end), kind(END) {}
} Token;

class Json {

public:
	Json *table_value;
	long number_value;
	std::string string_value;
	typedef enum Kind {
		Table,
		Number,
		String,
		List
	} Kind;
};

static Token Next(std::string data)
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
			std::cout << "Jump: " << data[cursor] << std::endl;
			cursor++;
			return (Token());
		}
		return (Token(token));
	}
	cursor++;
	return (Token());
}

long GetSum(std::string data)
{
	Token Tk = Next(data);
	long sum = 0;

	while (Tk.kind != Token::END)
	{
		// switch (Tk.kind)
		// {
		// 	case Token::Number: {
		// 		std::cout << "NuM?: " << Tk.num << std::endl;
		// 		sum+=Tk.num;
		// 	} break;
		// 	case Token::String: {
		// 		std::cout << "String?: " << Tk.str << std::endl;
		// 	} break;
		// 	case Token::END: {
		// 		std::cout << "END" << std::endl;
		// 	} break;
		// 	default: {
		// 		std::cout << "Other" << std::endl;
		// 	}
		// }
		Tk = Next(data);
	}
	return (sum);
}

int main(int ac, char **av)
{
	size_t n = 0;
	ssize_t nread = 1;
	char *Line = NULL;  
	if (ac == 1)
		return 0;
	FILE *fp = fopen(av[1], "r");

	if (!fp) return (1);
	nread = getline(&Line, &n, fp);
	if (nread == -1) return (1);
	// std::cout << Line;
	std::cout << "Answer: " << GetSum(Line) << std::endl;
	free(Line);
	return (0);
}
