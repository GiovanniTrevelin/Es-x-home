#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Errore");
        return 1;
    }

    int pidB = fork();

    if (pidB == -1) {
        perror("Errore");
        return 1;
    }

    if (pidB == 0) {
        int n, quadrato;
        while (read(pipe1[0], &n, sizeof(n)) > 0) {
            if (n >= 50) {
                quadrato = n * n;
                write(pipe2[1], &quadrato, sizeof(quadrato));
            }
        }
        exit(0);
    }

    int pidC = fork();
    if (pidC == -1) {
        perror("Errore");
        return 1;
    }

    if (pidC == 0) {
        int n, conteggio = 0;
        int somma = 0;

        while (read(pipe2[0], &n, sizeof(n)) > 0) {
            printf("Quadrato ricevuto: %d", n);
            somma += n;
            conteggio++;
        }

        if (conteggio > 0)
            printf("Media: %d", (double)somma / conteggio);
        else
            printf("Nessun quadrato ricevuto");


        exit(0);
    }
     srand(time(NULL));

    for (int i = 0; i < 20; i++) {
        int n = rand() % 100 + 1;
        printf("Numero generato: %d", n);
        write(pipe1[1], &n, sizeof(n));
    }
    wait(NULL);
    wait(NULL);

    return 0;
}
