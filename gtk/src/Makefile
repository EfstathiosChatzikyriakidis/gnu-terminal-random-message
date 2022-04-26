#######
#
#  Makefile -- this file is the makefile for the GNU xrm.
#
#  Copyright (C) 2006 Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com)
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
#
#######

# information which help the compilation part.

# the compiler which is used. GNU C COMPILER.
CC = gcc

# some options which help the gcc compiler.
ANSI_FLAGS = -D__USE_FIXED_PROTOTYPES__ -ansi  -pedantic -pedantic-errors -Wpointer-arith -Wall `pkg-config --cflags --libs gtk+-2.0`

# headers which are used by all targets.
BASIC_HEADERS = program_typedefs.h program_messages.h

# the name of the executable.
EXEC = gxrm

# the targets of the objects.
OBJS =  print_top_msg

# the targets.
# produce the executable.
gxrm_exec: $(OBJS)
	   $(CC)  $(ANSI_FLAGS) $(CFLAGS) -o $(EXEC)  gxrm.c
	   @echo
	   @echo "[Stop compiling process]"
	   @echo "[Make have produced:" \`$(EXEC)\']

# compilation message.
print_top_msg:
	@echo "[Make is going to produce:" \`$(EXEC)\']
	@echo "[Start compiling process]"
	@echo

# remove all objects and executable.
clean:
	@echo "[Make is going to remove:" \`$(EXEC)\']
	@echo "[Cleaning process]"
	rm -f *.o $(EXEC)
