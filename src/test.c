#include <stdio.h>
#include <unistd.h>

int main(){
int fd[2];
FILE *fp = fopen( “file1”, “r” );
dup2( fileno(fp), fileno(stdin) ); fclose( fp );
pipe( fd ); // populates both fd[0] and fd[1] if( fork() != 0 ) { // Parent
dup2( fd[1], fileno(stdout) );
close( fd[0] ); close( fd[1] ); // DON’T FORGET THIS! execl( “/usr/bin/sort”, “sort”, (char *) 0 ); exit( 2 );
} else { // child
dup2( fd[0], fileno(stdin) );
close( fd[0] ); close( fd[1] );
execl( “/usr/bin/uniq”, “uniq”, (char *) 0 ); exit( 3 );
return 0;
}
