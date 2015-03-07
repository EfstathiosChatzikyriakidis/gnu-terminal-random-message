/*
 * server_operations.c -- this file has the server operations.
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
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>

/* local defines. */
#define QLEN 10
#define MAX_LINE (1024 * 1024)        /* 1Mbyte. */

/* decide the input filename. */
#define FILENAME_IN ((fop->input_filename == NULL) ? environment_input : fop->input_filename)

/* my includes. */
#include "program_typedefs.h"

/* global variables. */
unique timestamp_file = "gtrm-server.ip-timestamp";
static char comm_str[MAX_LINE];
volatile sig_atomic_t keep_going = true;

/* global messages. */
static unique check_follow = "[ CHECK THE FOLLOWING ]";
static unique gtrm_problem = "  * the `gtrm' command exists.";
static unique path_problem =
  "  * the path for the input file starts with `/'.";
static unique file_problem = "  * the input file exists and is readable.";
static unique eof_problem = "  * the input file has size greater than zero.";

/* externs. */
/* functions. */
extern void log_report (cint, unique, unique);

/* variables. */
extern options_info *fop;
extern const char *environment_input;

/* function prototypes. */
static void handle_send (cint conn, const char *);
static bool handle_connection (cint conn);
bool server_run (struct in_addr, uint16_t);

/* functions. */
/* the following function tries to send
   the random `message' to the client. */
static void
handle_send (cint conn, const char *sentence)
{
  if (send (conn, sentence, strlen (sentence), 0) < 0)
    log_report (LOG_ERR, "send() [server]", NULL);
}

/* the following function handles a connection. */
static bool
handle_connection (cint conn)
{
  /* extern necessary elements. */
  extern char *stpcpy (char *, const char *);
  extern FILE *popen (const char *, const char *);
  extern int pclose (FILE *);

  /* the reading pipe edge. */
  FILE *pipe_stream = NULL;

  /* current line of chars. */
  char sentence[MAX_LINE] = { 0 };

  /* there was an error? */
  bool error = false;

  /* form the command. */
  stpcpy (stpcpy
          (stpcpy
           (stpcpy (comm_str, "gtrm "), fop->high_randomness ? "-r " : ""),
           "-i "), FILENAME_IN);

  /* start piping. */
  if ((pipe_stream = popen (comm_str, "r")) == NULL)
    {
      log_report (LOG_ERR, "popen() [server]", NULL);
      error = true;
    }
  else
    {
      /* get one random sentence. */
      if (fgets (sentence, MAX_LINE, pipe_stream) == NULL)
        {
          log_report (LOG_ERR, "fgets() [server]", NULL);
          error = true;
        }
      else
        handle_send (conn, sentence);

      /* close piping. */
      pclose (pipe_stream);
    }

  return error ? false : true;
}

/* the following function tries to run a
   server for `local_address' and 'port'. */
