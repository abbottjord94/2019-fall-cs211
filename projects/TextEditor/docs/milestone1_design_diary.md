# Design Diary (Milestone 1)
This milestone primarily focused on the initial design of the program and some of its basic functionality. In addition to a graphical user interface (GUI), most of the basic keyboard handling, usage details, and some command line arguments parsing were implemented.

The grahpical user interface is intended to be minimalistic, similar to those found in GNU NANO or VIM. It will consist only of a title, file name, line/column counters, a list of key commands, and a saved/modified indicator flag. All GUI components will be printed to the screen with a white background and black text (inverse of the normal text color).

During this milestone, keyboard handling was a primary concern after the creation of the GUI. This can be challenging as not all keys will output a character to the screen, but have other functionality (Backspace, Return, CTRL+C to close, or CTRL+O to save). In addition, the characters printed to the screen needed to be saved in another buffer in order to save the file data to the hard drive.

To solve this, a series of if-else statements check for the pre-defined keystrokes ASCII values as they are entered into the keyboard. If one of the special keys is pressed, the program will execute a separate routine instead of printing the character to the screen. 

The document itself is stored in a vector<string>, where each entry in the vector represents one line of the text. This makes it easier to correlate the row and column values with that location in the document (which can be accessed as document[row][col]). Vectors also include various functions that make editing them much easier. In addition, it is much easier to read in files by using the std::getline() function when the file is stored this way. I found this to be the most satisfying solution in this milestone, as it will make implementing later functionality much easier.
  
One problem that has not been solved yet is the handling of the Tab key. Because the tab represents eight spaces (or sometimes four), this would create a mismatch between the column counter and the document, as the tab character '\t' only takes one space in the string. One solution may be to include a column offset value on each row, which could indicate how many tabs are present in the line. When one is encountered, the program can be written to handle those in a specific way.
