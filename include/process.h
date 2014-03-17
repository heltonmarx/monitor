#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

/**
 *  Find the process ID of a running program, returning the PID.
 *
 *  @param  [in]    process     Name of process
 *
 *	@return     Success, positive PID is returned.
 *	@return     On error, -1 is returned.
 */
int pidof(const char *process);

#endif  /* PROCESS_H_INCLUDED */
