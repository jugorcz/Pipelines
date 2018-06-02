#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc < 3) 
	{
		printf("\nWrong arguments\n");
		char str[100];
		scanf("Sth: %s",str);
		printf("%s\n",str);
		return 0;
	}

	
	for(int i = 0; i < argc; i++)
		printf("%s\n",argv[i]);

	return 0;
}
