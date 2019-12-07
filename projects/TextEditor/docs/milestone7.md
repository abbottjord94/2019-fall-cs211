# Milestone 7 (Sorting)

For this milestone, various sorting algorithms were implemented, which sorts the words in the buffer and saves them to a file. The user has the ability to choose from four different sorting options: selection sort, insertion sort, bubblesort, and quicksort, which is selected by command line argument.

Each sorting algorithm has a different mechanism for sorting data. The selection sort in this example takes advantage of the priority queue's sorting feature, which was a problem during the previous milestone (the solution in that case was to create a comparison class which caused the priority queue to compare the proper values in the unordered map). Insertion sort and Quicksort are both recursive implementations, while Bubblesort is implemented iteratively. These iterations were covered in the lectures.

A gif demonstrating selection sorting can be seen here, as well as the menu options for the various other sorting algorithms:

![milestone6](https://github.com/abbottjord94/2019-fall-cs211/blob/master/projects/TextEditor/docs/milestone7.gif)
