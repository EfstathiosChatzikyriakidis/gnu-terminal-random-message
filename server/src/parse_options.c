/*
 * parse_options.c -- parsing command line arguments.
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
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netinet/in.h>

/* local defines. */
#define DEFAULT_PORT 4567        /* default port. */
#define DEFAULT_HOST INADDR_ANY  /* default host. (all addresses). */

/* my includes. */
#include "program_typedefs.h"
#include "program_messages.h"

/* the name of the executable. */
static const char *program_name = NULL;

/* the input environment variable. */
const char *environment_input = NULL;

/* command line arguments. */
static options_info input_options;
options_info *fop = &input_options;

/* function prototypes. */
static void print_usage (FILE *, cint);
static void options_init (void);
void parse_options (int, char **);

/* functions. */
/* the following function is used to report the
   usage of the program and where to send bugs. */
static void
print_usage (FILE * stream, cint exit_code)
{
  fprintf (stream, "%s %s %s\n", usage_part_a, program_name, usage_part_b);
  fprintf (stream, "\n%s%s%s\n", usage_part_c, usage_part_d, usage_part_e);
  fprintf (stream, "%s\n", reportbugs_msg);

  exit (exit_code);
}

/* the following function sets default values
   for all command line arguments and options. */
static void
options_init (void)
{
  /* for the program. */
  fop->verbose = false;
  fop->input_filename = NULL;
  fop->high_randomness = false;

  /* for the server. */
  fop->port = (uint16_t) htons (DEFAULT_PORT);
  fop->local_address.s_addr = DEFAULT_HOST;
  fop->ip_timestamp = false;
}

/* the following function parses
   input command line arguments. */
void
parse_options (int argc, char **argv)
{
  /* extern necessary elements. */
  extern char *strdup (const char *);
  extern bool ascii_file (const char *);
  extern char *program_invocation_short_name;

  /* the next option to get. */
  int next_option;

  /* short options. */
  unique short_options = "i:rp:vt";

  /* long options. */
  const struct option long_options[] = {
    {"input", 1, NULL, 'i'},
    {"high-randomness", 0, NULL, 'r'},
    {"port", 1, NULL, 'p'},
    {"verbose", 0, NULL, 'v'},
    {"ip-timestamp", 0, NULL, 't'},
    {"help", 0, NULL, 'h'},
    {"usage", 0, NULL, 'u'},
    {"version", 0, NULL, 'V'},
    {NULL, 0, NULL, 0}
  };

  /* get the command program name. */
  program_name = program_invocation_short_name;

  /* fetch input environment filename. */
  environment_input = getenv ("GTRM_SERVER_INPUT");

  /* set default values. */
  options_init ();

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
          printf ("%s %s\n", written_by_msg, developers_msg);
          printf ("\n%s", license_part_a);
          printf ("%s\n", license_part_b);
          printf ("%s\n", license_part_c);
          printf ("%s\n", reportbugs_msg);
          exit (EXIT_SUCCESS);

        /* user specified port. */
        case 'p':
          {
            long value;
            char *end;

            value = strtol (optarg, &end, 10);

            /* correct port value? */
            if (*end != '\0' || value <= 0 || value <= 1024)
              {
                fprintf (stderr, "%s\n", port_limits);
                print_usage (stderr, EXIT_FAILURE);
              }

            /* the port number needs to be converted
               to network (big endian) byte order. */
            fop->port = (uint16_t) htons (value);
          }
          break;

        /* enable verbose mode. */
        case 'v':
          fop->verbose = true;
          break;

        /* enable ip timestamp writing mode. */
        case 't':
          fop->ip_timestamp = true;
          break;

        /* enable high quality randomness. */
        case 'r':
          fop->high_randomness = true;
          break;

        case 'i':
          /* fetch input filename. */
          fop->input_filename = strdup (optarg);

          /* is the file a regular ascii file? */
          if (!ascii_file (fop->input_filename))
            {
              fprintf (stderr, "%s\n", input_file_limits_a);
              fprintf (stderr, "%s `%s':\n", input_file_limits_b,
                       fop->input_filename);
              fprintf (stderr, "%s\n\n", regular_file_limits);
              print_usage (stderr, EXIT_FAILURE);
            }
          break;

        /* unknown option. */
        case '?':
          print_usage (stderr, EXIT_FAILURE);

        /* no more options. */
        case -1:
          break;

        default:
          abort ();
      }
    }
  while (next_option != -1);

  /* the program does not take additional arguments. */
  if (optind != argc)
    {
      fprintf (stderr, "%s\n\n", additional_arguments);
      print_usage (stderr, EXIT_FAILURE);
    }

  /* is there any input filename? */
  if ((fop->input_filename == NULL) && (environment_input == NULL))
    {
      fprintf (stderr, "%s\n\n", input_file_is_missing);
      print_usage (stderr, EXIT_FAILURE);
    }

  /* if the input file was given by `-i'
     then go to the body of the program.
     Otherwise, check to see if the input
     environment variable is ok. */
  if (fop->input_filename != NULL)
    return;
  else
    if (!ascii_file (environment_input))
      {
        fprintf (stderr, "%s\n", environment_file_limits_a);
        fprintf (stderr, "%s `%s':\n", environment_file_limits_b,
                 environment_input);
        fprintf (stderr, "%s\n\n", regular_file_limits);
        print_usage (stderr, EXIT_FAILURE);
      }
}
