/*
 *  ordered_linked_list.c -- this file implements an ordered linked list.
 *
 *  Copyright (C) 2006 Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

/* standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* my includes. */
#include "program_typedefs.h"
#include "program_structures.h"

/* function prototypes. */
static char *save_string (unique);
LIST *init_list (void);
void free_list (LIST *);
bool enter (LIST *, unique, cint);
void print_line (FILE *, LIST *, cint);
static void memory_error (FILE *, cint);

/* global variables. */
static unique out_of_memory = "Error: Out of memory.";
static unique default_line = "A list of lines of chars." "\n";

/* functions. */
/* the following function prints an error message
   as far as memory allocation error is concerned. */
static void
memory_error (FILE * stream, cint exit_code)
{
  /* print the error. */
  fprintf (stream, "%s\n", out_of_memory);

  /* exit with the appropriate error code. */
  exit (exit_code);
}

/* the following function allocates enough memory
   space in order to save `string'. after that it
   returns the address of the space allocated. */
static char *
save_string (unique string)
{
  char *new_string;

  /* allocate enough memory space. */
  new_string = (char *) malloc (strlen (string) + 1);

  /* allocation failed? */
  if (new_string == NULL)
    memory_error (stderr, 8);

  /* save and return. */
  strcpy (new_string, string);
  return (new_string);
}

/* the following function allocates enough memory
   space in order to create a `line' node. after
   that the function initializes and return it. */
LIST *
init_list (void)
{
  LIST *new_list;

  /* allocate enough memory space. */
  new_list = (LIST *) malloc (sizeof (LIST_NODE));

  /* allocation failed? */
  if (new_list == NULL)
    memory_error (stderr, 8);

  /* initialize the list. */
  new_list->index = 0;
  new_list->line = save_string (default_line);
  new_list->next_ptr = NULL;

  /* return the address of the list. */
  return new_list;
}

/* the following function deallocates all the
   nodes from the list given. when all nodes
   are deallocated it returns. */
void
free_list (LIST * linked_list)
{
  while (linked_list != NULL)
    {
      LIST_NODE *next = linked_list->next_ptr;
      free (linked_list->line);
      free (linked_list);
      linked_list = next;
    }
}

/* the following function adds a new node to an
   ordered linked list. each node contains a line
   of chars and a line index which specifies it.
   also, it contains a pointer to the next node. */
bool
enter (LIST * first_ptr, unique line, cint index)
{

  int result;                   /* is used for some error return checks. */
  int i;                        /* is used for a loop dealing with the line. */

  /* is there any none space character. */
  int any_space_char = 0;

  LIST_NODE *before_ptr;
  LIST_NODE *after_ptr;
  LIST_NODE *new_node_ptr;

  /* either accept or ignore the line. */
  for (i = 0; i < strlen (line); i++)
    {
      /* check to see if the first char is a comment one? */
      /* if it is, we ignore it becase it's a comment. */
      if (line[i] == '#' || line[i] == '!')
        return false;

      /* have we found at least one none space character? */
      /* if we have then we accept the line as simple sentence. */
      if (!isspace (line[i]))
        {
          any_space_char = 1;
          break;
        }
    }

  /* is there any none space char in the line? */
  if (!any_space_char)
    return false;

  /* try to enter the line. */

  before_ptr = first_ptr;
  after_ptr = before_ptr->next_ptr;

  /* ordering. */
  while (true)
    {
      /* end of list or empty list? */
      if (after_ptr == NULL)
        break;

      /* the node has the same line index?
         if true entering is not necessary. */
      if (after_ptr->index == index)
        return false;

      result = strcmp (after_ptr->line, line);

      /* the node has the same line of chars?
         if true entering is not necessary. */
      if (result == 0)
        return false;

      if (after_ptr->index > index)
        break;

      after_ptr = after_ptr->next_ptr;
      before_ptr = before_ptr->next_ptr;
    }

  /* create a new node in order to save the elements. */
  new_node_ptr = (LIST_NODE *) malloc (sizeof (LIST_NODE));
  new_node_ptr->index = index;
  new_node_ptr->line = save_string (line);

  /* update pointers. */
  before_ptr->next_ptr = new_node_ptr;
  new_node_ptr->next_ptr = after_ptr;

  /* a new node added. */
  return true;
}

/* the following function searches an ordered
   linked list and prints the node where its
   line index is equal to `index'. */
void
print_line (FILE * stream, LIST * first_ptr, cint index)
{
  LIST *cur_ptr;

  for (cur_ptr = first_ptr; cur_ptr != NULL; cur_ptr = cur_ptr->next_ptr)
    if (cur_ptr->index == index)
      fprintf (stream, "%s", cur_ptr->line);
}
