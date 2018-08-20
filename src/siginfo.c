#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ptrace.h>

static char const *sigstr(int sign)
{
	static char signum[4];
	snprintf(signum, sizeof(signum), "%d", sign);
	return signum;
}

void dump_siginfo(siginfo_t const *info)
{
    fprintf(stderr, "SIGNAL: %s(%d), ERRNO : %d, CODE: %d\n",
            sigstr(info->si_signo), info->si_signo, info->si_errno, info->si_code);

    switch (info->si_signo) {
    case SIGCHLD:
        fprintf(stderr, "pid:    %d\n", info->si_pid);
        fprintf(stderr, "uid:    %d\n", info->si_uid);
        fprintf(stderr, "status: %x\n", info->si_status);
#ifdef linux
        fprintf(stderr, "utime:  %u\n", info->si_utime);
        fprintf(stderr, "stime:  %u\n", info->si_stime);
#endif
		break;
    case SIGILL:
    case SIGFPE:
    case SIGSEGV:
    case SIGBUS:
        fprintf(stderr, "address: %p\n", info->si_addr);
        break;
    }
}


