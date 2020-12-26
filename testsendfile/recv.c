#include <string.h> /* for strchr() */

void main()
{

    char address[] = "10.10.10.10/10";
    char *p = strchr(address, '/');
    if (!p)
    {
        /* deal with error: / not present" */
        ;
    }
    else
    {
        *p = 0;
    }
}