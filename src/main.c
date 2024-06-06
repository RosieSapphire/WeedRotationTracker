#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "prog.h"
#include "log.h"

static void main_loop(char **argv)
{
	char input;

	do {
		int i;

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
			       argv[member_indis[i] + 1 + is_randomized],
			       (i == member_cur) ? " <<" : "");
		}
		input = getchar();
	} while (input != 'e');
}

int main(int argc, char **argv)
{
	const char *last_hitter;
	int total_hits;

	if (argc < 2)
		print_usage_and_exit(argv[0]);

	cycle_cnt = 0;
	is_randomized = is_randomized_getopt(argc, argv);

	member_cnt = argc - 1 - is_randomized;
	member_indis_init();

	if (optind >= argc)
	{
		fprintf(stderr, "You don't need that many fucking flags!\n");
		exit(EXIT_FAILURE);
	}

	member_cur = -1;
	main_loop(argv);
	total_hits = member_cnt * cycle_cnt + member_cur;
	last_hitter = last_hitter_determine(argv);

	printf("\x1b[2J\x1b[1;1HYou all have smoked %d bowls\n", total_hits);
	printf("The last person to hit was %s\n", last_hitter);

	/* logging */
	log_write();
	exit(EXIT_SUCCESS);
}
