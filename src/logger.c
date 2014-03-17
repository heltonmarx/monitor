/**
 * @file logger.c
 * @brief LOGGER (SYSLOG) INTERFACE
 */

#include <logger.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#ifndef ON
#define ON		1
#endif	/* ifndef ON */

#ifndef OFF
#define OFF		0
#endif	/* ifndef OFF */

/**
 *	LOG_EMERG	0	->	system is unusable
 *	LOG_ALERT	1	->	action must be taken immediately
 *	LOG_CRIT	2	->	critical conditions
 *	LOG_ERR		3	->	error conditions
 *	LOG_WARNING	4	->	warning conditions
 *	LOG_NOTICE	5	->	normal but significant condition
 *	LOG_INFO	6	->	informational
 * 	LOG_DEBUG	7	->	debug-level messages
 */

static char verbosity_on = VERBOSITY_ENABLE;
static char syslog_on = SYSLOG_DISABLE;

void log_verbosity(enum verbosity_flag flag)
{
    verbosity_on = flag;
}

void log_syslog(enum syslog_flag flag)
{
    syslog_on = flag;
}

int _log(int prio, const char *format, ...)
{
    int log_prio;
    va_list ap;

    log_prio = LOG_LOCAL6 | prio;

    va_start(ap, format);

    if(syslog_on == SYSLOG_ENABLE) {
        if (verbosity_on == VERBOSITY_ENABLE) {
            vsyslog(log_prio, format, ap);
        } else if(prio <= LOG_WARNING) {
            vsyslog(log_prio, format, ap);
        }
    } else if (verbosity_on == VERBOSITY_ENABLE) {
        vfprintf(stdout,format, ap);
        fflush(stdout);
    } else if (prio <= LOG_WARNING) {
        vfprintf(stdout, format, ap);
        fflush(stdout);
    }
    va_end(ap);
    return 0;
}
