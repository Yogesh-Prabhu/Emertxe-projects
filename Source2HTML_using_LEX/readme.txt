1. 
Directory Source_files contains source code along with executable a.out and demo C source and demo html destination files.
To re-execute, just type
	make

2.
Directory Demo_files contains sample C source file and its processed destination file.

3.
Bugs:
Due to different sytax of HTML, this code handles multiple continuous Spaces and Tabs only inside comments or outside any specific token.
This is not handled if they occur inside tokens such as Double quoted strings, Preprocessor statesments. Here multiple spaces/tabs are converted into just single space (NOT tab).
