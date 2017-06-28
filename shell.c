#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define BUFF_SIZE 64
#define BUFF_NUM  16

#define LSH_ARGS_FOR_EACH(str, args) \
    for (str = &args[0]; str[0] != '\0'; str += BUFF_SIZE)  

#define LSH_EXEC_CB_FOR_EACH(_index_cb)  \
    for (_index_cb = 0; _index_cb < sizeof(lsh_func) / sizeof(lsh_func_t); _index_cb++)
   
typedef int (*lsh_exec_cb_t)(char *);

typedef struct lsh_func_s
{
    char cmd[BUFF_SIZE];
    lsh_exec_cb_t exec_cb;
} lsh_func_t;

int lsh_exec_help_cb(char *args);
int lsh_exec_show_cb(char *args);

lsh_func_t lsh_func[] = 
{
    {"help", lsh_exec_help_cb},
    {"show", lsh_exec_show_cb}

};

int lsh_exec_help_cb(char *args)
{
    int i;

    for (i = 0; i < sizeof(lsh_func) / sizeof(lsh_func_t); i++)
    {
        printf("%s\n", lsh_func[i].cmd);
    }

    return 0;
}

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

int lsh_exec_show_cb(char *args)
{
    char *str;

//    str = &args[(index++) * BUFF_SIZE];
//    while (str[0] != '\0')
    LSH_ARGS_FOR_EACH(str, args)
    {
        printf("%s\n", str);
    }
}

int lsh_exec_process_cb(char *args)
{
    int i;

    LSH_EXEC_CB_FOR_EACH(i)
    {
        if (strcmp((char *)&args[0], lsh_func[i].cmd) == 0)
        {
            return lsh_func[i].exec_cb(args);
        }
    }

    return 0;
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
        lsh_exec_process_cb(args);
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
