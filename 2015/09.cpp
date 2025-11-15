/*
--- Day 9: All in a Single Night ---

Every year, Santa manages to deliver all of his presents in a single night.

This year, however, he has some new locations to visit; his elves have provided him the distances between every pair of locations. He can start and end at any two (different) locations he wants, but he must visit each location exactly once. What is the shortest distance he can travel to achieve this?

For example, given the following distances:

London to Dublin = 464
London to Belfast = 518
Dublin to Belfast = 141

The possible routes are therefore:

Dublin -> London -> Belfast = 982
London -> Dublin -> Belfast = 605
London -> Belfast -> Dublin = 659
Dublin -> Belfast -> London = 659
Belfast -> Dublin -> London = 605
Belfast -> London -> Dublin = 982

The shortest of these is London -> Dublin -> Belfast = 605, and so the answer is 605 in this example.

What is the distance of the shortest route?
*/
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <vector>

typedef struct Entry
{
	char *from;
	char *to;
	int distance;
} Entry;

typedef struct Route {
	char *first;
	char *second;
	char *last;
	int distance;
} Route;

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "An input file was not given.\n");
		return (1);
	}
	FILE *fp = fopen(argv[1], "r");

	if (!fp) {
		fprintf(stderr, "Could not open `%s` for reading.\n", argv[1]);
		return (1);
	}

	ssize_t nread = 1;
	size_t n = 0;
	char *Line = NULL;

	std::vector<Entry> entries;
	std::vector<Route> routes;
	std::set<std::string> cities;
	while (1) {
		{
			if (Line) { free(Line); Line = NULL;}
			nread = getline(&Line, &n, fp);
			if (nread <= 0) break;
			if (nread-1 == 0) continue;
			Line[nread-1] = 0;
		}
		Entry e;
		{
			e.from = strdup(strtok(Line, " \t"));
			strtok(NULL, " \t");
			e.to = strdup(strtok(NULL, " \t"));
			strtok(NULL, " \t");
			e.distance = atoi(strtok(NULL, " \t"));
			cities.insert(e.to);
			cities.insert(e.from);
		}
		entries.push_back(e);
	}
	if (Line) free(Line);
	for (std::set<std::string>::iterator c = cities.begin(); c != cities.end(); c++)
	{
		std::cout << c->data() << "\n";
	}
	for (std::vector<Entry>::iterator it = entries.begin(); it != entries.end(); it++)
	  std::cout << it->from << " ===(" << it->distance << ")===> " << it->to << std::endl;
	fclose(fp);
	return (0);
}
