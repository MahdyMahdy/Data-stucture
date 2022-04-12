#include "proccess.h"
#define N 2000
typedef struct element
{
    process p;
    struct element *next;
}element;
typedef element* hashTab;

int hash(process p)
{
    return p.priority%N;
}

hashTab init()
{
    hashTab tab=(hashTab)malloc(N*sizeof(struct element));
    process p;
    p.priority=-1;
    p.id=-1;
    for(int i=0;i<N;i++)
    {
        tab[i].p=p;
        tab[i].next=NULL;
    }
    return tab;
}

void insert(hashTab tab,process *p,double *totalInsertTime)
{
    clock_t start=clock();
    int i=hash(*p);
    if(tab[i].p.priority==-1 || p->priority<tab[i].p.priority)
    {
        element *e=(element *)malloc(sizeof(element));
        e->next=tab[i].next;
        tab[i].next=e;
        e->p=tab[i].p;
        p->insertTime=start;
        tab[i].p=*p;
    }
    else
    {
        element *n=tab+i;
        while(n->next->p.priority!=-1 && n->next->p.priority<=p->priority)
        {
            n=n->next;
        }
        element *e=(element *)malloc(sizeof(element));
        p->insertTime=start;
        e->p=*p;
        e->next=n->next;
        n->next=e;
    }
    *totalInsertTime+=clock()-start;
}

int loadProcess(char *fileName,hashTab tab,double *totalInsertTime)
{
    int nbp=0;
    FILE *f=fopen(fileName,"r");
    process p;
    int id,priority,executionTime;
    while(fscanf(f,"%d %d %d",&id,&priority,&executionTime)>0)
    {
        nbp++;
        p.id=id;
        p.priority=priority;
        p.executionTime=executionTime;
        insert(tab,&p,totalInsertTime);
    }
    fclose(f);
    return nbp;
}

clock_t removeProcess(hashTab tab,process *p,int lastpriority,double *totalRemoveTime)
{
    clock_t start=clock();
    int i=lastpriority%N;
    if(tab[i].p.priority==lastpriority)
    {
        *p=tab[i].p;
        element *n=tab[i].next;
        tab[i].p=n->p;
        tab[i].next=n->next;
        free(n);
    }
    else
    {
        double t;
        removeProcess(tab,p,lastpriority+1,&t);
    }
    clock_t finish=clock();
    return finish;
}

void waitingList(hashTab tab,process p)
{
    int i=hash(p);
    printf("waiting list : ");
    element *n=tab+i;
    while(n->p.priority==p.priority)
    {
        printf("%d\t",n->p.id);
        n=n->next;
    }
    printf("\n");
}

int main()
{
    printf("===================CPU simulator using hash table===================\n");
    printf("Sorting the processes...\n");
    printf("==============================================================================\n");
    hashTab tab=init();
    process p;
    p.priority=0;
    clock_t removeFinish;
    double totalInsertTime=0,totalRemoveTime=0,totatWaitingTime=0,totalTurnAroundTime=0;
    int nbp=loadProcess("Proccesses.txt",tab,&totalInsertTime),i;
    for(i=0;i<nbp;i++)
    {
        removeFinish=removeProcess(tab,&p,p.priority,&totalRemoveTime);
        totatWaitingTime+=removeFinish-p.insertTime;
        //execut(p);
        //waitingList(tab,p);
        totalTurnAroundTime+=p.executionTime;
    }
    totalTurnAroundTime+=totatWaitingTime;
    totalInsertTime/=nbp*1000;
    totalRemoveTime/=nbp*1000;
    totatWaitingTime/=nbp*1000;
    totalTurnAroundTime/=nbp*1000;
    printf("AverageInsertTime : %lf s\n",totalInsertTime);
    printf("AverageDeleteTime : %lf s\n",totalRemoveTime);
    printf("AverageWaitingTime : %lf s\n",totatWaitingTime);
    printf("AverageTurnAroundTime : %lf s\n",totalTurnAroundTime);
    return 0;
}
