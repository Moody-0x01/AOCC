/*

--- Day 11: Corporate Policy ---

Santa's previous password expired, and he needs help choosing a new one.

To help him remember his new password after the old one expires, Santa has devised a method of coming up with a password based on the previous one. Corporate policy dictates that passwords must be exactly eight lowercase letters (for security reasons), so he finds his new password by incrementing his old password string repeatedly until it is valid.

Incrementing is just like counting with numbers: xx, xy, xz, ya, yb, and so on. Increase the rightmost letter one step; if it was z, it wraps around to a, and repeat with the next letter to the left until one doesn't wrap around.

Unfortunately for Santa, a new Security-Elf recently started, and he has imposed some additional password requirements:

    Passwords must include one increasing straight of at least three letters, like abc, bcd, cde, and so on, up to xyz. They cannot skip letters; abd doesn't count.
    Passwords may not contain the letters i, o, or l, as these letters can be mistaken for other characters and are therefore confusing.
    Passwords must contain at least two different, non-overlapping pairs of letters, like aa, bb, or zz.

For example:

    hijklmmn meets the first requirement (because it contains the straight hij) but fails the second requirement requirement (because it contains i and l).
    abbceffg meets the third requirement (because it repeats bb and ff) but fails the first requirement.
    abbcegjk fails the third requirement, because it only has one double letter (bb).
    The next password after abcdefgh is abcdffaa.
    The next password after ghijklmn is ghjaabcc, because you eventually skip all the passwords that start with ghi..., since i is not allowed.

Given Santa's current password (your puzzle input), what should his next password be?
*/
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>

bool ill(char c)
{
	switch (c)
	{
		case 'o': { return (true);};
		case 'i': { return (true);};
		case 'l': { return (true);};
		default: {};
	}
	return (false);
}

bool is_valid(std::string password)
{
	// Passwords must include one increasing straight 
	// of at least three letters, like abc, bcd, cde, 
	// and so on, up to xyz. They cannot skip letters; abd doesn't count.
	bool has_straight = false;
	// Passwords must contain at least two different, 
	// `non-overlapping` pairs of letters, like aa, bb, or zz
	int pair_count = 0;
	assert(password.size() >= 8 && "Not allowed!");

	// TODO: Check for pairs??
	for (size_t k = 0; k < password.size() - 1;)
	{
		char first, second;

		first = password[k];
		second = password[k+1];
		if (ill(second) || ill(first)) return (false);
		if (first == second)
		{
			k+=2;
			pair_count++;
		}
		else
			k++;
	}
	for (size_t k = 0; k < password.size() - 2;++k)
	{
		char one, two, three;
		one = password[k];
		two = password[k+1];
		three = password[k+2];
		if (one + 1 == two && two + 1 == three)
		{
			has_straight = true;
			break ;
		}
	}
	return (pair_count == 2 && has_straight);
}

std::string GetNew(std::string Old)
{
	bool wrapped = true;
	std::string New = Old;

	std::reverse(New.begin(),
			  New.end());
	std::cout << "Inid with: " << New << " From: " << Old << std::endl;
	// getc(stdin);
	for (size_t i = 0; i < New.size(); i++)
	{
		if (wrapped)
		{
			if (New[i] == 'z') New[i] = 'a';
			else {
				New[i]++;
				wrapped = false;
			};
		}
	}
	std::reverse(New.begin(), New.end());
	return (New);
}

int main(int ac, char **av)
{
	if (ac < 2)
		return (1);
	{
		std::string New = GetNew(av[1]);
		while (!is_valid(New)) New = GetNew(New);
		std::cout << "New :: " << New << std::endl;
	}
	return (0);
}
