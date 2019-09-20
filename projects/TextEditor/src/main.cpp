/*

		****TX Text Editor****
		version 0.1 (alpha)

*/

#include "curses.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define BACKSPACE_KEY 7
#define RETURN_KEY 10
#define LEFT_ARROW 4
#define RIGHT_ARROW 5
#define UP_ARROW 3
#define DOWN_ARROW 2
#define TAB_KEY 9
#define CTRL_O 15

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

delete_column(vector<string>, int, int), void function:
	-deletes the specified row

redraw_document(vector<string>, int, int, int), void function:
	-redraws the document from the given row.

redraw_column(vector<string>, int, int, int, int), void function:
	-redraws the given row from the given column.

print_usage(), void function:
	-prints usage details to the terminal.

quit_program(), void function:
	-quits the program
*/

vector<string> parse_args(int arg_count, char* args[]);
void init_terminal(vector<string> args);
void redraw_document(vector<string> doc, int r, int num_rows, int num_cols);
void redraw_column(vector<string> doc, int c, int r, int num_cols);
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
			parsed_arguments.push_back(args[i]);			//We'll push back the arguments into a vector for later processing.
		}
		return parsed_arguments;					//And return that vector so they can be accessed.
	}
}

void delete_column(vector<string> doc, int r, int c) {
	for(int i=0; i<doc[r].length(); i++) {
		mvdelch(r,i);
	}
}

void redraw_document(vector<string> doc, int r, int num_rows, int num_cols) {
	for(int i=0; i<doc.size(); i++) {
		redraw_column(doc,0,i,num_cols);
	}
}

