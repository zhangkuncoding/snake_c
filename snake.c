#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#include "snake.h"

#define NUM 60

int ch = 0; // to record the input command
int hour = 0, minute = 0, second = 0;
int scores = 0; 
int timer = 0; 
int level = 0;
struct Direct dir; // The direction of the snake
struct Direct food; // the position of the food
struct Node *head, *tail;

/* that structure is for the snake 
   A double list:
		when it is moving insert a node after head 
		               && delete a node before tail
 */
void delete_node_bofore_end(){
	struct Node *temp = tail->prev;
	struct Node *b_temp = temp->prev;
	b_temp->next = tail;
	tail->prev = b_temp;
	temp->next = NULL;
	temp->prev = NULL;

	free(temp);
	temp = NULL;
}

void create_dlist(){
	struct Node *temp = malloc(sizeof(struct Node));
	head = malloc(sizeof(struct Node));
	tail = malloc(sizeof(struct Node));

	// the begin position
	temp->m_x = COLS / 2;
	temp->m_y = LINES / 2;
	
	head->prev = NULL;
	tail->next = NULL;
	head->next = temp;
	temp->next = tail;
	tail->prev = temp;
	temp->prev = head;
}

void insert_node_after_head(int x, int y){
	struct Node *temp = malloc(sizeof(struct Node));
	
	temp->m_x = x;
	temp->m_y = y;
	
	temp->next = head->next;
	head->next = temp;
	temp->prev = head;
	temp->next->prev = temp;
}

void delete_dlist(){
	while(head->next != tail){
		delete_node_bofore_end();
	}

	head->next = NULL;
	tail->prev = NULL;

	free(head);
	free(tail);
}

void init_game(){
	// input will immediate response
	cbreak();

	// input don't appear on the screen
	noecho();

	// Make the cursor invisible
	curs_set(0);
	
	// catch the direction of the keypad input
	keypad(stdscr, true);

	// set a random seed
	srand(time(NULL));

	hour = minute = second = timer = 0;
    scores = 0;
    dir.m_x = 1;
    dir.m_y = 0;
    ch = 'A';

	// set the food randomly
    food.m_x = rand() % COLS;
    food.m_y = rand() % (LINES-2) + 2;

    create_dlist();    
	
	//set_ticker(20);
}

/*
   This is a clock program. 
   As soon as this function is called, 
   the frequency of the wake-up function 
   in the signal can be specified
 */
int set_ticker(int n_msecs){
	struct itimerval new_timeset;
	long n_sec = 0, n_usecs = 0;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;
	new_timeset.it_interval.tv_sec  = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    n_msecs = 1;
    n_sec = n_msecs / 1000 ;
    n_usecs = ( n_msecs % 1000 ) * 1000L ;
    new_timeset.it_value.tv_sec     = n_sec  ;
    new_timeset.it_value.tv_usec    = n_usecs ;
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
//#include<sys/time.h> 
//setitimer(int which, const struct itimerval* newval, 
//          struct itimerval *oldval);
//which: type of time ITMER_REAL, ITIMER_VIRTUAL, ITIMER_PROF.}
}

void show_information(){
	timer++;
	// Prevent overflow
	if(timer >= 1000000){
		timer = 0;
	}
	// continue until cycle ends 
	if(1 != timer % 50){
		return;
	}

	mvprintw(0,0, "time: %02d:%02d:%02d", hour, minute, second);
	
	++second;

	if(second > NUM){
		second = 0;
		++minute;
	}

	if(minute > NUM){
		minute = 0;
		++hour;
	}

	int i = 0;
	for(i=0; i < COLS; ++i){
        mvaddstr(1, i, "-");  
	}

    mvprintw(0, COLS / 2 - 5, "HAVE FUN!");  
    
	mvprintw(0, COLS - 10 , "scores: %02d", scores); 

	level = (scores + 1) / 3 + 1;
}

void show_snake(){
	// the speed of snake will increase
	if(1 != timer % (30 - level)){ 
        return;  
	}

    // judge the scores of snake whether change 
    bool lenChange = false;

    // print the food   
    mvprintw(food.m_y, food.m_x, "@");

    // if snake arrive the wall 
    if((COLS-1 == head->next->m_x && 1 == dir.m_x)  
        || (0 == head->next->m_x && -1 == dir.m_x)  
        || (LINES-1 == head->next->m_y && 1 == dir.m_y)  
        || (2 == head->next->m_y && -1 == dir.m_y)){
        over(1);  
        return;  
    }  

    // if crash itself
    if('*' == mvinch(head->next->m_y + dir.m_y, 
				     head->next->m_x + dir.m_x)){  
        over(2);  
        return;  
    }  

	insert_node_after_head(head->next->m_x+dir.m_x, 
			               head->next->m_y+dir.m_y);  
    // if catch food  
    if(head->next->m_x == food.m_x && 
	   head->next->m_y == food.m_y){  
        
		// when catch food, make a voice
		beep();

		lenChange = true;  
        ++scores;  
		
		// target is competed
		if(scores >= 5){  
            over(3);  
            return;  
        }

        //reset the food randomly  
        food.m_x = rand() % COLS;  
        food.m_y = rand() % (LINES - 2) + 2;  
    }  
	
    if(!lenChange)  {  
        mvprintw(tail->prev->m_y, tail->prev->m_x, " ");  
        
		delete_node_bofore_end();  
    }  

    mvprintw(head->next->m_y, head->next->m_x, "*"); 
}

void show(){
	signal(SIGALRM, show);

	show_information();
	show_snake();
	
	refresh();
}

void get_order(){
	/* Pressing the button opposite to the running direction is OK */
	while(1){
        ch = getch();
        if(KEY_LEFT == ch && dir.m_x != 1){
            dir.m_x = -1;
            dir.m_y = 0;
        } else if(KEY_UP == ch && dir.m_y != 1){
            dir.m_x = 0;
            dir.m_y = -1;
        } else if(KEY_RIGHT == ch && dir.m_x != -1){
            dir.m_x = 1;
            dir.m_y = 0;
        } else if(KEY_DOWN == ch && dir.m_y != -1){
            dir.m_x = 0;
            dir.m_y = 1;
        }
        
	set_ticker(20);
    }  
}

void over(int error_num){
    move(0, 0);  
    int j = 0;  
    for(j = 0; j < COLS; j++){  
        addstr(" ");  
	}

    move(0, 2);  
	switch(error_num){
		case 1:  
			addstr("Crash the wall. Game over");  
			break;
		case 2:  
			addstr("Crash youeself. Game over");  
			break;
		case 3:  
			addstr("Mission Complete");  
			break;
	}
    
	set_ticker(0); //close the timer  
    delete_dlist(); 
}
