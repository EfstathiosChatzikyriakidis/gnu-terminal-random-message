/*
 * program_messages.h -- messages used by the program.
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

/* informative messages. */
static unique useless_arguments = "(Useless additional arguments)";
static unique high_random_quality = "(High quality randomness)";

static unique use_environment_input = "(Use input environment variable)";
static unique use_environment_output = "(Use output environment variable)";

static unique use_file_input = "(Use input)";
static unique use_file_output = "(Use output)";

static unique input_file_msg = "Input file  :";
static unique output_file_msg = "Output file :";

static unique use_standard_input = "(Use standard input)";
static unique use_standard_output = "(Use standard output)";

static unique stdin_msg = "standard input";
static unique stdout_msg = "standard output";

static unique read_ok = " * Read ok.";
static unique write_ok = " * Write ok.";

static unique line_number_msg = "Line number :";
static unique found_eof = "* EOF character found. Program ends.";
static unique file_read_error = "Couldn't read file";
static unique file_write_error = "Couldn't write file";
static unique file_is_empty = "The file is empty:";

static unique loop_max_error_a = "Note: -a, --again `value'.";
static unique loop_max_error_b = "Only integers >= 1. eg: 1, 2, 3, 4, ...";

static unique writtenby_msg = "Written by";
static unique developers_msg = "Efstathios Chatzikyriakidis";
static unique version_msg = "version 0.1";
static unique package_msg = "GNU trm";
static unique reportbugs_msg = "Report bugs to <bug-gnu-trm@gnu.org>";

/* GNU General Public License. */
static unique license_part_a =
  "   Copyright (C) 2006 Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com) \n"
  "                                                                                       \n"
  "   This program is free software; you can redistribute it and/or modify                \n"
  "   it under the terms of the GNU General Public License as published by                \n"
  "   the Free Software Foundation; either version 2 of the License, or                   \n"
  "   (at your option) any later version.                                                 \n";

static unique license_part_b =
  "                                                                    \n"
  "   This program is distributed in the hope that it will be useful,  \n"
  "   but WITHOUT ANY WARRANTY; without even the implied warranty of   \n"
  "   This program is distributed in the hope that it will be useful,  \n"
  "   but WITHOUT ANY WARRANTY; without even the implied warranty of   \n"
  "   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the     \n";

static unique license_part_c =
  "   GNU General Public License for more details.                                \n"
  "                                                                               \n"
  "   You should have received a copy of the GNU General Public License           \n"
  "   along with this program; if not, write to the Free Software                 \n"
  "   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. \n";

/* Usage information. */
static unique usage_part_a = "Usage:";
static unique usage_part_b = "[options]";

static unique usage_part_c =
  "  -i, --input `file'       Read data from `file'.                                        \n"
  "                           Default is: `stdin' or environment variable: `GTRM_INPUT'.    \n"
  "                                                                                         \n"
  "  -o, --output `file'      Write data to `file'.                                         \n";

static unique usage_part_d =
  "                           Default is: `stdout' or environment variable: `GTRM_OUTPUT'.  \n"
  "                                                                                         \n"
  "  -r, --high-randomness    High quality randomness.                                      \n"
  "                           Use `/dev/random'. Default is: `/dev/urandom'.                \n";

static unique usage_part_e =
  "  -v, --verbose            Explain what is being done.           \n"
  "  -a, --again `value'      How many random messages do you want? \n"
  "                                                                 \n"
  "      --help, --usage      Display this help and exit.           \n"
  "      --version            Print version information and exit.   \n";
