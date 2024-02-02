#include <iostream>
#include <ctime>
#include <cstdlib>      //rand 함수 사용하기 귀함

#define SORTNUM 100000
#define PRINTNUM 20     //test용 크기


using namespace std;


int arrayResult[SORTNUM];						//저장 위한 배열



void randArray(int* arr) {

    //난수를 이용한 배열 초기화
    //프로그램 실행 시점마다 random 값 달라지게 하기 위한 코드
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < SORTNUM; i++) {
        int num = rand();
        arr[i] = num;
    }

}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int quickPartition(int* arr, int low, int high) {
    int pivot = arr[low];       //피벗을 배열의 첫 번째 인덱스로 선택
    int i = low + 1;            

    for (int j = low + 1; j <= high; j++) {
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]); //작은 숫자를 피벗의 왼쪽으로 이동
            i++;
        }
    }

    swap(arr[low], arr[i - 1]); //피벗의 최종 위치를 찾아서 교환
    return i - 1;               //피벗의 최종 위치 반환
}


void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pivotIndex = quickPartition(arr, low, high);

        //분할된 부분 배열에 대해 재귀적으로 퀵 정렬 수행
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}


void funcTimeFinish(clock_t timeStart) {

    clock_t timeFinish;     //실행 시간 저장 위한 변수
    double spendTime;       //시작 시간과 종료 시간 계산 후 저장할 변수
    timeFinish = clock();
    spendTime = (double)(timeFinish - timeStart) / CLOCKS_PER_SEC;
    printf("정렬 소요 시간 : %f초", spendTime);
}



int main() {


    randArray(arrayResult);

    clock_t timeStart = clock();
    quickSort(arrayResult, 0, SORTNUM);
    funcTimeFinish(timeStart);

    cout << endl;
    for (int i = 0; i < PRINTNUM; i++) {
        cout << arrayResult[i] << " ";
    }

}
