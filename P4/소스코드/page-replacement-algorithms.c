#include "page-replacement-algorithms.h"

optimalStack optS;
fifoQueue fifoQ;
lifoStack lifoS;
lruQueue lruQ;
lfuQueue lfuQ;
scQueue scQ;
escQueue escQ;
FILE *fileP;

//사용자 입력에 따라 선택된 페이지 교체 알고리즘 실행.
void initSelectedAlgorithms(int simulatorNums[], int totalPageNums, int (*pageString)[3],int pageStringNum){
    int c1=simulatorNums[0], c2=simulatorNums[1], c3=simulatorNums[2];
    if(c1==8){  //사용자 입력 중에 8 있으면 무조건 8만 인식하도록 함.
        simulatorNums[1]=-1;
        simulatorNums[2]=-1;
    }
    else{
        if(c1==c2)
            simulatorNums[1]=-1;
        if(c1==c3)
            simulatorNums[2]=-1;
        if(c2==c3)
            simulatorNums[2]=-1;
    }
    
    fileP = fopen("printResults.txt", "w"); //printResults.txt에 출력된 내용 저장.
    if(fileP==NULL){
        printf("file open failed!\n");
        exit(1);
    }
    
    printOutData(pageString, pageStringNum);    //page string 데이터들 출력
    
    //사용자 입력simulatorNums에 따라 optimal, fifo, lifo, lru, lfu, sc, esc 알고리즘으로 page string 처리.
    for(int i=0; i<3; i++){
        switch(simulatorNums[i]){
            case 1:
                optimalStackAlgorithm(&optS, totalPageNums, pageString, pageStringNum);
                break;
            case 2:
                fifoQueueAlgorithm(&fifoQ, totalPageNums, pageString, pageStringNum);
                break;
            case 3:
                lifoStackAlgorithm(&lifoS, totalPageNums, pageString, pageStringNum);
                break;
            case 4:
                lruQueueAlgorithm(&lruQ, totalPageNums, pageString, pageStringNum);
                break;
            case 5:
                lfuQueueAlgorithm(&lfuQ, totalPageNums, pageString, pageStringNum);
                break;
            case 6:
                scQueueAlgorithm(&scQ, totalPageNums, pageString, pageStringNum);
                break;
            case 7:
                escQueueAlgorithm(&escQ, totalPageNums, pageString, pageStringNum);
                break;
            case 8:
                optimalStackAlgorithm(&optS, totalPageNums, pageString, pageStringNum);
                fifoQueueAlgorithm(&fifoQ, totalPageNums, pageString, pageStringNum);
                lifoStackAlgorithm(&lifoS, totalPageNums, pageString, pageStringNum);
                lruQueueAlgorithm(&lruQ, totalPageNums, pageString, pageStringNum);
                lfuQueueAlgorithm(&lfuQ, totalPageNums, pageString, pageStringNum);
                scQueueAlgorithm(&scQ, totalPageNums, pageString, pageStringNum);
                escQueueAlgorithm(&escQ, totalPageNums, pageString, pageStringNum);
                break;
            default:
                break;
        }
    }
    fclose(fileP);
}



//----------------------------------1. OPTIMAL 페이지 교체 알고리즘 함수들------------------------------------------//
//optimal Stack init. totalPageNums+1개의 data를 저장할 배열(스택)을 만든다. user는 totalPageNums(총 페이지 개수) 그대로 넘겨줘도 됨.
void optimalInit(optimalStack *stack, int totalPageNums){
    stack->front=0;
    stack->rear=0;
    stack->data = (int*)malloc(sizeof(int)*(totalPageNums+1));  //Stack data 동적할당
}

//Stack가 비어있는지 확인하고, 비어있다면 true / 아니면 false Return
bool optimalIsEmpty(optimalStack *stack){
    if(stack->front==stack->rear)   //front 포인터와 rear 포인터가 같으면 Stack가 비어있다는 뜻.
        return true;    //Stack is empty
    else
        return false;   //Stack is not empty
}

//Stack가 차있는지 확인하고, 차있다면 true / 아니면 false Return
bool optimalIsFull(optimalStack *stack, int totalPageNums){
    if(stack->front==(stack->rear+1)%(totalPageNums+1)) //rear 포인터 다음 위치가 front 포인터 위치와 같다면 Stack가 가득 찼다는 뜻.
        return true;    //Stack is full
    else
        return false;   //Stack is not full
}

//optimal push, user는 totalPageNum 넘겨주면 내부적으로 Stack 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
bool optimalPush(optimalStack *stack, int totalPageNums, int data){
    if(optimalIsFull(stack, totalPageNums)){    //Stack가 찼으면 push 거부.
        return false;
    }
    else{
        stack->rear = (stack->rear+1)%(totalPageNums+1);    //Stack rear 포인터 다음 위치로 옮겨서 Stack가 관리하는 data 개수 1개 증가.
        stack->data[stack->rear] = data;    //Stack 맨 뒤에 새로운 데이터 넣음
        return true;
    }
}

//optimal pop, user는 totalPageNum 넘겨주면 내부적으로 circular Stack 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌. lifo방식으로 pop.
int optimalPop(optimalStack *stack, int totalPageNums){
    if(optimalIsEmpty(stack)){  //Stack가 비어있으면 destack 거부
        return 0;
    }
    else{
        if(stack->rear==0)  //Stack 배열의 0번째 위치를 rear가 가리키고 있으면, rear를 배열의 마지막 위치로 이동.
            stack->rear=totalPageNums+1;
        stack->rear = (stack->rear-1)%(totalPageNums+1);    //rear를 배열의 이전 위치로 이동.
        return stack->data[(stack->rear+1)%(totalPageNums+1)];  //pop시킨 data Return.
    }
}