bool
server_run (struct in_addr local_address, uint16_t port)
{
  /* extern necessary elements. */
  extern void clean_up_process (cint);
  extern bool log_ip_timestamp (unique, unique);

  /* is used for keeping server's address, port, type of socket, ... */
  struct sockaddr_in server_address;

  int server_socket;               /* is used for creating a socket. */
  int rval;                        /* is used for validating. */

  /* is used for reusing the port. */
  int reuse_address = 1;

  /* use a `SIGCHLD' handler that cleans up child processes. */
  signal (SIGCHLD, clean_up_process);

  /* try to create a tcp socket. */
  server_socket = socket (PF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
    {
      log_report (LOG_ERR, "socket() [server]", NULL);
      return false;
    }

  /* now is possible to use the same port. */
  rval = setsockopt (server_socket, SOL_SOCKET,
                     SO_REUSEADDR, &reuse_address, sizeof (reuse_address));

  /* what happened? */
  if (rval < 0)
    {
      log_report (LOG_ERR, "setsockopt() [server]", NULL);
      return false;
    }

  /* construct a socket address structure for the local
     address on which we want to listen for connections. */
  memset (&server_address, 0, sizeof (server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = port;
  server_address.sin_addr = local_address;

  /* try to bind the socket to that address. */
  rval =
    bind (server_socket, (struct sockaddr *) &server_address,
          sizeof (server_address));
  if (rval < 0)
    {
      log_report (LOG_ERR, "bind() [server]", NULL);
      return false;
    }

  /* try to instruct the socket to accept connections. */
  rval = listen (server_socket, QLEN);
  if (rval < 0)
    {
      log_report (LOG_ERR, "listen() [server]", NULL);
      return false;
    }

  /* verbose mode, display the local address
     and the port number we're listening on. */
  if (fop->verbose)
    {
      socklen_t address_length = sizeof (server_address);

      rval = getsockname (server_socket, (struct sockaddr *)
                          &server_address, &address_length);

      if (rval == 0)
        syslog (LOG_INFO, "listens on %s:%d",
                inet_ntoa (server_address.sin_addr),
                (int) ntohs (server_address.sin_port));
      else
        log_report (LOG_ERR, "getsockname() [server]", NULL);
    }

  /* loop forever, handle any connections. */
  while (keep_going)
    {
      /* is used for keeping client's address, port, type of socket, ... */
      struct sockaddr_in client_address;

      socklen_t address_length;       /* is used for holding the size of the address. */
      int conn;                       /* is used for checking accept calls. */
      pid_t child_pid;                /* is used for holding a process id. */

      address_length = sizeof (client_address);

      /* accept a connection. blocks until a connection is ready. */
      conn =
        accept (server_socket, (struct sockaddr *) &client_address,
                &address_length);

      /* what happened? */
      if (conn < 0)
        {
          /* the call was interrupted by a signal. try again. */
          if (errno == EINTR)
            continue;
          else
            {
              /* something else went wrong. continue to the next accept. */
              log_report (LOG_ERR, "accept() [server]", NULL);
              continue;
            }
        }

      /* write ip timestamp if `-t', is on. */
      if (fop->ip_timestamp)
        {
          socklen_t address_length = sizeof (client_address);

          rval =
            getpeername (conn, (struct sockaddr *) &client_address,
                         &address_length);
          if (rval == 0)
            {
              /* is used for checking. */
              bool t_state = true;

              /* try to write ip timestamp. */
              t_state =
                log_ip_timestamp (timestamp_file,
                                  inet_ntoa (client_address.sin_addr));

              /* what happened? */
              if (!t_state)
                syslog (LOG_ERR, "log_timestamp() [server] failed to run.");
            }
          else
            log_report (LOG_ERR, "getpeername() [server]", NULL);
        }

      /* fork a child process to handle the connection. */
      child_pid = fork ();

      /* what happened? */
      if (child_pid == 0)
        {
          /* child process. */

          /* useless here. */
          close (server_socket);

          /* try to handle the connection. */
          if (!handle_connection (conn))
            {
              /* inform the administrator to fix it. */
              syslog (LOG_ERR, "%s",
                      "handle_connection() [server] failed to run.");
              syslog (LOG_ERR, "%s", " ");
              syslog (LOG_ERR, "%s", check_follow);
              syslog (LOG_ERR, "%s", gtrm_problem);
              syslog (LOG_ERR, "%s", path_problem);
              syslog (LOG_ERR, "%s", file_problem);
              syslog (LOG_ERR, "%s", eof_problem);
              syslog (LOG_ERR, "%s", " ");
            }

          /* no more useful. */
          close (conn);

          /* terminate the child process. */
          _exit (EXIT_SUCCESS);
        }
      else if (child_pid > 0)
        {
          /* parent process. */
          close (conn);
        }
      else
        {
          /* fork failed. */
          log_report (LOG_ERR, "fork() [server]", NULL);
        }
    }

  /* server stopped due to `SIGTERM' signal. */
  return true;
}
