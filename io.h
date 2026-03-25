// arquivo para fazer entrada e saida de dados, leitura de arquivos, etc
#ifndef IO_H
#define IO_H


typedef struct {
    char name[50];
    int duration;
    char status;
} LogEntry;



#include "Schedulling.h"


void get_time_execution(int *time, FILE *routefile);
void get_tasks(Task *tasks, FILE *routefile, int *task_count);


void print_execution_blocks(ExecutionBlock log[], int log_count, int modo);
void print_task_report(Task tasks[], int task_count);



#endif