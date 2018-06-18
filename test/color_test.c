#include <stdio.h>  
#include <curses.h>  
  
int main(){  
    initscr();  
	if(start_color() == OK){  
        // init
		init_pair(1,COLOR_RED,COLOR_BLACK);  
        // start color show
		attron(A_BLINK);

		// print
        move(LINES/2,COLS/2);  
        addstr("hello!"); 

		// close color show
        attroff(A_BLINK);  
        refresh();  
    }  
	
	// stop there
	getch();

	endwin();  
    return 0;  
} 
