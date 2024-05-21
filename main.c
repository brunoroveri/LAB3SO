#define _GNU_SOURCE
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h> // Adicionado para write() e read()

#define FIBER_STACK (1024*64)

// Estrutura de dados para passar informações entre o pai e a thread filha
struct InfoThread {
    int dado;
    char mensagem[256];
};

int funcaoThread(void* argumento)
{
    struct InfoThread *info = (struct InfoThread *)argumento;

    printf("A thread filha recebeu o dado: %d\n", info->dado);
    printf("A thread filha está enviando uma mensagem de volta para o pai: %s\n", info->mensagem);

    // Enviando mensagem de volta para o pai
    write(STDOUT_FILENO, info->mensagem, sizeof(info->mensagem));

    return 0;
}

int main()
{
    void* stack;
    pid_t pid;

    // Alocar a pilha
    stack = malloc(FIBER_STACK);
    if (stack == NULL)
    {
        perror("malloc: não foi possível alocar a pilha");
        exit(1);
    }

    printf("Criando a thread filha\n");

    // Estrutura de dados para passar informações entre pai e thread filha
    struct InfoThread info;
    info.dado = 42; // Dado de exemplo
    snprintf(info.mensagem, sizeof(info.mensagem), "Olá do thread filho!");

    // Chamar a chamada de sistema clone para criar a thread filha
    pid = clone(&funcaoThread, (char*)stack + FIBER_STACK,
                SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &info);
    if (pid == -1)
    {
        perror("clone");
        exit(2);
    }

    // Esperar a thread filha sair
    pid = waitpid(pid, NULL, 0);
    if (pid == -1)
    {
        perror("waitpid");
        exit(3);
    }

    // Liberar a pilha
    free(stack);
    printf("A thread filha retornou e a pilha foi liberada.\n");

    return 0;
}

    return NULL;
}# LAB3SO
# LAB3SO