int optimalStackAlgorithm(optimalStack *stack, int totalPageNums, int (*pageString)[3], int pageStringNum){
    int pageFault=0;
    int i,j,k;
    int kickedOutPage;
    bool *checkOptimal;
    int count=0;
    
    checkOptimal = (bool*)malloc(sizeof(bool)*totalPageNums);   //totalPageNums만큼 크기의 배열을 만들어, optimal한지 예측하기 위한 배열 생성.
    
    optimalInit(stack, totalPageNums);
    for(i=0; i<totalPageNums+1; i++)
        stack->data[i]=-1;  //data가 비었음을 명시하기 위해 -1로 초기화.
    printf("\n----------------------------------------------\n");
    fprintf(fileP,"\n----------------------------------------------\n");
    printf("**optimal Stack Algorithm으로 Page Replacement 실행**\n\n");
    fprintf(fileP,"**optimal Stack Algorithm으로 Page Replacement 실행**\n\n");
    
    for(i=0; i<pageStringNum; i++){    //page string 개수만큼 for문 반복시켜야함.P4
        if(optimalIsFull(stack, totalPageNums)){
            for(j=0; j<totalPageNums+1; j++){
                if(stack->data[j]==pageString[i][0]&&stack->front!=j){  //같은 데이터 있으면 page fault 아니니까 패스.
                    break;
                }
            }
            if(j==totalPageNums+1){ //중복된 data 없다면 j==totalPageNUms+1, page fault인데
                count=0;
                for(j=0; j<totalPageNums; j++){
                    checkOptimal[j]=false;  //checkOptimal false으로 초기화.
                }
                for(j=i; j<pageStringNum; j++){ //pageStringNum만큼 돌면서 pageStack에 있는 원소가 있는지 확인. 있다면 checkOptimal 원소 배열로 체크
                    for(k=1;k<totalPageNums+1;k++){ //Stack이니까 index 1부터 totalPageNums까지 for문 반복.
                        if(pageString[j][0]==stack->data[k]){  //pageString 순회하면서 도는 것이 현재 i의 page
                            if(checkOptimal[k-1]!=true){
                                checkOptimal[k-1]=true;    //뒤의 pageString에 stack->data[k] 값이 있다면, checckoptimal[k-1] true로 변경하여 그 위치의 data가 추후에 쓰임을 명시.
                                count++;
                                break;
                            }
                        }
                    }
                    if(count==totalPageNums-1){ //count 1씩 더해 totalPageNums-1이 되면 마지막 하나(replace될 page)가 남은거니까, break
                        break;
                    }
                }
                if(count!=totalPageNums-1){ //현재 Stack에 있는 모든 page가 뒤의 Page string에 존재하지 않으면
                    pageFault++;
                    int flag=0;
                    for(j=1; j<totalPageNums+1; j++){
                        if(checkOptimal[j-1]==false){
                            flag = 1;
                            break;
                        }
                    }
                    if(flag==1){ //checkOptimal이 모두 true면
                        kickedOutPage = stack->data[j];
                        stack->data[j] = pageString[i][0];
                    }
                    else{
                        kickedOutPage=optimalPop(stack, totalPageNums); //Lifo방식으로 pop하여 Stack에 마지막으로 들어온 page(data)를 쫓아냄.
                        optimalPush(stack, totalPageNums, pageString[i][0]);
                    }
                    printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                    fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                }
                else{
                    for(j=1; j<totalPageNums+1;j++){
                        if(checkOptimal[j-1]==true)
                            continue;
                        else{
                            pageFault++;
                            kickedOutPage=stack->data[j];
                            stack->data[j] = pageString[i][0];  //특정 값 Pop and push 동시 진행
                            printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                            fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                                break;
                        }
                    }
                }
            }
            for(k=0; k<totalPageNums; k++){
                checkOptimal[k]=false;  //checkOptimal false으로 초기화.
            }
        }
        else{   //Stack가 가득 차있지 않는다면
            int flag=0;
            for(j=0; j<totalPageNums+1; j++){
                if(stack->data[j]==pageString[i][0]&&stack->front!=j){  //같은 데이터 있으면 pass
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                optimalPush(stack, totalPageNums, pageString[i][0]); //같은 데이터 없다면 push진행
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                pageFault++;
            }
        }
        printf("%d번째 page/frame 현황: ",i+1);
        fprintf(fileP,"%d번째 page/frame 현황: ",i+1);
        for(int k=0; k<totalPageNums+1;k++){
            if(stack->data[k]!=-1 && stack->front!=k){  //-1이면 Stack가 비어있다고 가정했으므로 비어있지 않은 page들의 현황을 보여줌.
                printf("%d ",stack->data[k]);
                fprintf(fileP,"%d ",stack->data[k]);
            }
        }
        printf("\n\n");
        fprintf(fileP,"\n\n");
    }
    printf("\npage fault 횟수 : %d\n",pageFault);
    fprintf(fileP,"\npage fault 횟수 : %d\n",pageFault);
    printf("**optimal Stack Algorithm Page Replacement 종료**\n");
    fprintf(fileP,"**optimal Stack Algorithm Page Replacement 종료**\n");
    printf("----------------------------------------------\n\n");
    fprintf(fileP,"----------------------------------------------\n\n");
    
    free(checkOptimal);
    free(optS.data);
    
    return pageFault;
}
//-----------------------------------------------------------------------------------------------------------//



//----------------------------------2. FIFO 페이지 교체 알고리즘 함수들------------------------------------------//
//fifo queue init. totalPageNums+1개의 data를 저장할 배열(큐)을 만든다. user는 totalPageNums(총 페이지 개수) 그대로 넘겨줘도 됨.
void fifoInit(fifoQueue *queue, int totalPageNums){
    queue->front=0;
    queue->rear=0;
    queue->data = (int*)malloc(sizeof(int)*(totalPageNums+1));  //queue data 동적할당
}

//queue가 비어있는지 확인하고, 비어있다면 true / 아니면 false Return
bool fifoIsEmpty(fifoQueue *queue){
    if(queue->front==queue->rear)   //front 포인터와 rear 포인터가 같으면 큐가 비어있다는 뜻.
        return true;    //queue is empty
    else
        return false;   //queue is not empty
}

//queue가 차있는지 확인하고, 차있다면 true / 아니면 false Return
bool fifoIsFull(fifoQueue *queue, int totalPageNums){
    if(queue->front==(queue->rear+1)%(totalPageNums+1)) //rear 포인터 다음 위치가 front 포인터 위치와 같다면 큐가 가득 찼다는 뜻.
        return true;    //queue is full
    else
        return false;   //queue is not full
}

//fifo enqueue, user는 totalPageNum 넘겨주면 내부적으로 circular queue 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
bool fifoEnqueue(fifoQueue *queue, int totalPageNums, int data){
    if(fifoIsFull(queue, totalPageNums)){   //큐가 찼으면 enqueue 거부.
        return false;
    }
    else{
        queue->rear = (queue->rear+1)%(totalPageNums+1);    //queue rear 포인터 다음 위치로 옮겨서 queue가 관리하는 data 개수 1개 증가.
        queue->data[queue->rear] = data;    //queue 맨 뒤에 새로운 데이터 넣음
        return true;
    }
}

//fifo dequeue, user는 totalPageNum 넘겨주면 내부적으로 circular queue 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
int fifoDequeue(fifoQueue *queue, int totalPageNums){
    if(fifoIsEmpty(queue)){ //큐가 비어있으면 dequeue 거부.
        return 0;
    }
    else{
        queue->front = (queue->front+1)%(totalPageNums+1);  //front 포인터를 배열의 다음 위치로 이동.
        return queue->data[queue->front];   //dequeue시킨 data Return.
    }
}

int fifoQueueAlgorithm(fifoQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum){
    int pageFault=0;
    int i,j;
    int kickedOutPage;
    fifoInit(queue, totalPageNums);
    for(i=0; i<totalPageNums+1; i++)
        queue->data[i]=-1;  //data가 비었음을 명시하기 위해 -1로 초기화.
    printf("\n----------------------------------------------\n");
    fprintf(fileP,"\n----------------------------------------------\n");
    printf("**FIFO Queue Algorithm으로 Page Replacement 실행**\n\n");
    fprintf(fileP,"**FIFO Queue Algorithm으로 Page Replacement 실행**\n\n");
    for(i=0; i<pageStringNum; i++){    //page string 개수만큼 for문 반복시켜야함.P4
        if(fifoIsFull(queue, totalPageNums)){   //queue가 가득 찼으면 page들을 모두 순회하면서 같은 data가 있는지 확인. 있으면 Page fault 아님.
            for(j=0; j<totalPageNums+1; j++){
                if(queue->data[j]==pageString[i][0]&&queue->front!=j)   //같은 데이터 있으면 page fault 아니니까 패스.
                    break;
            }
            if(j==totalPageNums+1){ //queue가 가득 찼고 같은 data가 없을 때, 가장 먼저 들어온(front에 있는) data 삭제 후 Page fault 셈.(원형 큐라 삭제는 아니고 -1로 설정하고 front pointer가 이동)
                pageFault++;
                kickedOutPage=fifoDequeue(queue, totalPageNums);
                fifoEnqueue(queue, totalPageNums, pageString[i][0]);
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
            }
            
        }
        else{   //page 꽉 차지 않았으면
            int flag=0;
            for(j=0; j<totalPageNums+1; j++){   //page 모두 순회하면서 중복 data 있는지 확인. 있으면 page fault 아님.
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){
                    flag=1;
                    break;
                }
            }
            if(flag==0){    //중복 데이터 없으면, enqueue하고 page fault 셈.
                fifoEnqueue(queue, totalPageNums, pageString[i][0]);
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                pageFault++;
            }
        }
        printf("%d번째 page/frame 현황: ",i+1); //page frame에 저장된 data 출력.
        fprintf(fileP,"%d번째 page/frame 현황: ",i+1);
        for(int k=0; k<totalPageNums+1;k++){
            if(queue->data[k]!=-1 && queue->front!=k){  //-1이면 queue가 비어있다고 가정했으므로 비어있지 않은 page들의 현황을 보여줌.
                printf("%d ",queue->data[k]);
                fprintf(fileP,"%d ",queue->data[k]);
            }
        }
        printf("\n\n");
        fprintf(fileP,"\n\n");
    }
    printf("\npage fault 횟수 : %d\n",pageFault);
    fprintf(fileP,"\npage fault 횟수 : %d\n",pageFault);
    printf("**FIFO Queue Algorithm Page Replacement 종료**\n");
    fprintf(fileP,"**FIFO Queue Algorithm Page Replacement 종료**\n");
    printf("----------------------------------------------\n\n");
    fprintf(fileP,"----------------------------------------------\n\n");
    
    free(fifoQ.data);
    
    return pageFault;
}
//-----------------------------------------------------------------------------------------------------------//



