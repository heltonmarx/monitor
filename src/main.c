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

#include "logger.h"
#include "utils.h"
#include "process.h"

typedef struct {
    char *process;
    char args[1024];
    int sleepsec;
} monitor_t;

static monitor_t monitor;

static struct argp_option options[] = {
    {"sleep",       's',    "NUM",      0,      "Sleep seconds before re-executing.",   0},
    {"process",     'c',    "STRING",   0,      "Process name to be monitored.",        0},
    {"version",     'v',    NULL,       0,      "Show version and exit.",               0},
    {"syslog",      'l',    NULL,       0,      "Send log messages to syslog.",         0},
    {"quiet",       'q',    NULL,       0,      "Reduce the verbosity.",                0},
    {NULL,           0,     NULL,       0,      NULL, 0},
};

static int parse_opt (int key, char *arg, struct argp_state *state)
{
    monitor_t * m = (monitor_t *)state->input;

    switch(key) {
    case 's':   /* sleep */
        if (check_valid_integer(arg) == 0) {
            m->sleepsec = atoi(arg);
        }
        break;

    case 'c':   /* process */
        if (!arg || !strlen(arg)) {
            log_error("Invalid process name\n");
            exit(EXIT_FAILURE);
        }
        m->process = arg;
        break;

    case 'v':   /* version */
        fprintf(stderr, "monitor v%s\n\n", RELEASE);
        exit(0);
        break;

    case 'l':   /* syslog */
        log_syslog(SYSLOG_ENABLE);
        break;

    case 'q':   /* quiet */
        log_verbosity(VERBOSITY_DISABLE);
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num == 0) {
            strncpy(m->args, arg, sizeof(m->args));
        } else {
            strcat(m->args, " ");
            strcat(m->args, arg);
        }
        break;

    case ARGP_KEY_END:
        if (state->arg_num > 4) {
            argp_failure (state, 1, 0, "too few arguments");
        }
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

int start(monitor_t * mon)
{
    int ret;
    pid_t pid;
    int status;
    char process[2048];

    while(1) {
        pid = fork();
        switch(pid) {
        case -1:
            log_error("fork()(%s)\n", strerror(errno));
            return -1;
        case 0:
            signal(SIGTERM, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            memset(process, 0x00, sizeof(process));
            if (!mon->args || !strlen(mon->args)) {
                snprintf(process, sizeof(process), "%s ", mon->process);
            } else {
                snprintf(process, sizeof(process), "%s %s", mon->process, mon->args);
            }
            log_debug("sh -c \"%s\"", process);
            ret = execl("/bin/sh", "sh", "-c", process, NULL);
            if (ret < 0) {
                log_error("execl() error:(%s)\n", strerror(errno));
                return -1;
            }
            break;
        default:
            /** wait for exit */
            waitpid(pid, &status, 0);
            log_debug("Application %s die\n", mon->process);

            if (mon->sleepsec > 0) {
                /** waiting some seconds to restart the app */
                log_debug("restarting application in "
                          "%u second(s)\n",
                          mon->sleepsec);
                sleep(mon->sleepsec);
            } else {
                return -1;
            }
        }
    }
    return 0;
}

static struct argp parser = {
    options,
    parse_opt,
    NULL,
    "Super simple process monitor\n",
    NULL,
    NULL,
    NULL
};

void _exit(int sig)
{
    pid_t pid = getpid();
    pid_t ppid = get_pidof(monitor.process);

    log_debug("kill by signal %d - pid: %d, ppid: %d\n", sig, (int)pid, (int)ppid);

    /** kill the monitor process */
    kill(pid, sig);

    /** kill user process */
    if (ppid >= 0) {
        kill(ppid, sig);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, _exit);
    signal(SIGQUIT, _exit);

    memset(&monitor, 0x00, sizeof(monitor_t));
    monitor.sleepsec = 1;   /** default: 1 second */

    argp_parse(&parser, argc, argv, 0, 0, &monitor);

    if (monitor.process && strlen(monitor.process)) {
        start(&monitor);
    }
    return 0;
}

