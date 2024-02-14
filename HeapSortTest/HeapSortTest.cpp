#include <iostream>
#include <cstdlib>      //rand 함수 사용하기 귀함

#define SORTNUM 100000
#define PRINTNUM 20

using namespace std;


int arr[SORTNUM];						//저장 후 저장 위한 배열
int arrBeforeSort[SORTNUM];             //정렬 전 랜덤값 저장 위한 배열
int arrTemp[SORTNUM];                   //배열 랜덤값을 모든 정렬에서 같은 값으로 쓰기 위해 잠시 저장 위한 배열


void randArray(int* arr) {


    // 난수를 이용한 배열 초기화
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

    for (int i = 0; i < PRINTNUM; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    clock_t timeStart = clock();

    heapSort(arr, SORTNUM);

    funcTimeFinish(timeStart);

    for (int i = 0; i < PRINTNUM; i++) {
        cout << arr[i] << " ";
    }
}
