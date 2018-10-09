#ifndef parse_h
#define parse_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct command{
    int argc;
    char* argv[20];
};

struct commandline{
    int command_num;
    struct command * commands;
    char * dinput;
    char * doutput;
};


struct commandline* Parse(char * input_str);



#endif