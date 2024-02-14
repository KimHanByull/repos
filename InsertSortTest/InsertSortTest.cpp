#include <iostream>
#include <cstdlib>      //rand 함수 사용하기 귀함

#define SORTNUM 100000
#define PRINTNUM 20     //test용 크기


using namespace std;


int arr[SORTNUM];						//저장 후 저장 위한 배열


void randArray(int* arr) {


	// 난수를 이용한 배열 초기화
	srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < SORTNUM; i++) {
        int num = rand();
        arr[i] = num;
    }

}

void insertSort(int* arr) {
	int key, j, i;
	for (i = 1; i < SORTNUM; i++) {
		key = arr[i];
		for (j = i - 1; j >= 0 && arr[j] > key; j--) {
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = key;
	}
}


void funcTimeFinish(clock_t timeStart) {

	clock_t timeFinish;     //실행 시간 저장 위한 변수
	double spendTime;       //시작 시간과 종료 시간 계산 후 저장할 변수
	timeFinish = clock();
	spendTime = (double)(timeFinish - timeStart) / CLOCKS_PER_SEC;
	printf("정렬 소요 시간 : %f초", spendTime);
}


int main()
{
    randArray(arr);

	clock_t timeStart = clock();
    insertSort(arr);
	funcTimeFinish(timeStart);

}
