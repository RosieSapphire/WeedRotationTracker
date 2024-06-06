#include "log.h"

void log_write(void)
#if LOGGING_ENABLED
{
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

	sprintf(fullbuf, "%s %s took %d bong rips, with %s hitting last.\n",
		timebuf, namebuf, total_hits, last_hitter);
	fwrite(fullbuf, 1, strlen(fullbuf), logfile);
	fclose(logfile);
}
#else
{}
#endif
