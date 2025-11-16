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
#include <limits.h>
#include <cinttypes>
#include <cmath>
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
#include <utility>
#include <vector>

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
typedef struct Entry
{
	char *from;
	char *to;
	int distance;
} Entry;

class Graph {
	typedef std::map<std::string, std::vector<std::pair<std::string, int> > >::iterator GraphIterator;
	typedef std::set<std::string>::iterator Viter;
	std::map<std::string, std::vector<std::pair<std::string, int> > > adjList;
	std::set<std::string> vertices;
	t_permutations perms;
	public:
		void permutations()
		{
			this->perms = __permutations(this->getVertices());
		}
		int getSmallestRoute()
		{
			int distance = INT_MIN;
			
			for (size_t perm = 0; perm < this->perms.size(); perm++)
			{
				int _dis = 0;
				for (size_t e = 0; e < this->perms[perm].size()-1; ++e)
					_dis += this->getDistance(this->perms[perm][e], this->perms[perm][e+1]);
				if (_dis > distance) distance = _dis;
			}
			return (distance);
		}
		void addEdge(char *from, char *to, int distance) {
			vertices.insert(from);
			vertices.insert(to);
			adjList[from].push_back(std::make_pair(to, distance));
			adjList[to].push_back(std::make_pair(from, distance));
		}
		void log() {
			std::cout << "VERTICES\n";
			for (Viter it = this->vertices.begin(); it != this->vertices.end(); ++it) std::cout << *it << "\n";
			std::cout << "Graph\n";
			for (GraphIterator it = this->adjList.begin(); it != this->adjList.end(); ++it)
			{
				std::cout << it->first << std::endl;
				for (size_t i = 0; i < it->second.size(); ++i)
					std::cout << "    =[ " << it->second[i].second << " ]> " << it->second[i].first << "\n";
			}
		}
		int getDistance(std::string from, std::string to)
		{
			GraphIterator found = this->adjList.find(from);
			if (found != this->adjList.end())
			{
				for (size_t i = 0; i < found->second.size(); ++i)
				{
					if (found->second[i].first == to)
						return (found->second[i].second);
				}
			}
			return (INT_MAX);
		}
		std::vector<std::string> getVertices(void) {
			std::vector<std::string>vertices_;
			for (Viter it = this->vertices.begin(); it != this->vertices.end(); ++it) vertices_.push_back(*it);
			return (vertices_);
		}

};

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
	// std::vector<Route> routes;
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

	Graph graph;
	for (std::set<std::string>::iterator c = cities.begin(); c != cities.end(); c++)
	{
		std::cout << c->data() << "\n";
	}
	for (std::vector<Entry>::iterator it = entries.begin(); it != entries.end(); it++)
	{
		std::cout << it->from << " ===(" << it->distance << ")===> " << it->to << std::endl;
		graph.addEdge(it->from,
			it->to,
			it->distance
		);
	}
		
	// graph.log();
	// minDistance = graph.resolve();
	graph.permutations();
	int distance = graph.getSmallestRoute();
	std::cout << "distance " << distance;
	// graph.
	// std::vector<std::string> strings = graph.getVertices();
	// for (size_t i = 0; i < strings.size(); i++)
	// 	std::cout << i << ": " << strings[i] << std::endl;
	// t_permutations pems = permutations(strings);
	// std::cout << "size: "<< pems.size() << std::endl;
	//
	// for (size_t x = 0; x < pems.size(); ++x)
	// {
	// 	std::cout << "[ ";
	// 	for (size_t i = 0; i < pems[x].size(); i++) {
	// 		std::cout << pems[x][i];
	// 		if (i != pems[x].size() - 1)
	// 			std::cout << " ";
	// 	}
	// 	std::cout << " ]\n";
	// }
	// TODO: Calculate the value!!

	fclose(fp);
	return (0);
}
