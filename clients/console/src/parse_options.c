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
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>

/* local defines. */
#define DEFAULT_PORT 4567        /* default port. */
#define DEFAULT_HOST "localhost" /* default host. */

/* my includes. */
#include "program_typedefs.h"
#include "program_messages.h"

/* global variables. */
static const char *program_name = NULL;

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
  fprintf (stream, "\n%s", usage_part_c);
  fprintf (stream, "\n%s\n", usage_part_d);
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

  /* for the client. */
  fop->port = (uint16_t) htons (DEFAULT_PORT);
  fop->hostinfo = gethostbyname (DEFAULT_HOST);
}

/* the following function parses
   input command line arguments. */
void
parse_options (int argc, char **argv)
{
  /* extern necessary elements. */
  extern char *strdup (const char *);
  extern char *program_invocation_short_name;

  /* the next option to get. */
  int next_option;

  /* short options. */
  unique short_options = "z:p:v";

  /* long options. */
  const struct option long_options[] = {
    {"host", 1, NULL, 'z'},
    {"port", 1, NULL, 'p'},
    {"verbose", 0, NULL, 'v'},
    {"help", 0, NULL, 'h'},
    {"usage", 0, NULL, 'u'},
    {"version", 0, NULL, 'V'},
    {NULL, 0, NULL, 0}
  };

  /* get the command program name. */
  program_name = program_invocation_short_name;

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

            /* the user specified non-digits in the port number.  */
            if (*end != '\0' || value <= 0)
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

        case 'z':
          /* fetch hostname. */
          fop->hostinfo = gethostbyname (strdup (optarg));

          /* what happened? */
          if (fop->hostinfo == NULL)
            {
              fprintf (stderr, "%s\n\n", unknown_host);
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
    } while (next_option != -1);

  /* the program does not take additional arguments. */
  if (optind != argc)
    {
      fprintf (stderr, "%s\n\n", additional_arguments);
      print_usage (stderr, EXIT_FAILURE);
    }
}
