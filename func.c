#include <stdio.h>

typedef int (*func)(int);


int test(int a);
int test1(int a);

typedef struct func_s
{
    char cmd[13];
    func cb;
} func_t;

func_t g_cb[] = 
{
    {"a", test},
    {"a", test1}
};

int test(int a)
{
    printf("%d\n", a);
}

int test1(int a)
{
    printf("%d\n", a);
}

//func g_cb = test;


int main()
{
    g_cb[0].cb(1);
    g_cb[1].cb(2);
}
