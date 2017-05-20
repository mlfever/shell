#include "term.h"
#include "debug.h"

int main(int argc, char **argv)
{
    int rv;
    int status;

    /* main process */

    /* loop to process user input string */
    while (1)
    {
        /* put user input to term process */
        rv = term_input(&status);
        if (rv != 0)
        {
            SHELL_ERR("term get error rv: %d, status: %d\n", rv, status);
            return -1;
        }

        if (status == SHELL_STATUS_EXIT)
        {
            SHELL_INFO("user exit\n");
            return 0;
        }
    }

    return 0;
}

