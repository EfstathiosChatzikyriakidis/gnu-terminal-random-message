/*
 * daemonize.c -- daemonize the program.
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <netinet/in.h>

/* local defines. */
#define MAX_LINE (1024 * 1024)        /* 1Mbyte. */

/* my includes. */
#include "program_typedefs.h"

/* filenames. */
static unique null_file = "/dev/null";
static unique chdir_path = "/tmp";
unique lock_file = "/var/run/gtrm-server.pid";

/* externs. */
extern void log_report (cint, unique, unique);
extern bool remove_file (unique);
extern void close_all_fds (register int);
extern char *stpcpy (char *, const char *);

/* function prototypes. */
bool be_daemon (void);

/* functions. */
/* the following function daemonizes the program. */
bool
be_daemon (void)
{
  /* use it to keep the id of a process. */
  pid_t ppid, pid;

  /* check to see if it is already a daemon. */
  if ((ppid = getppid ()) == 1)
    return true;

  /*** try creating the daemon. ***/

  /* create a new process. */
  if ((pid = fork ()) < 0)
    {
      /* fork failed. */
      log_report (LOG_ERR, "fork() [daemon]", NULL);
      return false;
    }
  else if (pid > 0)
    {
      /* parent exits. */
      exit (EXIT_SUCCESS);
    }
  else
    {
      /*** child (daemon) continues. ***/

      int fd;                   /* for redirecting stdin, stdout, stderr. */
      int lfd;                  /* for locking the `lock_file'. */
      pid_t sid;                /* process indepedency. */

      /* for writing the pid to `lock_file'. */
      char pid_str[32];

      /* is used for locking the `lock_file'. */
      struct flock exclusive_lock;

      /*** create a new SID for the child process. ***/

      /* make the process a session and process group leader. */
      if ((sid = setsid ()) < 0)
        {
          log_report (LOG_ERR, "setsid() [daemon]", NULL);
          return false;
        }

      /*** close all inherited opened files. ***/
      close_all_fds (STDIN_FILENO);

      /*** redirect stdin, stdout and stderr. ***/

      /* open `/dev/null'. */
      if ((fd = open (null_file, O_RDWR)) < 0)
        {
          log_report (LOG_ERR, "open() [daemon]", null_file);
          return false;
        }

      /* use dup2() also for `stdin' because in case close_all_fds()
         fails to close all fds, especially `stdin' then, there will
         cause an error. */

      /* stdin. */
      if (dup2 (fd, STDIN_FILENO) < 0)
        {
          log_report (LOG_ERR, "dup2() [stdin]", NULL);
          return false;
        }

      /* stdout. */
      if (dup2 (fd, STDOUT_FILENO) < 0)
        {
          log_report (LOG_ERR, "dup2() [stdout]", NULL);
          return false;
        }

      /* stderr. */
      if (dup2 (fd, STDERR_FILENO) < 0)
        {
          log_report (LOG_ERR, "dup2() [stderr]", NULL);
          return false;
        }

      /*** change file creation mask. ***/
      umask (022);

      /*** change working directory. ***/
      if (chdir (chdir_path) < 0)
        {
          log_report (LOG_ERR, "chdir() [daemon]", chdir_path);
          return false;
        }

      /*** lock the daemon. ***/

      /* first, form the pid. */
      if (sprintf (pid_str, "%d\n", getpid ()) < 0)
        {
          log_report (LOG_ERR, "sprintf() [1] [daemon]", NULL);
          return false;
        }

      /* try to access the file. */
      lfd = open (lock_file, O_RDWR | O_CREAT | O_EXCL, 0660);

      /* what happened? */
      if (lfd < 0)
        {
          log_report (LOG_ERR, "open() [daemon]", lock_file);

          /* in case of a defunct process inform admin. */
          if (access (lock_file, F_OK) == 0)
            {
              /* are used for checking the `PID' code. */
              char comm_str[MAX_LINE];
              int s_state;

              /* form the killing command. */
              if (sprintf (comm_str, "kill -s 0 `cat %s`", lock_file) < 0)
                {
                  log_report (LOG_ERR, "sprintf() [2] [daemon]", NULL);
                  return false;
                }

              /* tell us if PID is correct. */
              if ((s_state = system (comm_str)) < 0)
                {
                  log_report (LOG_ERR, "system() [daemon]", NULL);
                  return false;
                }

              if (s_state > 0)
                {
                  syslog (LOG_ERR, " ");
                  syslog (LOG_ERR, "                [ CHECK THE FOLLOWING ]");
                  syslog (LOG_ERR, "    A lock file found, but, no process corresponds");
                  syslog (LOG_ERR, "    with its PID code. So, the file is removed now.");
                  syslog (LOG_ERR, " ");

                  /* try to remove the `lock_file' if exists. */
                  if (!remove_file (lock_file))
                    {
                      log_report (LOG_ERR, "remove_file() [1] [daemon]",
                                  lock_file);
                      return false;
                    }

                  syslog (LOG_ERR, "Now you can start the server.");
                }
            }

          return false;
        }

      /* we have got this far so we have acquired
         access to the lock file. set a lock on it. */
      exclusive_lock.l_type = F_WRLCK;                    /* exclusive write lock. */
      exclusive_lock.l_whence = SEEK_SET;                 /* use start and len. */
      exclusive_lock.l_len = exclusive_lock.l_start = 0;  /* whole file. */
      exclusive_lock.l_pid = 0;                           /* don't care about this. */

      /* try to lock the `lock_file'. */
      if (fcntl (lfd, F_SETLK, &exclusive_lock) < 0)
        {
          log_report (LOG_ERR, "fcntl() [daemon]", lock_file);
          close (lfd);
          return false;
        }

      /*** only first instance continues. ***/

      /* write pid to `lock_file'. */
      if (write (lfd, pid_str, strlen (pid_str)) < 0)
        {
          log_report (LOG_ERR, "write() [daemon]", lock_file);

          if (!remove_file (lock_file))
            log_report (LOG_ERR, "remove_file() [2] [daemon]", lock_file);

          return false;
        }
    }

  /* runs as a daemon. */
  return true;
}