//----------------------------------3. LIFO 페이지 교체 알고리즘 함수들------------------------------------------//
//lifo Stack init. totalPageNums+1개의 data를 저장할 배열(큐)을 만든다. user는 totalPageNums(총 페이지 개수) 그대로 넘겨줘도 됨.
void lifoInit(lifoStack *stack, int totalPageNums){
    stack->front=0;
    stack->rear=0;
    stack->data = (int*)malloc(sizeof(int)*(totalPageNums+1));
}

//stack가 비어있는지 확인하고, 비어있다면 true / 아니면 false Return
bool lifoIsEmpty(lifoStack *stack){
    if(stack->front==stack->rear)
        return true;    //stack is empty
    else
        return false;   //stack is not empty
}

//stack가 차있는지 확인하고, 차있다면 true / 아니면 false Return
bool lifoIsFull(lifoStack *stack, int totalPageNums){
    if(stack->front==(stack->rear+1)%(totalPageNums+1))
        return true;    //stack is full
    else
        return false;   //stack is not full
}

//lifo Push, user는 totalPageNum 넘겨주면 내부적으로 circular stack 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
bool lifoPush(lifoStack *stack, int totalPageNums, int data){
    if(lifoIsFull(stack, totalPageNums)){
        return false;
    }
    else{
        stack->rear = (stack->rear+1)%(totalPageNums+1);
        stack->data[stack->rear] = data;
        return true;
    }
}

