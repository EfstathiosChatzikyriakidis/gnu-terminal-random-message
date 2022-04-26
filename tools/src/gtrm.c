/*
 * gtrm.c -- the body of the program.
 *
 *       The GNU trm program is a program which prints a random
 *       message when you run it. However, it supports options.
 *
 *       See Texinfo manual page for more information.
 *
 * Copyright (C) 2006 Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com)
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
#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <error.h>

/* my includes. */
#include "program_typedefs.h"
#include "program_structures.h"
#include "program_messages.h"

/* local defines. */
#define MAX_LINE (1024 * 1024)        /* 1 Mbyte. */

#define FILENAME_RANDOM ((high_randomness != false) ? "/dev/random"      : "/dev/urandom")
#define FILENAME_IN     ((input_filename  == NULL)  ? environment_input  : input_filename)
#define FILENAME_OUT    ((output_filename == NULL)  ? environment_output : output_filename)

#define WRITE_DESTINATION  ((ofp != NULL) ? ofp : stdout)
#define READ_SOURCE        ((ifp != NULL) ? ifp : stdin)

/* global variables. */
static const char *input_filename = NULL;
static const char *output_filename = NULL;

static const char *environment_input = NULL;
static const char *environment_output = NULL;

static const char *program_name = NULL;

static bool verbose = false;
static bool high_randomness = false;

static long loop_min = 0;
static long loop_max = 1;

/* linked lists. */
static LIST *list_a_ptr = NULL;

/* externs. */
extern int random_number (cint, cint, unique);
extern LIST *init_list (void);
extern void free_list (LIST *);
extern bool enter (LIST *, unique, cint);
extern void print_line (FILE *, LIST *, cint);
extern char *strdup (const char *);

/* the name of the executable only. */
extern char *program_invocation_short_name;

/* function prototypes. */
static void parse_options (int, char **);
static void print_usage (FILE *, cint);

/* functions. */
/* the body of the program. */
int
main (int argc, char **argv)
{
  FILE *rfp = NULL;                /* randomness input file pointer. */
  FILE *ofp = NULL;                /* data output file pointer. */
  FILE *ifp = NULL;                /* data input file pointer. */
  bool enter_node = true;          /* state of entered new node. */
  int max_lines = 0;               /* max lines found in the input file. */
  int random_position = 0;         /* random position for the printing. */
  char sentence[MAX_LINE] = { 0 }; /* current line of chars. */

  /* parse all input command line options. */
  parse_options (argc, argv);

  /* if there are any input files print them. */
  if ((argc != optind) && verbose)
    {
      register int i, z;

      fprintf (stderr, "%s\n", useless_arguments);
      for (z = 1, i = optind; i < argc; ++z, ++i)
        fprintf (stderr, " (%d): `%s'\n", z, argv[i]);

      /* for the interface. */
      fprintf (stderr, "\n");
    }

  /* print some verbose messages. */
  if (high_randomness && verbose)
    fprintf (stderr, "%s\n", high_random_quality);

  if ((input_filename == NULL) && (environment_input != NULL) && verbose)
    fprintf (stderr, "%s\n", use_environment_input);

  if ((output_filename == NULL) && (environment_output != NULL) && verbose)
    fprintf (stderr, "%s\n", use_environment_output);

  /* open the randomness input file. */
  if ((rfp = fopen (FILENAME_RANDOM, "r")) == NULL)
    error (EXIT_FAILURE, errno, "%s `%s'", file_read_error, FILENAME_RANDOM);
  else
    {
      /* open the appropriate input file. */
      if ((input_filename != NULL) || (environment_input != NULL))
        if ((ifp = fopen (FILENAME_IN, "r")) == NULL)
          error (EXIT_FAILURE, errno, "%s `%s'", file_read_error,
                 FILENAME_IN);

      /* open the appropriate output file. */
      if ((output_filename != NULL) || (environment_output != NULL))
        if ((ofp = fopen (FILENAME_OUT, "w")) == NULL)
          error (EXIT_FAILURE, errno, "%s `%s'", file_write_error,
                 FILENAME_OUT);

      /* print both input source and output destination. */
      if (verbose)
        {
          fprintf (stderr, "(%s `%s')\n", input_file_msg,
                   (ifp != NULL) ? FILENAME_IN : stdin_msg);
          fprintf (stderr, "(%s `%s')\n", output_file_msg,
                   (ofp != NULL) ? FILENAME_OUT : stdout_msg);

          /* for the interface. */
          fprintf (stderr, "\n");
        }

      /* initialize the linked list. */
      list_a_ptr = init_list ();

      if (verbose)
        {
          if (ifp == NULL)
            fprintf (stderr, "%s\n", use_standard_input);
          else
            fprintf (stderr, "%s\n", use_file_input);
        }

      /* start fetching lines until EOF found. */
      while ((fgets (sentence, MAX_LINE, READ_SOURCE)) != NULL)
        {
          /* try to enter a new node. */
          enter_node = enter (list_a_ptr, sentence, ++max_lines);

          /* !ok fix the index in case `enter' return false. */
          if (!enter_node)
            --max_lines;
        }

      /* no lines have been read. the file is empty. */
      if (max_lines == 0)
        {
          if (ifp != NULL)
            {
              fprintf (stderr, "%s `%s'.\n", file_is_empty, FILENAME_IN);

              /* close input file pointer. */
              fclose (ifp);
            }
          else if (verbose)
            fprintf (stderr, "%s\n", found_eof);

          /* close and remove output file. */
          if (ofp != NULL)
            {
              fclose (ofp);
              unlink (FILENAME_OUT);
            }

          /* close randomness input file pointer. */
          fclose (rfp);

          /* free the linked list. */
          free (list_a_ptr);

          return EXIT_SUCCESS;
        }

      /* reading finished. */
      if (verbose)
        {
          fprintf (stderr, "\n%s\n\n", read_ok);

          if (ofp == NULL)
            fprintf (stderr, "%s\n", use_standard_output);
          else
            fprintf (stderr, "%s\n", use_file_output);
        }

      /* get a random integer between `1' and max_lines.
         use `print_line' to print the line which has
         the same index value as random_position. */
      while (loop_max > loop_min)
        {
          random_position = random_number (1, max_lines, FILENAME_RANDOM);

          if (verbose)
            fprintf (WRITE_DESTINATION, "(%s %d)\n", line_number_msg,
                     random_position);

          /* that may not happen. but, why we are so sure? */
          assert (random_position > 0);

          /* print the sentence. */
          if (verbose)
            fprintf (WRITE_DESTINATION, "  -> ");

          print_line (WRITE_DESTINATION, list_a_ptr, random_position);

          /* for the interface. */
          if (loop_max < 1)
            fprintf (WRITE_DESTINATION, "\n");

          --loop_max;
        }

      /* writing finished. */
      if (verbose)
        fprintf (stderr, "\n%s\n\n", write_ok);

      /* close all input/output data pointers. */
      if (ifp != NULL)
        fclose (ifp);

      if (ofp != NULL)
        fclose (ofp);

      /* close randomness file pointer. */
      fclose (rfp);

      /* free the linked list. */
      free_list (list_a_ptr);
    }

  return EXIT_SUCCESS;
}

