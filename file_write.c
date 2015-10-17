#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int n = 10000, m = 1000;
int a;

int main(){
	FILE *output;
	char * filename = "output.txt";
	int count;

	output = fopen(filename, "w");
	if (output == NULL){
		printf("Can't open file '%s'/n", filename);
		return -1;
	}
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			a = rand() % (n*m);
			fprintf(output, "%d ", a);
		}
		fprintf(output, "\n");
	}
	fclose(output);

	return 0;
}