void redraw_column(vector<string> doc, int c, int r, int num_cols) {
	for(int i=0; i<doc[r].length(); i++) {
		mvdelch(r,i);
	}
	for(int i=0; i<doc[r].length(); i++) {
		mvaddch(r,i,doc[r][i]);
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
		row_offset (int): the row offset value
		col_offset (int): the column offset value
		input (char): the character that was last grabbed from the keyboard
		quit (bool): a true/false value to indicate if the user wanted to quit
		file_buf (string): the file buffer, which will store the contents of the file before it's saved.
		args (vector<string>): arguments parsed before the terminal was started
		file_loc (string): the string displaying the current row and column at the top of the screen
		title (string): the title of the program, displayed along with the file name.
		t (const char*): the combined title and filename which will be converted to a C-string.
		document (vector<string>): the document data, with each line stored in its own index as a string

	*/

	WINDOW* main_window = NULL;
	int num_cols = 0, num_rows = 0, row = 0, col = 0, row_offset=1, col_offset=0;
	char input = NULL;
	const char* t;
	bool quit = false;
	bool hide_gui = false, word_wrap = false;
	string file_buf = "";
	string filename;
	string file_loc;
	string title = "TX Text Editor v0.1\tFile: ";
	vector<string> document;

	//We'll parse the arguments first

	if(args.size() == 0) {						//For now we'll require the user to enter a file name, so we'll print usage and quit.
		print_usage();
		return;
	}
	else {
		for(int i=0; i<args.size(); i++) {
			if(args[i].front() == '-') {
				if(args[i] == "-h" || args[i] == "--help") {					//Checking for the help flag
					print_usage();
					return;
				}
				else if(args[i] == "--hide-gui" || args[i] == "-H") {				//Checking for the hide-gui flag
					hide_gui = true;
					args.erase(args.begin()+i);
					if(args.size() == 0) {
						print_usage();
						return;
					}
				}
				else if(args[i] == "-W" || args[i] == "--word-wrap") {				//Checking for the word wrap flag
					word_wrap = true;
					args.erase(args.begin()+i);
					if(args.size() == 0) {
						print_usage();
						return;
					}
				}
				else {							//We found an undefined argument, so print the usage details and quit.
					cout << "Invalid argument: " << args[i] << endl;
					print_usage();
					return;
				}
			}
		}
		filename = args.back();							//We assume the filename is the last argument the user gave
		title += filename;
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

	if(!hide_gui) {
		attron(COLOR_PAIR(1));						//GUI Stuff
		t = title.c_str();
		mvaddstr(0,0,t);						//Title at the top (add filename here at some point)
		mvaddstr(num_rows-2,0,"^C: Close\t^O: Write to file");		//Short list of commands at the bottom
		file_loc = "Row: " + to_string(row) + " / Column: " + to_string(col) + " ";
		for(int i=0; i<file_loc.length(); i++) {
			mvaddch(0,num_cols-file_loc.length()+i,file_loc[i]);
		}
		attroff(COLOR_PAIR(1));
		refresh();
	}

	ifstream infile(filename);						//Now we'll check for that file and try to open it in the editor
	if(infile.good()) {
		while(infile.good()) {						//Checking to make sure we aren't at the end of the file
			string line;
			getline(infile, line);					//get the next line from the input file and store it in the temporary "line" variable
			document.push_back(line);				//Push the line into the document vector.
		}
		if(document.size() > num_rows-2) {				//We only want to render as many characters as we have space for, so we'll check the boundaries.
			for(int i=1; i<num_rows-2; i++) {
				for(int j=0; j<document[i-1].length(); j++) {
					mvaddch(i,j,document[i-1][j]);
				}
			}
		} else {
			for(int i=1; i<document.size(); i++) {			//If the document is smaller than the screen, we'll just render the whole thing.
				for(int j=0; j<document[i-1].length(); j++) {
					mvaddch(i,j,document[i-1][j]);
				}
			}
		}
	}
	else {
		document.push_back(string());					//Used as a buffer since the title will take one row - this way, we won't need to offset the row variable later					//This will be changed later
	}
	while(!quit) {
		input = getch();
		if(input == BACKSPACE_KEY) {					//7 is the backspace character defined on this machine
			if(col > 0) {
				col--;
				document[row].erase(document[row].begin()+col);
				redraw_column(document,col,row,num_cols);
			}
			else if(col == 0 && row > 0) {
				document[row-1] += document[row];
				document.erase(document.begin()+row);
				delete_column(document,row,col);
				row--;
				redraw_document(document,row,num_rows,num_cols);
			}
			else if(row <= 1 && col == 0) {

			}

		}
		else if(input == RETURN_KEY) { 					//13 or 10 are the carriage return characters

			document[row].insert(document[row].begin()+col,'\n');
			delete_column(document,row,col);
			row++;							//Moves to the next row
			col = 0;						//Sets the column number to 0
			if(row == document.size()-1) {
				document.push_back(string());			//If we're at the end of the document, just push back an empty string
			} else {						//Otherwise, insert an empty string where the cursor is.
				document.insert(document.begin()+row,string());
			}
			redraw_document(document,row,num_rows,num_cols);
			refresh();						//Redraw the screen

		}
		else if(input == TAB_KEY) {					//Handling tabs (needs work)
			//document[row].push_back('\t');			//Push back the tab character
			//mvaddstr(row,col,"    ");				//Add the tab character to the screen
			//col+=4;
			refresh();
		}
		else if(input == CTRL_O) {					//When the user presses Ctrl+O to save the file
			ofstream outfile(filename);				//Create the output file stream
			for(int i=1; i<document.size(); i++) {			//Loop through each line of the document
				file_buf += document[i];			//Append the line to the file buffer
			}
			outfile << file_buf;					//Write the file buffer to the file output buffer
			outfile.close();					//Close the file
		}
		else if(input == UP_ARROW) {					//Up arrow key
			if(row > 1) {
				row--;						//Move up one row
				if(col > document[row-1].length()) {		//If the current column is greater than the current row's max column, set the column to the end of the line.
					col = document[row-1].length();
				}
			}
		}
		else if(input == LEFT_ARROW) {					//Left arrow key
			if(col > 0) col--;					//Move back one column if possible
			if(col == 0 && row > 1) {				//If we have a row above us, we can go back one row
				row--;						//Same behavior as the up arrow key at this point.
				col = document[row].length();
			}
		}
		else if(input == DOWN_ARROW) {					//Down arrow key
			if(row < document.size()) {
				row++;						//Go down one row if we aren't on the last row of the document
				if(col >= document[row-1].length()) {
					col = document[row-1].length();
				}
			}
		}
		else if(input == RIGHT_ARROW) {					//Right arrow key
			if(col < document[row].length()) {
				col++;
			}
			if(col >= document[row].length()) {			//If we overflow to the next line
				row++;
				col = 0;
			}
			else if(col > document[row].length() && row < document.size()) {	//If we're at the very end of the document
				row = row;
				col = document[row].length();
			}
		}
		else if(input != NULL) {
			document[row].insert(document[row].begin()+col,input);
			mvaddch(row,col,input);					//For every other input, just print the character to the terminal and move to the next column.
			col++;
			redraw_document(document,row,num_rows,num_cols);							//Move to the next column
			refresh();
		}
		if(!hide_gui) {							//As long as the GUI isn't hidden, display the current row and column at the top-right corner of the screen
			attron(COLOR_PAIR(1));
			for(int i=0; i<=file_loc.length(); i++) {
				mvdelch(0,num_cols-file_loc.length()+i);	//Delete the entire top row
			}
			file_loc = "Row: " + to_string(row) + " / Column: " + to_string(col) + " ";
			for(int i=0; i<file_loc.length(); i++) {
				mvaddch(0,num_cols-file_loc.length()+i,file_loc[i]);
			}
			attroff(COLOR_PAIR(1));
		}
		move(row,col);
		refresh();							//Redraw everything
	}

	//END PROGRAM LOGIC

	//Magic to quit the program
	input = getch();
	quit_program();
	return;
}

void print_usage() {

	//Lines in comments are to be implemented later

	cout << "TX Text Editor v0.1 Help\n";
	cout << "Usage: ./tx [args][filename] - program assumes last argument is the filename. User MUST give a filename.\n";
	cout << "Arguments:\n";
	cout << "\t--help, -h: Show this help page and quit\n";
	cout << "\t--hide-gui, -H: Hide the graphical user interface (GUI)\n";
	cout << "\t--word-wrap, -W: Enable word wrap (disabled by default). Currently not implemented.\n";
	return;
}


//Quits the program
void quit_program() {
	endwin();
	return;
}
