/*
 * random_number.c -- this file implements a function which
 *                    returns random numbers when we call it.
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
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/* my includes. */
#include "program_typedefs.h"

/* function prototypes. */
int random_number (cint, cint, unique);

/* functions. */
/* the following function uses either `/dev/random' or
   `/dev/urandom' to fetch a random integer. it returns
   an integer, which is always between `min' and `max'. */
int
random_number (cint min, cint max, unique filename)
{
  /* store a file descriptor opened to `filename' in a
     static variable. that way, we don't need to open
     the file every time this function is called.  */
  static int dev_random_fd = -1;

  char *next_random_byte;
  int bytes_to_read;
  unsigned random_value;

  /* make sure `max' is greater than `min'. */
  /* assert (max > min); */

  /* if this is the first time this function is
     called, open a file descriptor to `filename'.  */
  if (dev_random_fd == -1)
    {
      dev_random_fd = open (filename, O_RDONLY);
      assert (dev_random_fd != -1);
    }

  /* read enough random bytes to fill an integer variable.  */
  next_random_byte = (char *) &random_value;
  bytes_to_read = sizeof (random_value);

  /* loop until we've read enough bytes. since `filename' is
     filled from user-generated actions, the read may block,
     and may only return a single random byte at a time.  */
  do
    {
      register int bytes_read;
      bytes_read = read (dev_random_fd, next_random_byte, bytes_to_read);
      bytes_to_read -= bytes_read;
      next_random_byte += bytes_read;
    }
  while (bytes_to_read > 0);

  /* compute a random number in the correct range.  */
  return min + (random_value % (max - min + 1));
}
