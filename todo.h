#ifndef TODO_H
#define TODO_H


typedef struct{
  char *name;
  char *desc;
  int completed;
} Task;

struct TaskStorage{
  int numTasks;
  Task *array;
};

extern struct TaskStorage T;

void addTask(char *name, char *desc);
void delTask(int selected);
void initTasks();

#endif
