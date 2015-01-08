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
#include <regex.h>

#include <stddef.h>
#include <errno.h>
#include <dirent.h>

#include "utils.h"
#include "process.h"
#include "logger.h"

int check_regex(const char *er, const char *txt)
{
    regex_t regex;
    int ret;

    if (!txt || !strlen(txt) || !er || !strlen(er)) {
        return -1;
    }
    regcomp(&regex, er, REG_EXTENDED|REG_NOSUB);
    ret = regexec(&regex, txt,  0, NULL, 0);
    regfree(&regex);

    return ((ret != 0) ? -1 : 0);
}

int check_valid_integer(const char *integer)
{
    const char *integer_er = "^[0-9]{1,4}\\d+*$";
    return check_regex(integer_er, integer);
}

int is_directory(const char *dirname)
{
    struct stat buf;
    int n;

    if (!dirname || !strlen(dirname)) {
        return -1;
    }
    n = stat(dirname, &buf);
    return (n == 0 && (buf.st_mode & S_IFDIR) != 0)? 0 : -1;
}

int is_wholly_numeric(const char *str)
{
    if (!str || !strlen(str)) {
        return -1;
    }

    while (*str != '\0') {
        if (!isdigit(*str)) {
            return -1;
        }
        str++;
    }
    return 0;
}

pid_t get_pidof(const char *process)
{
    char name[2048];
    const char *p;
    long len;

    if (!process || (strlen(process) == 0)) {
        return -1;
    }
    memset(name,0x00, sizeof(name));

    p = strchr(process, ' ');
    len = (p == NULL) ? (long)sizeof(name) : (p - process);
    strncpy(name, process, len);
    return pidof(name);
}
