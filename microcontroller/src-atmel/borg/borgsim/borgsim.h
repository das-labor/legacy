void init_curses(void)
{
	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on*/
	curs_set(0);			        /* everty thing to me 		*/
}
void end_curses(void){
	getch();
	endwin();
}

