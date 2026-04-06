#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//pipe entre père et fils

int main() {
    int tube[2];
    if (pipe(tube) == -1) { perror("pipe"); exit(1); }

    pid_t fils = fork();
    if (fils == -1) { perror("fork"); exit(1); }

    if (fils == 0) {  // ── FILS ──────────────────────────
        close(tube[1]);  // fils n'écrit pas

        // Connecter tube[0] à stdin
        dup2(tube[0], STDIN_FILENO);
        close(tube[0]);

        // scanf lit maintenant depuis le pipe
        char buf[256];
        scanf("%[^\n]", buf);
        printf("Fils reçoit : %s\n", buf);

    } else {           // ── PÈRE ──────────────────────────
        close(tube[0]);  // père ne lit pas

        // Connecter tube[1] à stdout
        dup2(tube[1], STDOUT_FILENO);
        close(tube[1]);

        // printf écrit maintenant dans le pipe
        printf("Message du père");
        fflush(stdout);

        // Fermer stdout pour signaler EOF au fils
        close(STDOUT_FILENO);

        wait(NULL);
    }

    return 0;
}