//lifo Pop, user는 totalPageNum 넘겨주면 내부적으로 circular stack 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
int lifoPop(lifoStack *stack, int totalPageNums){
    if(lifoIsEmpty(stack)){
        return 0;
    }
    else{
        if(stack->rear==0)
            stack->rear=totalPageNums+1;
        stack->rear = (stack->rear-1)%(totalPageNums+1);
        return stack->data[(stack->rear+1)%(totalPageNums+1)];
    }
}

int lifoStackAlgorithm(lifoStack *stack, int totalPageNums, int (*pageString)[3], int pageStringNum){
    int pageFault=0;
    int i,j;
    int kickedOutPage;
    lifoInit(stack, totalPageNums);
    for(i=0; i<totalPageNums+1; i++)
        stack->data[i]=-1;
    printf("\n----------------------------------------------\n");
    fprintf(fileP,"\n----------------------------------------------\n");
    printf("**lifo stack Algorithm으로 Page Replacement 실행**\n\n");
    fprintf(fileP,"**lifo stack Algorithm으로 Page Replacement 실행**\n\n");
    for(i=0; i<pageStringNum; i++){   //page string 개수만큼 for문 반복시켜야함.P4
        if(lifoIsFull(stack, totalPageNums)){   //stack가 가득 찼으면 page들을 모두 순회하면서 같은 data가 있는지 확인. 있으면 Page fault 아님.
            for(j=0; j<totalPageNums+1; j++){
                if(stack->data[j]==pageString[i][0]&&stack->front!=j)
                    break;
            }
            if(j==totalPageNums+1){ //stack가 가득 찼고 같은 data가 없을 때, 가장 나중에 들어온(rear에 있는) data 삭제 후 Page fault 셈.(원형 큐라 삭제는 아니고 -1로 설정하고 rear pointer가 이동)
                pageFault++;
                kickedOutPage=lifoPop(stack, totalPageNums);
                lifoPush(stack, totalPageNums, pageString[i][0]);
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
            }
            
        }
        else{   //page 꽉 차지 않았으면
            int flag=0;
            for(j=0; j<totalPageNums+1; j++){   //page 모두 순회하면서 중복 data 있는지 확인. 있으면 page fault 아님.
                if(stack->data[j]==pageString[i][0]&&stack->front!=j){
                    flag=1;
                    break;
                }
            }
            if(flag==0){//중복 데이터 없으면, rear에 데이터 추가하고 page fault 셈.
                lifoPush(stack, totalPageNums, pageString[i][0]);
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                pageFault++;
            }
        }
        printf("%d번째 page/frame 현황: ",i+1); //page frame에 저장된 data 출력.
        fprintf(fileP,"%d번째 page/frame 현황: ",i+1);
        for(int k=0; k<totalPageNums+1;k++){
            if(stack->data[k]!=-1 && stack->front!=k){  //-1이면 stack가 비어있다고 가정했으므로 비어있지 않은 page들의 현황을 보여줌.
                printf("%d ",stack->data[k]);
                fprintf(fileP,"%d ",stack->data[k]);
            }
        }
        printf("\n\n");
        fprintf(fileP,"\n\n");
    }
    printf("\npage fault 횟수 : %d\n",pageFault);
    fprintf(fileP,"\npage fault 횟수 : %d\n",pageFault);
    printf("**lifo stack Algorithm Page Replacement 종료**\n");
    fprintf(fileP,"**lifo stack Algorithm Page Replacement 종료**\n");
    printf("----------------------------------------------\n\n");
    fprintf(fileP,"----------------------------------------------\n\n");
    
    free(lifoS.data);
    
    return pageFault;
}
//-----------------------------------------------------------------------------------------------------------//



//----------------------------------4. LRU 페이지 교체 알고리즘 함수들------------------------------------------//
//lru queue init. totalPageNums+1개의 data를 저장할 배열(큐)을 만든다. user는 totalPageNums(총 페이지 개수) 그대로 넘겨줘도 됨.
void lruInit(lruQueue *queue, int totalPageNums){
    queue->front=0;
    queue->rear=0;
    queue->data = (int*)malloc(sizeof(int)*(totalPageNums+1));
    queue->timeStamp = (int*)malloc(sizeof(int)*(totalPageNums+1));
}

//queue가 비어있는지 확인하고, 비어있다면 true / 아니면 false Return
bool lruIsEmpty(lruQueue *queue){
    if(queue->front==queue->rear)
        return true;    //queue is empty
    else
        return false;   //queue is not empty
}

//queue가 차있는지 확인하고, 차있다면 true / 아니면 false Return
bool lruIsFull(lruQueue *queue, int totalPageNums){
    if(queue->front==(queue->rear+1)%(totalPageNums+1))
        return true;    //queue is full
    else
        return false;   //queue is not full
}

//lru enqueue, user는 totalPageNum 넘겨주면 내부적으로 circular queue 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
bool lruEnqueue(lruQueue *queue, int totalPageNums, int data){
    if(lruIsFull(queue, totalPageNums)){
        return false;
    }
    else{
        queue->rear = (queue->rear+1)%(totalPageNums+1);
        queue->data[queue->rear] = data;
        return true;
    }
}

int checkTimeStamp(lruQueue *queue, int totalPageNums){
    int temp=99999;
    int min=0;
    for(int i=0; i<totalPageNums+1; i++){
        if(temp>queue->timeStamp[i]&&queue->data[i]>=0){
            temp=queue->timeStamp[i];
            min=i;
        }
    }
    
    return min;
}

