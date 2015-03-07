/*
 * gxrm.c -- the body of the program.
 *
 *       The GNU xrm program is a program which prints a
 *       random message each time you run it (X version).
 *
 *       However, it supports some useful options.
 *
 *       See Texinfo manual page for more information.
 *
 * Copyright (C) 2006 Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

/* standard includes. */
#include <gtk/gtk.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* my includes. */
#include "program_typedefs.h"
#include "program_messages.h"

/* local defines. */
#define MAX_LINE (1024 * 1024)        /* 1 Mbyte. */

/* global variables. */
static const char *input_filename = NULL;
static const char *program_name = NULL;

/* externs. */
extern char *strdup (const char *);
extern char *stpcpy (char *, const char *);
extern FILE *popen (const char *, const char *);
extern int pclose (FILE *);

/* the name of the executable only. */
extern char *program_invocation_short_name;

/* function prototypes. */
static void parse_options (int, char **);
static void print_usage (FILE *, cint);

/* functions. */
/* the body of the program. */
int
main (int argc, char *argv[])
{
  /* the reading pipe edge. */
  FILE *pipe_stream = NULL;

  /* the window and the label which has the message. */
  GtkWidget *window = NULL;
  GtkWidget *label = NULL;

  /* any error produced? */
  bool error = false;

  /* current line of chars. */
  char sentence[MAX_LINE] = { 0 };

  /* the command which fetches the sentences. */
  char comm_str[MAX_LINE] = { 0 };

  /* parse all input command line options. */
  parse_options (argc, argv);

  /* input file is necessary! */
  if (input_filename == NULL)
    print_usage (stderr, EXIT_FAILURE);

  /* form the command. */
  stpcpy (stpcpy (comm_str, "gtrm -i "), input_filename);

  /* initialize gtk system. */
  gtk_init (&argc, &argv);

  /* create a window. */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  /* set some attributes for the window. */
  gtk_window_set_title (GTK_WINDOW (window), "GNU X random message (gxrm)");
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW (window), 240, 35);
  g_signal_connect (G_OBJECT (window), "destroy", gtk_main_quit, NULL);

  /* start piping. */
  pipe_stream = popen (comm_str, "r");

  /* start reading. */

  /* check to see if there is a problem. */
  if (fgets (sentence, MAX_LINE, pipe_stream) == NULL)
    {
      label = gtk_label_new (found_problem);
      error = true;
    }
  else
    {
      /* give some space on left and right. */
      char *sentence_fixed = (char *) malloc (strlen (sentence) + 1 + 10);

      strcpy (sentence_fixed, "   ");
      strcat (sentence_fixed, sentence);
      sentence_fixed[strlen (sentence_fixed) - 1] = '\0';
      strcat (sentence_fixed, "   ");

      label = gtk_label_new (sentence_fixed);

      free (sentence_fixed);
    }

  /* show the window with the message. */
  gtk_container_add (GTK_CONTAINER (window), label);
  gtk_widget_show_all (window);
  gtk_main ();

  /* close piping. */
  pclose (pipe_stream);

  return error ? EXIT_FAILURE : EXIT_SUCCESS;
}

/* the following function prints some
   usage information for the program. */
static void
print_usage (FILE * stream, cint exit_code)
{
  fprintf (stream, "%s %s %s\n", usage_part_a, program_name, usage_part_b);
  fprintf (stream, "\n%s\n", usage_part_c);
  fprintf (stream, "%s\n", reportbugs_msg);
  exit (exit_code);
}

/* the following function parses the command line. */
static void
parse_options (int argc, char **argv)
{
  /* the next option. */
  register int next_option;

  /* both short and long options. */
  unique short_options = "i:";
  const struct option long_options[] = {
    {"input", 1, NULL, 'i'},
    {"help", 0, NULL, 'h'},
    {"usage", 0, NULL, 'u'},
    {"version", 0, NULL, 'V'},
    {NULL, 0, NULL, 0}
  };

  /* fetch program's name. */
  program_name = program_invocation_short_name;

  /* start scanning. */
  do
    {
      /* get next option. */
      next_option =
        getopt_long (argc, argv, short_options, long_options, NULL);

      switch (next_option)
      {
        /* print usage and exit. */
        case 'h':
        case 'u':
          print_usage (stdout, EXIT_SUCCESS);

        /* print version information and exit. */
        case 'V':
          printf ("%s %s\n", package_msg, version_msg);
          printf ("%s %s\n", writtenby_msg, developers_msg);
          printf ("\n%s", license_part_a);
          printf ("%s\n", license_part_b);
          printf ("%s\n", license_part_c);
          printf ("%s\n", reportbugs_msg);
          exit (EXIT_SUCCESS);

        /* get input filename. */
        case 'i':
          input_filename = strdup (optarg);
          break;

        /* unknown option. */
        case '?':
          print_usage (stderr, EXIT_FAILURE);

        case -1:
          break;

        default:
          abort ();
      }
    } while (next_option != -1);
}
