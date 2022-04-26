/*
 * gtrm_server.c -- the body of the server.
 *
 *       The GNU trm server sends random messages to any
 *       client request. However, it works like a daemon.
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
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include <netinet/in.h>

/* my includes. */
#include "program_typedefs.h"

/* global variables. */
static unique syslog_ident = "gtrm-server";

/* externs. */
/* functions. */
extern void parse_options (int, char **);
extern bool be_daemon (void);
extern bool server_run (struct in_addr, uint16_t);
extern void cleanup_files (void);
extern void signals_support (bool);

/* command line options. */
extern options_info *fop;

/* functions. */
/* the body of the server. */
int
main (int argc, char *argv[])
{
  /* hold somes states. */
  bool daemon_state = true;        /* failed to run as a daemon? */
  bool server_state = true;        /* failed to run the server? */

  /*** support syslog facility. ***/
  openlog (syslog_ident, LOG_CONS, LOG_DAEMON);

  /*** parse all input command line options. ***/
  parse_options (argc, argv);

  syslog (LOG_INFO, "first heartbeat. alive.");

  /*** try to run as a daemon. ***/
  daemon_state = be_daemon ();

  /*** support signal handling. ***/
  signals_support (true);

  /* failed to run as a daemon? */
  if (!daemon_state)
    syslog (LOG_ERR, "be_daemon() failed to run.");
  else
    {
      /*** try to run the server. ***/
      server_state = server_run (fop->local_address, fop->port);

      /* failed to run the server? */
      if (!server_state)
        syslog (LOG_ERR, "server_run() failed to run.");

      /* try to remove files. */
      cleanup_files ();
    }

  syslog (LOG_INFO, "last heartbeat. dead.");

  /*** stop syslog facility. ***/
  closelog ();

  /* daemon (server) exits. */
  exit (server_state && daemon_state ? EXIT_SUCCESS : EXIT_FAILURE);
}
