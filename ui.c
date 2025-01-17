#include "ui.h"
#include "file.h"
#include "utils.h"
#include <string.h>

#define CTRL_KEY(k) ((k) & 0x1f)

Option optionList[20]; //Hardcoded option limit, this makes sense because these aren't user-defined

struct programConfig C;


void addOption(char *name, int type, int *value){
  strcpy(optionList[C.optionIndex].name, name);
  optionList[C.optionIndex].type = type;
  optionList[C.optionIndex].value = value;
  C.optionIndex++;
}

int testValue;

void initOptions(){
  C.optionIndex = 0;
  addOption("Adjust name column width", valued, &C.nameWidth);
  addOption("Test setting", valued, &testValue);

  loadSettings(optionList, C.optionIndex);
}


WINDOW *create_window(int height, int width, int starty, int startx){
  WINDOW *local_win;
  local_win = newwin(height, width, starty, startx);
  box(local_win, 0,0); //0 gives default characters for lines
  //box is shorthand for a call to wborder without specifying corner characters
  wrefresh(local_win);
  return local_win;
}

void destroy_win(WINDOW *local_win){
  wborder(local_win, ' ',' ',' ',' ',' ',' ',' ', ' ');
  //the 2-9th arguments define characters used for:
  //left side
  //right side
  //top side
  //bottom side
  //top left corner
  //top right corner
  //bottom left corner
  //bottom right corner

  wrefresh(local_win);
  delwin(local_win);
}

void initColors(){
  start_color();
  use_default_colors();
  init_pair(INVERTED, COLOR_BLACK, COLOR_WHITE);
  init_pair(TITLE, COLOR_BLUE, COLOR_WHITE);
  init_pair(TASKNAME, COLOR_GREEN, -1);
  init_pair(TASKNAMEHL, COLOR_GREEN, COLOR_WHITE);
  init_pair(NORMAL, COLOR_WHITE, -1);
}

/** DRAWING */

void draw_todo_list(int selected){
  WINDOW *todoList, *todoDescriptions;
  char title[] = "WILSONTODO";

  //Render windows
  todoList = newwin(LINES-3, C.nameWidth, 0, 0);
  box(todoList, 0,0);
  todoDescriptions = newwin(LINES-3, COLS - C.nameWidth, 0, C.nameWidth);
  box(todoDescriptions, 0, 0);
  wnoutrefresh(todoDescriptions);

  //Render title
  wcprintw(todoList, 0, 1, A_NORMAL, TITLE, "%s", title);

  //Draw task names
  int i;
  int namey = 2;
  for(i = 0; i < T.numTasks; i++){
    if(i == selected){
      //Hightlight hovered task
      wcprintw(todoList, namey, 2, A_NORMAL, TASKNAMEHL, "%s", T.array[i].name);
    } else {
      //Draw with normal attribute
      wcprintw(todoList, namey, 2, A_NORMAL, TASKNAME, "%s", T.array[i].name);
    }
    namey++;
  }
  wnoutrefresh(todoList);

  if(T.numTasks > 0){
    //Draw selected task desc in child window
    WINDOW *child;
    int childWidth = getmaxx(todoDescriptions);
    int childHeight = getmaxy(todoDescriptions);
    child = newwin(childHeight - 2, childWidth - 2, getbegy(todoDescriptions) + 1, getbegx(todoDescriptions)+1);
    mvwprintw(child, 1, 0, T.array[selected].desc);
    wnoutrefresh(child);
  } 

}

void draw_help_menu(){
  WINDOW *helpMenu;
  helpMenu = create_window(3, COLS, LINES-3, 0);
  mvwprintw(helpMenu, 1, 1, "Q Quit O Options A Add Task D Delete Task");
  mvwchgat(helpMenu, 1, 1, 1, A_NORMAL, INVERTED, NULL);
  mvwchgat(helpMenu, 1, 8, 1, A_NORMAL, INVERTED, NULL);
  mvwchgat(helpMenu, 1, 18, 1, A_NORMAL, INVERTED, NULL);
  mvwchgat(helpMenu, 1, 29, 1, A_NORMAL, INVERTED, NULL);

  wnoutrefresh(helpMenu);
}

