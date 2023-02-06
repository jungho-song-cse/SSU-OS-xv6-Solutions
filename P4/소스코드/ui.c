#include <stdio.h>
#include <time.h>   //srand을 위해 사용.
#include <stdlib.h>

//getPageString함수는 pageString의 개수(data의 개수)를 사용자로부터 입력받음 ->명세서에서 최소 500개의 pageString이라고 명시되어 있어 사용자에게 개수 지정하도록 만듦.
void getPageString(int *pageStringNum, int (**pageString)[3]){
    printf("Page String 개수를 선택하세요.(0보다 큰 정수만 입력)(후에 랜덤 혹은 파일로 선택가능): ");
    while(1){
        scanf("%d",pageStringNum);
        if(getchar()!='\n'){
            printf("잘못된 형식입니다. 0보다 큰 정수만 입력하시오 : ");
            while(getchar()!='\n');
            continue;
        }
        if(*pageStringNum>0)
            break;
        printf("0보다 큰 수를 입력하시오: ");
    }
    *pageString = (int(*)[3])malloc(sizeof(int)*(*pageStringNum)*3);    //사용자가 입력한 page string 개수만큼 pagestring 할당.
    
    if(pageString==NULL)
        return;
}


//시뮬레이터 알고리즘 번호 선택, 3개의 int 원소 갖는 배열로 받고, 사용자 입력이 3개 이상이면 앞의 3개만 받고 뒤는 무시.
void getSimulatorNums(int simulatorNums[]){
    printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오.\n(최대 3개, 4개 이상 입력시 오류, 중복 값 입력시에도 최대 3개 받고 중복 무시. ex->1 1 3 입력시 1,3 두개의 알고리즘만 실행.)\n");
    printf("(입력 팁: 숫자 1부터 8 사이를 (1) (1 2) (1 2 3) 꼴로 공백으로 구분하여 써야 인식.)\n");
    printf("(1) Optimal  (2) FIFO  (3) LIFO  (4) LRU  (5) LFU  (6) SC  (7) ESC  (8) ALL\n");
    
    char temp;
    for(int i=0; i<3; i++){
        scanf("%d", &simulatorNums[i]);
        temp=getchar();
        if(temp!='\n'&&temp!=' '){
            printf("잘못된 입력입니다. 1~8 정수만 최대 3개 공백으로 구분하여 입력하시오(1)또는(1 2)또는(1 2 3)꼴 : ");
            while(getchar()!='\n');
            i=-1;
            continue;
        }
        if(simulatorNums[i]<1 || simulatorNums[i]>8){
            printf("잘못된 입력입니다. 1~8 정수만 최대 3개 공백으로 구분하여 입력하시오(1)또는(1 2)또는(1 2 3)꼴 : ");
            i=-1;
            if(temp!='\n')
                while(getchar()!='\n');
            for(int j=0; j<3; j++)
                simulatorNums[j]=0;
            continue;
        }
        if(simulatorNums[i]==8){
            simulatorNums[0]=8;
        }
        if(temp=='\n'){
            break;
        }
        if(i==2&&temp!='\n'){
            printf("잘못된 입력입니다. 1~8 정수만 최대 3개 공백으로 구분하여 입력하시오(1)또는(1 2)또는(1 2 3)꼴 : ");
            i=-1;
            while(getchar()!='\n');
            for(int j=0; j<3; j++)
                simulatorNums[j]=0;
            continue;
        }
    }
}

//페이지/프레임의 개수 받음
void getTotalPageNums(int *totalPageNums){
    printf("B. 페이지/프레임의 개수를 입력하시오.(3~10)\n");
    while(1){
        scanf("%d", totalPageNums);
        if(getchar()!='\n'){
            printf("잘못된 페이지/프레임 개수. 다시 입력하시오. (3~10)\n");
            while(getchar()!='\n');
            continue;
        }
        if(*totalPageNums>=3 && *totalPageNums<=10)
            break;
        else{
            printf("잘못된 페이지/프레임 개수. 다시 입력하시오. (3~10)\n");
        }
    }
}

