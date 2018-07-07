#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <argp.h>

#include <stddef.h>
#include <errno.h>
#include <dirent.h>

#include "process.h"
#include "utils.h"
#include "logger.h"

#define FILENAMELEN         256
#define LINELEN             256

#ifndef PAGE_SIZE
#define PAGE_SIZE           4096
#endif  /* PAGE_SIZE */

#define PROC_DIR_NAME       "/proc"
#define CMDLINE_NAME        "cmdline"
#define STAT_NAME           "stat"

static int getname(const char *src, char *dst, int n)
{
    FILE *fp = NULL;

    /** clear dst buffer */
    memset(dst, 0x00, n);

    /* open process file for read */
    fp = fopen(src, "r");
    if (fp == NULL) {
        log_error("Unable to open \"%s\" for reading (errno=%s)\n",
                  src, strerror(errno));
        return -1;
    }
    char *r = fgets(dst, LINELEN, fp);
    fclose(fp);
    UNUSED_ARG(r);
    return 0;
}

int pidof(const char *process)
{
    int pid = -1;

    DIR *proc = NULL;
    struct dirent *de;
    char cl_name[1024];
    char p_name[LINELEN];
    char p_search[LINELEN];

    if (!process  || !strlen(process)) {
        log_error("Invalid process name: %s\n", process);
        return -1;
    }

    int len = strlen(process);
    memset(p_search, 0x00, sizeof(p_search));
    while(len--) {
        if ((process[len] == '/') || (len == 0)) {
            snprintf(p_search, sizeof(p_search), "(%s)",
                     (process[len] == '/') ?
                     (process + len + 1) : process);
            break;
        }
    }
    if (strlen(p_search) == 0) {
        log_error("Invalid process name: %s\n", p_search);
        return -1;
    }

    /* open "/proc/" directory */
    proc = opendir(PROC_DIR_NAME);
    if (proc == NULL) {
        log_error("Unable to open directory \"%s\" for traversal (errno=%s)\n",
                  PROC_DIR_NAME, strerror(errno));
        return -1;
    }

    while ((pid < 0) && (de = readdir(proc))) {
        int n = (FILENAMELEN + sizeof(PROC_DIR_NAME));
		char d_name[n];
        snprintf(d_name, n, "%s/%s", PROC_DIR_NAME, de->d_name);

        /* ...if the entry is a numerically-named directory...*/
        if ((is_directory(d_name) == 0) &&
            (is_wholly_numeric(de->d_name) == 0)) {
            /* Open pid/cmdline file for reading.  Try for command-line. */
            snprintf(cl_name, sizeof(cl_name), "%s/%s/%s", PROC_DIR_NAME, de->d_name, CMDLINE_NAME);
            if (getname((const char *)cl_name, p_name, sizeof(p_name)) != 0) {
                goto on_exit;
            }
            /*
             * If no command-line was available, get the second field of the first
             *  line of the pid/stat file.
             */
            if (strlen(p_name) != 0) {
                char st_name[1024];
                char stat_name[LINELEN];
                char *s;
                int i;
                /*
                * Open pid/stat file for reading.
                */
                snprintf(st_name, sizeof(st_name), "%s/%s/%s", PROC_DIR_NAME, de->d_name, STAT_NAME);
                if (getname((const char *)st_name, stat_name, sizeof(stat_name)) != 0) {
                    goto on_exit;
                }
                s = stat_name;
                i = 0;
                while (*s != '\0' && !isspace(*s)) s++;
                while (*s != '\0' &&  isspace(*s)) s++;
                while (*s != '\0' && !isspace(*s)) p_name[i++] = *s++;
                p_name[i] = '\0';
                if (strcmp(p_name, p_search) == 0) {
                    pid = atoi(de->d_name);
                    goto on_exit;
                }
            }
        }
    }
on_exit:
    closedir(proc);
    return pid;
}

