#include "todo.h"
#include "ui.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>
// #include "storage.h"


void init(){
  logStarted = FALSE;
  initOptions();
  programLog("Options loaded");
  initTasks();
  programLog("Tasks loaded");
}

int main(){
  //Init structures
  init();

  //Initialize ncurses
  initscr();
  raw(); //Enable raw mode
  noecho(); //Disable echo
  keypad(stdscr, TRUE); //Enable function keys
  curs_set(0); //Hide cursor
  refresh();

  run_todo_app();

  //Before quit
  saveSettings(optionList, C.optionIndex);
  saveTasks();

  endwin();
  return 0;

}