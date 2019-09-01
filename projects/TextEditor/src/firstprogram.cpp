#include "curses.h"
#include <ctime>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	WINDOW* main_window = NULL;
	int num_cols = 0, num_rows = 0, row = 0, col = 0;
	char input = NULL;
	bool quit = false;
	main_window = initscr();
	getmaxyx(main_window, num_rows, num_cols);
	resize_term(num_rows - 1, num_cols - 1);
	noecho();
	keypad(main_window, TRUE);
	curs_set(TRUE);

	//BEGIN PROGRAM LOGIC:
	while(!quit) {
		input = getch();
		if(input == '\b') {
			col--;
		}
		else if(input == 13 || input == 10) {
			row++;
			col = 0;
			refresh();
		}
		else if(input != NULL) {
			mvaddch(row,col,input);
			col++;
		}
		refresh();
	}

	//END PROGRAM LOGIC

	input = getch();
	endwin();
	return 0;
}
