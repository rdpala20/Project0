/*
About this program:
- This program counts words.
- The specific words that will be counted are passed in as command-line
  arguments.
- The program reads words (one word per line) from standard input until EOF or
  an input line starting with a dot '.'
- The program prints out a summary of the number of times each word has
  appeared.
- Various command-line options alter the behavior of the program.

E.g., count the number of times 'cat', 'nap' or 'dog' appears.
> ./main cat nap dog
Given input:
 cat
 .
Expected output:
 Looking for 3 words
 Result:
 cat:1
 nap:0
 dog:0
*/
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smp0_tests.h"

/* Structures */
typedef struct {
  char *word; //contains the word
  int counter; //contains the number of times a word appears
} WordCountEntry; //name of struct

//Function: Obtains entry strings and count of entries
int process_stream(WordCountEntry entries[], int entry_count)
{
  short line_count = 0; //counter initialized to zero
  char buffer[30]; //space designated for words
  char *token; //variable to designate each word

//reads the input
  while(fgets(buffer,sizeof(buffer),stdin)!= NULL) {
    if (*buffer == '.') // if it reached the end
       break;
    //declare token to equal word until space, tab or newline
    token = strtok(buffer, " \t\n");
    //loops until it reaches the end of the list of words
    while (token != NULL){
      //loop though separate entries to see if they match and add to counter if they do
     for (int i=0; i < entry_count; i++) {
       if (!strcmp(entries[i].word, token))
        entries[i].counter++;
    }
     //initialize token to be NULL once finished looping
     token = strtok(NULL, " \t\n"); //
  }
  //incrment
  line_count++;
}
  return line_count;
}

//Function:Prints the result of each specific entry using a loop
void print_result(WordCountEntry entries[], int entry_count, FILE * output)
{
  unsigned int i; //declare variable as an unisgned int
  fprintf(output,"Result:\n"); //prints the result in stdout

  //prints each entry with its specific cunter looping through them using indexes
  for (i = 0; entry_count> i; i++) {
   fprintf(output,"%s:%d\n", entries[i].word, entries[i].counter);
  }
}

//Function: Prints for help to standard error
void printHelp(const char *name)
{
  fprintf(stderr,"usage: %s [-h] [-fFILENAME] <word1> ... <wordN>\n", name);
}

// Obtains argument counter(int) and arrray of strings as a double ptr
int main(int argc, char **argv)
{
  const char *prog_name = *argv; //declare contant to equal the argument

  WordCountEntry *entries; //declare struct pointer entries
  int entryCount = 0; // initialaize entrycunt to zero
  FILE *output = stdout; // initialize file output to standard output

  /* Entry point for the testrunner program */
  if (argc > 1 && !strcmp(argv[1], "-test")) {
    run_smp0_tests(argc - 1, argv + 1);
    return EXIT_SUCCESS;
}

 //process to generate more space dpending on amount of words inputted using malloc
 if((entries = malloc(sizeof(WordCountEntry) * (argc - 1))) == NULL){
 return EXIT_FAILURE;
}
  //increment argv to 1, ignoring the ./main argument
  argv+=1;
  // loops until there is no argument
  while (*argv != NULL) {
    //checks for dash character in the arguments
    if (**argv ==  '-') {
      //uses switch to check if after the dash there is a letter
      switch ((*argv)[1]) {
       // if its " -f" creates file for output
       case 'f':
       output = fopen(*argv+2,"w"); // creates file with argument name and writes binary
        //wasnt able to create/find the txt file
        if(output  == NULL){
          fprintf(stderr, "Cannot open file \"%s\".n", *argv +2);
          free(entries);
          return EXIT_FAILURE;
        }
          // escapes the rest of the loop
          break;
         // if it is "-h" asks for help
         case 'h':
          printHelp(prog_name); //use function that prints help statement
          return EXIT_FAILURE; // eits the program to stop it from keeping it going
        // if none of the 2 cases are satisfied print the following arguments in stderr
        default:
         fprintf(stderr,"%s: Invalid option %s. Use -h for help.\n", prog_name, *argv);
         fprintf(stderr,"%s: Please supply at least one word. Use -h for help.\n", prog_name);
      }

    }
    else {
      // checks if count is less than number of arguments passed
      if (entryCount < argc-1) {
        entries[entryCount].word = *argv; // initialize argument to specifc index
        entries[entryCount++].counter = 0; // initialize counter to zero
      }
    }
    //move on to the next argument and keeps on looping
    argv++;
  }
 // if the argument passed is just ./main print statement and exit
 if (entryCount == 0) {
    //print in stderr that a word is needed
    fprintf(stderr,"%s: Please supply at least one word. Use -h for help.\n", prog_name);
        //clear up memory
        free(entries);
        //closes the output
        if(output !=stdout){
        fclose(output);
        }
    return EXIT_FAILURE;
  }
// if there is only one word passed, then it specifies that
if (entryCount == 1) {
   fprintf(output,"Looking for a single word\n");
  }
// multiple words being passed
 else {
   fprintf(output,"Looking for %d words\n", entryCount);
  }
  //declares function to loop though each word and compare them
  process_stream(entries, entryCount);
  //declares function to loop through the specifc values and print them
  print_result(entries, entryCount, output);

  //clean up
 free(entries);
 if (output!= stdout){
 fclose(output);
 }

  return EXIT_SUCCESS;
}
