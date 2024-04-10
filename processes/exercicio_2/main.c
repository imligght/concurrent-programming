// Dupla: Luan da Silva Moraes e Caio César Rodrigues de Aquino

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

int main(int argc, char** argv) {

    // ....

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    pid_t pid;

    for (int i = 0; i < 2; i++) {
        if ((pid = fork()) == 0) {
            fflush(stdout);
            printf("Processo %d, filho de %d\n", getpid(), getppid());
            for (int j = 0; j < 3; j++) {
                if ((pid = fork()) == 0) {
                    fflush(stdout);
                    printf("Processo %d, filho de %d\n", getpid(), getppid());
                    sleep(5);
                    return 0;
                }
            }
            while (wait(NULL) != -1) {
                ;
            }
            printf("Processo %d finalizado\n", getpid());
            return 0;
        }
        while (wait(NULL) != -1) {
            ;
        }
        printf("Processo %d finalizado\n", getpid());
    }

    printf("Processo principal %d finalizado\n", getpid());
    return 0;
}
