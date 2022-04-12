//proccess.h
#include <stdio.h>
#include <windows.h>
#include <time.h>
typedef struct process
{
    int id;
    int priority;
    int executionTime;
    clock_t insertTime;

}process;

void execut(process p)
{
    printf("%d\t",p.id);
    Sleep(p.executionTime);
    printf("done!\n");
}
