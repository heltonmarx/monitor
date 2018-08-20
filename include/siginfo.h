#ifndef SIGINFO_H_INCLUDED
#define SIGINFO_H_INCLUDED

/**
 *	@file siginfo.h
 *	@brief siginfo dump functions.
 */

/**
 * Dump the siginfo_t stack data.
 *
 * @param [in] info 	Pointer to siginfo_t data
 *
 */
void dump_siginfo(siginfo_t const *info);

#endif /* SIGINFO_H_INCLUDED */
