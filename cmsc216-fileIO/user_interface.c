/* 
   Evelyn Zhao
   UID: 117901054
   Directory ID: zevelyn
*/
#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include <ctype.h>
#include "document.h"
#define MAX_BUF_LEN 1024

static int find_command(Document *doc, char *buf);
static void remove_quotes(char *ptr, char *target);

/*   The main method takes command line arguments to determine  */
/*             where the user input will come from.             */

/* If there is only one argument, the function takes input from */
/*                        standard input.                       */

/*  If there are two arguments, the function takes input from a */
/*                  file provided by the user.                  */

/*   If there are any other number of arguments, the function   */
/*             prints the correct usage of the code.            */

int main(int argc, char *argv[]) {
  FILE *file;
  Document main_document;
  char buf[MAX_BUF_LEN + 1];

  init_document(&main_document, "main_document");

  /* Deciding where the input will come from: */

  /* The input is from standard input */
  if (argc == 1) {
    printf("> ");
    while (fgets(buf, MAX_BUF_LEN + 1, stdin) != NULL && 
	   find_command(&main_document, buf) != -1) {
      printf("> ");
    }

  /* The input is from a file provided by the user */
  } else if (argc == 2) {
    file = fopen(argv[1], "r");
    if (file) {
      while(fgets(buf, MAX_BUF_LEN + 1, file) != NULL && 
	    find_command(&main_document, buf) != -1);
      fclose(file);
    } else {
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      return EX_OSERR;
    }

  /* Invalid number of arguments */
  } else {
    fprintf(stderr, "Usage: user_interface\n");
    fprintf(stderr, "Usage: user_interface <filename>\n");
    return EX_USAGE;
  }
  return 0;
}

/* This function takes a Document to add to and a char * to read from. */

