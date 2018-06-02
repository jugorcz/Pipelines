#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Error: wrong arguments number, should be:\n");
		printf("[1] file name\n");
		printf("[2] pipe name\n");
		printf("[3] number\n");
		exit(1);
	}

	FILE* pipe = fopen(argv[1], "a");
	if(pipe == NULL)
	{
		printf("Error: can't open pipe\n");
		exit(1);
	}

	pid_t pid = getpid();
	printf("Process %d starting writtitg to %s.\n",(int)pid, argv[1]);

	srand(time(NULL));
	int N = atoi(argv[2]);
	for(int i = 0; i < N; i++)
	{
		int seconds = rand() % (10 + 1);
		sleep(seconds);
		time_t result = time(NULL);
		printf("%d %s\n", (int)pid, asctime(gmtime(&result)));
		fprintf(pipe, "%d %s", (int)pid, asctime(gmtime(&result)));
	}
}