/** TODO
 * Implement regeneration of config files if not found
 * Implement correction of config files if in improper format
 * Store config files in different path, so that config files are portable
 */


#include "file.h" 
#include "ui.h"
#include "todo.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

FILE *logfile;
bool logStarted;

/** TASKS */

void loadTasks(){
  //Need to improve newline stripping, memmove is a bit heavy handed
  //Also, need to not core dump if there is no task file yet :)
  FILE *tasks = fopen("tasks.txt", "r");

  //If task file is not found.
  if(!tasks){
    FILE *out = fopen("tasks.txt", "w");
    if(!out){
      perror("Could not create new tasks file.");
      exit(1);
    }
    fclose(out);
    
    T.numTasks = 0;
    T.array = malloc(sizeof(Task) * (T.numTasks + 1));
    return;
  }



  //File exists

  fscanf(tasks, "%d", &T.numTasks);
  while(fgetc(tasks) != '\n'); //Move past end of line

  T.array = malloc(sizeof(Task) * (T.numTasks + 1));

  //Need to read name, desc, index, completion
  char *line = NULL;
  size_t linecap = 0; //Set initial linecap to 0, this forces getline to malloc an appropriate buffer
  ssize_t namelen, desclen;

  int i;
  for(i = 0; i < T.numTasks; i++){
    //Get name
    namelen = getline(&T.array[i].name, &linecap, tasks);
    //Strip newline character
    if(namelen > 0 && T.array[i].name[namelen-1] == '\n'){
      T.array[i].name[--namelen] = '\0';
    }

    //Reset linecap to 0 to force getline to malloc
    linecap = 0;

    //Get desc
    desclen = getline(&T.array[i].desc, &linecap, tasks);
    //Strip newline
    if(desclen > 0 && T.array[i].desc[desclen-1] == '\n'){
      T.array[i].desc[--desclen] = '\0';
    }

    //Get completion status
    fscanf(tasks, "%d", &T.array[i].completed);
    while(fgetc(tasks) != '\n'); //Advance to next line twice
    while(fgetc(tasks) != '\n');
  }
    
  fclose(tasks);
} 

void saveTasks(){
  FILE *tasks = fopen("tasks.txt", "w");
  if(!tasks){
    perror("fopen failed");
    return;
  }
  //Task Name
  //Task Description
  //Task Index
  //Task Completion

  int i;

  fprintf(tasks, "%d\n", T.numTasks);
  for(i = 0; i < T.numTasks; i++){
    fprintf(tasks, "%s\n", T.array[i].name);
    fprintf(tasks, "%s\n", T.array[i].desc);
    fprintf(tasks, "%d\n", T.array[i].completed);
    fprintf(tasks, "###\n");
  }

  fclose(tasks);
}

/** SETTINGS */

void loadSettings(Option *optionList, int numOptions){
  FILE *settings = fopen("settings.txt", "r");
  int i;
  //If settings not found

  if(!settings){
    for(i = 0; i < numOptions; i++){
      *optionList[i].value = optionDefaults[i];
    }
    return;
  }


  for(i = 0; i < numOptions; i++){
    fscanf(settings, "%d", optionList[i].value);
    fscanf(settings, "%*[^\n]"); //Read until end of line and discard value
  }

  fclose(settings);
}

void saveSettings(Option *optionList, int numOptions){
  FILE *settings = fopen("settings.txt", "w");

  int i;
  for(i = 0; i < numOptions; i++){
    fprintf(settings, "%d : %s : %d\n", *optionList[i].value, optionList[i].name, optionList[i].type);
  }
  fclose(settings);
}

/** LOGGING */


void programLog(const char *message){
  if(!logStarted){ //We want to overwrite the log for each program execution
    logStarted = TRUE;
    logfile = fopen("log.txt", "w");
    fprintf(logfile, "%s\n", message);
  } else { //If already opened, we only want to append the log file
    logfile = fopen("log.txt", "a");
    fprintf(logfile, "%s\n", message);
  }
  //Close file each time we log, ensures logs are saved before program crash
  fclose(logfile);
}