static int find_command(Document *doc, char *buf) {
  int paragraph_number = -1, line_number = -1;
  char command[MAX_BUF_LEN + 1] = "", extra_values[MAX_BUF_LEN + 1] = "",
    filename[MAX_STR_SIZE + 1] = "", target[MAX_BUF_LEN + 1] = "", 
    replacement[MAX_BUF_LEN + 1] = "";
  char *line, *target_ptr, *replacement_ptr, character = ' ';

  sscanf(buf, "%s", command);
  buf[strcspn(buf, "\n")] = '\0'; /* remove \n from the end */

  /* If the line is empty or is a comment */
  if (isspace(command[0]) || strcmp(command, "#") == 0 || 
      strlen(command) == 0 || command[0] == '#') {
    return 0;

    /* add_paragraph_after */
  } else if (strcmp(command, "add_paragraph_after") == 0) {
    sscanf(buf, "%s %d %s", command, &paragraph_number, extra_values);
    if (paragraph_number >= 0 && strlen(extra_values) == 0) {
      if (add_paragraph_after(doc, paragraph_number) == FAILURE) {
	printf("add_paragraph_after failed\n");
      }
    } else {
      fprintf(stdout, "Invalid Command\n");
    }

    /* add_line_after */
  } else if (strcmp(command, "add_line_after") == 0) {
    sscanf(buf, "%s %d %d %c", command, &paragraph_number, &line_number, 
	   &character);
    line = strstr(buf, "*") + 1;
    if (paragraph_number > 0 && line_number >= 0 && character == '*') {
      if (add_line_after(doc, paragraph_number, line_number, line) == FAILURE) {
	printf("%s failed\n", command);
      }
    } else {
      fprintf(stdout, "Invalid Command\n");
    }

    /* print_document */
  } else if (strcmp(command, "print_document") == 0) {
    sscanf(buf, "%s %s", command, extra_values);
    if (strlen(extra_values) == 0) {
      print_document(doc);
    } else {
      printf("Invalid Command\n");
    }

    /* quit */
  } else if (strcmp(command, "quit") == 0) {
    sscanf(buf, "%s %s", command, extra_values);
    if (strlen(extra_values) != 0) {
	printf("Invalid Command\n");
    } else {
      return -1;
    }

    /* exit */
  } else if (strcmp(command, "exit") == 0) {
    sscanf(buf, "%s %s", command, extra_values);
    if (strlen(extra_values) != 0) {
	printf("Invalid Command\n");
    } else {
      return -1;
    }

    /* append_line */
  } else if (strcmp(command, "append_line") == 0) {
    sscanf(buf, "%s %d %c", command, &paragraph_number, &character);
    line = strstr(buf, "*") + 1;
    if (paragraph_number > 0 && character == '*') {
      if (append_line(doc, paragraph_number, line) == FAILURE) {
	printf("%s failed\n", command);
      }
    } else {
      fprintf(stdout, "Invalid Command\n");
    } 

    /* remove_line */
  } else if (strcmp(command, "remove_line") == 0) {
    sscanf(buf, "%s %d %d %s", command, &paragraph_number, &line_number, 
	   extra_values);
    if (paragraph_number > 0 && line_number > 0 && 
	strlen(extra_values) == 0) {
      if (remove_line(doc, paragraph_number, line_number) == FAILURE) {
	printf("%s failed\n", command);
      }
    } else {
      fprintf(stdout, "Invalid Command\n");
    }

    /* load file */
  } else if (strcmp(command, "load_file") == 0) {
    sscanf(buf, "%s %s %s", command, filename, extra_values);
    if (strlen(filename) != 0 && strlen(extra_values) == 0) {
      if (load_file(doc, filename) == FAILURE) {
	printf("%s failed\n", command);
      }
    } else {
      fprintf(stdout, "Invalid Command\n");
    } 

    /* replace_text */
  } else if (strcmp(command, "replace_text") == 0) {
    sscanf(buf, "%s", command);
    /* get a pointer to the first quote of target string */
    target_ptr = strstr(buf, "\"");
    remove_quotes(target_ptr, target);
    /* get a pointer to the first quote of replacement string */
    replacement_ptr = strstr(target_ptr + strlen(target) + 2, "\"");
    remove_quotes(replacement_ptr, replacement);
    if (strlen(replacement) != 0 || strlen(target) != 0) {
      if (replace_text(doc, target, replacement) == FAILURE) {
	printf("%s failed\n", command);
      }
    } else {
      printf("Invalid Command\n");
    }

    /* highlight_text */
  } else if (strcmp(command, "highlight_text") == 0) {
    sscanf(buf, "%s", command);
    target_ptr = strstr(buf, "\"");
    remove_quotes(target_ptr, target);
    if (strlen(target) != 0) {
      highlight_text(doc, target);
    } else {
      printf("Invalid Command\n");
    }

    /* remove_text */
  } else if (strcmp(command, "remove_text") == 0) {
    sscanf(buf, "%s", command);
    target_ptr = strstr(buf, "\"");
    remove_quotes(target_ptr, target);
    if (strlen(target) != 0) {
      remove_text(doc, target);
    } else {
      printf("Invalid Command\n");
    }

    /* save_document */
  } else if (strcmp(command, "save_document") == 0) {
    sscanf(buf, "%s %s %s", command, filename, extra_values);
    if (strlen(filename) != 0 && strlen(extra_values) == 0) {
      if (save_document(doc, filename) == FAILURE) {
	printf("%s failed\n", command);
      }
    } else {
      printf("Invalid Command\n");
    }

    /* reset_document */
  } else if (strcmp(command, "reset_document") == 0) {
    sscanf(buf, "%s %s", command, extra_values);
    if (strlen(extra_values) == 0) {
      reset_document(doc);
    } else {
      printf("Invalid Command\n");
    }
  } else {
    printf("Invalid Command\n");
  }
  return 0;
}

/* Removes quotes from the string pointed to by the ptr & copies */
/*                   the string into target.                     */
static void remove_quotes(char *ptr, char *target) {
  strcpy(target, ptr + 1);
  target[strstr(ptr + 1, "\"") - ptr - 1] = '\0';
}
