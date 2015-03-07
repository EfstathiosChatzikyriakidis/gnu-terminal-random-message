/*
 * gtrm_client.c -- the body of the client.
 *
 *       The GNU trm client sends a request to a GNU
 *       trm server for a terminal random message.
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

/* my includes. */
#include "program_typedefs.h"

/* externs. */
/* functions. */
extern void parse_options (int, char **);
extern void log_report (FILE *, unique, unique);
extern bool client_run (struct hostent *, uint16_t);

/* command line options. */
extern options_info *fop;

/* global messages. */
static unique client_failed = "client_run() failed to run.";

/* functions. */
/* the body of the client. */
int
main (int argc, char *argv[])
{
  /* keep client_run() state. */
  bool client_state = true;

  /*** parse all input command line options. ***/
  parse_options (argc, argv);

  /*** try to run the client. ***/
  client_state = client_run (fop->hostinfo, fop->port);

  /* failed to run the client? */
  if (!client_state && fop->verbose)
    fprintf (stderr, "%s\n", client_failed);

  /* client exits. */
  exit (client_state ? EXIT_SUCCESS : EXIT_FAILURE);
}
