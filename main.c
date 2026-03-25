#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Schedulling.h"
#include "io.h"



int main(int argc, char *argv[]) {
    
    int current_task;
    int block_start_time;
    ExecutionBlock log[MAX_LOG];
    int log_count;

    int schedulling_modo;

    // definindo o modo = 0 p/ rate modo = 1 p/ monotonic :
    if (strcmp(argv[0],"rate") == 0){
        schedulling_modo = 0;
    }
    else if (strcmp(argv[0],"edf") == 0){
        schedulling_modo = 1; 
    }

    //temporario
    char *input_file = "input_teste_tasks.txt"; // depois vai ser igual ao argv[1]

    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada: %s\n", input_file);
        exit(1);
    }

    Task tasks[MAX_TASKS];
    int task_count = 0; // quantidade de tarefas lidas do arquivo de entrada
    int total_time = 0; // tempo total de execucao do simulador 
    
    get_time_execution(&total_time, file); // pega o tempo total de execucao do simulador
    get_tasks(tasks, file, &task_count); // pega as tarefas do arquivo de entrada e armazena no vetor tasks, atualiza a quantidade de tarefas lidas no task_count

    // printf("Tempo total de execucao: %d\n", total_time);

    // for (int i = 0; i < task_count; i++) {
    //     printf("Tarefa %d: nome=%s, periodo=%d, burst=%d\n", i + 1, tasks[i].name, tasks[i].period, tasks[i].burst);
    // }

    //inicializando o loop principal
    for(int time = 0 ; time < total_time ; time++){
        if (release_tasks(tasks, task_count, time) != -1){
            // passo o index pra a funcao de registrar o block

        }

        int index_exe = choose_task(schedulling_modo, tasks, task_count);

        execute_task(tasks, index_exe);
        printf("Nome: %s   Remaining: %d\n",tasks[index_exe].name, tasks[index_exe].remaining);

    }


    return 0;
}
