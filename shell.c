#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define BUFF_SIZE 64
#define BUFF_NUM  16

char *lsh_read_line()
{
    char *line;
    char input;
    int index;

    line = malloc(MAX_LINE);
    if (line == NULL)
    {
        printf("malloc return NULL\n");
        return NULL;
    }

    memset(line, 0, MAX_LINE);
    index = 0;

    while ((input = getchar()) != '\n')
    {
        line[index++] = input;
        
        if (index == 1024)
        {
            printf("input line bigger than %d, returen NULL\n", MAX_LINE);
            free(line);
            return NULL;
        }
    }

    return line;
}

char *lsh_split_line(char *line)
{
    char *args;
    char *str1;
    int index = 0;

    args = malloc(BUFF_SIZE * BUFF_NUM);
    if (args == NULL)
    {
        printf("malloc buff error\n");
        free(line);
        return NULL;
    }
    memset(args, 0, BUFF_SIZE * BUFF_NUM);


    // strtoke first called, use string ptr
    str1 = strtok(line, " ");
    strncpy(&args[index++], str1, BUFF_SIZE);

    // after first called, use NULL
    while (str1 != NULL)
    {
        str1 = strtok(NULL, " ");
        if (str1 == NULL)
        {
            break;
        }

        strncpy(&args[(index++) * BUFF_SIZE], str1, BUFF_SIZE);
        if (index == BUFF_NUM)
        {
            printf("args num bigger then: %d\n", BUFF_NUM);
            free(args);
            free(line);
            return NULL;
        }
    }

    return args;
}

void show_args(char *args)
{
    char *str;
    int index = 0;

    str = &args[(index++) * BUFF_SIZE];
    while (str[0] != '\0')
    {
        printf("%s\n", str);
        str = &args[(index++) * BUFF_SIZE];
    }
}

int lsh_execute(char *args)
{
    int rv;
    int pid;
    int status;

    
    pid = fork();
    switch (pid)
    {
    case 0:
        // child process
        show_args(args);
        return 0;
    case -1:
        printf("fork error\n");
        return -1;
    default:
        // parent process
        rv = waitpid(pid, &status, WUNTRACED);
    }

}

void lsh_loop()
{
    char *line;
    char *args;
    int status;

    do {
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv)
{
    lsh_loop();

    return 0;
}