//데이터 입력 방식 선택 1은 랜덤, 2는 이미 있는 파일 불러옴.
void getDataInputType(int *dataInputType){
    printf("C. 데이터 입력 방식을 선택하시오.(1,2)\n");
    printf("(1) 랜덤하게 생성  (2) 사용자 생성 파일 오픈\n");
    
    while(1){
        scanf("%d", dataInputType);
        if(getchar()!='\n'){
            printf("잘못된 데이터 입력 방식 선택. 다시 입력하시오. (1,2)\n");
            while(getchar()!='\n');
            continue;
        }
        if(*dataInputType==1 || *dataInputType==2)
            break;
        else{
            printf("잘못된 데이터 입력 방식 선택. 다시 입력하시오. (1,2)\n");
        }
    }
}

//랜덤으로 page string, reference bit, write bit 생성. (0,0), (1,0), (1,1)
void randomDataInput(int (*pageString)[3], int pageStringNum){
    srand((unsigned int)time(NULL));
    for(int i=0; i<pageStringNum; i++){
        pageString[i][0]=rand()%30+1;   //1~31 사이의 page stream을 가진다는 제약조건이 있으니, data는 무조건 1~31 값 중 하나를 가짐.
        pageString[i][1]=rand()%2;
        pageString[i][2]=rand()%2;
        if(pageString[i][2]==1) //write bit가 1이면 reference는 무조건 1로 설정.
            pageString[i][1]=1;
    }
}

//랜덤으로 page string, reference bit, write bit 생성 후 파일에 저장.
void makeRandomDataFile(int pageStringNum){
    FILE *fileP;
    
    fileP = fopen("stringPattern.txt", "w");
    if(fileP==NULL){
        printf("file open failed!\n");
        exit(1);
    }
    
    srand((unsigned int)time(NULL));
    for(int i=0; i<pageStringNum; i++){
        fprintf(fileP,"%d %d %d\n",rand()%30+1, rand()%2, rand()%2);
    }
    fclose(fileP);
}

//파일을 사용할 때, 랜덤 값을 파일에 저장하고 사용하려면 0번, 이미 파일에 있는 값을 쓰려면 1번을
void selectFile(int pageStringNum){
    int select;
    printf("파일에 있는 내용을 그대로 읽으려면 0번. 파일에 무작위 page string(data,r,w) 저장 후 그 파일 읽으려면 1번. : ");
    while(1){
        scanf("%d",&select);
        if(select==0||select==1)
            break;
        while(getchar()!='\n');
        printf("0 또는 1 중 선택하시오. : ");
    }
    while(getchar()!='\n');
    if(select==1){
        makeRandomDataFile(pageStringNum);
    }
}

//파일에 저장된 page string, reference bit, write bit 불러옴.
void fileDataInput(int (*pageString)[3],int pageStringNum, char argv[]){
    FILE *fileP;
    int temp;
    char temp2;
    
    if(argv==NULL){ //실행파일 뒤에 인자가 오지 않으면 기본으로 stringPattern.txt파일에서 읽어옴.
        fileP = fopen("stringPattern.txt", "r");
    }
    else{
        fileP = fopen(argv, "r");
    }
    
    if(fileP==NULL){
        printf("file open failed!\n");
        exit(1);
    }
    fscanf(fileP, "%d", &temp);
    fscanf(fileP, "%c", &temp2);
    if(temp2!='\n'){
        //아래는 rw비트도 파일에 저장되어 있으면 읽도록 만듦.
        rewind(fileP);
        for(int i=0; i<pageStringNum; i++){
            fscanf(fileP, "%d %d %d", &pageString[i][0], &pageString[i][1], &pageString[i][2]);
        }
    }
    else{
        //아래는 rw비트 없이 파일 스트링만 있을 경우 사용됨. r w 비트는 0 0으로 고정 설정.
        rewind(fileP);
        for(int i=0; i<pageStringNum; i++){
            fscanf(fileP, "%d", &pageString[i][0]);
            pageString[i][1]=0;
            pageString[i][2]=0;
        }
    }
    fclose(fileP);
}

//dataInputType에 따라 pageString 생성을 랜덤에서 불러올지, 파일에서 불러올지 결정
void dataInput(int (*pageString)[3], int dataInputType, int pageStringNum, char argv[]){
    if(dataInputType==1)
        randomDataInput(pageString, pageStringNum);
    else{
        selectFile(pageStringNum);   //file에서 그냥 데이터를 읽을지, file에 랜덤 데이터 저장하고 그 file 읽을지 선택.
        fileDataInput(pageString, pageStringNum, argv);
    }
}
