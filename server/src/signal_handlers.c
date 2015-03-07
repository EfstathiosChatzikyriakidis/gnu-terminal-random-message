/*
 * signal_handlers.c -- signal handling support.
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
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <netinet/in.h>

/* my includes. */
#include "program_typedefs.h"

/* externs. */
/* functions. */
extern void cleanup_files (void);

/* variables. */
extern volatile sig_atomic_t keep_going;

/* function prototypes. */
static void signals_handler (cint);
static void term_handler (cint);
void clean_up_process (cint);
void signals_support (bool);

/* functions. */
/* the following function cleans up all
   zombie process each time you run it.
   is a handler for `SIGCHLD' signal. */
void
clean_up_process (cint sig)
{
  /* are used for cleaning up zombies. */
  int status;
  pid_t pid;

  /* clean up all zombie processes. */
  while ((pid = waitpid (-1, &status, WNOHANG)) > 0);
}

/* the following function is
   a handler for term signal. */
static void
term_handler (cint sig)
{
  /* log informative message. */
  syslog (LOG_INFO, "termination signal found.");

  /* try to remove files. */
  cleanup_files ();

  /* stop the server. */
  keep_going = false;
}

/* the following function is called each time a
   fresh signal occured. then calls a specific
   handler depending for that singal. */
static void
signals_handler (cint sig)
{
  switch (sig)
    {
      /* finalize the server */
    case SIGTERM:
      term_handler (sig);
      break;

      /* cleanup a zombie process. */
    case SIGCHLD:
      clean_up_process (sig);
      break;

      /* just inform. */
    default:
      syslog (LOG_INFO, "signal found: `%d'.", sig);
      break;
    }
}

/* the following function enables or
   disables signals handling support. */
void
signals_support (bool state)
{
  /* is used for handling the signals. */
  register int i;

  /* enable signals handler. */
  if (state)
    for (i = SIGHUP; i <= SIGTERM; i++)
      signal (i, signals_handler);

  /* disable signals handler. */
  if (!state)
    for (i = SIGHUP; i <= SIGTERM; i++)
      signal (i, SIG_IGN);
}
