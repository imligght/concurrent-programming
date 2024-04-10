// Dupla: Luan da Silva Moraes e Caio César Rodrigues de Aquino

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {

    // ....

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    pid_t pid;
    pid_t child_pid;

    for (int i = 0; i < 2; i++) {
        if ((pid = fork()) == 0) {
            child_pid = getpid();
            printf("Processo pai criou %d\n", child_pid);
            return 0;
        }
        else {
            printf("Processo filho criado %d\n", pid);
        }
    }
    while (wait(NULL) != -1) {
        ;
    }

    printf("Processo pai finalizado!\n");
    return 0;
}
