#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Error: wrong arguments number, should be:\n");
		printf("[1] file name\n");
		printf("[2] pipe name\n");
		exit(1);
	}

	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	int fifo = mkfifo(argv[1], mode);
	if( fifo != 0)
	{
		printf("Error: can't create pipe\n");
		exit(1);
	}

	FILE* pipe = fopen(argv[1], "r");
	if(pipe == NULL)
	{
		printf("Error: can't open pipe\n");
		exit(1);
	}


	int pid = 0;
	char day_name[4];
	char month_name[4];
	int day = 0;
	char hour[10];
	int year = 0;
	

	while(!feof(pipe))
	{
		fscanf(pipe,"%d%s%s%d%s%d\n", &pid, day_name,month_name,&day,hour,&year);
		printf("%d ", pid);
		printf("%s ", day_name);
		printf("%s ", month_name);
		printf("%d ", day);
		printf("%s ", hour);
		printf("%d\n",year);
	}

	fclose(pipe);

}