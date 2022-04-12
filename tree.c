#include "proccess.h"
typedef struct node
{
    process p;
    struct node *right;
    struct node *left;
    struct node *parent;
}node;

typedef node *tree;

typedef struct element
{
    tree t;
    struct element *next;
}element;

typedef struct queue{
    element *front;
    element *rear;
}queue;

queue initQ()
{
    queue q;
    q.front=NULL;
    q.rear=NULL;
    return q;
}

void enqueue(queue *q,tree t)
{
    if(q->front==NULL)
    {
        q->front=(element *)malloc(sizeof(element));
        q->front->t=t;
        q->front->next=NULL;
        q->rear=q->front;
    }
    else
    {
        element *e=(element *)malloc(sizeof(element));
        e->t=t;
        e->next=NULL;
        q->rear->next=e;
        q->rear=e;
    }
}

int dequeue(queue *q)
{
    if(q->front==NULL)
        return 0;
    else
    {
        element *e=q->front;
        q->front=q->front->next;
        free(e);
        return 0;
    }
}

int front(queue q,tree *t)
{
    if(q.front==NULL)
        return 0;
    else
    {
        *t=q.front->t;
        return 1;
    }
}

tree init()
{
    return NULL;
}

int isEmptyTree(tree t)
{
    return t==NULL;
}

tree insertProcess(tree *t,tree parent,process p,int index)
{
    if(isEmptyTree(*t))
    {
        *t=(tree)malloc(sizeof(node));
        (*t)->parent=parent;
        (*t)->p=p;
        (*t)->left=NULL;
        (*t)->right=NULL;
        return *t;
    }
    else if(((p.id)%index)>=index/2)
    {
        return insertProcess(&(*t)->left,*t,p,2*index);
    }
    else
    {
        return insertProcess(&(*t)->right,*t,p,2*index);
    }
}

void triParTas(tree *t)
{
    if((*t)->parent==NULL || (*t)->p.priority>(*t)->parent->p.priority)
    {
        return;
    }
    else if((*t)->p.priority<(*t)->parent->p.priority)
    {
        process p;
        p=(*t)->parent->p;
        (*t)->parent->p=(*t)->p;
        (*t)->p=p;
        triParTas(&(*t)->parent);
    }
    else if((*t)->p.priority==(*t)->parent->p.priority && (*t)->p.id<(*t)->parent->p.id)
    {
        process p;
        p=(*t)->parent->p;
        (*t)->parent->p=(*t)->p;
        (*t)->p=p;
        triParTas(&(*t)->parent);
    }
}

void insert(tree *t,process *p,double *totalInsertTime)
{
    p->insertTime=clock();
    tree temp=insertProcess(t,NULL,*p,2);
    triParTas(&temp);
    totalInsertTime+=clock()-p->insertTime;
}

clock_t removeProcess(tree *t,process *p,double *totalRemoveTime)
{
    clock_t start=clock();
    *p=(*t)->p;
    if((*t)->left==NULL && (*t)->right==NULL)
    {
        free(*t);
        (*t)=NULL;
        clock_t finish=clock();
        *totalRemoveTime+=finish-start;
        return finish;
    }
    else if((*t)->left==NULL)
    {
        return removeProcess(&(*t)->right,&(*t)->p,totalRemoveTime);
    }
    else if((*t)->right==NULL)
    {
        return removeProcess(&(*t)->left,&(*t)->p,totalRemoveTime);
    }
    else if((*t)->left->p.priority<(*t)->right->p.priority)
    {
        return removeProcess(&(*t)->left,&(*t)->p,totalRemoveTime);
    }
    else if((*t)->right->p.priority<(*t)->left->p.priority)
    {
        return removeProcess(&(*t)->right,&(*t)->p,totalRemoveTime);
    }
    else if((*t)->right->p.id>(*t)->left->p.id)
    {
        return removeProcess(&(*t)->left,&(*t)->p,totalRemoveTime);
    }
    else
    {
        return removeProcess(&(*t)->right,&(*t)->p,totalRemoveTime);
    }
}

int loadProcess(char fileName[],tree *t,double *totalInsertTime)
{
    FILE *file=fopen(fileName,"r");
    if(file==NULL)
        return -1;
    process p;
    int id,priority,executionTime,nbp=0;
    while(fscanf(file,"%d %d %d",&id,&priority,&executionTime)>0)
    {
        p.id=id;
        p.priority=priority;
        p.executionTime=executionTime;
        insert(t,&p,totalInsertTime);
        nbp++;
    }
    return nbp;
}

void waitingList(tree t,process p)
{
    if(t==NULL)
        return;
    printf("waiting list: ");
    queue q=initQ();
    tree courant=t;
    if(courant->p.priority==p.priority)
    {
        enqueue(&q,courant);
    }
    while(q.front!=NULL)
    {
        front(q,&courant);
        printf("%d\t",courant->p.id);
        if(courant->left!=NULL && courant->left->p.priority==p.priority)
        {
            enqueue(&q,courant->left);
        }
        if(courant->right!=NULL && courant->right->p.priority==p.priority)
        {
            enqueue(&q,courant->right);
        }
        dequeue(&q);
    }
    printf("\n");
}
int main()
{
    printf("===================CPU simulator using binary tree===================\n");
    printf("Sorting the processes...\n");
    printf("==============================================================================\n");
    tree t=init();
    process p;
    clock_t removeFinish;
    double totalInsertTime=0,totalRemoveTime=0,totatWaitingTime=0,totalTurnAroundTime=0;
    int nbp=loadProcess("Proccesses.txt",&t,&totalInsertTime),i;
    for(i=0;i<nbp;i++)
    {
        removeFinish=removeProcess(&t,&p,&totalRemoveTime);
        totatWaitingTime+=removeFinish-p.insertTime;
        //execut(p);
        //waitingList(t,p);
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
