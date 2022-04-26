/*
 * program_structures.h -- this file contains the appropriate
 *                         structures which the program needs.
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

/* each node of the ordered linked
   list has `list_node' structure. */
struct list_node
{
  int index;
  char *line;
  struct list_node *next_ptr;
};
