#include <stdio.h>

#include "ui.h"
#include "page-replacement-algorithms.h"

int main(int argc, char *argv[]) {
    int simulatorNums[3]={0,};      //알고리즘 선택용 배열
    int totalPageNums=0;            //총 페이지/프레임 개수 저장
    int dataInputType=0;            //데이터 입력 타입 1또는2 저장
    int pageStringNum=0;   //page string 개수에 따라 값 설정.(명세서에 최소 500개의 페이지 스트링 원칙 외에 특별한 조건 없어 우선 500으로 고정해놓음.)P4
    int (*pageString)[3]=NULL;    //string(data), read bit, write bit 순으로 저장하므로 2차원 배열의 행은 3개로 고정.
    
    getPageString(&pageStringNum, &pageString);
    
    getSimulatorNums(simulatorNums);    //사용자가 선택하고자 하는 알고리즘 보여주고, 선택한 값 배열에 저장.
    
    getTotalPageNums(&totalPageNums);   //페이지 개수 고르게 함.
    
    getDataInputType(&dataInputType);   //데이터 입력 형식 랜덤/파일 중 하나 고르도록 함.
   
    dataInput(pageString, dataInputType, pageStringNum, argv[1]);   //사용자 dataInputType에 따라 page string 생성하기.
    
    initSelectedAlgorithms(simulatorNums, totalPageNums, pageString, pageStringNum);   //선택한 알고리즘대로 페이지 교체 실행.
    
    free(pageString);
    
    return 0;
}
