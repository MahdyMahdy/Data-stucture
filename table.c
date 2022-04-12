#include <stdlib.h>
#include "proccess.h"
typedef struct node
{
    process p;
    struct node *next;
}node;

typedef struct queue
{
    node *start;
    node *finish;
}queue;

queue init()
{
    queue q;
    q.start=NULL;
    q.finish=NULL;
    return q;
}

clock_t removeFromFirst(queue *q,double *totalRemoveTime)
{
    clock_t start=clock();
    node *n=q->start;
    q->start=n->next;
    free(n);
    clock_t finish=clock();
    *totalRemoveTime+=finish-start;
    return finish;
}

void insertInOrdre(queue *q,process *p,double *totalInsertTime)
{
    p->insertTime=clock();
    if(q->start==NULL || p->priority<q->start->p.priority)
    {
        node *n=(node *)malloc(sizeof(node));
       n->next=q->start;
       q->start=n;
       n->p=*p;
    }
    else
    {
        node *n=(node *)malloc(sizeof(node));
        node *place=q->start;
        while(place->next!=NULL && place->next->p.priority<=p->priority )
            place=place->next;
        n->next=place->next;
        place->next=n;
        n->p=*p;
    }
    *totalInsertTime+=clock()-p->insertTime;

}

void waitingList(queue q,process p)
{
    if(q.start==NULL)
        return;
    printf("waiting list : ");
    node *n=q.start;
    while(n!=NULL && n->p.priority==p.priority)
    {
        printf("%d\t",n->p.id);
        n=n->next;
    }
    printf("\n");
}

int loadProcess(char fileName[],queue *q,double *totalInsertTime)
{
    int nbp=0;
    FILE *file=fopen(fileName,"r");
    if(file==NULL)
        return 0;
    process p;
    int id,priority,executionTime;
    while(fscanf(file,"%d %d %d",&id,&priority,&executionTime)>0)
    {
        nbp++;
        p.id=id;
        p.priority=priority;
        p.executionTime=executionTime;
        insertInOrdre(q,&p,totalInsertTime);
    }
    fclose(file);
    return nbp;
}
int main()
{
    printf("======================CPU simulator using priority table======================\n");
    printf("Sorting the processes\n");
    printf("please wait...\n");
    printf("==============================================================================\n");
    queue q=init();
    process p;
    clock_t removeFinish;
    double totalInsertTime=0,totalRemoveTime=0,totatWaitingTime=0,totalTurnAroundTime=0;
    int nbp=loadProcess("Proccesses.txt",&q,&totalInsertTime);
    for(int i=0;i<nbp;i++)
    {
        p=q.start->p;
        removeFinish=removeFromFirst(&q,&totalRemoveTime);
        totatWaitingTime+=removeFinish-p.insertTime;
        //execut(p);
        //waitingList(q,p);
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
