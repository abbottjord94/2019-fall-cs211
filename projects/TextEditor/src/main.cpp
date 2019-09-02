#include "curses.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
/*

		****FUNCTIONS****
main(int, char**), returns int:
	-main function for the program

search_back(string, string::iterator), returns int
	-used to search a string backwards for either the string's beginning or the last newline ('\n') character.
	-returns the number of characters from the iterator argument's position to the next newline or the beginning of the string
*/

int search_back(string& s, string::iterator cursor_pos);

int main(int argc, char* argv[]) {

	/*
			****VARIABLES IN FUNCTION main()****
		main_window (WINDOW*): the main window (initialized to NULL)
		num_cols (int): the maximum number of columns
		num_rows (int): the maximum number of rows
		col (int): the current column number
		row (int): the current row number
		input (char): the character that was last grabbed from the keyboard
		quit (bool): a true/false value to indicate if the user wanted to quit
		file_buf(string): the file buffer, which will store the contents of the file before it's saved.

	*/

	WINDOW* main_window = NULL;
	int num_cols = 0, num_rows = 0, row = 0, col = 0;
	char input = NULL;
	bool quit = false;
	string file_buf = "";
	string::iterator file_buf_cursor = file_buf.begin();

	//This is where the magic happens

	main_window = initscr(); 			//initialize screen to main window
	getmaxyx(main_window, num_rows, num_cols); 	//get the max X and Y values in the terminal
	resize_term(num_rows - 1, num_cols - 1); 	//resize the terminal
	noecho(); 					//disable character echo
	keypad(main_window, TRUE);
	curs_set(TRUE);	 				//Show the cursor

	//BEGIN PROGRAM LOGIC:

	while(!quit) {
		input = getch();
		if(input == 7) {						//7 is the backspace character defined on this machine
			if(col > 0) col--;					//Ensures that the column number does not go below 0.
			else if(col == 0 && row > 0) {				//If the beginning of a row is reached, go to the row above it(if available)
										//TO DO: Fix this so that it goes to the last column of that row.
				row--;						//Go backwards one row
				file_buf.pop_back();				//Remove the last character from the file buffer
				col = search_back(file_buf, file_buf_cursor);	//Use the search_back function to find what column the cursor needs to be set at.
			}
			if(!file_buf.empty()) {					//As long as the file buffer isn't empty (this caused some really funny Heartbleed-like errors before I implemented this)
				file_buf.pop_back();				//Remove the last character from the file buffer
				file_buf_cursor = file_buf.end();		//Set the file buffer cursor to the end of the file buffer (TO DO: Change this to allow editing in the middle of the file)
			}
			mvaddch(row,col,' ');					//Add a space character, this actually removes the last character from the terminal for some reason (this does not add the space to the file buffer)
		}
		else if(input == 13 || input == 10) { 				//13 or 10 are the carriage return characters
			row++;							//Moves to the next row
			col = 0;						//Sets the column number to 0
			file_buf += '\n';
			refresh();						//Redraw the screen
			file_buf_cursor = file_buf.end();
		}
		else if(input != NULL) {
			file_buf += input;					//Append the input character to the end of the file buffer
			mvaddch(row,col,input);					//For every other input, just print the character to the terminal and move to the next column.
			col++;							//Move to the next column (this will need to be changed to handle columns above the maximum)
			file_buf_cursor = file_buf.end();			//Move the file buffer cursor to the end (TO DO: Change this to allow editing in the middle of the file)
		}
		refresh();							//Redraw everything
	}

	//END PROGRAM LOGIC

	//Magic to quit the program
	input = getch();
	endwin();
	return 0;
}

int search_back(string& s, string::iterator cursor_pos) {
	/*
			****VARIABLES IN FUNCTION search_back(string, string::iterator)****
		col_num (int): the return value, used as a counter for the number of columns until the beginning of the string or the last newline ('\n') character is found
		cursor_pos(string::iterator, argument) - the current cursor position of the file_buf string in main()
		s (string&, argument) - the file_buf string itself

	-

	*/
	int col_num = 0;
	while(cursor_pos != s.begin() && *cursor_pos != '\n') {		//Continue the loop as long as a newline or the string's beginning is not found
		cursor_pos--;						//Move the file_buf cursor backwards one character
		col_num++;						//Increment the column counter
	}
	cursor_pos = s.end();						//Reset the cursor position to the end (TO DO: Change this to allow editing in the middle of the file)
	return col_num;							//Return the column number
}
