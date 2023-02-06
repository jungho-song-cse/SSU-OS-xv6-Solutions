#ifndef page_replacement_algorithms_h
#define page_replacement_algorithms_h

#include <stdio.h>
#include <stdlib.h>
typedef enum { false, true } bool;  //false=0, true=0으로 bool 자료형 선언

void initSelectedAlgorithms(int simulatorNums[], int totalPageNums, int (*pageString)[3],int pageStringNum);

//----------------------------------1. OPTIMAL 페이지 교체 알고리즘 선언부------------------------------------------//
typedef struct optimalStack{
    int front;
    int rear;
    int *data;
}optimalStack;

void optimalInit(optimalStack *stack, int totalPageNums);
bool optimalIsEmpty(optimalStack *stack);
bool optimalIsFull(optimalStack *stack, int totalPageNums);
bool optimalPush(optimalStack *stack, int totalPageNums, int data);
int optimalPop(optimalStack *stack, int totalPageNums);
int optimalStackAlgorithm(optimalStack *stack, int totalPageNums, int (*pageString)[3], int pageStringNum);
//-----------------------------------------------------------------------------------------------------------//


//----------------------------------2. FIFO 페이지 교체 알고리즘 선언부------------------------------------------//
typedef struct fifoQueue{
    int front;
    int rear;
    int *data;
}fifoQueue;

void fifoInit(fifoQueue *queue, int totalPageNums);
bool fifoIsEmpty(fifoQueue *queue);
bool fifoIsFull(fifoQueue *queue, int totalPageNums);
bool fifoEnqueue(fifoQueue *queue, int totalPageNums, int data);
int fifoDequeue(fifoQueue *queue, int totalPageNums);
int fifoQueueAlgorithm(fifoQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum);
//-----------------------------------------------------------------------------------------------------------//


//----------------------------------3. LIFO 페이지 교체 알고리즘 선언부------------------------------------------//
typedef struct lifoStack{
    int front;
    int rear;
    int *data;
}lifoStack;

void lifoInit(lifoStack *stack, int totalPageNums);
bool lifoIsEmpty(lifoStack *stack);
bool lifoIsFull(lifoStack *stack, int totalPageNums);
bool lifoPush(lifoStack *stack, int totalPageNums, int data);
int lifoPop(lifoStack *stack, int totalPageNums);
int lifoStackAlgorithm(lifoStack *stack, int totalPageNums, int (*pageString)[3], int pageStringNum);
//-----------------------------------------------------------------------------------------------------------//


//----------------------------------4. lru 페이지 교체 알고리즘 선언부------------------------------------------//
typedef struct lruQueue{
    int front;
    int rear;
    int *data;
    int *timeStamp;
}lruQueue;

void lruInit(lruQueue *queue, int totalPageNums);
bool lruIsEmpty(lruQueue *queue);
bool lruIsFull(lruQueue *queue, int totalPageNums);
bool lruEnqueue(lruQueue *queue, int totalPageNums, int data);
int checkTimeStamp(lruQueue *queue, int totalPageNums);
int lruQueueAlgorithm(lruQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum);
//-----------------------------------------------------------------------------------------------------------//


//----------------------------------5. lfu 페이지 교체 알고리즘 선언부------------------------------------------//
typedef struct lfuQueue{
    int front;
    int rear;
    int *data;
    int *count;
}lfuQueue;

void lfuInit(lfuQueue *queue, int totalPageNums);
bool lfuIsEmpty(lfuQueue *queue);
bool lfuIsFull(lfuQueue *queue, int totalPageNums);
bool lfuEnqueue(lfuQueue *queue, int totalPageNums, int data);
int getLowestCount(lfuQueue *queue, int totalPageNums);
int lfuQueueAlgorithm(lfuQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum);
//-----------------------------------------------------------------------------------------------------------//


//----------------------------------6. SC 페이지 교체 알고리즘 선언부------------------------------------------//
typedef struct scQueue{
    int front;
    int rear;
    int *data;
    int *referenceBit;
}scQueue;

void scInit(scQueue *queue, int totalPageNums);
bool scIsEmpty(scQueue *queue);
bool scIsFull(scQueue *queue, int totalPageNums);
bool scEnqueue(scQueue *queue, int totalPageNums, int data, int referenceBit);
int scDequeue(scQueue *queue, int totalPageNums);
int scQueueAlgorithm(scQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum);
//-----------------------------------------------------------------------------------------------------------//

//----------------------------------7. ESC 페이지 교체 알고리즘 선언부------------------------------------------//
typedef struct escQueue{
    int front;
    int rear;
    int *data;
    int *referenceBit;
    int *writeBit;
}escQueue;

void escInit(escQueue *queue, int totalPageNums);
bool escIsEmpty(escQueue *queue);
bool escIsFull(escQueue *queue, int totalPageNums);
bool escEnqueue(escQueue *queue, int totalPageNums, int data, int referenceBit, int writeBit);
int escDequeue(escQueue *queue, int totalPageNums);
int escQueueAlgorithm(escQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum);
//-----------------------------------------------------------------------------------------------------------//

void printOutData(int (*pageString)[3],int pageStringNum);

#endif /* page_replacement_algorithms_h */
