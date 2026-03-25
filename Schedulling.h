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


int choose_task(int modo, Task *tasks, int task_count);
void release_tasks(Task tasks[], int task_count, int current_time);
void execute_task(Task tasks[], int selected_index);



#endif
