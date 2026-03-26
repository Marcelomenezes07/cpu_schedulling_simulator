#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Schedulling.h"
#include "io.h"

int main(int argc, char *argv[]) {

    ExecutionBlock log[MAX_LOG];
    int schedulling_modo;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        exit(1);
    }

    // argv[0] vem com caminho tipo "./rate", entao extrai so o nome
    char *prog_name = strrchr(argv[0], '/');
    if (prog_name != NULL) {
        prog_name = prog_name + 1;
    } else {
        prog_name = argv[0];
    }

    if (strcmp(prog_name, "rate") == 0) {
        schedulling_modo = 0;
    } else if (strcmp(prog_name, "edf") == 0) {
        schedulling_modo = 1;
    } else {
        fprintf(stderr, "Erro: executavel deve se chamar 'rate' ou 'edf'\n");
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
    int total_time  = 0;

    get_time_execution(&total_time, file);
    get_tasks(tasks, file, &task_count);
    fclose(file);

    int log_count  = 0;
    int last_block = -1;
    int start_time = 0;

    for (int time = 0; time < total_time; time++) {

        // salva o remaining ANTES do release, senao get_status ve o valor ja resetado
        int prev_remaining = 0;
        int current_task_lost = 0;

        if (last_block >= 0) {
            prev_remaining = tasks[last_block].remaining;

            if (time == tasks[last_block].next_release && tasks[last_block].remaining > 0) {
                current_task_lost = 1;
            }
        }

        release_tasks(tasks, task_count, time);
        int index_exe  = choose_task(schedulling_modo, tasks, task_count);

        // fecha o bloco quando muda de task OU quando a mesma task perde deadline
        if (last_block != index_exe || current_task_lost) {

            if (last_block != -1) {
                int close_reason = 0;
                if (current_task_lost) {
                    close_reason = 1;
                }
                // usa prev_remaining porque release_tasks ja resetou o remaining
                Task prev_state = tasks[last_block];
                prev_state.remaining = prev_remaining;
                char status = get_status(prev_state, close_reason);
                close_block(log, &log_count, tasks, last_block, start_time, time, status); // BUG O7 corrigido: last_block no lugar de index_exe
            }
            else if (time > start_time) {
                // fecha o bloco idle que estava rodando
                close_block(log, &log_count, tasks, -1, start_time, time, ' ');
            }

            start_time = time;
            last_block = index_exe;
        }

        execute_task(tasks, index_exe);

       
    }

    if (last_block != -1) {
            char status = get_status(tasks[last_block], 2);
            close_block(log, &log_count, tasks, last_block, start_time, total_time, status);
        }
    else if (total_time > start_time) {
            close_block(log, &log_count, tasks, -1, start_time, total_time, ' ');
        }

    print_execution_blocks(log, log_count, schedulling_modo);
    print_task_report(tasks, task_count);

    return 0;
}