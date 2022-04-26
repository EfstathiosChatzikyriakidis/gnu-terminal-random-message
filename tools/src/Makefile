#######
#
#  Makefile -- this file is the makefile for the GNU trm.
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

# some options which help the compiler.
ANSI_FLAGS = -D__USE_FIXED_PROTOTYPES__ -ansi  \
             -pedantic -pedantic-errors -Wall  \
             -Wpointer-arith -Wstrict-prototypes

# headers which are used by all targets.
BASIC_HEADERS = program_typedefs.h program_structures.h

# the name of the executable.
EXEC = gtrm

# the targets of the objects.
OBJS =         print_top_msg \
           random_number_obj \
     ordered_linked_list_obj \
                    gtrm_obj

# the targets.

# produce the executable.
gtrm_exec: $(OBJS)
	   $(CC)  $(ANSI_FLAGS) $(CFLAGS) -o  $(EXEC)  *.o
	   @echo
	   @echo "[Stop compiling process]"
	   @echo "[Make have produced:" \`$(EXEC)\']

# compilation message.
print_top_msg:
	@echo "[Make is going to produce:" \`$(EXEC)\']
	@echo "[Start compiling process]"
	@echo

# produce the objects.
random_number_obj: random_number.c $(BASIC_HEADERS)
		   $(CC)  $(ANSI_FLAGS) $(CFLAGS) -c  random_number.c

ordered_linked_list_obj: ordered_linked_list.c $(BASIC_HEADERS)
			 $(CC)  $(ANSI_FLAGS) $(CFLAGS) -c  ordered_linked_list.c

gtrm_obj: gtrm.c program_messages.h $(BASIC_HEADERS)
	  $(CC)  $(ANSI_FLAGS) $(CFLAGS) -c  gtrm.c

# remove all objects and executable.
clean:
	@echo "[Make is going to remove:" \`$(EXEC)\']
	@echo "[Cleaning process]"
	rm -f *.o $(EXEC)
