#include <stdio.h>
#include <stdlib.h>
#include "Schedulling.h"
#include "io.h"



int main(int argc, char *argv[]) {

    int current_task;
    int block_start_time;
    ExecutionBlock log[];
    int log_count;

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
    
    get_time_execution(&total_time, file);
    get_tasks(tasks, file, &task_count);

    printf("Tempo total de execucao: %d\n", total_time);

    for (int i = 0; i < task_count; i++) {
        printf("Tarefa %d: nome=%s, periodo=%d, burst=%d\n", i + 1, tasks[i].name, tasks[i].period, tasks[i].burst);
    }


    return 0;
}
