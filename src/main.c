#include <stdio.h>
#include "parse.h"

void executeCommand(struct commandline *cl){
	fprintf(stderr,"test0\n");
	int pipe_num = cl->command_num -1;
	int * pids = (int*)malloc(sizeof(int)*cl->command_num);
	fprintf(stderr,"test\n");
	int ** fds = (int**)malloc(pipe_num*sizeof(int *));
	printf("test1\n");
	for(int i = 0; i<pipe_num; i++){
		*(fds+i) = (int*)malloc(sizeof(int)*2);
		pipe(*(fds+i));
	}
	fprintf(stderr,"test2\n");
	//handle the situation of one command
	if(cl->command_num == 1){
		
	}
	//if there is input redirection
	// if(cl->dinput != NULL){
	// 	FILE* inputf = fopen(cl->dinput,"r");
	// 	dup2(fileno(inputf),fileno(stdin));
	// 	fclose(inputf);
	// }
	// if(cl->doutput != NULL){
	// 	FILE* outputf = fopen(cl->doutput,"w");
	// 	dup2(fileno(outputf),fileno(stdout));
	// 	fclose(outputf);
	// }
	for(int i = 0; i< cl->command_num;i++){
		//i indicate which command it is
		int pid = fork();
		if(pid == 0){
			fprintf(stderr,"procedure id: %d \n",pid);
			//execute the subprocess
			if(i == cl->command_num -1){//last command
				//if there is output redirction
				if(cl->doutput != NULL){
					FILE* outputf = fopen(cl->doutput,"w");
					dup2(fileno(outputf),fileno(stdout));
					fclose(outputf);
				}
				int* in_fd = *(fds+i-1);
				dup2(fileno(*(in_fd)),fileno(stdin));//redirect pipeline to input
			}
			else if(i == 0){//the first command
				//if there is input redirction
				if(cl->dinput != NULL){
					FILE* inputf = fopen(cl->dinput,"r");
					dup2(fileno(inputf),fileno(stdin));
					fclose(inputf);
				}
				int * out_fd = *fds;
				fprintf(stderr,"test input1");
				dup2(fileno(*(out_fd+1)),fileno(stdout));
				fprintf(stderr,"test input2");
			}
			else{
				int* in_fd = *(fds+i-1);
				int* out_fd = *(fds+i);
				dup2(fileno(*(in_fd)),fileno(stdin));//redirect pipeline to input
				dup2(fileno(*(out_fd+1)),fileno(stdout));
			}
			fprintf(stderr,"finish redirection");
			//close all the pipeline
			for(int j = 0; j< pipe_num; j++){
				close(**(fds+j));
				close(*(*(fds+j)+1));
			}
			char * command_p = (char*)malloc(sizeof(char)*100);
			strcpy(command_p,"/usr/bin/");
			strcat(command_p,(cl->commands+i)->argv[0]);
			fprintf(stderr,"%s",command_p);
			//execl();
			exit(0);
		}
		*(pids+i) = pid;
		fprintf(stderr,"procedure id: %d\n",*(pids+i));
	}
	//close all the pipeline
	for(int j = 0; j< pipe_num; j++){
		close(**(fds+j));
		close(*(*(fds+j)+1));
	}
	wait(pids+cl->command_num-1);
	return;

}

int main(){
	char * inputstr;
	struct commandline* cl;
	while(1){
		printf("?:");
		if(fgets(inputstr,1024,stdin) == NULL)
			break;//handles the ctrl+d
		cl = Parse(inputstr);
		printf("%d: ",cl->command_num);
		if(cl->dinput != NULL)
			printf("<'%s'",cl->dinput);
		for(int i = 0; i< cl->command_num; i++){
			struct command * tc = cl->commands+i;
			for(int j = 0; j< tc->argc; j++){
				printf("'%s'",tc->argv[j]);
			}
			if(i == cl->command_num-1){
				break;
			}
			printf("|");
		}	
		if(cl->doutput != NULL)
			printf(">'%s'",cl->doutput);
		printf("\n");
		fprintf(stderr,"test-1");
		executeCommand(cl);
		free(cl);
	}


	return 0;
}
