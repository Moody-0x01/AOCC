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
#include <iterator>
#include <utility>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <limits.h>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>
#include <string.h>


typedef struct Condition {
	int q;
	std::string who;
} Condition;


std::vector<std::string> split(std::string s)
{
	std::vector<std::string> vec;

	for (size_t i = 0; i < s.size(); ++i)
	{
		std::string slice;
		while (i < s.size() && s[i] == ' ') slice.push_back(s[i++]);
		while (i < s.size() && s[i] != ' ') slice.push_back(s[i++]);
		if (slice.size()) vec.push_back(slice);
	}
	return (vec);
}
typedef std::map<std::pair<std::string, std::string>, int> ptable_t;
typedef std::vector<std::vector<std::string> > t_permutations;
t_permutations __permutations(std::vector<std::string> entries)
{
	t_permutations perms;
	t_permutations res;
	std::vector<std::string>cp_perm;
	std::string rest;

	if (entries.size() == 1)
		perms.push_back(entries);
	if (entries.size() <= 1)
		return (perms);
	rest = entries.front();
	entries.erase(entries.begin());
	perms = __permutations(entries);
	for (size_t perm = 0; perm < perms.size(); ++perm)
	{
		for (size_t e = 0; e <= perms[perm].size(); ++e)
		{
			cp_perm = perms[perm];
			cp_perm.insert(cp_perm.begin()+e, rest);
			res.push_back(cp_perm);
		}
	}
	return (res);
}

int get_happiness(std::vector<std::string> perm, ptable_t &hash)
{
	int h = 0;
	ptable_t::iterator it;
	std::string left, right, mid;
	left = perm[perm.size() - 1];
	mid = perm[0];
	right = perm[1];

	it = hash.find(std::make_pair(mid, left));
	if (it != hash.end())
		h += it->second;
	it = hash.find(std::make_pair(mid, right));
	if (it != hash.end())
		h += it->second;
	for (size_t i = 1; i < perm.size()-1; ++i)
	{
		left = perm[i - 1];
		mid = perm[i];
		right = perm[i + 1];
			
		it = hash.find(std::make_pair(mid, left));
		if (it != hash.end())
			h += it->second;
		it = hash.find(std::make_pair(mid, right));
		if (it != hash.end())
			h += it->second;
	}
	left = perm[0];
	mid = perm[perm.size() - 1];
	right = perm[perm.size() - 2];
	it = hash.find(std::make_pair(mid, left));
	if (it != hash.end())
		h += it->second;
	it = hash.find(std::make_pair(mid, right));
	if (it != hash.end())
		h += it->second;
	return (h);
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
	/*  std::vector<Person> people;  */
	ptable_t hash;
	std::set<std::string> people_set;
	while (1)
	{
		if (Line)
		{
			free(Line);
			Line = NULL;
		}
		nread = getline(&Line, &n, fp);
		if (nread == -1) break;
		/*  Process the line???  */	
		std::string s(Line);
		std::vector<std::string> fs = split(s);
		std::string name = fs[0];
		std::string state = fs[2];
		std::string effect = fs[3];
		std::string who = fs[10].erase(fs[10].size() - 2, 2);
		std::pair<std::string, std::string> p = std::make_pair(name, who);

		if (state == "gain")
			hash.insert({p, std::atoi(effect.c_str())});
		else if (state == "lose")
			hash.insert({p, -1 * std::atoi(effect.c_str())});
		people_set.insert(name);
	}
	if (Line)
	{
		free(Line);
		Line = NULL;
	}

	/*  for (auto &p: hash)  */
	/*  {  */
	/*  	std::cout << p.second << "\n";  */
	/*  }  */
	std::string first = std::prev(people_set.end())->c_str();
	people_set.erase(first);
	std::vector<std::string> entries(people_set.begin(), people_set.end());
	/* TODO: Generate all perms...  */
	t_permutations perms =__permutations(entries);
	int max = INT_MIN;
	/*  for (int rot = 0; rot < people_set.size(); )  */
	for (size_t i = 0; i < perms.size(); i++)
	{
		perms[i].push_back(first);
		int new_ = get_happiness(perms[i], hash);
		std::cout << "[";
			std::for_each(perms[i].begin(),  perms[i].end(), [](std::string &name){
				std::cout << name;
				std::cout << ", ";
			});
		std::cout << "] -> " << new_ << "\n";
		max = std::max(new_, max);
	}
	std::cout << "Happiness: " << max << "\n";
	fclose(fp);
	return (0);
}
