#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int numeri[10] = {1, 67, 3, 67, 5, 67, 7, 67, 9, 67};
    int fd[2];
    int sommaPari = 0;
    int sommaDispari = 0;
    int risultato;

    if (pipe(fd) == -1) {
        perror("Errore");
        return 1;
    }
    int pid = fork();

    if (pid == 0) {
        for (int i = 0; i < 10; i++) {
            if (numeri[i] % 2 == 0) {
                sommaPari += numeri[i];
            }
        }

        write(fd[1], &sommaPari, sizeof(sommaPari));
        exit(0);
    } else {
        for (int i = 0; i < 10; i++) {
            if (numeri[i] % 2 != 0) {
                sommaDispari += numeri[i];
            }
        }
        read(fd[0], &risultato, sizeof(risultato));

        wait(NULL);
        printf("Somma pari: %d", risultato);
        printf("Somma dispari: %d", sommaDispari);
        printf("Somma totale: %d", risultato + sommaDispari);
    }

    return 0;
}