# Milestone 5

For this milestone, a trie was implemented to add a keyword autocomplete feature. A keywords text file is used to populate a trie, which can be searched for a key word from a partial word that the user enters. This structure is far less complex to search across when compared to searching a list. 

The primary challenge of this milestone was understanding the trie structure and completing the necessary functions for the feature to work. In this case, the addWord and search functions were implemented. The search function is recursive, and has a helper function called "traverseSubtree" that acts as the recursive call.

Once these functions were implemented and tested, it had to be integrated into the final program. This turned out to be relatively simple, however many bugs are still present in this implementation. Currently, the user must press Ctrl-A to use the autocomplete function. The program will then return the first match from the trie and enter it on the screen.

A gif demonstrating this feature can be seen here: 

![trie](milestone5.gif)
