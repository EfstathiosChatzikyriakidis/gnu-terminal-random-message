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
static unique found_problem = "* unable to get a random message *";

static unique writtenby_msg = "Written by";
static unique developers_msg = "Efstathios Chatzikyriakidis";
static unique version_msg = "version 0.1";
static unique package_msg = "GNU xrm";
static unique reportbugs_msg = "Report bugs to <bug-gnu-xrm@gnu.org>";

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
static unique usage_part_b = "-i `file'";

static unique usage_part_c =
  "  -i, --input `file'       Read data from `file'. (necessary).          \n"
  "                                                                        \n"
  "      --help, --usage      Display this help and exit.                  \n"
  "      --version            Print version information and exit.          \n";
