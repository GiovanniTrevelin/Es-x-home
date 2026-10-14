#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

struct Messaggio {
    char testo[100];
    int vocali;
};

int main() {
    int pipe1[2], pipe2[2];
    char testo[100];
    struct Messaggio msg;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Errore");
        return 1;
    }

    printf("Inserisci una frase: ");
    scanf("%s", testo);


    int pid = fork();

    if (pid == -1) {
        perror("Errore ");
        return 1;
    }

    if (pid == 0) {

        read(pipe1[0], testo, sizeof(testo));

        msg.vocali = 0;

        for (int i = 0; testo[i] != '\0'; i++) {
            testo[i] = toupper((unsigned char)testo[i]);

            if (testo[i] == 'A' || testo[i] == 'E' ||
                testo[i] == 'I' || testo[i] == 'O' ||
                testo[i] == 'U') {
                msg.vocali++;
                }
        }

        strcpy(msg.testo, testo);


        write(pipe2[1], &msg, sizeof(msg));

        exit(0);

    } else {

        write(pipe1[1], testo, strlen(testo) + 1);


        read(pipe2[0], &msg, sizeof(msg));

        wait(NULL);

        printf("Frase maiuscola: %s", msg.testo);
        printf("Numero di vocali: %d", msg.vocali);
    }
    return 0;
}