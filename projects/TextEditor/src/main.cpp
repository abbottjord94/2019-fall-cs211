#include "curses.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
/*

		****FUNCTIONS****
main(int, char**), returns int:
	-main function for the program.
	-only parses the arguments and initializes the screen.

init_terminal(), void function:
	-initializes the terminal and handles the text editor.

parse_args(int, char**), returns vector<string>:
	-parses the arguments and places them into a vector for later processing.

print_usage(), void function:
	-prints usage details to the terminal.

quit_program(), void function:
	-quits the program
*/

vector<string> parse_args(int arg_count, char* args[]);
void init_terminal(vector<string> args);
void print_usage();
void quit_program();

//Main function
int main(int argc, char* argv[]) {

	init_terminal(parse_args(argc,argv));
	quit_program();
	return 0;
}

//Still need to fix this so the arguments actually get parsed properly.
vector<string> parse_args(int arg_count, char* args[]) {
/*
		****VARIABLES IN FUNCTION parse_args()****

	-arg_count (int): the number of arguments.
	-args (char**): the arguments themselves
	-parsed_arguments (vector<string>): the arguments placed into a vector
*/

	vector<string> parsed_arguments;

	if(arg_count == 1) {							//User only started the program with no args, so return an empty vector
		return parsed_arguments;
	}
	else {
		for(int i=1; i<arg_count; i++) {
			parsed_arguments.push_back(args[i]);
		}
		return parsed_arguments;
	}
}

void init_terminal(vector<string> args) {

	/*
			****VARIABLES IN FUNCTION init_terminal()****
		main_window (WINDOW*): the main window (initialized to NULL)
		num_cols (int): the maximum number of columns
		num_rows (int): the maximum number of rows
		col (int): the current column number
		row (int): the current row number
		input (char): the character that was last grabbed from the keyboard
		quit (bool): a true/false value to indicate if the user wanted to quit
		file_buf(string): the file buffer, which will store the contents of the file before it's saved.
		args (vector<string>): arguments parsed before the terminal was started

	*/

	WINDOW* main_window = NULL;
	int num_cols = 0, num_rows = 0, row = 1, col = 0;
	char input = NULL;
	bool quit = false;
	string file_buf = "";
	string filename;
	//string title = "TX Text Editor v0.1\tFile: " + filename;
	vector<string> document;

	//We'll parse the arguments first

	if(args.size() == 0) {						//For now we'll require the user to enter a file name, so we'll print usage and quit.
		print_usage();
		return;
	}
	else {
		for(int i=0; i<args.size(); i++) {
			if(args[i].front() == '-') {
				if(args[i] == "-h") {		//Checking for the help flag
					print_usage();
					return;
				}
				else if(args[i] == "--help") {
					print_usage();
					return;
				}
				else {
					cout << "Invalid argument: " << args[i] << endl;
					print_usage();
					return;
				}
			}
		}
		filename = args.back();					//We assume the filename is the last argument the user gave
	}

	//This is where the magic happens

	main_window = initscr(); 					//initialize screen to main window
	getmaxyx(main_window, num_rows, num_cols); 			//get the max X and Y values in the terminal
	resize_term(num_rows - 1, num_cols - 1); 			//resize the terminal
	noecho(); 							//disable character echo
	keypad(main_window, TRUE);
	curs_set(TRUE);	 						//Show the cursor

	//BEGIN PROGRAM LOGIC:

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);				//Inverted color setting
	init_pair(2, COLOR_WHITE, COLOR_BLACK);				//Normal color setting

	attron(COLOR_PAIR(1));						//GUI Stuff
	mvaddstr(0,0,"TX Text Editor v0.1");				//Title at the top (add filename here at some point)
	mvaddstr(num_rows-2,0,"^C: Close\t^O: Write to file");		//Short list of commands at the bottom
	attroff(COLOR_PAIR(1));
	refresh();
	document.push_back(string());					//Used as a buffer since the title will take one row - this way, we won't need to offset the row variable later
	document.push_back(string());
	while(!quit) {
		input = getch();
		if(input == 7) {						//7 is the backspace character defined on this machine

			if(col > 0) col--;					//Ensures that the column number does not go below 0.

			//if(document[row][col-4] == '\t') {			//More tab handling stuff
			//	document[row].pop_back();
			//	col -= 4;
			//}

			else if(col == 0 && row > 1) {				//If the beginning of a row is reached, go to the row above it(if available)
				row--;						//Go backwards one row
				col = document[row].length() - 1;		//Go to the end of the line, with an offset for the newline '\n' character.
			}
			if(!document[row].empty()) {				//As long as the current line isn't empty (this caused some really funny Heartbleed-like errors before I implemented this)
				document[row].pop_back();			//Remove the last character from the document
			}
			mvdelch(row,col);					//Delete the character at this location
		}
		else if(input == 13 || input == 10) { 				//13 or 10 are the carriage return characters
			document[row].push_back('\n');
			row++;							//Moves to the next row
			col = 0;						//Sets the column number to 0
			document.push_back(string());
			refresh();						//Redraw the screen
		}
		else if(input == 9) {						//Handling tabs (needs work)
			//document[row].push_back('\t');			//Push back the tab character
			//mvaddstr(row,col,"    ");				//Add the tab character to the screen
			//col+=4;
			refresh();
		}
		else if(input == 15) {						//When the user presses Ctrl+O to save the file
			ofstream outfile(filename);				//Create the output file stream
			for(int i=1; i<document.size(); i++) {			//Loop through each line of the document
				file_buf += document[i];			//Append the line to the file buffer
			}
			outfile << file_buf;					//Write the file buffer to the file output buffer
			outfile.close();					//Close the file
		}
		else if(input != NULL) {
			document[row].push_back(input);
			mvaddch(row,col,input);					//For every other input, just print the character to the terminal and move to the next column.
			col++;							//Move to the next column
			refresh();
		}
		refresh();							//Redraw everything
	}

	//END PROGRAM LOGIC

	//Magic to quit the program
	input = getch();
	quit_program();
	return;
}

void print_usage() {
	cout << "TX Text Editor v0.1 Help\n";
	cout << "Usage: ./tx [args][filename] - program assumes last argument is the filename. User MUST give a filename.\n";
	cout << "Arguments:\n";
	cout << "\t--help, -h: Show this help page and quit\n";
	return;
}


//Quits the program
void quit_program() {
	endwin();
	return;
}
