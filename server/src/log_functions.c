/*
 * log_functions.c -- this file has functions log reports.
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
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>
#include <fcntl.h>

#include <netinet/in.h>

/* local defines. */
#define MAX_LINE (1024 * 1024)        /* 1Mbyte. */

/* my includes. */
#include "program_typedefs.h"

/* function prototypes. */
void log_report_exit (cint, unique, unique, cint, bool);
static char *get_timestamp (void);
void log_report (cint, unique, unique);
bool log_ip_timestamp (unique, unique);

/* functions. */
/* the following function uses the syslog
   facility to report a message and then
   terminates the program `exit' call. */
void
log_report_exit (cint p, unique msg, unique file, cint e, bool eb)
{
  /* is there any file for the report? */
  if (file != NULL)
    syslog (p, "%s error for `%s': %s.", msg, file, strerror (errno));
  else
    syslog (p, "%s error: %s.", msg, strerror (errno));

  /* do you want to exit the program? */
  if (eb)
    exit (e);
}

/* the following function is exactly the same as
   `log_report_exit'. however, does not terminate
   the program. */
void
log_report (cint p, unique msg, unique file)
{
  /* is there any file for the report? */
  if (file != NULL)
    syslog (p, "%s error for `%s': %s.", msg, file, strerror (errno));
  else
    syslog (p, "%s error: %s.", msg, strerror (errno));
}

/* the following function returns a character
   string representing the current date, time. */
static char *
get_timestamp (void)
{
  time_t now = time (NULL);
  return asctime (localtime (&now));
}

/* the following function writes an ip timestamp. */
bool
log_ip_timestamp (unique file, unique message)
{
  /* extern necessary elements. */
  extern int fsync (int);

  /* the timestamp/ip entry. */
  char log_entry[MAX_LINE];

  /* open the `file' for writing. if it exists,
     append to it: otherwise, create a new file. */
  int fd = open (file, O_WRONLY | O_CREAT | O_APPEND, 0660);

  /* does the `file' opened / created? */
  if (fd < 0)
    {
      log_report (LOG_ERR, "open() [timestamp]", file);
      return false;
    }

  sprintf (log_entry, "* connection accepted from: `%s'. timestamp: %s",
           message, get_timestamp ());

  if (write (fd, log_entry, strlen (log_entry)) < 0)
    {
      log_report (LOG_ERR, "write() [timestamp]", file);
      close (fd);
      return false;
    }

  /* write data, close file. */
  fsync (fd);
  close (fd);

  return true;
}
