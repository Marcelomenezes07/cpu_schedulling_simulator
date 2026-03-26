#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "Schedulling.h"
#include "io.h"
 
int main(int argc, char *argv[]) {
 
    ExecutionBlock log[MAX_LOG];
    int schedulling_modo;
 
    // ===== FIX 1: Validacao de argc =====
    // Uso esperado: ./rate arquivo.txt  OU  ./edf arquivo.txt
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        fprintf(stderr, "Erro: numero incorreto de argumentos\n");
        exit(1);
    }
 
    char *prog_name = strrchr(argv[0], '/');
    if (prog_name != NULL) {
        prog_name++; // pula o '/'
    } else {
        prog_name = argv[0];
    }
 
    if (strcmp(prog_name, "rate") == 0) {
        schedulling_modo = 0;
    } else if (strcmp(prog_name, "edf") == 0) {
        schedulling_modo = 1;
    } else {
        fprintf(stderr, "Erro: o executavel deve se chamar 'rate' ou 'edf' (recebido: '%s')\n", prog_name);
        exit(1);
    }
 
    char *input_file = argv[1];
 
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada: %s\n", input_file);
        exit(1);
    }
 
    Task tasks[MAX_TASKS];
    int task_count = 0;
    int total_time = 0;
 
    get_time_execution(&total_time, file);
    get_tasks(tasks, file, &task_count);
    fclose(file);
 
    int log_count  = 0;
    int last_block = -1;
    int start_time = 0;
 
    for (int time = 0; time < total_time; time++) {
 
        int current_task_lost = 0;
        int prev_remaining = (last_block >= 0) ? tasks[last_block].remaining : 0;
        if (last_block >= 0 &&
            time == tasks[last_block].next_release &&
            tasks[last_block].remaining > 0) {
            current_task_lost = 1;
        }
 
        release_tasks(tasks, task_count, time);
        int index_exe = choose_task(schedulling_modo, tasks, task_count);
 
        // ===== FIX 4: Fechar bloco quando muda task OU quando a mesma task perde deadline =====
        if (last_block != index_exe || current_task_lost) {
 
            if (last_block >= 0) {
        
                int close_reason = current_task_lost ? 1 : 0;
                Task prev_state = tasks[last_block];
                prev_state.remaining = prev_remaining;
                char status = get_status(prev_state, close_reason);
                close_block(log, &log_count, tasks, last_block, start_time, time, status);
            }
            else if (time > start_time) {
                close_block(log, &log_count, tasks, -1, start_time, time, ' ');
            }
 
            start_time = time;
            last_block = index_exe;
        }
 
        execute_task(tasks, index_exe);
    }
 
    // Fecha o ultimo bloco apos o loop
    if (last_block >= 0) {
        char status = get_status(tasks[last_block], 2);
        close_block(log, &log_count, tasks, last_block, start_time, total_time, status);
    } else if (total_time > start_time) {
        close_block(log, &log_count, tasks, -1, start_time, total_time, ' ');
    }
 
    print_execution_blocks(log, log_count, schedulling_modo);
    print_task_report(tasks, task_count);
 
    return 0;
}