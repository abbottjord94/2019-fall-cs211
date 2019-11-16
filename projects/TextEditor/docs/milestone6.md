# Milestone 6 (Compression)

For this milestone, a compression feature was implemented, so that the user can choose to compress the files they've created.

To compress the text in the file, the program first creates a frequency distribution of all words in the file, delimited by either spaces, parentheses, or punctuation marks (periods, commas, colons, and semicolons). A string key is then assigned to each word, beginning with "a" for the most frequent word. Once the key reaches "z", it wraps around to "aa", and continues.  The keys and their associated words are written to a text file, named by the file name with the ".codes" extension added on. This will allow the program to correctly pick the code file for a given file, without requiring the user to specify a code file separately.

Once the code file is generated, the compress() function within the Compression.h file returns a compressed string using the file data. This is done by simply iterating over the file data, and for every word that is encountered, the key is appended to the compressed string output. This output is then written to the file, concurrent with the code file.

In addition to the alphabetical codes, a function for generating binary code files is also present in the Compression.h file, as this was specified in this assignment. Without a more advanced technique for parsing individual bits, such binary codes must be stored as strings. This is less advantageous, as the keys that are appended to the compressed string output become longer as more unique words are present in a given input; this can exceed the length of the original word in some cases, resulting in a larger output file than the given input.

While the compression feature is implemented successfully in the text editor, the decompression feature is subject to a bug which causes a crash when attempting to read in the code file. This is most likely caused by a logistical issue related to the program reading the codes from the file, rather than the decompression algorithm itself, as this was tested in a separate program.

A GIF demonstrating this feature can be seen here:

![milestone6](https://github.com/abbottjord94/2019-fall-cs211/blob/master/projects/TextEditor/docs/milestone6.gif)
