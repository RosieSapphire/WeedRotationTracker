#ifndef _PROG_H_
#define _PROG_H_

extern int is_randomized;
extern int member_cnt;
extern int member_cur;
extern int *member_indis;
extern int cycle_cnt;

void print_usage_and_exit(const char *argv0);
int is_randomized_getopt(int argc, char **argv);
void member_indis_init(void);
const char *last_hitter_determine(char **argv);

#endif /* _PROG_H_ */
