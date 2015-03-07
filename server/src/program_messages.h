/*
 * program_messages.h -- messages used by the program.
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

/* informative messages. */
static unique written_by_msg = "Written by";
static unique developers_msg = "Efstathios Chatzikyriakidis";
static unique package_msg = "GNU trm server";
static unique version_msg = "version 0.1";
static unique reportbugs_msg = "Report bugs to <bug-gnu-trm-server@gnu.org>";

/* GNU General Public License. */
static unique license_part_a =
  "   Copyright (C) 2006 Efstathios Chatzikyriakidis (contact@efxa.org)     \n"
  "                                                                        \n"
  "   This program is free software; you can redistribute it and/or modify \n"
  "   it under the terms of the GNU General Public License as published by \n"
  "   the Free Software Foundation; either version 2 of the License, or    \n"
  "   (at your option) any later version.                                  \n";

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

/* usage information. */
static unique usage_part_a = "Usage:";
static unique usage_part_b = "[options]";

static unique input_file_is_missing =
  "Note: Use either `-i' or `GTRM_SERVER_INPUT' environment variable for input file.";

static unique input_file_limits_a = "Note: -i, --input `file'.";

static unique input_file_limits_b = "  Check that the input file";

static unique environment_file_limits_a =
  "Note: `GTRM_SERVER_INPUT' environment input variable.";

static unique environment_file_limits_b = "  Check that the input file";

static unique regular_file_limits =
  "    * exists and is a readable regular ascii file.";

static unique port_limits =
  "Note: -p, --port `port'.                                             \n"
  "  There are limits for `port'. It must be a positive integer > 1024. \n";

static unique additional_arguments =
  "Note: The program does not take additional arguments.";

static unique usage_part_c =
  "  -i, --input `file'       Read data from `file'.                                            \n"
  "                           You can use also the environment variable: `GTRM_SERVER_INPUT'.   \n"
  "                                                                                             \n"
  "  -r, --high-randomness    High quality randomness.                                          \n";

static unique usage_part_d =
  "                           Use `/dev/random'. Default is: `/dev/urandom'.                    \n"
  "                                                                                             \n"
  "  -p, --port `port'        Bind to specified `port'. (optional).                             \n"
  "                                                                                             \n";


static unique usage_part_e =
  "  -v, --verbose            Explain what is being done.                \n"
  "  -t, --ip-timestamp       For each connection write an ip timestamp. \n"
  "                                                                      \n"
  "      --help, --usage      Display this help and exit.                \n"
  "      --version            Print version information and exit.        \n";
