#ifndef FILE_H
#define FILE_H

#include "ui.h"
#include <stdio.h>
#include <string.h>

extern FILE *logfile;

void loadTasks();
void saveTasks();
void loadSettings(Option *optionList, int numOptions);
void saveSettings(Option *optionList, int numOptions);
void programLog(const char *message);



#endif