int lruQueueAlgorithm(lruQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum){
    int pageFault=0;
    int i,j;
    int kickedOutPage;
    int kickedOutIndex;
    lruInit(queue, totalPageNums);
    for(i=0; i<totalPageNums+1; i++)
        queue->data[i]=-1;
    printf("\n----------------------------------------------\n");
    fprintf(fileP,"\n----------------------------------------------\n");
    printf("**lru Queue Algorithm으로 Page Replacement 실행**\n\n");
    fprintf(fileP,"**lru Queue Algorithm으로 Page Replacement 실행**\n\n");
    for(i=0; i<pageStringNum; i++){    //page string 개수만큼 for문 반복시켜야함.P4
        if(lruIsFull(queue, totalPageNums)){   //queue가 가득 찼으면 page들을 모두 순회하면서 같은 data가 있는지 확인. 있으면 Page fault 아님.
            for(j=0; j<totalPageNums+1; j++){
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){
                    queue->timeStamp[j]=i;  //현재 data[j]가 참조된 시간을 현재 스트링 위치로 설정한다.
                    break;
                }
            }
            if(j==totalPageNums+1){ //queue가 가득 찼고 같은 data가 없을 때, 가장 먼저 들어온(front에 있는) data 삭제 후 Page fault 셈.(원형 큐라 삭제는 아니고 -1로 설정하고 front pointer가 이동)
                pageFault++;
                kickedOutIndex=checkTimeStamp(queue, totalPageNums);    //least recently used page의 index 찾기
                kickedOutPage=queue->data[kickedOutIndex];  //least recently used page의 index 위치에 있는 page 값 kickedOutPage에 저장.
                queue->data[kickedOutIndex]=pageString[i][0];   //찾는 index의 배열값 넣으려는 Page로 변경.
                queue->timeStamp[kickedOutIndex]=i; //현재 집어넣은 page의 timeStamp i로 설정하여 i값을 현재 시간으로 기록하기.
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
            }
        }
        else{   //page 꽉 차지 않았으면
            int flag=0;
            for(j=0; j<totalPageNums+1; j++){   //page 모두 순회하면서 중복 data 있는지 확인. 있으면 page fault 아님.
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){
                    flag=1;
                    queue->timeStamp[j]=i;  //현재 data[j]가 참조된 시간을 현재 스트링 위치로 설정한다.
                    break;
                }
            }
            if(flag==0){    //중복 데이터 없으면, rear에 데이터 추가하고 page fault 셈.
                lruEnqueue(queue, totalPageNums, pageString[i][0]);
                queue->timeStamp[queue->rear]=i;  //현재 data[j]가 참조된 시간을 현재 스트링 위치로 설정한다.
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                pageFault++;
            }
        }
        printf("%d번째 page/frame 현황: ",i+1); //page frame에 저장된 data 출력.
        fprintf(fileP,"%d번째 page/frame 현황: ",i+1);
        for(int k=0; k<totalPageNums+1;k++){
            if(queue->data[k]!=-1 && queue->front!=k){  //-1이면 queue가 비어있다고 가정했으므로 비어있지 않은 page들의 현황을 보여줌.
                printf("%d ",queue->data[k]);
                fprintf(fileP,"%d ",queue->data[k]);
            }
        }
        printf("\n\n");
        fprintf(fileP,"\n\n");
    }
    printf("\npage fault 횟수 : %d\n",pageFault);
    fprintf(fileP,"\npage fault 횟수 : %d\n",pageFault);
    printf("**lru Queue Algorithm Page Replacement 종료**\n");
    fprintf(fileP,"**lru Queue Algorithm Page Replacement 종료**\n");
    printf("----------------------------------------------\n\n");
    fprintf(fileP,"----------------------------------------------\n\n");
    
    free(lruQ.data);
    free(lruQ.timeStamp);
    
    return pageFault;
}
//-----------------------------------------------------------------------------------------------------------//


//----------------------------------5. LFU 페이지 교체 알고리즘 함수들------------------------------------------//
//lfu queue init. totalPageNums+1개의 data를 저장할 배열(큐)을 만든다. user는 totalPageNums(총 페이지 개수) 그대로 넘겨줘도 됨.
void lfuInit(lfuQueue *queue, int totalPageNums){
    queue->front=0;
    queue->rear=0;
    queue->data = (int*)malloc(sizeof(int)*(totalPageNums+1));
    queue->count = (int*)malloc(sizeof(int)*(32));   //frequent 세기 위해 30개 만큼 크기의 배열 생성.(제약조건에 의해 총 페이지 스트림은 1~30 30개니까.)
}

//queue가 비어있는지 확인하고, 비어있다면 true / 아니면 false Return
bool lfuIsEmpty(lfuQueue *queue){
    if(queue->front==queue->rear)
        return true;    //queue is empty
    else
        return false;   //queue is not empty
}

//queue가 차있는지 확인하고, 차있다면 true / 아니면 false Return
bool lfuIsFull(lfuQueue *queue, int totalPageNums){
    if(queue->front==(queue->rear+1)%(totalPageNums+1))
        return true;    //queue is full
    else
        return false;   //queue is not full
}

//lfu enqueue, user는 totalPageNum 넘겨주면 내부적으로 circular queue 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
bool lfuEnqueue(lfuQueue *queue, int totalPageNums, int data){
    if(lfuIsFull(queue, totalPageNums)){
        return false;
    }
    else{
        queue->rear = (queue->rear+1)%(totalPageNums+1);
        queue->data[queue->rear] = data;
        return true;
    }
}

int getLowestCount(lfuQueue *queue, int totalPageNums){
    int temp=9999;
    int i=0;
    int k=0;
    
    for(i=0; i<totalPageNums+1; i++){
        if((temp>queue->count[queue->data[i]])&&(queue->count[queue->data[i]]>0)&&(queue->data[i]>=0)){
            temp=queue->count[queue->data[i]];
            k=i;
        }
    }
    return k;
}

