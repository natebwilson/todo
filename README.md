# WilsonToDo

This is the first solo project I've built in C. I was inspired to make this after completing the famous [kilo.c text editor](https://viewsourcecode.org/snaptoken/kilo/). This program is a bit more high level - I decided to use ncurses as a more convenient graphics wrapper. This served two purposes:
1. Making the program easier to read for humans
2. Saving myself (a human) from the pain of incessant escape sequence usage
It's a very straightforward todo app - tasks can be added/deleted and have both a name and description. There is also an option menu, which currently only allows the user to adjust the size of the different windows in the app, but I am planning to add custom color schemes and maybe some other features if I can think of. Current issues/limitations include:
1. Program can only be executed in the project directory. The program saves to two files, tasks.txt and settings.txt, but these are hardcoded to be found in the directory and I will only improve this when I can come up with a portable way to have constant file paths across systems. I don't expect anybody to use this - it is simply a toy project.
2. settings.txt needs to exist in order to boot. This issue doesn't exist with tasks.txt, I have just simply not gotten around to fixing this ¯\\_(ツ)_/¯

## Project structure
main.c - Entry point of program, contains main()  
ui.c - Heavy lifting of the program, handles drawing the interfaces as well as handling most user input  
todo.c - Functions that manage tasks on a higher level - just add/delete for now  
file.c - Handles file I/O as well as loading the options/tasks into the program  
utils.c - Contains wrapper functions for ncurses  

The header files contain most of the structs/enums, but these are a little disorganized - which I will blame on my being a novice C developer :)

## Personal Notes
This being my first 'solo' foray into C, I was a little nervous/apprehensive of all the memory management that is incumbent upon the programmer. I took a lot of notes from the string/buffer handling in kilo.c, but I still had to alter a lot of these implementations to better support my Task structs. Though I feel generally accomplished for simply completing the project, there are a few things I am particularly proud of achieving:
1. This is a multifile program, while kilo.c (from which this project was inspired) is not. Though I quickly figured out most of the relevant details and made it work, I am still unsure if I declare all my data types/functions/structures in the "best" way for C.
2. I wrote a few wrapper functions to make ncurses a little less tedious, found in utils.c. These include a version of getstr() which provides a dynamically allocated buffer, and a print command which also accepts attribute and color pair arguments.
