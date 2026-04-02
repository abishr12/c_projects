#include <_stdio.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

struct passwd *getpwnam(const char *name) {
    struct passwd *pwd;
    
    // Rewind to the beginning in case same 
    // process was mid-iteration
    setpwent();

    // Go through all the passwords
    while ((pwd = getpwent()) != NULL) {
    // Compare against username - if true return else return false
        if (strcmp(name, pwd->pw_name) == 0) {
            endpwent();
            return pwd;
        }

    }

    endpwent();

    return NULL;
}
