#include <iostream>
#include <cstdlib>      //rand 함수 사용하기 귀함

#define SORTNUM 1000000
#define PRINTNUM 20     //test용 크기


using namespace std;


int arrayResult[SORTNUM];						//저장 후 저장 위한 배열
int arrBeforeSort[SORTNUM];             //정렬 전 랜덤값 저장 위한 배열
int arrTemp[SORTNUM];

clock_t timeStart, timeFinish;                                  //실행 시간 저장 위한 변수

void randArray(int* arr) {

	// 난수를 이용한 배열 초기화
	srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < SORTNUM; i++) {
        int num = rand();
        arr[i] = num;
    }

}
void mergeArray(int* arr, int start, int end) {

    int mid = (start + end) / 2;

	int i = start;
	int j = mid + 1;
	int k = end;


	while (i <= mid && j <= end)
	{
		if (arr[i] <= arr[j])
			arrBeforeSort[k++] = arr[i++];
		else
			arrBeforeSort[k++] = arr[j++];
	}

	int temp = i > mid ? j : i;

	while (k <= end) 
		arrBeforeSort[k++] = arr[temp++];

	for (int i = start; i <= end; i++) 
		arr[i] = arrBeforeSort[i];
}

void mergePartition(int start, int end)
{

	int mid;



	if (start < end)
	{
		mid = (start + end) / 2;
		mergePartition(start, mid);
		mergePartition(mid + 1, end);
		mergeArray(arrayResult, start, end);
	}


}


void funcTimeStart() {
	timeStart = clock();
}

void funcTimeFinish() {

	double spendTime;                                       //시작 시간과 종료 시간 계산 후 저장할 변수
	timeFinish = clock();
	spendTime = (double)(timeFinish - timeStart) / CLOCKS_PER_SEC;
	printf("정렬 소요 시간 : %f초", spendTime);
}


int main()
{


	randArray(arrayResult);
	for (int i = 0; i < PRINTNUM; i++) {
		cout << arrayResult[i]<<" ";
	}
	cout << endl;
	funcTimeStart();
	mergePartition(0, SORTNUM);
	funcTimeFinish();

	for (int i = 0; i < PRINTNUM; i++) {
		cout << arrayResult[i]<<" ";
	}
	cout << endl;
}
