#include <_stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int setenv(const char *name, const char *value, int overwrite) {

    if (overwrite == 0 && (getenv(name) != NULL))
        return 0;

    // allocate memory for name=value\0
    char *p = malloc(strlen(name)+ strlen(value) + 2);

    if (p == NULL)
        return -1;

    sprintf(p, "%s=%s", name, value);
    
    // copy values into putenv
    return putenv(p);
}

int unsetenv(const char *name) {
    extern char **environ;
    size_t len = strlen(name);

    for (char **ep = environ; *ep != NULL; ) {
        if (strncmp(*ep, name, len) == 0 && (*ep)[len] == '=') {
            for (char **sp = ep; *sp != NULL; sp++)
                *sp = *(sp + 1);
        } else {
            ep++;
        }
    }

    return 0;
}
