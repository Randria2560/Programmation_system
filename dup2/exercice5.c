//simuler ls | wc -l
// sortie any ls : capture de atao entrée anile wc 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main()
{
    int tube[2];
    if(pipe(tube) == -1){ perror("pipe"); exit(1);}

    pid_t fils = fork();
    if(fils == -1) { perror("fork"); exit(1);}

    if(fils == 0)
    {
        close(tube[0]); //fermer in

        dup2(tube[1], STDOUT_FILENO);
        close(tube[1]);
        
        execlp("ls", "ls", NULL);
        perror("execlp ls");
        exit(1);
    
    }
    else
    {
        close(tube[1]); //le père n'écrit pas

        dup2(tube[0], STDIN_FILENO);
        close(tube[0]);

        wait(NULL); //on attend le fils
    
        execlp("wc","wc","-l", NULL);
        perror("execlp wc");
        exit(1);
    

    }
    return 0;
}