#include "parse.h"

struct commandline* Parse(char * input_str){
    int command_num = 1; //increase by 1 when meet |
    char * cursor = input_str;
    while(*cursor != 0){
        if(*cursor == '|')
            command_num++;
        cursor++;
    }
    struct commandline* cl = (struct commandline*)malloc(sizeof(struct commandline));
    cl->command_num = command_num;
    cl->dinput = NULL;
    cl->doutput = NULL;
    cl->commands = (struct command*)malloc(command_num*sizeof(struct command));
    for(int i = 0; i < command_num; i++){
        (cl->commands+i)->argc = 0; //init as 0
    }
    cursor = input_str;
    struct command* tempc = cl->commands;
    char tempchar[100];
    memset(tempchar,0,100);//initialize the tempchar
    int ccounter  = 0;
    while(1){
        if(*cursor == ' '){//when meets a space
            if(tempchar[0] == 0){//if there is space before
                cursor++;
                continue;
            }
            //store the previous string as arg
            tempc->argv[tempc->argc] = (char*)malloc(sizeof(ccounter+1));
            strcpy(tempc->argv[tempc->argc],tempchar);
            tempc->argc++;
            memset(tempchar,0,100);
            ccounter = 0;
            cursor++;
            continue;
        }
        else if(*cursor == '<'){
            cursor++;
            //get next string to be input
            while(*cursor == ' ')
                cursor++; //skip all the blanks after <
            while(1){
                if(*cursor == ' '|| *cursor == '|'||*cursor == '\n'){
                    cl->dinput = (char*)malloc(sizeof(ccounter+1));
                    strcpy(cl->dinput,tempchar);
                    memset(tempchar,0,100);
                    ccounter = 0;
                    if(*cursor == '|') tempc++;
                    cursor++;
                    break;
                }
                tempchar[ccounter] = *cursor;
                ccounter++;
                cursor++;
                }
            continue;
        }
        else if(*cursor == '>'){
            //get next string to be output
            cursor++;
            //get next string to be input
            while(*cursor == ' ')
                cursor++; //skip all the blanks after <
            while(1){
                if(*cursor == ' ' || *cursor == '|' ||*cursor == '\n'){
                    cl->doutput = (char*)malloc(sizeof(ccounter+1));
                    strcpy(cl->doutput,tempchar);
                    memset(tempchar,0,100);
                    ccounter = 0;
                    if(*cursor == '|') tempc++;
                    cursor++;
                    break;
                }
                tempchar[ccounter] = *cursor;
                ccounter++;
                cursor++;
            }
            continue;
        }
        else if(*cursor == '|'){
            if(tempchar[0] == 0){
                tempc++;
                cursor++;
                continue;
            }
            //store the previous string as arg
            tempc->argv[tempc->argc] = (char*)malloc(sizeof(ccounter+1));
            strcpy(tempc->argv[tempc->argc],tempchar);
            tempc->argc++;
            memset(tempchar,0,100);
            ccounter = 0;
            tempc++;//store args for next command
            cursor++;
            continue;
        }
        else if(*cursor == '\n' || *cursor == 0){//meets the end of input
            if(tempchar[0] == 0){
                break;
            }
            //store the previous string as arg
            tempc->argv[tempc->argc] = (char*)malloc(sizeof(ccounter+1));
            strcpy(tempc->argv[tempc->argc],tempchar);
            tempc->argc++;
            break;
        }
        tempchar[ccounter] = *cursor;
        ccounter++;
        cursor++;//to next char
    }
    return cl;
}

