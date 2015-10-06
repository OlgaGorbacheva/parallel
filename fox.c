#include <stdio.h> 
#include <time.h> 
#include <stdlib.h> 
#include <pthread.h>

#define PTHR_NUM 4

int n = 5, m = 5;
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
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			a[i][j] = rand() % (n*m);
		}
	}
	pthread_t pthr[PTHR_NUM];
	int max_el[PTHR_NUM];
	int num[PTHR_NUM];
	int max_i = 0;
	for (int i = 0; i < PTHR_NUM; i++) {
		num[i] = i;
		int rc = pthread_create(&(pthr[i]), NULL, max_for_thr, (void*) (&num[i]));
		if (rc) {
			printf("Error: return code from pthread_create(): %d\n", rc);
			return rc;
		}
	}
	void *ret;
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
		printf ("%d\n", max_el[i]);
		if (i != 0 && max_el[i] > max_el[max_i])
			max_i = i; 
	}
	printf("max: %d\n\n\n", max_el[max_i]);
	for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                        printf("%d ", a[i][j]);
                }
		printf("\n");
        }
	for (int i = 0; i < n; i++) {
		free(a[i]);
	}
	free(a);
	return 0;
}
