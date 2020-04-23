#include <stdlib.h>
#include <stdio.h>

int openFiles(char * sourceName,  char * listingName);
/*  Open the source file whose name is given in sourceName
  If listingName is not NULL open the listing file whose name is given
  in listingName
  If listingName is NULL, the output goes to stdout
  Return 1 if the file open(s) were successful, otherwise return 0
*/

void closeFiles();
/*Close the source file and the listing file if
  one was created
*/

char getNextSourceChar();
/*Return the next source char
  This function is also responsible for echoing the lines in the source
  file to the listing file (if one exists)
  The lines in the listing file should be numbered
  Return EOF when the end of the source file is reached
*/

void writeIndicator(int column);
/*Write a line containing a single ‘^’ character in the indicated column
  If there is no listing file then the current line should be echoed to
  stdout the first time (for that line) that writeIndicator or writeMessage
  is called.
  Note: 1-indexed
*/

void writeMessage(char * message);
/* Write the message on a separate line
  Call this method after the message has been fully
  parsed by getNextSourceChar (When '\n' is found at end of line)
*/

void writeError(char * message);
/* Write the error message on a separate line
*/

int getCurrentLineNum();

int getCurrentColumnNum();
