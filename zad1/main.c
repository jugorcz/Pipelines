#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARGS_NUMBER 20
int instructionsNumber;

typedef struct node_t
{
	struct node_t* next;
	char* args[ARGS_NUMBER];
	int pipefd[2];
} node_t;

void cleanTable(char* table[], int elementsNumber)
{
	int i;
	for(i = 0; i < elementsNumber; i++)
        table[i] = NULL;
}


node_t* getInstructions(char* line)
{	
	node_t* head = NULL;
    head = malloc(sizeof(node_t));
    head -> next = NULL;
    cleanTable(head -> args,ARGS_NUMBER);

    char* separatedData = strtok(line,"\t \n");
    if(separatedData == NULL) return NULL;
    instructionsNumber++;

    node_t* node = head;
    int i = 0;	
    while(separatedData != NULL && i < ARGS_NUMBER - 1)
    {
    	if(strcmp(separatedData,"|") != 0)
    	{
    		node -> args[i] = separatedData;
    		i++;    		
    	}
    	else if(node -> args[0] == NULL) //jeśli wystąpił znak "|" ale nic przed nim
    	{
    		i = 0;
    	}
    	else // wystąpił znak "|"
    	{
    		instructionsNumber++;
    		node_t* next_node = malloc(sizeof(node_t));
    		next_node -> next = NULL;
    		cleanTable(next_node -> args, ARGS_NUMBER);
    		i = 0;
    		node -> next = next_node;
    		node = node -> next;
    	}

    	separatedData = strtok(NULL,"\t \n");   	
    }

    return head;
    	
}


void displayNode(node_t* head)
{
	node_t* node = head;
	int i = 0;
	while(node)
	{
		printf("\nNode %d: ",i);
		for(int j = 0 ; j < ARGS_NUMBER; j++)
		{
			if(node -> args[j] == NULL) break;
			printf("%s ",node -> args[j]);
		}
		i++;
		node = node -> next;
	}
	printf("\n");
}

void deleteListNode(node_t* head)
{
	node_t* node;
	while(head)
	{
		node = head;
		head = head -> next;
		node -> next = NULL;
		free(node);
	}
	
}

int main(int argc, char* argv[])
{

	if(argc < 2)
	{
		printf("You need to enter name of file with commands!\n");
		return 0;
	}
	
	FILE* file  = fopen(argv[1],"r");
    if(file == NULL)
        return 0;

    int status;
    size_t bufferSize = 80;
    char *buffer = malloc(bufferSize * sizeof(char));
	int lineNumber = 0;
	while(-1 != getline(&buffer, &bufferSize, file))
	{
		instructionsNumber = 0;
		printf("\n%d: %s", ++lineNumber, buffer);
		node_t* head = getInstructions(buffer);
		if(head == NULL) continue;
		displayNode(head);


		pid_t pid;
		pid = fork();
		if(pid == 0) //first child 
		{
			pipe(head -> pipefd); 
			/*
			 * pipefd[0] read end of first instruction
			 * pipefd[1] write end of first instruction
			 */
			dup2(head -> pipefd[1], STDOUT_FILENO);
			execvp(head -> args[0], head -> args);
		}
		else //parent
		{	
			node_t* current_node;
			current_node = head -> next;
			while(current_node -> next)  // ... -> head -> current node
			{
				pid = fork();
				if(pid == 0) //child
				{
					dup2(head -> pipefd[0], STDIN_FILENO); //program będzie czytał z poprzedniego potoku
					pipe(current_node -> pipefd);
					dup2(current_node -> pipefd[1], STDOUT_FILENO); //program będzie zapisywał do obecnego potoku
					execvp(current_node -> args[0], current_node -> args);
				}
				else
				{
					head = head -> next;
					current_node = head -> next;
				}
			}
			// ... -> head -> current node -> NULL
			pid = fork();
			if(pid == 0) //last child
			{
				dup2(head -> pipefd[0], STDIN_FILENO); //czytaj z poprzedniego 
				execvp(current_node -> args[0], current_node -> args);
			}
			else //parent
			{
				for(int i = 0; i < instructionsNumber; i++)
					wait(&status);
			}
		}

		deleteListNode(head);
	}

}