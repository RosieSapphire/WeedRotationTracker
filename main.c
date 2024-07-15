#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>

#define CONSOLE_CLEAR() puts("\x1b[2J\x1b[H")

static int member_cnt, member_cur, *member_indis, cycle_cnt, is_randomized;

static void print_usage_and_exit(const char *argv0);
static int is_randomized_getopt(const int argc, char *const *argv);
static void member_indis_init(void);
static const char *last_hitter_determine(char *const *argv);
static void log_write(char *const *argv, const int total_hits,
		      const char *last_hitter);

int main(const int argc, char *const *argv)
{
	const char *last_hitter;
	int total_hits;
	char c;

	cycle_cnt = 0;
	is_randomized = is_randomized_getopt(argc, argv);
	member_cnt = argc - 1 - is_randomized;
	member_indis_init();
	member_cur = -1;
	do {
		int i;

		if (++member_cur >= member_cnt) {
			member_cur = 0;
			cycle_cnt++;
		}
		CONSOLE_CLEAR();
		printf("ROTATIONS: %d\n", cycle_cnt);
		for (i = 0; i < member_cnt; i++) {
			printf("\t%d. %s%s\n", i + 1,
			       argv[member_indis[i] + 1 + is_randomized],
			       (i == member_cur) ? " <<" : "");
		}
	} while ((c = getchar()) != 'e');

	total_hits = member_cnt * cycle_cnt + member_cur;
	last_hitter = last_hitter_determine(argv);
	CONSOLE_CLEAR();
	printf("\nYou all have smoked %d bowls\n", total_hits);
	printf("The last person to hit was %s\n", last_hitter);
	log_write(argv, total_hits, last_hitter);

	return 0;
}

static void print_usage_and_exit(const char *argv0)
{
	fprintf(stderr, "Usage: %s [-r] <person1> <person2> <...>\n", argv0);
	exit(EXIT_FAILURE);
}

static int is_randomized_getopt(const int argc, char *const *argv)
{
	int opt;

	while ((opt = getopt(argc, argv, ":r")) != 1) {
		switch (opt) {
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
	if (is_randomized) {
		srand(time(NULL));
		for (i = 0; i < member_cnt; i++) {
retry_rand:
			member_indis[i] = rand() % member_cnt;
			for (j = 0; j < i; j++) {
				if (member_indis[j] == member_indis[i])
					goto retry_rand;
			}
		}

		return;
	}

	for (i = 0; i < member_cnt; i++)
		member_indis[i] = i;
}

static const char *last_hitter_determine(char *const *argv)
{
	if (member_cur)
		return (argv[1 + member_indis[member_cur - 1] + is_randomized]);

	if (!cycle_cnt)
		return ("FUCKING NOBODY! GOD, YOU GUYS ARE LAME!");

	return (argv[1 +
		     member_indis[((member_cur - 1 < 0) ? member_cnt :
							  member_cur) -
				  1] +
		     is_randomized]);
}

static void log_write(char *const *argv, const int total_hits,
		      const char *last_hitter)
{
	FILE *logfile;
	char namebuf[1024] = { 0 };
	char fullbuf[2048] = { 0 };
	int i;

	logfile = fopen("log.txt", "a");
	for (i = 0; i < member_cnt; i++) {
		if (i > (member_cnt - 3)) {
			sprintf(namebuf + strlen(namebuf),
				(i == member_cnt - 2) ? "%s and " : "%s",
				argv[member_indis[i] + 1 + is_randomized]);
			continue;
		}
		sprintf(namebuf + strlen(namebuf), "%s, ",
			argv[member_indis[i] + 1]);
	}
	sprintf(fullbuf, "%s took %d bong rips, with %s hitting last.\n",
		namebuf, total_hits, last_hitter);
	fwrite(fullbuf, 1, strlen(fullbuf), logfile);
	fclose(logfile);
}
