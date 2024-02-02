#include <iostream>
#include <cstdlib>      //rand 함수 사용하기 귀함

#define SORTNUM 100000
#define PRINTNUM 20     //test용 크기


using namespace std;


int arrayResult[SORTNUM];					//정렬 후 저장 위한 배열
int arrayRand[SORTNUM];						//처음 랜덤 값 저장을 위한 배열

// 난수를 이용한 배열 초기화
void randArray(int* arr) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < SORTNUM; i++) {
        int num = rand();
        arr[i] = num;
    }
}

// 정렬 소요 시간 측정
void funcTimeFinish(clock_t timeStart) {
	clock_t timeFinish;     //실행 시간 저장 위한 변수
	double spendTime;       //시작 시간과 종료 시간 계산 후 저장할 변수
	timeFinish = clock();
	spendTime = (double)(timeFinish - timeStart) / CLOCKS_PER_SEC;
	printf("정렬 소요 시간 : %f초\n", spendTime);
}

// 값 교환
void swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

//-----------------------------병합 정렬-------------------------
void mergeSort(int* arr, int start, int end) {

	int mid = (start + end) / 2;			//기준을 배열의 중심으로 설정	
	int i = start;
	int j = mid + 1;
	int k = end;


	while (i <= mid && j <= end)
	{
		if (arr[i] <= arr[j])
			arr[k++] = arr[i++];
		else
			arr[k++] = arr[j++];
	}

	int temp = i > mid ? j : i;

	while (k <= end)
		arr[k++] = arr[temp++];

	for (int i = start; i <= end; i++)
		arr[i] = arr[i];
}

void mergePartition(int* arr, int start, int end)
{
	int mid;

	if (start < end)
	{
		mid = (start + end) / 2;
		mergePartition(arr, start, mid);
		mergePartition(arr, mid + 1, end);
		mergeSort(arr, start, end);
	}
}


//--------------------퀵정렬--------------------
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

//--------------------삽입정렬--------------------
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

//--------------------힙정렬--------------------
void heapify(int arr[], int len, int parent) { //인덱스를 0부터 카운트하는 경우
	int largest = parent; //부모 노드
	int l = 2 * parent + 1; //왼쪽 자식
	int r = 2 * parent + 2; //오른쪽 자식
	//heapify는 힙구조를 만드는 과정을 의미

	if (l < len && arr[l] > arr[largest]) {
		largest = l;
	}
	if (r < len && arr[r] > arr[largest]) {
		largest = r;
	}
	/*왼쪽 자식과 오른쪽 자식과 비교해서
	더 큰 값의 인덱스를 largest에 넣는 과정*/

	if (largest != parent) {

		swap(arr[parent], arr[largest]);

		heapify(arr, len, largest);
	}
}/* 큰 값이 바뀐경우 스왑을 하고 순환호출을 해서
 바뀐 인덱스에 대해서 한 번 더 heapify*/


void heapSort(int arr[], int len) {

	for (int i = len / 2 - 1; i >= 0; i--) {
		heapify(arr, len, i);
	}
	//최대힙을 만드는 과정
	for (int i = len - 1; i >= 0; i--) {

		swap(arr[0], arr[i]);
		//arr[0]은 최댓값, arr[i]는 마지막 인덱스
		heapify(arr, i, 0);
	}//교체후 다시 힙정렬을 만든다.
}


int main()
{

	
	randArray(arrayRand);

	cout << "----- 퀵정렬 -----" << endl;
	copy_n(arrayRand, SORTNUM, arrayResult);
	clock_t timeStart = clock();
	quickSort(arrayResult, 0, SORTNUM);
	funcTimeFinish(timeStart);

	cout << "----- 삽입정렬 -----" << endl;
	copy_n(arrayRand, SORTNUM, arrayResult);
	timeStart = clock();
	insertSort(arrayResult);
	funcTimeFinish(timeStart);

	cout << "----- 힙정렬 -----" << endl;
	copy_n(arrayRand, SORTNUM, arrayResult);
	timeStart = clock();
	heapSort(arrayResult, SORTNUM);
	funcTimeFinish(timeStart);

	cout << "----- 병합정렬 -----" << endl;
	copy_n(arrayRand, SORTNUM, arrayResult);
	timeStart = clock();
	mergePartition(arrayResult, 0, SORTNUM);
	funcTimeFinish(timeStart);



}
