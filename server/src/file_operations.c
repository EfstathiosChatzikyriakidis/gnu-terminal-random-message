/*
 * file_operations.c -- operations which are performed on files.
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
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

#include <netinet/in.h>

/* my includes. */
#include "program_typedefs.h"

/* local defines. */
#define OPEN_MAX_GUESS 256        /* this is a suitable value. */
#define BIN_BUF_LEN 128           /* is used for binary checking. */

/* function prototypes. */
bool check_file (const char *, cint);
void close_all_fds (register int);
bool remove_file (unique);
bool ascii_file (const char *);

/* functions. */
/* the following function tries to check a file. */
bool
check_file (const char *file, cint mode)
{
  int rval;
  bool f_state = false;

  /* check the file. */
  if ((rval = access (file, mode)) < 0)
    f_state = true;

  return (f_state) ? false : true;
}

/* the following function closes
   all file descriptors >= `fd'. */
void
close_all_fds (register int fd)
{
  /* max opened files a process can have. */
  int fdlimit = sysconf (_SC_OPEN_MAX);

  /* what happened? */
  if (fdlimit < 0)
    fdlimit = OPEN_MAX_GUESS;

  /* try closing all file descriptors >= `fd'. */
  while (fd < fdlimit)
    close (fd++);
}

/* the following function tries to remove the file
   from the system. on success returns true, but,
   on error false. */
bool
remove_file (unique file)
{
  bool state = true;

  /* does the file exist? is the file removable? */
  if (check_file (file, F_OK))
    if (unlink (file) < 0)
      state = false;

  return state;
}

/* the following function tries to see
   if the file is a regular ascii file. */
bool
ascii_file (const char *file)
{
  /* does the file exist and is readable? */
  if (!check_file (file, F_OK | R_OK))
    return false;
  else
    {
      /* structure for keeping file's information. */
      struct stat stbuf;

      /* try to get file's information. */
      if (stat (file, &stbuf) < 0)
        {
          fprintf (stderr, "%s: `%s' %s", "stat() error for", file,
                   strerror (errno));
          return false;
        }

      /* is the file a directory? */
      if (S_ISDIR (stbuf.st_mode))
        return false;

      /* is the file a block device file? */
      if (S_ISBLK (stbuf.st_mode))
        return false;

      /* is the file a character device file? */
      if (S_ISCHR (stbuf.st_mode))
        return false;

      /* is the file a fifo file? */
      if (S_ISFIFO (stbuf.st_mode))
        return false;
    }

  /* it seems that the file is a regular ascii one. */
  return true;
}