int lfuQueueAlgorithm(lfuQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum){
    int pageFault=0;
    int i,j;
    int kickedOutPage;
    int lowestCount=0;
    lfuInit(queue, totalPageNums);
    for(i=0; i<totalPageNums+1; i++)
        queue->data[i]=-1;
    for(i=0; i<32; i++)
        queue->count[i]=0;
    printf("\n----------------------------------------------\n");
    fprintf(fileP,"\n----------------------------------------------\n");
    printf("**lfu Queue Algorithm으로 Page Replacement 실행**\n\n");
    fprintf(fileP,"**lfu Queue Algorithm으로 Page Replacement 실행**\n\n");
    for(i=0; i<pageStringNum; i++){   //page string 개수만큼 for문 반복시켜야함.P4
        if(lfuIsFull(queue, totalPageNums)){   //queue가 가득 찼으면 page들을 모두 순회하면서 같은 data가 있는지 확인. 있으면 Page fault 아님.
            for(j=0; j<totalPageNums+1; j++){
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){
                    queue->count[pageString[i][0]]++;   //참조하면, 현재 넣으려는 page(data)인 pageString[i][0] 에 해당하는 위치의 count값 +1;
                    break;
                }
            }
            if(j==totalPageNums+1){ //queue가 가득 찼고 같은 data가 없을 때, 가장 나중에 들어온(rear에 있는) data 삭제 후 Page fault 셈.(원형 큐라 삭제는 아니고 -1로 설정하고 rear pointer가 이동)
                pageFault++;
                lowestCount = getLowestCount(queue, totalPageNums); //kickedOutPage를 queue->count[queue->data]값이 가장 작은것에 해당하는 것으로 설정하고 쫓아냄.
                kickedOutPage = queue->data[lowestCount];
                queue->data[lowestCount] = pageString[i][0];
                queue->count[pageString[i][0]]++;   //참조하면, 현재 넣으려는 page(data)인 pageString[i][0] 에 해당하는 위치의 count값 +1;
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
            }
            
        }
        else{   //page 꽉 차지 않았으면
            int flag=0;
            for(j=0; j<totalPageNums+1; j++){   //page 모두 순회하면서 중복 data 있는지 확인. 있으면 page fault 아님.
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){
                    flag=1;
                    queue->count[pageString[i][0]]++;   //참조하면, 현재 넣으려는 page(data)인 pageString[i][0] 에 해당하는 위치의 count값 +1;
                    break;
                }
            }
            if(flag==0){//중복 데이터 없으면, rear에 데이터 추가하고 page fault 셈.
                lfuEnqueue(queue, totalPageNums, pageString[i][0]);
                queue->count[pageString[i][0]]++;   //참조하면, 현재 넣으려는 page(data)인 pageString[i][0] 에 해당하는 위치의 count값 +1;
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                pageFault++;
            }
        }
        printf("%d번째 page/frame 현황: ",i+1); //page frame에 저장된 data 출력.
        fprintf(fileP,"%d번째 page/frame 현황: ",i+1);
        for(int k=0; k<totalPageNums+1;k++){
            if(queue->data[k]!=-1 && queue->front!=k){  //-1이면 queue가 비어있다고 가정했으므로 비어있지 않은 page들의 현황을 보여줌.
                printf("%d ",queue->data[k]);
                fprintf(fileP,"%d ",queue->data[k]);
            }
        }
        printf("\n\n");
        fprintf(fileP,"\n\n");
    }
    printf("\npage fault 횟수 : %d\n",pageFault);
    fprintf(fileP,"\npage fault 횟수 : %d\n",pageFault);
    printf("**lfu Queue Algorithm Page Replacement 종료**\n");
    fprintf(fileP,"**lfu Queue Algorithm Page Replacement 종료**\n");
    printf("----------------------------------------------\n\n");
    fprintf(fileP,"----------------------------------------------\n\n");
    
    free(lfuQ.data);
    free(lfuQ.count);
    
    return pageFault;
}
//-----------------------------------------------------------------------------------------------------------//


//----------------------------------6. SC 페이지 교체 알고리즘 함수들------------------------------------------//
//sc queue init. totalPageNums+1개의 data를 저장할 배열(큐)을 만든다. user는 totalPageNums(총 페이지 개수) 그대로 넘겨줘도 됨.
void scInit(scQueue *queue, int totalPageNums){
    queue->front=0;
    queue->rear=0;
    queue->data = (int*)malloc(sizeof(int)*(totalPageNums+1));
    queue->referenceBit = (int*)malloc(sizeof(int)*(totalPageNums+1));  //reference bit 배열 따로 유지하기 위해 할당.
    for(int i=0; i<totalPageNums+1; i++)  //reference bit 0으로 초기화.
        queue->referenceBit[i]=0;
}

//queue가 비어있는지 확인하고, 비어있다면 true / 아니면 false Return
bool scIsEmpty(scQueue *queue){
    if(queue->front==queue->rear)
        return true;    //queue is empty
    else
        return false;   //queue is not empty
}

//queue가 차있는지 확인하고, 차있다면 true / 아니면 false Return
bool scIsFull(scQueue *queue, int totalPageNums){
    if(queue->front==(queue->rear+1)%(totalPageNums+1))
        return true;    //queue is full
    else
        return false;   //queue is not full
}

//sc enqueue, user는 totalPageNum 넘겨주면 내부적으로 circular queue 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
bool scEnqueue(scQueue *queue, int totalPageNums, int data, int referenceBit){
    if(scIsFull(queue, totalPageNums)){
        return false;
    }
    else{
        queue->rear = (queue->rear+1)%(totalPageNums+1);
        queue->data[queue->rear] = data;
        queue->referenceBit[queue->rear] = referenceBit;
        return true;
    }
}

//sc dequeue, user는 totalPageNum 넘겨주면 내부적으로 circular queue 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
int scDequeue(scQueue *queue, int totalPageNums){
    if(scIsEmpty(queue)){
        return 0;
    }
    else{
        queue->front = (queue->front+1)%(totalPageNums+1);
        return queue->data[queue->front];
    }
}