int draw_prompt(int type){
  int retval;
  WINDOW *prompt, *child;

  prompt = newwin((2*LINES) / 3, (2*COLS) / 3, LINES / 6, COLS / 6);
  box(prompt, 0, 0);
  wrefresh(prompt);

  int childWidth = getmaxx(prompt) - getbegy(prompt);
  int childHeight = getmaxy(prompt) - getbegy(prompt);
  child = newwin(childHeight+3, childWidth+3, getbegy(prompt) + 1, getbegx(prompt) + 1);


  char *nameBuffer, *descBuffer;
  switch(type){
    case(promptNewTask):
      mvwprintw(child, 0, 0, "Enter new task name: ");

      //Enable cursor and echo, push prompt to screen
      curs_set(1);
      wrefresh(child);

      nameBuffer = wdynamicGetStr(child);

      if(!nameBuffer){
        curs_set(0);
        retval = 0;
        break;
      }

      mvwprintw(child, getcury(child) + 1, 0, "Enter description: ");
      wrefresh(child);

      descBuffer = wdynamicGetStr(child);

      addTask(nameBuffer, descBuffer);

      //Disable cursor and echo.
      curs_set(0);
      retval = 1;
      break;

    
    case(promptDelTask):
      {
      char delPrompt[] = "Are you sure you want to delete task?";
      int ch;
      bool deleteHover = FALSE;
      mvwprintw(child, getmaxy(child)/4, (getmaxx(child) - strlen(delPrompt)) / 2, "%s", delPrompt);
      wcprintw(child, (3*getmaxy(child))/4, (getmaxx(child) / 2) - 13, A_NORMAL, NORMAL, "YES");
      wcprintw(child, (3*getmaxy(child))/4, (getmaxx(child) / 2) + 10, A_NORMAL, INVERTED, "NO");
      keypad(child, TRUE);
      wrefresh(child);

      while((ch = getch()) != '\n'){
        switch(ch){
          case KEY_LEFT:
          case KEY_RIGHT:
            deleteHover = !deleteHover;
            break;
          default:
            break;
        }
        if(deleteHover){
          wcprintw(child, (3*getmaxy(child))/4, (getmaxx(child) / 2) - 13, A_NORMAL, INVERTED, "YES");
          wcprintw(child, (3*getmaxy(child))/4, (getmaxx(child) / 2) + 10, A_NORMAL, NORMAL, "NO");
          wrefresh(child);
        } else {
          wcprintw(child, (3*getmaxy(child))/4, (getmaxx(child) / 2) - 13, A_NORMAL, NORMAL, "YES");
          wcprintw(child, (3*getmaxy(child))/4, (getmaxx(child) / 2) + 10, A_NORMAL, INVERTED, "NO");
          wrefresh(child);
        }
        
      }
      

      retval = (int)deleteHover;
      break;
      }
  }

  destroy_win(child);
  destroy_win(prompt);
  return retval;
}

void draw_options(){
  /** TODO
   * Add support for toggleable options
   */

  WINDOW *options;
  options = create_window(LINES, COLS, 0,0);
  int highlight = 0;
  mvwprintw(options, 1,1, "Options | O to exit");
  curs_set(0);

  int i;
  int optiony = 3;

  //Draw initial options
  for(i = 0; i < C.optionIndex; i++){
    //Check type of option
    if(optionList[i].type == valued){
      //Valued Options
      mvwprintw(options, optiony, 3, "# %s | %.2d", optionList[i].name, *optionList[i].value);
    } else{
      //Toggle Options
      mvwprintw(options, optiony, 3, "# %s | %s", optionList[i].name, (*optionList[i].value) ? "ON " : "OFF");
    }

    //Render highlight
    if(i == highlight){
      mvwchgat(options, optiony, 3, 1, A_NORMAL, INVERTED, NULL);
    }
    optiony += 2;
  }
  wrefresh(options);

  //Control loop
  int ch;
  while((ch = getch()) != 'o'){
    switch(ch){
      case KEY_UP:
        highlight = (highlight == 0) ? C.optionIndex - 1 : highlight - 1;
        break;
      case KEY_DOWN:
        highlight = (highlight == C.optionIndex - 1) ? 0 : highlight + 1;
        break;
      case KEY_LEFT:
        //Check if option valued
        if(optionList[highlight].type == valued){
          if(*optionList[highlight].value > 3){
            *optionList[highlight].value -= 1;
          }
        } 


        break;
      case KEY_RIGHT:
        //Check if option valued
        if(optionList[highlight].type == valued){
          if(*optionList[highlight].value < COLS - 3){
            *optionList[highlight].value += 1;
          }
        } else if (optionList[highlight].type == toggle){
          *optionList[highlight].value = !(*optionList[highlight].value);
        }


        break;
    }

    //Redraw
    optiony = 3; //Reset beginning draw coordinate

    for(i = 0; i < C.optionIndex; i++){
    //Check type of setting
    if(optionList[i].type == valued){
      mvwprintw(options, optiony, 3, "# %s | %.2d", optionList[i].name, *optionList[i].value);
    } else if (optionList[i].type == toggle){
      mvwprintw(options, optiony, 3, "# %s | %s", optionList[i].name, (*optionList[i].value) ? "ON " : "OFF");
    }

    //Render highlight
    if(i == highlight){
      mvwchgat(options, optiony, 3, 1, A_NORMAL, INVERTED, NULL);
    }
    optiony += 2;
    }

    wnoutrefresh(options);
    doupdate();
  }
  
  destroy_win(options);
}

/** RUN */

void run_todo_app(){
  int ch;
  int selected = 0;
  initColors();
  draw_todo_list(selected);
  draw_help_menu();
  doupdate();

  while((ch = getch()) != 'q'){
    //This is where we handle arrow keys/todo operations, so prob need to pass highlight to draw_todo_list
    //Process keypresses
    //highlight = (highlight == 0) ? C.optionIndex - 1 : highlight - 1;
    switch(ch){
      case('o'):
        draw_options();
        break;
      case('a'):
        draw_prompt(promptNewTask);
        break;
      case('d'):
        if(draw_prompt(promptDelTask)) delTask(selected);
        selected = (selected == T.numTasks) ? selected - 1 : selected;
        break;
      case KEY_UP:
        if(T.numTasks == 0) break;
        selected = (selected == 0) ? T.numTasks - 1 : selected - 1;
        break;
      case KEY_DOWN:
        if(T.numTasks == 0) break;
        selected = (selected == T.numTasks - 1) ? 0 : selected + 1;
        break;
    }



    draw_todo_list(selected);
    draw_help_menu();
    doupdate();
  }

}
