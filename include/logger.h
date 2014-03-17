#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

/**
 * @file logger.h
 * @brief LOGGER (SYSLOG) interface
 */

#include <stdarg.h>
#include <syslog.h>

extern char	*__progname;		/* Program name, from crt0. */

#define log_debug(fmt, args...)     (_log(LOG_NOTICE, "[%s]::[%s.%d]:"fmt,\
										__progname,__FUNCTION__, __LINE__, ##args))

#define log_error(fmt, args...)     (_log(LOG_ERR, "[%s]::[%s.%d]:ERROR:"fmt, \
										__progname,__FUNCTION__, __LINE__, ##args))

enum syslog_flag {
    SYSLOG_DISABLE,
    SYSLOG_ENABLE,
};

enum verbosity_flag {
    VERBOSITY_DISABLE,
    VERBOSITY_ENABLE,
};

/**
 *  Enable or Disable verbosity.
 *
 *  @param [in] flag    VERBOSITY_DISABLE or VERBOSITY_ENABLE,
 */
void log_verbosity(enum verbosity_flag flag);

/**
 *  Enable or Disable syslog.
 *
 *  @param [in] flag    SYSLOG_DISABLE or SYSLOG_ENABLE
 */
void log_syslog(enum syslog_flag flag);

/**
 *	Set to stderr a log message, like a printf,
 *	but, have some others information about software.
 *
 *	@param[in] prio		LOG_EMERG	0	->	system is unusable.
 *						LOG_ALERT	1	->	action must be taken immediately.
 *						LOG_CRIT	2	->	critical conditions.
 *						LOG_ERR		3	->	error conditions.
 *						LOG_WARNING	4	->	warning conditions.
 *						LOG_NOTICE	5	->	normal but significant condition.
 *						LOG_INFO	6	->	informational.
 * 						LOG_DEBUG	7	->	debug-level messages.
 *	@param[in] format	String.
 *	@param[in] args		Arguments.
 *
 *	@return		Success, zero is returned.
 *	@return		On error, -1 is returned.
 */
int _log(int prio, const char *format, ...);

#endif  /* LOGGER_H_INCLUDED */