int scQueueAlgorithm(scQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum){
    int pageFault=0;
    int i,j;
    int kickedOutPage;
    int clockHand;  //circular queue의 현재 위치 가리키고 있을 pointer 지정.
    int referenceBit;
    
    scInit(queue, totalPageNums);
    clockHand=(queue->rear+1)%(totalPageNums+1);    //clockhand를 rear 다음 위치로 초기화.
    for(i=0; i<totalPageNums+1; i++)
        queue->data[i]=-1;  //data가 비었음을 명시하기 위해 -1로 초기화.
    printf("\n----------------------------------------------\n");
    fprintf(fileP,"\n----------------------------------------------\n");
    printf("**sc Queue Algorithm으로 Page Replacement 실행**\n\n");
    fprintf(fileP,"**sc Queue Algorithm으로 Page Replacement 실행**\n\n");
    for(i=0; i<pageStringNum; i++){    //page string 개수만큼 for문 반복시켜야함.P4
        if(scIsFull(queue, totalPageNums)){
            for(j=0; j<totalPageNums+1; j++){   //중복된 data 있는지 확인, 있으면 reference bit 1로 만들고, page fault 아니니 패스.
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){
                    queue->referenceBit[j]=1;   //넣으려는 page number가 이미 있다면 그 page의 reference bit를 1로 설정.
                    break;
                }
            }
            if(j==totalPageNums+1){ //중복된 data 없다면, page fault인데, clockHand 위치에서 시작하여 reference bit를 비교하여 0이면 그 page 쫓아내고 그 자리에 현재 pageString의 Page 번호를 집어넣음.
                for(j=clockHand;; j=(j+1)%(totalPageNums+1)){   //circular queue에서 clockhand 계속 순환시킴.
                    if(j==queue->front) //clockhand가 front면 그 위치 확인하지 말고 계속 순환시킴.
                        continue;
                    if(queue->referenceBit[j]==0){  //referenceBit가 0이면 그 위치의 Page 교체!
                        pageFault++;
                        kickedOutPage=queue->data[j];
                        queue->data[j] = pageString[i][0];  //dequeue enqueue 동시에 진행.
                        queue->referenceBit[j]=1;   //page 넣을때 그 page reference bit는 초기에 1로 설정.
                        clockHand=(j+1)%(totalPageNums+1);  //clockhand를 교체한 페이지의 위치의 다음 위치로 설정
                        if(clockHand==queue->front) //clockhand front 위치면 계속 이동시킴
                            clockHand=(queue->front+1)%(totalPageNums+1);
                        printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                        fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                        break;
                    }
                    else{
                        queue->referenceBit[j]=0;   //reference bit==1이면 second chance 줘서 reference bit만 0으로 바꾸고 for문으로 돌아감.
                    }
                }
            }
            
        }
        else{
            int flag=0;
            for(j=0; j<totalPageNums+1; j++){
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){
                    flag=1;
                    queue->referenceBit[j]=1;   //넣으려는 page number가 이미 있다면 그 page의 reference bit를 1로 설정.
                    break;
                }
            }
            if(flag==0){
                referenceBit=1; //data 참조되면 무조건 Reference bit는 1
                scEnqueue(queue, totalPageNums, pageString[i][0], 1);
                clockHand=(queue->rear+1)%(totalPageNums+1);
                if(clockHand==queue->front)
                    clockHand=(queue->front+1)%(totalPageNums+1);
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                pageFault++;
            }
        }
        printf("%d번째 page/frame 현황: ",i+1);
        fprintf(fileP,"%d번째 page/frame 현황: ",i+1);
        for(int k=0; k<totalPageNums+1;k++){
            if(queue->data[k]!=-1 && queue->front!=k){  //-1이면 queue가 비어있다고 가정했으므로 비어있지 않은 page들의 현황을 보여줌.
                printf("%d(%d) ",queue->data[k], queue->referenceBit[k]);
                fprintf(fileP,"%d(%d) ",queue->data[k], queue->referenceBit[k]);
            }
        }
        printf("\n\n");
        fprintf(fileP,"\n\n");
    }
    printf("\npage fault 횟수 : %d\n",pageFault);
    fprintf(fileP,"\npage fault 횟수 : %d\n",pageFault);
    printf("**sc Queue Algorithm Page Replacement 종료**\n");
    fprintf(fileP,"**sc Queue Algorithm Page Replacement 종료**\n");
    printf("----------------------------------------------\n\n");
    fprintf(fileP,"----------------------------------------------\n\n");
    
    free(scQ.data);
    free(scQ.referenceBit);
    
    return pageFault;
}
//-----------------------------------------------------------------------------------------------------------//



//----------------------------------7. ESC 페이지 교체 알고리즘 함수들------------------------------------------//
//esc queue init. totalPageNums+1개의 data를 저장할 배열(큐)을 만든다. user는 totalPageNums(총 페이지 개수) 그대로 넘겨줘도 됨.
void escInit(escQueue *queue, int totalPageNums){
    queue->front=0;
    queue->rear=0;
    queue->data = (int*)malloc(sizeof(int)*(totalPageNums+1));
    queue->referenceBit = (int*)malloc(sizeof(int)*(totalPageNums+1));  //reference bit 따로 배열로 유지
    queue->writeBit = (int*)malloc(sizeof(int)*(totalPageNums+1));  //write bit 따로 배열로 유지
    for(int i=0; i<totalPageNums+1; i++){  //reference bit, write bit 0으로 초기화.
        queue->referenceBit[i]=0;
        queue->writeBit[i]=0;
    }
}

//queue가 비어있는지 확인하고, 비어있다면 true / 아니면 false Return
bool escIsEmpty(escQueue *queue){
    if(queue->front==queue->rear)
        return true;    //queue is empty
    else
        return false;   //queue is not empty
}

//queue가 차있는지 확인하고, 차있다면 true / 아니면 false Return
bool escIsFull(escQueue *queue, int totalPageNums){
    if(queue->front==(queue->rear+1)%(totalPageNums+1))
        return true;    //queue is full
    else
        return false;   //queue is not full
}

//esc enqueue, user는 totalPageNum 넘겨주면 내부적으로 circular queue 구현 방식에 맞춰 총 totalPageNum+1개 자리 가진 배열로 수행해줌.
bool escEnqueue(escQueue *queue, int totalPageNums, int data, int referenceBit, int writeBit){
    if(escIsFull(queue, totalPageNums)){
        return false;
    }
    else{
        queue->rear = (queue->rear+1)%(totalPageNums+1);
        queue->data[queue->rear] = data;
        queue->referenceBit[queue->rear] = referenceBit;
        return true;
    }
}

