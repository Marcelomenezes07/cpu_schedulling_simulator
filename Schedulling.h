#ifndef SCHEDULLING_H
#define SCHEDULLING_H

#define MAX_TASKS 150
#define MAX_NAME 20
#define MAX_LOG 1000

typedef struct {
    char name[20];
    int period;
    int burst;

    int remaining;
    int next_release;
    int abs_deadline;

    int lost_deadlines;
    int complete_execution;
    int killed;
} Task;

typedef struct {
    char name[20];
    int start_time;
    int end_time;
    int duration;
    char status;   // F, H, L, K
} ExecutionBlock;


int choose_task(int modo, Task *tasks, int task_count); // escolhe a tarefa a ser executada com base no modo (RATE ou EDF) e nas tarefas disponíveis. e retorna o indice da tarefa escolhida
int release_tasks(Task tasks[], int task_count, int current_time); // verifca se tem codigo pra ser executado verifica se o retorno for diferente de -1 ele retornou um indice que perdeu a deadline
void execute_task(Task tasks[], int selected_index); // executa um ciclo de CPU para a tarefa selecionada, reduzindo seu tempo restante
void close_block(ExecutionBlock log[], int *log_count, Task tasks[],
                 int task_index, int start_time, int end_time, char status) ; // fecha um bloco de execução, seja de uma tarefa ou de idle, e registra no log



#endif
