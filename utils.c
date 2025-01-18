#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "file.h"

//Improves ncurses getStr to return a buffer of dynamically allocated size
char *wdynamicGetStr(WINDOW *window){
  int ch;
  int size = 32;
  int len = 0;
  char *buffer = malloc(size);

  //While enter is not pressed
  while((ch = getch()) != '\n'){
    //Backspace handle
    if(ch == KEY_BACKSPACE){
      if(len > 0){
        mvwaddch(window, getcury(window), getcurx(window)-1, ' ');
        wmove(window, getcury(window), getcurx(window)-1);
        memmove(&buffer[len-1], &buffer[len], sizeof(char));
        len--;
        wrefresh(window);
      }
    } else if(ch == '\x1b'){
      free(buffer);
      return NULL;
    } else if (!iscntrl(ch) && ch < 127){
      //Check that we don't overflow buffer
      if(len == size - 1){
        size *= 2;
        buffer = realloc(buffer, size);
      }

      //Add char to buffer
      buffer[len++] = ch;
      waddch(window, ch);
      wrefresh(window); //Draw updated window
    } 
  }

  //Check that user didn't just press enter
  if(len == 0){
    free(buffer);
    return NULL;
  }

  //Send to correct size and add null byte
  buffer = realloc(buffer, len + 1);
  buffer[len] = '\0';
  return buffer;
}

//Extension of wprintw that allows you to supply attribute and color pair
void wcprintw(WINDOW *window, int cury, int curx, attr_t attr, short colorpair, const char *fmt, ...){
  va_list arglist;
  va_start(arglist, fmt);
  //Get the length of the string to be read
  int len = vsnprintf(NULL, 0, fmt, arglist);
  va_end(arglist);

  //Create appropriately sized buffer and read in
  va_start(arglist, fmt);
  char message[len + 1];
  vsnprintf(message, len + 1, fmt, arglist);
  va_end(arglist);

  //Print and apply attribute
  mvwprintw(window, cury, curx, message);
  //Need to improve to allow multiline wrapping  
  mvwchgat(window, cury, curx, len, attr, colorpair, NULL);
}

// void editorSetStatusMessage(const char *fmt, ...){
//   va_list ap;
//   va_start(ap, fmt);
//   vsnprintf(E.statusmessage, sizeof(E.statusmessage), fmt, ap);
//   va_end(ap);
//   E.statusmsg_time = time(NULL);
// }


// //Render title
// mvwprintw(todoList, 0, 1, "%s", title);
// mvwchgat(todoList, 0, 1, strlen(title), A_NORMAL, TITLE, NULL);