/*
--- Day 10: Elves Look, Elves Say ---

Today, the Elves are playing a game called look-and-say. They take turns making sequences by reading aloud the previous sequence and using that reading as the next sequence. For example, 211 is read as "one two, two ones", which becomes 1221 (1 2, 2 1s).

Look-and-say sequences are generated iteratively, using the previous value as input for the next step. For each step, take the previous value, and replace each run of digits (like 111) with the number of digits (3) followed by the digit itself (1).

For example:

    1 becomes 11 (1 copy of digit 1).
    11 becomes 21 (2 copies of digit 1).
    21 becomes 1211 (one 2 followed by one 1).
    1211 becomes 111221 (one 1, one 2, and two 1s).
    111221 becomes 312211 (three 1s, two 2s, and one 1).

Starting with the digits in your puzzle input, apply this process 40 times. What is the length of the result?

Your puzzle input is 3113322113.
*/
#include <cstddef>
#include <iostream>
#include <stdlib.h>

std::string ltos(long c)
{
	std::string s;
	long iter;
	int digit;

	for (iter = c; iter > 0; iter/=10)
	{
		digit = (iter%10)+'0';
		s.insert(s.begin(), digit);
	}
	return (s);
}

int main()
{
	long in = 3113322113;
	std::string say;
	std::string next;
	std::string ascii = ltos(in);
	size_t times = 0;

	while (times < 50)
	{
		next = "";
		for (size_t i = 0; i < ascii.size();)
		{
			char current = ascii[i++];
			long count = 1;
			while (i < ascii.size() && current == ascii[i])
				i++, count++;
			// std::cout << count << current;
			// TODO: process the count..
			next += ltos(count)+current;
		}
		ascii = next;
		times++;
	}
	std::cout << next.size();
	std::cout << std::endl;
	return(0);
}