/* the following function prints some
   usage information for the program. */
static void
print_usage (FILE * stream, cint exit_code)
{
  fprintf (stream, "%s %s %s\n", usage_part_a, program_name, usage_part_b);
  fprintf (stream, "\n%s", usage_part_c);
  fprintf (stream, "%s\n", usage_part_d);
  fprintf (stream, "%s\n", usage_part_e);
  fprintf (stream, "%s\n", reportbugs_msg);
  exit (exit_code);
}

/* the following function parses the command line. */
static void
parse_options (int argc, char **argv)
{
  register int next_option;

  /* short options. */
  unique short_options = "i:o:rva:";

  /* long options. */
  const struct option long_options[] = {
    {"input", 1, NULL, 'i'},
    {"output", 1, NULL, 'o'},
    {"high-randomness", 0, NULL, 'r'},
    {"verbose", 0, NULL, 'v'},
    {"again", 1, NULL, 'a'},
    {"help", 0, NULL, 'h'},
    {"usage", 0, NULL, 'u'},
    {"version", 0, NULL, 'V'},
    {NULL, 0, NULL, 0}
  };

  /* get the command program name. */
  program_name = program_invocation_short_name;

  /* fetch both input and output environment filenames. */
  environment_input = getenv ("GTRM_INPUT");
  environment_output = getenv ("GTRM_OUTPUT");

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

        /* print version and exit. */
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

        /* get output filename. */
        case 'o':
          output_filename = strdup (optarg);
          break;

        /* enable high quality randomness. */
        case 'r':
          high_randomness = true;
          break;

        /* enable verbose mode. */
        case 'v':
          verbose = true;
          break;

        /* get how many times to repeat. */
        case 'a':
          {
            char *end;

            /* convert the argument to long. */
            loop_max = strtol (optarg, &end, 10);

            /* check to see if it is an integer. */
            if ((*end != '\0') || (loop_max <= 0))
              {
                fprintf (stderr, "%s\n", loop_max_error_a);
                fprintf (stderr, "  %s\n\n", loop_max_error_b);
                print_usage (stderr, EXIT_FAILURE);
              }
          }
          break;

        /* unknown option. */
        case '?':
          print_usage (stderr, EXIT_FAILURE);

        /* eop. */
        case -1:
          break;

        default:
          abort ();
      }
    } while (next_option != -1);
}
