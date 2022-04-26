/*
 * cleanup.c -- this files has functions, which basically clean up.
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
#include <syslog.h>

#include <netinet/in.h>

/* my includes. */
#include "program_typedefs.h"

/* externs. */
extern void log_report (cint, unique, unique);

/* function prototypes. */
void cleanup_files (void);

/* functions. */
/* the following function tries to remove
   all the files that the program creates. */
void
cleanup_files (void)
{
  /* extern necessary elements. */
  extern bool remove_file (unique);
  extern unique lock_file;

  /* try to remove files. */
  if (!remove_file (lock_file))
    log_report (LOG_ERR, "remove_file() [cleanup]", lock_file);

  return;
}
