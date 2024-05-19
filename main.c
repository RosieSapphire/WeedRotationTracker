#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

static int is_randomized;
static int member_cnt;
static int member_cur;
static int *member_indis;
static int cycle_cnt;

static void print_usage_and_exit(const char *argv0)
{
	fprintf(stderr, "Usage: %s <person1> <person2> <...>\n", argv0);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int compensation, i, j, opt;

	if (argc < 2)
		print_usage_and_exit(argv[0]);

	cycle_cnt = 0;
	is_randomized = 0;
	while ((opt = getopt(argc, argv, ":r")) != 1)
	{
		int no_flags;

		switch (opt)
		{
		case 'r':
			no_flags = 0;
			is_randomized = 1;
			printf("Flag 'R' is present\n");
			break;

		default:
			no_flags = 1;
			printf("Flag 'R' is NOT present\n");
			break;
		}

		if (no_flags)
			break;
	}

	compensation = is_randomized ? 2 : 1;
	member_cnt = argc - compensation;
	member_indis = malloc(sizeof(*member_indis) * member_cnt);
	if (is_randomized)
	{
		srand(time(NULL));
		for (i = 0; i < member_cnt; i++)
		{
retry:
			member_indis[i] = rand() % member_cnt;
			for (j = 0; j < i; j++)
			{
				if (member_indis[j] == member_indis[i])
					goto retry;
			}
		}
	}
	else
	{
		for (i = 0; i < member_cnt; i++)
			member_indis[i] = i;
	}

	printf("%d\n", member_cnt);
	if (optind >= argc)
	{
		fprintf(stderr, "You don't need that many fucking flags!\n");
		exit(EXIT_FAILURE);
	}

	member_cur = -1;
	do {
		if (++member_cur >= member_cnt)
		{
			member_cur = 0;
			cycle_cnt++;
		}
		printf("\x1b[2J\x1b[1;1H");
		printf("ROTATIONS: %d\n", cycle_cnt);
		for (i = 0; i < member_cnt; i++)
		{
			printf("\t%d. %s%s\n", i + 1,
			       argv[member_indis[i] + compensation],
			       (i == member_cur) ? " <<" : "");
		}
	} while (getchar());

	exit(EXIT_SUCCESS);
}
