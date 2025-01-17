#ifndef UI_H
#define UI_H


#include "todo.h"
#include <ncurses.h>

typedef struct {
  char name[80];
  int type;
  int *value; //Points to config value
} Option;

extern Option optionList[20];

struct programConfig{
  int nameWidth;
  int optionIndex;
  int menuEntries;
};

extern struct programConfig C;

enum promptType{
  promptNewTask = 0,
  promptDelTask
};

enum colorPair{
  INVERTED = 1,
  TITLE,
  TASKNAME,
  TASKNAMEHL,
  NORMAL
};

enum optionType{
  toggle = 0,
  valued
};

void draw_todo_list(int selected);
void draw_help_menu();
int draw_prompt(int type);
void run_todo_app();
void initOptions();

#endif