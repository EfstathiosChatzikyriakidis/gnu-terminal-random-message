/*
 * client_operations.c -- this file has the client operations.
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
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

/* local defines. */
#define MAX_LINE (1024 * 1024)        /* 1Mbyte. */

/* my includes. */
#include "program_typedefs.h"

/* externs. */
extern options_info *fop;

/* global messages. */
static unique error_message = "[ unable to get a random message ]";
static unique handle_failed = "handle_get() [client] failed to run.";

/* function prototypes. */
static bool handle_get (cint conn);
void log_report (FILE *, unique, unique);
bool client_run (struct hostent *, uint16_t);

/* functions. */
/* the following function tries to get
   a random msg from the GNU trm server
   and then prints it. */
static bool
handle_get (cint socket_fd)
{
  /* buffer for the message. */
  char sentence[MAX_LINE] = { 0 };

  /* is used for validating. */
  int rval;

  /* try to fetch the message. */
  rval = recv (socket_fd, sentence, MAX_LINE, 0);

  /* what happened? */
  if (rval < 0)
    {
      if (fop->verbose)
        log_report (stderr, "recv()", NULL);

      return false;
    }
  else if (rval == 0)
    {
      /* peer has performed an orderly shutdown. */
      printf ("%s\n", error_message);
      return false;
    }
  else
    printf ("%s", sentence);

  return true;
}

/* the following function tries to connect with 
   a GNU trm server by using the `hostinfo' and
   `port'. */
bool
client_run (struct hostent * hostinfo, uint16_t port)
{
  /* is used for creating a socket. */
  int client_socket;

  /* is used for keeping server's address, port, type of socket, ... */
  struct sockaddr_in server_address;

  /* try to create a tcp socket. */
  client_socket = socket (PF_INET, SOCK_STREAM, 0);

  /* what happened? */
  if (client_socket < 0)
    {
      if (fop->verbose)
        log_report (stderr, "socket() [client]", NULL);

      return false;
    }

  /* construct a socket address structure for the
     server's address on which we want to connect. */
  memset (&server_address, 0, sizeof (server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr = *((struct in_addr *) hostinfo->h_addr);
  server_address.sin_port = port;

  /* try to connect to the server's socket. */
  if (connect
      (client_socket, (struct sockaddr *) &server_address,
       sizeof (struct sockaddr_in)) < 0)
    {
      if (fop->verbose)
        log_report (stderr, "connect() [client]", NULL);

      return false;
    }

  /* try to fetch a sentence and print it. */
  if (!handle_get (client_socket))
    {
      if (fop->verbose)
        fprintf (stderr, "%s\n", handle_failed);

      return false;
    }

  return true;
}
