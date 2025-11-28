/*
 *
--- Day 14: Reindeer Olympics ---

This year is the Reindeer Olympics! Reindeer can fly at high speeds, but must rest occasionally to recover their energy. Santa would like to know which of his reindeer is fastest, and so he has them race.

Reindeer can only either be flying (always at their top speed) or resting (not moving at all), and always spend whole seconds in either state.

For example, suppose you have the following Reindeer:

    Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
    Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.

After one second, Comet has gone 14 km, while Dancer has gone 16 km. After ten seconds, Comet has gone 140 km, while Dancer has gone 160 km. On the eleventh second, Comet begins resting (staying at 140 km), and Dancer continues on for a total distance of 176 km. On the 12th second, both reindeer are resting. They continue to rest until the 138th second, when Comet flies for another ten seconds. On the 174th second, Dancer flies for another 11 seconds.

In this example, after the 1000th second, both reindeer are resting, and Comet is in the lead at 1120 km (poor Dancer has only gotten 1056 km by that point). So, in this situation, Comet would win (if the race ended at 1000 seconds).

Given the descriptions of each reindeer (in your puzzle input), after exactly 2503 seconds, what distance has the winning reindeer traveled?

*/
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cstddef>
#include <limits.h>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include <utility>
#include <map>
#include <vector>

#define SPEED  0
#define FLY_DURATION  1
#define REST_DURATION  2
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

typedef std::map<std::string, std::vector<int> > reindeer_t;
/*  static int points = INT_MIN;  */

long distance(int seconds, std::vector<int> data)
{
	/*  If someone is on the lead he has to get one point??  */
	int ds = 0;
	long now = 0;

	while (now < seconds)
	{
		/*  fly  */
		long dt = 0;
		while (dt + now < seconds && dt < data[FLY_DURATION])
		{
			ds+=data[SPEED];
			dt++;
		}
		now += dt;
		dt = 0;
		/*  rest  */
		while (dt < data[REST_DURATION]) 
		{
			dt++;
		}
		now += dt;
	}
	return (ds);
}

int main(int ac, char **av)
{
	size_t n = 0;
	ssize_t nread = 1;
	char *Line = NULL;
	reindeer_t reindeers;
	if (ac == 1)
		return 0;
	
	FILE *fp = fopen(av[1], "r");

	if (!fp) return (1);
	while (1)
	{
		if (Line)
		{
			free(Line);
			Line = NULL;
		}
		// Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
		nread = getline(&Line, &n, fp);
		if (nread == -1) break;
		/*  Process the line???  */	
		std::vector<int>  v;
		std::vector<std::string> line = split(Line);

		std::string name = line[0];
		int speed = std::atoi(line[3].c_str());
		int fly_duration = std::atoi(line[6].c_str());
		int rest_duration = std::atoi(line[13].c_str());
		v.push_back(speed);
		v.push_back(fly_duration);
		v.push_back(rest_duration);

		reindeers.insert({name, v});
	}
	if (Line)
	{
		free(Line);
		Line = NULL;
	}
	long max = INT_MIN;
	for (auto &key: reindeers)
	{
		long ds = distance(2503, key.second);
		max = std::max(ds, max);
		std::cout << key.first << " Has traveled " << ds << " after 1000 seconds\n";
	}
	std::cout << "Max : " << max << "\n";
	fclose(fp);
	return (0);
}
