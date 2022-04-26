#######
#
#  Makefile -- this file is the makefile for the GNU trm server.
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
FLAGS = -D__USE_FIXED_PROTOTYPES__ -pedantic -pedantic-errors -Wall -Wpointer-arith -Wstrict-prototypes

# headers which are used by all targets.
BASIC_HEADERS = program_typedefs.h

# the name of the executable.
EXEC = gtrm-server

# the targets of the objects.
OBJS =     print_top_msg \
         gtrm_server_obj \
           daemonize_obj \
     file_operations_obj \
             cleanup_obj \
   server_operations_obj \
       parse_options_obj \
     signal_handlers_obj \
       log_functions_obj

# the targets.

# produce the executable.
gtrm_server_exec: $(OBJS)
		  $(CC)  $(FLAGS) $(CFLAGS) -o  $(EXEC)  *.o
		  @echo
		  @echo "[Stop compiling process]"
		  @echo "[Make have produced:" \`$(EXEC)\']

# compilation message.
print_top_msg:
	@echo "[Make is going to produce:" \`$(EXEC)\']
	@echo "[Start compiling process]"
	@echo

# produce the objects.
gtrm_server_obj: gtrm_server.c $(BASIC_HEADERS)
		 $(CC)  $(FLAGS) $(CFLAGS) -c  gtrm_server.c

daemonize_obj: daemonize.c $(BASIC_HEADERS)
	       $(CC)  $(FLAGS) $(CFLAGS) -c  daemonize.c

file_operations_obj: file_operations.c $(BASIC_HEADERS)
		     $(CC)  $(FLAGS) $(CFLAGS) -c  file_operations.c

cleanup_obj: cleanup.c $(BASIC_HEADERS)
	     $(CC)  $(FLAGS) $(CFLAGS) -c  cleanup.c

server_operations_obj: server_operations.c $(BASIC_HEADERS)
		       $(CC)  $(FLAGS) $(CFLAGS) -c  server_operations.c

parse_options_obj: parse_options.c program_messages.h $(BASIC_HEADERS)
		   $(CC)  $(FLAGS) $(CFLAGS) -c  parse_options.c

signal_handlers_obj: signal_handlers.c $(BASIC_HEADERS)
		     $(CC)  $(FLAGS) $(CFLAGS) -c  signal_handlers.c

log_functions_obj: log_functions.c $(BASIC_HEADERS)
		   $(CC)  $(FLAGS) $(CFLAGS) -c  log_functions.c

# remove all objects and executable.
clean:
	@echo "[Make is going to remove:" \`$(EXEC)\']
	@echo "[Cleaning process]"
	rm -f *.o $(EXEC)
