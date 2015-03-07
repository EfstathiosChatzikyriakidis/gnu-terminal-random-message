/*
 * log_functions.c -- this file has functions which print messages.
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
#include <errno.h>
#include <string.h>
#include <netinet/in.h>

/* my includes. */
#include "program_typedefs.h"

/* function prototypes. */
void log_report (FILE *, unique, unique);

/* functions. */
/* the following function prints an error. */
void
log_report (FILE * stream, unique msg, unique file)
{
  /* is there any file for the report? */
  if (file != NULL)
    fprintf (stream, "%s error for `%s': %s.\n", msg, file, strerror (errno));
  else
    fprintf (stream, "%s error: %s.\n", msg, strerror (errno));
}
