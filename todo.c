#include "todo.h"
#include "file.h"
#include <string.h>
#include <stdlib.h>

struct TaskStorage T;

void initTasks(){
  loadTasks();
}

void addTask(char *name, char *desc){
  T.array[T.numTasks].name = name;
  T.array[T.numTasks].desc = desc;
  T.array[T.numTasks].completed = 0; //Init to zero (not completed)
  T.array = realloc(T.array, sizeof(Task) * (++T.numTasks + 1));
}

void delTask(int selected){
  free(T.array[selected].name);
  free(T.array[selected].desc);
  memmove(&T.array[selected], &T.array[selected+1], ((T.numTasks - selected - 1) * sizeof(Task)));
  T.numTasks--;
  return;
}

void toggleCompletion(Task *task){  
  return;
}