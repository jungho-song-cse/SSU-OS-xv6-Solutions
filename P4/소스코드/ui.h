#ifndef ui_h
#define ui_h

void getPageString(int *pageStringNum, int (**pageString)[3]);    //pageString 개수 사용자에게서 입력받음.
void getSimulatorNums(int simulatorNums[3]); //앞 3개 값 중 8입력받으면 무조건 가장 첫 Index의 배열 값이 8로 설정됨.
void getTotalPageNums(int *totalPageNums);  //페이지/프레임 개수 받기.
void getDataInputType(int *dataInputType);  //데이터 입력 방식 선택
void randomDataInput(int (*pageString)[3], int pageStringNum);     //데이터 입력 방식 (1) 인 경우, 랜덤한 페이지 스트링 생성 경우 사용.
void makeRandomDataFile(int pageStringNum);              //페이지 스트링 랜덤으로 파일로 저장할 때, 편의를 위해 사용.
void selectFile(int pageStringNum); //file에서 그냥 데이터를 읽을지, file에 랜덤 데이터 저장하고 그 file 읽을지 선택.
void fileDataInput(int (*pageString)[3],int pageStringNum, char argv[]);       //데이터 입력 방식 (2) 인 경우, 파일에서 페이지 스트링 생성 경우 사용.
void dataInput(int (*pageString)[3], int dataInputType, int pageStringNum, char argv[]);        //dataInputType에 따라 data input을 랜덤으로 할지, 파일에서 읽어올지 결정.

#endif /* ui_h */
