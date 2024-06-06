#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#include "prog.h"

int is_randomized;
int member_cnt;
int member_cur;
int *member_indis;
int cycle_cnt;

void print_usage_and_exit(const char *argv0)
{
	fprintf(stderr, "Usage: %s [-r] <person1> <person2> <...>\n", argv0);
	exit(EXIT_FAILURE);
}

int is_randomized_getopt(int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, ":r")) != 1)
	{
		switch (opt)
		{
		case 'r':
			if (optind != 2)
				print_usage_and_exit(*argv);
			return (1);

		default:
			return (0);
		}
	}

	return (0);
}

void member_indis_init(void)
{
	int i, j;

	member_indis = malloc(sizeof(*member_indis) * member_cnt);
	if (is_randomized)
	{
		srand(time(NULL));
		for (i = 0; i < member_cnt; i++)
		{
retry_rand:
			member_indis[i] = rand() % member_cnt;
			for (j = 0; j < i; j++)
			{
				if (member_indis[j] == member_indis[i])
					goto retry_rand;
			}
		}

		return;
	}

	for (i = 0; i < member_cnt; i++)
		member_indis[i] = i;
}

const char *last_hitter_determine(char **argv)
{
	if (member_cur)
		return (argv[1 + member_indis[member_cur - 1] + is_randomized]);

	if (!cycle_cnt)
		return ("FUCKING NOBODY! GOD, YOU GUYS ARE LAME!");

	return (argv[1 + member_indis[((member_cur - 1 < 0) ?
		member_cnt : member_cur) - 1] + is_randomized]);
}
