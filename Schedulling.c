#include <stdio.h>
#include <string.h>

#include "Schedulling.h"

int choose_task(int modo, Task *tasks, int task_count) {
    int escolhido = -1;

    for (int i = 0; i < task_count; i++) {
        if (tasks[i].remaining > 0) {

            if (escolhido == -1) {
                escolhido = i;
            }
            else {
                // RATE -> menor periodo
                if (modo == 0) {
                    if (tasks[i].period < tasks[escolhido].period) {
                        escolhido = i;
                    }
                }

                // EDF -> menor deadline absoluto
                else if (modo == 1) {
                    if (tasks[i].abs_deadline < tasks[escolhido].abs_deadline) {
                        escolhido = i;
                    }
                }
            }
        }
    }

    return escolhido;
}

int release_tasks(Task tasks[], int task_count, int current_time) { // retorna o index que perdeu o deadline
    int lost_index = -1;

    for (int i = 0; i < task_count; i++) {
        if (current_time == tasks[i].next_release) {

            if (tasks[i].remaining > 0) {
                tasks[i].lost_deadlines++;
                tasks[i].killed++;
                lost_index = i;
            }
            else {
                if (current_time != 0) {
                    tasks[i].complete_execution++;
                }
            }

            tasks[i].remaining = tasks[i].burst;
            tasks[i].abs_deadline = current_time + tasks[i].period;
            tasks[i].next_release = current_time + tasks[i].period;
        }
    }

    return lost_index;
}

void execute_task(Task tasks[], int selected_index) {
    if (selected_index == -1) {
        return;
    }

    if (tasks[selected_index].remaining > 0) {
        tasks[selected_index].remaining--;
    }
}



void close_block(ExecutionBlock log[], int *log_count, Task tasks[],
                 int task_index, int start_time, int end_time, char status) {


    // FIX: antes fazia "Task task = tasks[task_index]" aqui em cima,
    // mas se task_index == -1 isso acessa tasks[-1] (undefined behavior)
    if (start_time == -1 || end_time <= start_time) {
        return;
    }

    if (task_index == -1) {
        strcpy(log[*log_count].name, "idle");
    } else {
        strcpy(log[*log_count].name, tasks[task_index].name);
    }

    log[*log_count].start_time = start_time; // faço isso pra pegar o tempo que ele comecou a ser executado
    log[*log_count].end_time = end_time;
    log[*log_count].duration = end_time - start_time;
    log[*log_count].status = status;

    (*log_count)++;
}



char get_status(Task task, int close_reason) {

    // Deadline perdido: periodo chegou mas ainda tinha trabalho
    if (close_reason == 1) {
        return 'L';
    }

    // Fim da simulacao com trabalho pendente: processo foi morto
    if (close_reason == 2 && task.remaining > 0) {
        return 'K';
    }

    // Terminou o burst completamente
    if (task.remaining == 0) {
        return 'F';
    }

    // Ainda tinha trabalho mas foi preemptado por outra task
    return 'H';
}