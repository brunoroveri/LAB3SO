#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double **Matriz;   
double *VetorEntrada; 
double *VetorResultado; 
int linhas, colunas; 
int num_threads;

void *Multiplicar(void* rank);

int main(int argc, char* argv[]) {
    pthread_t* threads;

    printf("Digite o número de linhas da matriz: ");
    scanf("%d", &linhas);
    printf("Digite o número de colunas da matriz (e tamanho do vetor): ");
    scanf("%d", &colunas);

    Matriz = malloc(linhas * sizeof(double*));
    for (int i = 0; i < linhas; i++) {
        Matriz[i] = malloc(colunas * sizeof(double));
        for (int j = 0; j < colunas; j++) {
            Matriz[i][j] = i + j;
        }
    }

    VetorEntrada = malloc(colunas * sizeof(double));
    for (int j = 0; j < colunas; j++) {
        VetorEntrada[j] = j;
    }

    VetorResultado = malloc(linhas * sizeof(double));

    num_threads = strtol(argv[1], NULL, 10);
    threads = malloc(num_threads * sizeof(pthread_t));

    for (long t = 0; t < num_threads; t++) {
        pthread_create(&threads[t], NULL, Multiplicar, (void*) t);
    }

    for (long t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("\nVetor Resultado:\n");
    for (int i = 0; i < linhas; i++) {
        printf("%.2f ", VetorResultado[i]);
    }
    printf("\n");

    for (int i = 0; i < linhas; i++) {
        free(Matriz[i]);
    }
    free(Matriz);
    free(VetorEntrada);
    free(VetorResultado);
    free(threads);

    return 0;
}

void *Multiplicar(void* rank) {
    long id_thread = (long) rank;
    int num_linhas_por_thread = linhas / num_threads;
    int inicio = id_thread * num_linhas_por_thread;
    int fim = inicio + num_linhas_por_thread;

    if (id_thread == num_threads - 1) {
        fim = linhas; 
    }

    for (int i = inicio; i < fim; i++) {
        VetorResultado[i] = 0.0;
        for (int j = 0; j < colunas; j++) {
            VetorResultado[i] += Matriz[i][j] * VetorEntrada[j];
        }
    }

    return NULL;
}# LAB3SO
# LAB3SO
