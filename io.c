#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Schedulling.h"
#include "io.h"

void get_time_execution(int *time, FILE *routefile) {
    char line[100];

    if (fgets(line, sizeof(line), routefile) == NULL) {
        fprintf(stderr, "Erro ao ler o tempo total de execucao\n");
        exit(1);
    }

    *time = atoi(line); // atoi converte string para inteiro

    if (*time <= 0) {
        fprintf(stderr, "Erro: tempo total invalido\n");
        exit(1);
    }
}

void get_tasks(Task *tasks, FILE *routefile, int *task_count) {
    char line[100];
    int i = 0;

    while (fgets(line, sizeof(line), routefile) != NULL) {
        char *token_name;
        char *token_period;
        char *token_burst;

        token_name = strtok(line, " \n");
        token_period = strtok(NULL, " \n");
        token_burst = strtok(NULL, " \n");

        // checando se foi possivel ler a tarefa corretamente
        if (token_name == NULL || token_period == NULL || token_burst == NULL) {
            fprintf(stderr, "Erro ao ler a tarefa na linha %d\n", i + 2);
            exit(1);
        }

        strcpy(tasks[i].name, token_name);
        tasks[i].period = atoi(token_period);
        tasks[i].burst = atoi(token_burst);

        if (tasks[i].period <= 0 || tasks[i].burst <= 0) {
            fprintf(stderr, "Erro: valores invalidos na tarefa da linha %d\n", i + 2);
            exit(1);
        }

        // inicializa os campos de controle zerados
        tasks[i].remaining = 0;
        tasks[i].next_release = 0;
        tasks[i].abs_deadline = 0;

        tasks[i].lost_deadlines = NULL; 
        tasks[i].complete_execution = NULL;
        tasks[i].killed = NULL;

        i++;
    }

    *task_count = i;
}



// saida
void print_execution_blocks(ExecutionBlock log[], int log_count, int modo) {
    if (modo == 0) {
        printf("EXECUTION BY RATE\n\n");
    } else {
        printf("EXECUTION BY EDF\n\n");
    }

    for (int i = 0; i < log_count; i++) {
        if (strcmp(log[i].name, "idle") == 0) {
            printf("idle for %d units\n", log[i].duration);
        } else {
            printf("[%s] for %d units - %c\n",
                   log[i].name,
                   log[i].duration,
                   log[i].status);
        }
    }
}


void print_task_report(Task tasks[], int task_count) {
    printf("\nLOST DEADLINES\n\n");
    for (int i = 0; i < task_count; i++) {
        printf("[%s] %d\n", tasks[i].name, tasks[i].lost_deadlines);
    }

    printf("\nCOMPLETE EXECUTION\n\n");
    for (int i = 0; i < task_count; i++) {
        printf("[%s] %d\n", tasks[i].name, tasks[i].complete_execution);
    }

    printf("\nKILLED\n\n");
    for (int i = 0; i < task_count; i++) {
        printf("[%s] %d\n", tasks[i].name, tasks[i].killed);
    }
}
