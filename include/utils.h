#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/**
 *	@file utils.h
 *	@brief utils functions
 */


#include <sys/user.h>
#include <sys/sysctl.h>
#include <sys/types.h>

#ifndef UNUSED_ARG
#define UNUSED_ARG(arg)  (void)arg
#endif  /* UNUSED_ARG   */

/**
 * Function to check a string using a regular expression.
 *
 * @param [in] er   Regular Expression string
 * @param [in] txt  String to check
 *
 * @return     Success, zero is returned.
 * @return     On error, -1 is returned.
 */
int check_regex(const char *er, const char *txt);

/**
 *  Check if integer is valis using regular expression bellow:
 *      "^[0-9]{1,4}\\d+*$"
 *
 *  @param [in] integer     Integer string in decimal format (e.g.:1234)
 *
 *  @return     Success, zero is returned.
 *  @return     On error, -1 is returned.
 */
int check_valid_integer(const char *integer);

/**
 *  Verify if dirname is a valid directory.
 *
 *  @param [in] dirname Directory name
 *
 *  @return     Success, zero is returned.
 *  @return     On error, -1 is returned.
 */
int is_directory(const char *dirname);

/**
 *  Verify if string is a numeric string, e.g.: "1234".
 *
 *  @param [in] str     String.
 *
 *  @return     Success, zero is returned.
 *  @return     On error, -1 is returned.
 */
int is_wholly_numeric(const char *str);

/**
 *  Get the pid of process, parsing process removing arguments
 *  of process.
 *
 *  @param [in] process String with process name
 *
 *  @return     Success, pid of process is returned.
 *  @return     On error, -1 is returned.
 */
pid_t get_pidof(const char *process);

#endif	/* UTILS_H_INCLUDED */