int escQueueAlgorithm(escQueue *queue, int totalPageNums, int (*pageString)[3], int pageStringNum){
    int pageFault=0;
    int i,j;
    int kickedOutPage;
    int clockHand;  //circular queue의 현재 위치 가리키고 있을 pointer 지정.
    int referenceBit, writeBit;
    
    escInit(queue, totalPageNums);
    clockHand=(queue->rear+1)%(totalPageNums+1);
    for(i=0; i<totalPageNums+1; i++)
        queue->data[i]=-1;   //data가 비었음을 명시하기 위해 -1로 초기화.
    printf("\n----------------------------------------------\n");
    fprintf(fileP,"\n----------------------------------------------\n");
    printf("**esc Queue Algorithm으로 Page Replacement 실행**\n\n");
    fprintf(fileP,"**esc Queue Algorithm으로 Page Replacement 실행**\n\n");
    for(i=0; i<pageStringNum; i++){    //page string 개수만큼 for문 반복시켜야함.P4
        if(escIsFull(queue, totalPageNums)){
            for(j=0; j<totalPageNums+1; j++){   //중복된 data 있는지 확인, 있으면 reference bit 1로 만들고, page fault 아니니 패스.
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){  //같은 데이터 있으면
                    queue->referenceBit[j]=1;   //reference bit는 무조건 1로
                    if(queue->writeBit[j]==0&&pageString[i][2]==1)  //넣으려는 page number가 이미 있고, writeBit가 0이고 넣는 pageString의 writeBit가 1이면 1로 설정
                        queue->writeBit[j]=1;
                    break;
                }
            }
            if(j==totalPageNums+1){ //중복된 data 없다면, page fault인데, clockHand 위치에서 시작하여 reference bit와 write bit를 비교하여 (0,0)이면 그 page 쫓아내고 그 자리에 현재 pageString의 Page 번호를 집어넣음.
                for(j=clockHand;; j=(j+1)%(totalPageNums+1)){
                    if(j==queue->front)
                        continue;
                    if(queue->referenceBit[j]==0 && queue->writeBit[j]==0){
                        pageFault++;
                        kickedOutPage=queue->data[j];
                        queue->data[j] = pageString[i][0];  //dequeue enqueue 동시에 진행.
                        queue->referenceBit[j] = 1; //pageString에 랜덤으로 생성된 reference 상관없이 새로 들어가는 referenceBit는 1로 설정.
                        queue->writeBit[j] = pageString[i][2];  //pageString에 생성되어있는 write bit가 1이면 writebit도 1로 설정.
                        clockHand=(j+1)%(totalPageNums+1);
                        if(clockHand==queue->front)
                            clockHand=(queue->front+1)%(totalPageNums+1);
                        printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                        fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!(%d page 쫓겨남)\n",i+1,pageString[i][0], kickedOutPage);
                        break;
                    }
                    else{   //clockHand돌면서 현재 clockHand위치의 reference bit/write bit을 0으로 지워나감.
                        if(queue->referenceBit[j]==1 && queue->writeBit[j]==0){ //reference bit만 1이면 0으로
                            queue->referenceBit[j]=0;
                        }
                        else if(queue->writeBit[j]==1 && queue->referenceBit[j]==1){    //write bit 와 reference bit 둘다 1이면 write bit만 0으로
                            queue->writeBit[j]=0;
                        }
                        else
                            queue->referenceBit[j]=0;   //reference bit==1이면 second chance 줘서 reference bit만 0으로 바꾸고 for문으로 돌아감.
                    }
                    
                    
                }
            }
            
        }
        else{
            int flag=0;
            for(j=0; j<totalPageNums+1; j++){
                if(queue->data[j]==pageString[i][0]&&queue->front!=j){  //같은 데이터 있으면
                    flag=1;
                    queue->referenceBit[j]=1;   //reference bit는 무조건 1로
                    if(queue->writeBit[j]==0&&pageString[i][2]==1)  //넣으려는 page number가 이미 있고, writeBit가 0이고 넣는 pageString의 writeBit가 1이면 1로 설정
                        queue->writeBit[j]=1;
                    break;
                }
            }
            if(flag==0){
                referenceBit=1;
                writeBit=0;
                escEnqueue(queue, totalPageNums, pageString[i][0], referenceBit, writeBit);
                clockHand=(queue->rear+1)%(totalPageNums+1);
                if(clockHand==queue->front)
                    clockHand=(queue->front+1)%(totalPageNums+1);
        
                printf("%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                fprintf(fileP,"%d번째 page/frame에 string %d 교체 시 pagefault 발생!\n",i+1,pageString[i][0]);
                pageFault++;
            }
        }
        printf("%d번째 page/frame 현황: ",i+1);
        fprintf(fileP,"%d번째 page/frame 현황: ",i+1);
        for(int k=0; k<totalPageNums+1;k++){
            if(queue->data[k]!=-1 && queue->front!=k){  //-1이면 queue가 비어있다고 가정했으므로 비어있지 않은 page들의 현황을 보여줌.
                printf("%d(r:%d, w:%d) ",queue->data[k], queue->referenceBit[k], queue->writeBit[k]);
                fprintf(fileP,"%d(r:%d, w:%d) ",queue->data[k], queue->referenceBit[k], queue->writeBit[k]);
            }
        }
        printf("\n\n");
        fprintf(fileP,"\n\n");
    }
    printf("\npage fault 횟수 : %d\n",pageFault);
    fprintf(fileP,"\npage fault 횟수 : %d\n",pageFault);
    printf("**esc Queue Algorithm Page Replacement 종료**\n");
    fprintf(fileP,"**esc Queue Algorithm Page Replacement 종료**\n");
    printf("----------------------------------------------\n\n");
    fprintf(fileP,"----------------------------------------------\n\n");
    
    free(escQ.data);
    free(escQ.referenceBit);
    free(escQ.writeBit);
    
    return pageFault;
}
//-----------------------------------------------------------------------------------------------------------//

//printOutData 함수는 랜덤으로 생성되거나 파일에서 불러온 파일 스트링 출력해줌.
void printOutData(int (*pageString)[3], int pageStringNum){
    for(int i=0; i<pageStringNum; i++){
        printf("%d번째 데이터-> 페이지 번호: %d, R: %d W: %d\n",i+1, pageString[i][0],pageString[i][1],pageString[i][2]);
        fprintf(fileP,"%d번째 데이터-> 페이지 번호: %d, R: %d W: %d\n",i+1, pageString[i][0],pageString[i][1],pageString[i][2]);
    }
}
