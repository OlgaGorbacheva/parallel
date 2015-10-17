
#include <stdio.h> 
#include <time.h> 
#include <stdlib.h> 
#include <pthread.h>

#define PTHR_NUM 12

int n = 10000, m = 1000;
int **a;

void * max_for_thr(void *_num) {
	int num = *((int*) _num);
	int i = num;
	int i_max = num, j_max = 0;
	for (; i < n; i += PTHR_NUM) {
		for (int j = 0; j < m; j++) {
			if (a[i][j] > a[i_max][j_max] && !(i == num && j == 0)) {
				i_max = i;
				j_max = j;
			}
		}
	}
	pthread_exit((void*) &a[i_max][j_max]);
}

int main(int argc, char *argv[]) {
//	struct timespec begin, end;
	FILE *f = fopen("output.txt", "r");
	clock_t t;
	double elapsed;
	a = malloc (n * sizeof(int*));
	if (a == NULL) {
		printf("Error: bad allocation\n");
		return -1;
	}
	for (int i = 0; i < n; i++) {
		a[i] = malloc (m * sizeof(int));
		if (a[i] == NULL) {
                	printf("Error: bad allocation\n");
                	return -1;
        	}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
//			a[i][j] = rand() % (m *n + 1);
			fscanf(f, "%d", &a[i][j]);
		}
	}
	pthread_t pthr[PTHR_NUM];
	int max_el[PTHR_NUM];
	int num[PTHR_NUM];
	int max_i = 0;
	//clock_gettime(CLOCK_REALTIME, &begin);
	t = clock();
	for (int i = 0; i < PTHR_NUM; i++) {
		num[i] = i;
		int rc = pthread_create(&(pthr[i]), NULL, max_for_thr, (void*) (&num[i]));
		if (rc) {
			printf("Error: return code from pthread_create(): %d\n", rc);
			return rc;
		}
	}
	void *ret;
//	t = clock();
	for (int i = 0; i < PTHR_NUM; i++) {
		int rc = pthread_join(pthr[i], &ret);
		if (ret == NULL) {
			printf ("Error: ой-ой-ой");
			return -1;
		}
		max_el[i] = *((int *)ret);
		if (rc) {
                        printf("Error: return code from pthread_join(): %d\n", rc);
                        return rc;
                }
//		printf ("%d\n", max_el[i]);
		if (i != 0 && max_el[i] > max_el[max_i])
			max_i = i;
	}
//	clock_gettime(CLOCK_REALTIME, &end);
	t = clock() - t;
//	elapsed = end.tv_sec - begin.tv_sec;
	printf("%f\n", (float)t/CLOCKS_PER_SEC);
//	printf("max: %d\n\n\n", max_el[max_i]);
/*	for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                        printf("%d ", a[i][j]);
                }
		printf("\n");
        }*/
	for (int i = 0; i < n; i++) {
		free(a[i]);
	}
	free(a);
	return 0;
}
