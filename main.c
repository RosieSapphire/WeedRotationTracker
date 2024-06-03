#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

#define LOGGING_ENABLED 0

static int is_randomized;
static int member_cnt;
static int member_cur;
static int *member_indis;
static int cycle_cnt;

static void print_usage_and_exit(const char *argv0)
{
	fprintf(stderr, "Usage: %s [-r] <person1> <person2> <...>\n", argv0);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int i, j, opt, total_hits, noflags;
	const char *last_hitter;

	if (argc < 2)
		print_usage_and_exit(argv[0]);

	cycle_cnt = 0;
	is_randomized = 0;
	while ((opt = getopt(argc, argv, ":r")) != 1)
	{
		switch (opt)
		{
		case 'r':
			noflags = 0;
			is_randomized = 1;
			if (optind != 2)
				print_usage_and_exit(*argv);
			break;

		default:
			noflags = 1;
			break;
		}

		if (noflags)
			break;
	}

	member_cnt = argc - 1 - is_randomized;
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
	}
	else
	{
		for (i = 0; i < member_cnt; i++)
			member_indis[i] = i;
	}

	if (optind >= argc)
	{
		fprintf(stderr, "You don't need that many fucking flags!\n");
		exit(EXIT_FAILURE);
	}

	member_cur = -1;

	char input;

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
			       argv[member_indis[i] + 1 + is_randomized],
			       (i == member_cur) ? " <<" : "");
		}
		input = getchar();
	} while (input != 'e');

	total_hits = member_cnt * cycle_cnt + member_cur;
	if (member_cur)
	{
		last_hitter =
			argv[1 + member_indis[member_cur - 1] + is_randomized];
	}
	else
	{
		if (!cycle_cnt)
		{
			last_hitter = "FUCKING NOBODY! GOD, YOU GUYS ARE LAME!";
			exit(EXIT_SUCCESS);
		}

		last_hitter =
			argv[1 + member_indis[((member_cur - 1 < 0) ?
			     member_cnt : member_cur) - 1] + is_randomized];
	}

	printf("\x1b[2J\x1b[1;1HYou all have smoked %d bowls\n", total_hits);
	printf("The last person to hit was %s\n", last_hitter);

	/* logging */
#if LOGGING_ENABLED 
	time_t t = time(NULL);
	struct tm lt;
	FILE *logfile;
	char timebuf[64] = {0};
	char namebuf[1024] = {0};
	char fullbuf[2048] = {0};

	logfile = fopen("log", "a");
	localtime_r(&t, &lt);
	strftime(timebuf, 1024, "On %A, %B %d, %Y @ %r,", &lt);
	for (int i = 0; i < member_cnt; i++)
	{
		if (i > (member_cnt - 3))
		{
			sprintf(namebuf + strlen(namebuf),
				(i == member_cnt - 2) ? "%s and " : "%s",
				argv[member_indis[i] + 1 + is_randomized]);
			continue;
		}

		sprintf(namebuf + strlen(namebuf), "%s, ",
			argv[member_indis[i] + 1 + is_randomized]);
	}

	sprintf(fullbuf, "%s %s took %d bong rips, "
		"with %s hitting last.\n",
		timebuf, namebuf, total_hits, last_hitter);
	fwrite(fullbuf, 1, strlen(fullbuf), logfile);
	fclose(logfile);
#endif
	exit(EXIT_SUCCESS);
}
