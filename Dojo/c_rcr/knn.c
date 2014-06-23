/* By rcr */
#ifndef NUM_THREADS
	#define NUM_THREADS 6
#endif

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUM_C 785  /* Columns per row */
#define NUM_V 500  /* Rows in the validation sample */
#define NUM_T 5000 /* Rows in the training sample */

#define die(en, msg) \
	do {errno = en; perror(msg); exit(EXIT_FAILURE);} while (0)

/* 8 bit pixels */
typedef unsigned char pixel;

pixel t_rows[NUM_T * NUM_C] = {0};
pixel v_rows[NUM_V * NUM_C] = {0};

struct thread_info
{
	pthread_t tid; /* Thread id */
	int from, to;  /* Start and end points to parse validation rows */
	int result;    /* Number of matches */
};

void
parse_csv(FILE *csv, pixel *parsed)
{
	char c;

	/* skip the header */
	while ((c = fgetc(csv)) != EOF && c != '\n');

	while ((c = fgetc(csv)) != EOF) {
		if (c == '\n' || c == ',')
			parsed++;
		else
			*parsed = *parsed * 10 + (c - '0');
	}
}

int
distance(pixel *v, pixel **train)
{
	int d, result = 0;
	pixel *end = v + NUM_C - 1, *t = *train;

	while (v < end) {
		d = *v++ - *t++;
		result += d * d;
	}

	*train = t;

	return result;
}

void*
match(void *arg)
{
	struct thread_info *tinfo = arg;

	int i, j, temp, dist, min_dist, v_label, t_label, match_count = 0;

	pixel *t_ptr, *v_ptr = &v_rows[tinfo->from * NUM_C];

	for (i = 0; i < (tinfo->to - tinfo->from); i++) {

		/* Validation row's label */
		v_label = *v_ptr++;
		t_label = 0;

		min_dist = INT_MAX;
		
		t_ptr = t_rows;

		for (j = 0; j < NUM_T; j++) {

			/* Training row's label */
			temp = *t_ptr++;

			if ((dist = distance(v_ptr, &t_ptr)) < min_dist) {
				min_dist = dist;
				t_label = temp;
			}
		}

		if (v_label == t_label)
			match_count++;

		v_ptr += NUM_C - 1;
	}

	tinfo->result = match_count;

	return &tinfo->result;
}

int
main(void)
{
	int i, s, total = 0;
	void *res;

	FILE *train = fopen("../trainingsample.csv", "r");
	FILE *valid = fopen("../validationsample.csv", "r");

	parse_csv(train, t_rows);
	parse_csv(valid, v_rows);

	struct thread_info threads[NUM_THREADS];

	struct timeval start, end;

	/* Start */
	gettimeofday(&start, NULL);

	/* Create the threads */
	for (i = 0; i < NUM_THREADS; i++) {

		/* Partition NUM_V into ideal ammounts per thread */
		threads[i].from = i * NUM_V / NUM_THREADS;
		threads[i].to = (i+1) * NUM_V / NUM_THREADS;

		if ((s = pthread_create(&threads[i].tid, NULL, &match, &threads[i])))
			die(s, "pthread_create");
	}

	/* Join the threads */
	for (i = 0; i < NUM_THREADS; i++) {

		if ((s = pthread_join(threads[i].tid, &res)))
			die(s, "pthread_join");

		total += *(int *)res;
	}

	/* End */
	gettimeofday(&end, NULL);

	long int str_ms = start.tv_sec * 1000 + start.tv_usec / 1000;
	long int end_ms = end.tv_sec * 1000 + end.tv_usec / 1000;
	printf("matched: %d/%d : %2.1f%%\n",
			total, NUM_V, ((float)total / (float)NUM_V) * 100);
	printf("time:    %fs\n", ((double)(end_ms - str_ms) / 1000));

	return 0;
}
