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
static unique package_msg = "GNU trm client";
static unique version_msg = "version 0.1";
static unique reportbugs_msg = "Report bugs to <bug-gnu-trm-client@gnu.org>";

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

static unique port_limits =
  "Note: -p, --port `port'.                                 \n"
  "  There are limits for `port'. Must be positive integer. \n";

static unique additional_arguments =
  "Note: The program does not take additional arguments.";

static unique unknown_host = "Note: The host given does not exist.";

static unique usage_part_c =
  "  -p, --port `port'      Protocol port number server is using. (optional). \n"
  "  -z, --host `host'      Name of the computer on which the GNU             \n"
  "                         trm server is executing. (optional).              \n"
  "                                                                           \n";

static unique usage_part_d =
  "  -v, --verbose          Explain what is being done.                       \n"
  "                                                                           \n"
  "      --help, --usage    Display this help and exit.                       \n"
  "      --version          Print version information and exit.               \n";
