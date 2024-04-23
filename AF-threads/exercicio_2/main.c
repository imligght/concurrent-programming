#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    int size;
    double* vector_slice_a;
    double* vector_slice_b;
    double* vector_slice_c;
} ThreadsData;

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);

void* thread(void* arg) {
    ThreadsData* threads_data = (ThreadsData*) arg;
    for (int i = 0; i < threads_data->size; i++) {
        threads_data->vector_slice_c[i] = threads_data->vector_slice_b[i] + threads_data->vector_slice_a[i];
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }
    //Cria vetor do resultado
    double* c = malloc(a_size*sizeof(double));

    // Calcula com uma thread só. Programador original só deixou a leitura 
    // do argumento e fugiu pro caribe. É essa computação que você precisa 
    // paralelizar
    // for (int i = 0; i < a_size; ++i) 
    //     c[i] = a[i] + b[i];

    if (n_threads > a_size) {
        n_threads = a_size;
    }

    pthread_t* threads = (pthread_t*) malloc(n_threads * sizeof(pthread_t));

    if (!threads) {
        printf("Impossível criar %d threads!", n_threads);
        return 1;
    }

    ThreadsData* threads_datas = (ThreadsData*) malloc(n_threads * sizeof(ThreadsData));

    if (!threads_datas) {
        printf("Impossível criar as structs!");
        return 1;
    }

    double* current_slice_a = a;
    double* current_slice_b = b;
    double* current_slice_c = c;

    int cicles_per_thread = a_size / n_threads;
    int remains = a_size % n_threads;

    for (int i = 0; i < n_threads; i++) {

        int additional_cicle = 0;
        int cicles;

        if (remains > 0) {
            remains--;
            additional_cicle++;
        }
        cicles = cicles_per_thread + additional_cicle;
        threads_datas[i].vector_slice_a = current_slice_a;
        threads_datas[i].vector_slice_b = current_slice_b;
        threads_datas[i].vector_slice_c = current_slice_c;
        threads_datas[i].size = cicles;
        current_slice_a = &current_slice_a[cicles];
        current_slice_b = &current_slice_b[cicles];
        current_slice_c = &current_slice_c[cicles];
        pthread_create(&threads[i], NULL, thread, (void*)&threads_datas[i]);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);

    //Importante: libera memória
    free(a);
    free(b);
    free(c);
    free(threads);
    free(threads_datas);

    return 0;
}
