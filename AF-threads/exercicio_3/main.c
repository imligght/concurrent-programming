#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    double* a;
    double* b;
    double result;
    int size;
    int next_start;
} ThreadData;

void* thread(void* arg) {
    ThreadData* thread_data = (ThreadData*)arg;
    int index_correction;
    for (int i = 0; i < thread_data->size; i++) {
        index_correction = i + thread_data->next_start;
        thread_data->result += thread_data->a[index_correction] * thread_data->b[index_correction];
    }
    return 0;
}

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar);

int main(int argc, char* argv[]) {
    srand(time(NULL));

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

    //Calcula produto escalar. Paralelize essa parte
    // double result = 0;
    // for (int i = 0; i < a_size; ++i) 
    //     result += a[i] * b[i];
    
    if (n_threads > a_size) {
        n_threads = a_size;
    }

    pthread_t* threads = (pthread_t*) malloc(n_threads * sizeof(pthread_t));

    if (!threads) {
        printf("Impossível alocar %d threads!", n_threads);
        return 1;
    }

    ThreadData* thread_datas = (ThreadData*) malloc(n_threads * sizeof(ThreadData));

    if (!thread_datas) {
        printf("Impossível alocar as thread_datas!");
        return 1;
    }

    int cicles_per_thread = a_size / n_threads;
    int cicles_remains = a_size % n_threads;

    double* current_a = a;
    double* current_b = b;
    int next_start = 0;

    for (int i = 0; i < n_threads; i++) {

        thread_datas[i].next_start = next_start;
        int additional_cicles = 0;

        if (cicles_remains > 0) {
            cicles_remains--;
            additional_cicles++;
        }
        thread_datas[i].a = current_a;
        thread_datas[i].b = current_b;
        thread_datas[i].size = cicles_per_thread + additional_cicles;
        current_a = &current_a[i];
        current_b = &current_b[i];

        next_start = thread_datas[i].size;

        pthread_create(&threads[i], NULL, thread, (void*)&thread_datas[i]);
    }

    double result = 0.0;

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
        result += thread_datas[i].result;
    }

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, a_size, result);

    //Libera memória
    free(a);
    free(b);
    free(threads);
    free(thread_datas);

    return 0;
}
