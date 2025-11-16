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
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <utility>

static bool contains(std::string haystack, std::string needle)
{
	for (size_t j = 0; j < needle.size();j++)
		if (haystack.find(needle[j]) != std::string::npos) return (true);
	return (false);
}

static bool contains_seq(std::string string)
{
	for (size_t k = 0; k < string.size(); ++k)
	{
		for (size_t p = k; p < string.size() - 2; ++p)
		{
			if (string[p]+1 == string[p+1] && string[p+1]+1 == string[p+2]) return (true);
		}
}
	return (false);
}

static bool contains_pairs(std::string string)
{
	int pairs;

	pairs = 0;
	for (size_t k = 0; k < string.size()-1;)
	{
		if (string[k] == string[k+1])
		{
			pairs++;
			k+=2;
		}
		else
			k++;
	}
	std::cout << pairs << std::endl;
	return (pairs >= 2);
}

void increment(std::string &New)
{
	bool valid = false;
	while (true)
	{
		for (size_t i = 0; i < New.size(); ++i)
		{
			New[i]++;
			if (New[i] == 'z')
			{
				New[i] = 'a';
				continue ;
			}
			valid = !contains(New, "iol");
			valid = valid && contains_seq(New);
			valid = valid && contains_pairs(New);
			if (valid) return ;
		}
		return ;
	}
}

std::string next(std::string previous)
{
	// TODO: Generate the next password
	//     RULE1: Passwords may not contain the letters i, o, or l
	//     DONE
	//     RULE2: should contain one stright seq like abc, bcd
	//     RULE3: should contain two pairs bb zz abcdffaa	
	std::string New;
	bool valid = false;

	New = previous;
	while (!valid)
	{
		std::cout << "Not Valid: " <<  New << "\n";
		increment(New);
		valid = !contains(New, "iol");
		valid = valid && contains_seq(New);
		valid = valid && contains_pairs(New);
	}
	return (New);
}

std::string _next(std::string previous)
{
	std::string New;

	New = previous;
}

int main()
{
	std::cout << "Valid: " << next("vzbxkghb") << "\n";
	// assert(contains_seq("abcd") && "Fail??");
	// assert(!contains("abcd", "iol") && "Fail??");
	// assert(contains_pairs("abcaddccxx") && "Fail??");
	return (0);
}
