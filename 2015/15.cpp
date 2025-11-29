
/*
 *
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

int main(int ac, char **av)
{
	size_t n = 0;
	ssize_t nread = 1;
	char *Line = NULL;
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
		nread = getline(&Line, &n, fp);
		if (nread == -1) break;
		/*  Process the line???  */	
	}
	if (Line)
	{
		free(Line);
		Line = NULL;
	}
	fclose(fp);
	return (0);
}
