/* 
   Evelyn Zhao
   UID: 117901054
   Directory ID: zevelyn
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "document.h"

/* Initialize the document (struct) to be empty by giving the document */
/*           a name and setting number_of_paragraphs to zero.          */

int init_document(Document *doc, const char *name) {
  if (doc && name && strlen(name) <= MAX_STR_SIZE) {
    strcpy(doc->name, name);
    doc->number_of_paragraphs = 0;
    return SUCCESS;
  }
  return FAILURE;
}

/* Sets number_of_paragraphs to zero. */

int reset_document(Document *doc) {
  if (doc) {
    doc->number_of_paragraphs = 0;
    return SUCCESS;
  }
  return FAILURE;
}

/* Prints the document with its name, number of paragraphs, and the */
/*              paragraphs and lines in the document.               */

int print_document(Document *doc) {
  Paragraph *paragraph;
  int i, j;

  if (doc) {
    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
      paragraph = &doc->paragraphs[i];
      
      for (j = 0; j < paragraph->number_of_lines; j++) {
	printf("%s\n", paragraph->lines[j]);
      }
      if (i < doc->number_of_paragraphs - 1) {
	printf("\n");
      }
    }
    return SUCCESS;
  }
  return FAILURE;
}

/* Adds a paragraph after the paragraph_number parameter. Paragraph */
/*      numbers start at one. If paragraph_number is zero, adds     */
/*          a paragraph to the beginning of the document.           */

int add_paragraph_after(Document *doc, int paragraph_number) {
  int i;

  if (doc && doc->number_of_paragraphs < MAX_PARAGRAPHS && 
      paragraph_number <= doc->number_of_paragraphs) {
    /* goes through the array of paragraphs and moves every paragraph */
    /*      down one index if there are already paragraphs after      */
    /*                        paragraph_number.                       */
    for (i = doc->number_of_paragraphs; i > paragraph_number; i--) {
      doc->paragraphs[i] = doc->paragraphs[i - 1];
    }
    doc->paragraphs[paragraph_number].number_of_lines = 0;
    doc->number_of_paragraphs++;
    return SUCCESS;
  }
  return FAILURE;
}

int add_line_after(Document *doc, int paragraph_number, int line_number, 
		   const char *new_line) {
  
  Paragraph *paragraph;
  int i;

  if (doc && paragraph_number <= doc->number_of_paragraphs && new_line) {
    paragraph = &doc->paragraphs[paragraph_number - 1];

    if (paragraph->number_of_lines < MAX_PARAGRAPH_LINES && 
	line_number <= paragraph->number_of_lines) {
      /* goes through the array of string and moves every line down */
      /*   one index if there are already lines after line_number.  */
      for (i = paragraph->number_of_lines; i > line_number; i--) {
	strcpy(paragraph->lines[i], paragraph->lines[i - 1]);
      }
      strcpy(paragraph->lines[line_number], new_line);
      paragraph->number_of_lines++;
      
      return SUCCESS;
    }
  }
  return FAILURE;
}

/* Changes the number_of_lines parameter to equal the number of lines in */
/*             the document at the given paragraph number.               */

int get_number_lines_paragraph(Document *doc, int paragraph_number, 
			       int *number_of_lines) {
  if (doc && number_of_lines && paragraph_number <= doc->number_of_paragraphs) {
    *number_of_lines = doc->paragraphs[paragraph_number].number_of_lines;
    return SUCCESS;
  }
  return FAILURE;
}

/* Appends a line to the end of the paragraph at paragraph_number in the */
/*                               document.                               */

int append_line(Document *doc, int paragraph_number, const char *new_line) {
  Paragraph *paragraph;

  if (doc && paragraph_number <= doc->number_of_paragraphs && new_line) {
    paragraph = &doc->paragraphs[paragraph_number - 1];

    if (paragraph->number_of_lines < MAX_PARAGRAPH_LINES) {
      strcpy(paragraph->lines[paragraph->number_of_lines++], new_line);
    }
    return SUCCESS;
  }
  return FAILURE;
}

/* Removes a line at line_number in the paragraph at paragraph_number */
/*                          in the document.                          */

int remove_line(Document *doc, int paragraph_number, int line_number) {
  Paragraph *paragraph;
  int i;

  if (doc && paragraph_number <= doc->number_of_paragraphs) {
    paragraph = &doc->paragraphs[paragraph_number - 1];

    if (line_number <= paragraph->number_of_lines) {
      /* loop through moving every line after the line_number back one */
      for (i = line_number - 1; i < paragraph->number_of_lines; i++) {
	/* if i + 1 is not past the line, else (end of paragraph), */
	/*       remove last line by making it a null byte.        */
	if (i + 1 < paragraph->number_of_lines) {
	  strcpy(paragraph->lines[i], paragraph->lines[i + 1]);
	} else {
	  strcpy(paragraph->lines[i], "\0");
	  paragraph->number_of_lines--;
	}
      }
      return SUCCESS;
    }
  }
  return FAILURE;
}

