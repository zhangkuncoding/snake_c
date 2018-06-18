#include "snake.h"
#include <signal.h>
#include <curses.h>

int main(){
	// IBM: 初始化 cursess 子例程库及其数据结构
	initscr();

	init_game();  
	
	// show() will be waken by the set_ticker()'s parameter
	signal(SIGALRM, show);  
	
	get_order();  
    
	// IBM: 终止 curses 子例程及其数据结构
	endwin();  
    
	return 0;  
}