/* Loads data_lines number of lines into the document from the data */
/*    array of strings. This method calls the add_paragraph_after   */
/*       and add_line_after functions to add to the document.       */

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
		  int data_lines) {
  int i, paragraph_index = 0, line_index = 0;

  if (doc && data && data_lines > 0) {
   /* make the first paragraph to add lines to */
    add_paragraph_after(doc, 0);

    for (i = 0; i < data_lines; i++) {
      /* if there is an empty line, make a new paragraph, else, add */
      /*       lines to the paragraph at paragraph_index + 1.       */
      if (strlen(data[i]) == 0) {
	add_paragraph_after(doc, ++paragraph_index);
	line_index = 0;
      } else {
	add_line_after(doc, paragraph_index + 1, line_index++, data[i]);
      }
    }    
    return SUCCESS;
  }
  return FAILURE;
}

/* Replaces the target text found in the document with replacement. */

int replace_text(Document *doc, const char *target, const char *replacement) {
  int i, j, replacement_len, target_len;
  Paragraph *paragraph;
  char *line, *found_target;
  char end_str[MAX_STR_SIZE];

  if (doc && target && replacement) {
    replacement_len = strlen(replacement);
    target_len = strlen(target);
    
    for (i = 0; i < doc->number_of_paragraphs; i++) {
      paragraph = &doc->paragraphs[i];

      for (j = 0; j <= paragraph->number_of_lines; j++) {
	line = paragraph->lines[j];
	
	/* while the target is still found in the current line */
	while(strstr(line, target)) {
	  /* pointer to the first index of the found target */
	  found_target = strstr(line, target);
	  /* save the string after target to add after the replacement */
	  strcpy(end_str, found_target + target_len);

	  /* copy the end_str to the indices after the replacement string */
	  strcpy(found_target + replacement_len, end_str);
	  /* change the target string to replacement string */
	  strncpy(found_target, replacement, replacement_len);
	  
	  /* increment the line to after the replacement that just occurred */
	  line = found_target + replacement_len;
	}
      }
    }
    return SUCCESS;
  }
  return FAILURE;
}

/*   Highlights the target by adding the HIGHLIGHT_START_STR before &    */
/* HIGHLIGHT_END_STR after the target at every instance in the document. */
/*   This method calls replace_text with the target and the highlighted  */
/*                    text as the replacement string.                    */

int highlight_text(Document *doc, const char *target) {
  char highlighted_text[MAX_STR_SIZE];

  if (doc && target) {
    /* create the target surrounded with the highlight */
    strcat(highlighted_text, HIGHLIGHT_START_STR);
    strcat(highlighted_text, target);
    strcat(highlighted_text, HIGHLIGHT_END_STR);
    
    replace_text(doc, target, highlighted_text);
    return SUCCESS;
  }
  return FAILURE;
}

/*             Removes the target string from the document.             */
/* This method calls replce_text with the target and an empty string as */
/*                       the replacement string.                        */

int remove_text(Document *doc, const char *target) {
  char empty[] = "";

  if (doc && target) {
    replace_text(doc, target, empty);
    return SUCCESS;
  }
  return FAILURE;
}

/* Load a file into document. */

int load_file(Document *doc, const char *filename) {
  FILE *file;
  int paragraph_index = 0, line_index = 0, buf_len, i, is_blank;
  char buf[MAX_STR_SIZE + 1];
  
  if (doc && filename) {
    /* open the file to read */
    file = fopen(filename, "r");
    if (file) {
      add_paragraph_after(doc, 0);

      while (fgets(buf, MAX_STR_SIZE + 1, file) != NULL) {
	is_blank = 1; /* boolean value */
	buf_len = strlen(buf);

	for (i = 0; i < buf_len; i++) {
	  if (!isspace(buf[i])) {
	    /* is_blank becomes false if there is a non-space character */
	    is_blank = 0;
	  }
        }

	if (is_blank == 1) {
	  /* return failure if it goes over max number of paragraphs */
          if (add_paragraph_after(doc, ++paragraph_index) == FAILURE) {
            return FAILURE;
          }
          line_index = 0;
	} else {
	  buf[strcspn(buf, "\n")] = '\0';
	  add_line_after(doc, paragraph_index + 1, line_index++, buf);
	}
      }
      fclose(file);
      return SUCCESS;
    }
  }
  return FAILURE;
}

/* Save the document into the file provided. This will overwrite the */
/*                  current contents of the file.                    */

int save_document(Document *doc, const char *filename) {
  FILE *file;
  Paragraph *paragraph;
  char line[MAX_STR_SIZE + 2];
  int i, j;

  if (doc && filename) {
    /* open the file to write */
    file = fopen(filename, "w");
    if (file) {
      for (i = 0; i < doc->number_of_paragraphs; i++) {
	paragraph = &doc->paragraphs[i];

	for (j = 0; j < paragraph->number_of_lines; j++) {
	  strcpy(line, paragraph->lines[j]);
	  strcat(line, "\n"); /* append a \n at the end of each line */
	  fputs(line, file);
	}

	/* add \n after all paragraphs except last */
	if (i < doc->number_of_paragraphs - 1) {
	  fputs("\n", file);
	}
      }
      fclose(file);
      return SUCCESS;
    }
  }
  return FAILURE;
}
