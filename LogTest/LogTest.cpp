#include <iostream>
#include <cmath>        //log2 함수 사용하기 위함
#include <vector>       //백터 사용 위함
#include <cstdlib>      //rand 함수 사용하기 귀함

#define SORTNUM 100000

using namespace std;


int arr[SORTNUM];						//저장 후 저장 위한 배열
int arrBeforeSort[SORTNUM];             //정렬 전 랜덤값 저장 위한 배열
int arrTemp[SORTNUM];                   //배열 랜덤값을 모든 정렬에서 같은 값으로 쓰기 위해 잠시 저장 위한 배열

vector<int> v;


void randArray(int* arr) {

    for (int i = 0; i < SORTNUM; i++) {
        int num = rand();
        arr[i] = num;
    }

}


void buildHeap(int* arr, int* end) {						//일단 maxheap의 형태로 만들자.
    int root = 1, parent = 1, child = 1;
    v.push_back(0);
    for (int i = 0; i < end - arr; i++) {
        v.push_back(arr[i]);
    }
    for (int i = end - arr - 1; i > 1; i--) {
        child = i;
        parent = child / 2;
        while (v[child] > v[parent] && parent != 0) {
            swap(v[child], v[parent]);
            child = parent;
            parent = child / 2;
        }
    }
}

void heapify(int start, int end) {							//start(거의 1)에서 end범위에서 heap을 만들어준다. 제일 큰값이 1의 위치로 오게
    int current = start;									//current커서에 start값 넣어줌
    int leftchild = 2 * current;							//leftchild에 2*current
    int rightchild = 2 * current + 1;							//rightchild에 2*current+1

    if (leftchild <= end && v[current] < v[leftchild]) current = leftchild;		//leftchild 노드값이 end값 넘는 지 체크, current값이 leftchild값보다 작으면 current 커서 옮겨줌
    if (rightchild <= end && v[current] < v[rightchild]) current = rightchild;	//rightchild 노드값이 end값 넘는 지 체크, current값이 rightchild값보다 작으면 current 커서 옮겨줌

    if (start != current) {														//current커서가 움직였다면 parent노드보다 큰 child노드 하나 있는것이므로
        swap(v[start], v[current]);												//해당 current노드 값과 start노드 값 바꿔주고
        heapify(current, end);													//해당 current노드에서부터 다시 heapify시작
    }



}

void heapSort(int* arr, int* end) {
    clock_t start, finish;                                  //실행 시간 저장 위한 변수
    double spendTime;                                       //시작 시간과 종료 시간 계산 후 저장할 변수
    start = clock();

    buildHeap(arr, end);									//max힙의 형태로 만든후
    for (int i = end - arr; i > 1; i--) {					//맨 마지막 원소부터 2번째 원소까지  

        swap(v[1], v[i]);									//i번째 원소와 제일큰원소 바꾼 후,
        heapify(1, i - 1);										//제일 큰원소는 정렬 되어 있으므로 정렬된값 제외하고 다시 maxheap으로 만들어주고 반복
    }
    for (int i = 0; i < SORTNUM; i++) {					//정렬된 v값 넣어주기
        arr[i] = v[i + 1];
    }

    finish = clock();
    spendTime = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("힙 정렬 소요 시간 : %f초", spendTime);
}

void heapPrint() {

    //힙정렬 프린트문

    for (int elem : arr) {
        cout << elem << " ";
    }
    cout << " " << endl;
}






int main()
{

#if 0


    int i = 0;

    // l1과 r1을 저장할 벡터 선언
    std::vector<int> l1Values;
    std::vector<int> r1Values;

    while (true) {
        int l1 = 8 * i * i;
        int r1 = 64 * i * log2(i);

        // l1과 r1 값을 각각의 벡터에 추가
        l1Values.push_back(l1);
        r1Values.push_back(r1);

        std::cout << i << "번 째, 8*n^2: " << l1 << ", " << i << "번 째, 64*n log2(n): " << r1 << "\n";

        if (l1 > r1 && r1 > 0)
            break;

        i++;
    }

    std::cout << "\n-----------------------------------------------------" << std::endl;

    for (int j = 0; j < l1Values.size() && j < r1Values.size(); j++) {
        std::cout << j << "번 째, 8*n^2: " << l1Values[j] << ", " << j << "번 째, 64*n log2(n): " << r1Values[j] << "\n";
    }

    std::cout << "\n처음으로 삽입정렬이 병합정렬보다 빨라지는 정수는 " << i << "번 째입니다." << std::endl;

    return 0;


#endif // 0


#if 0

    //동일한 기계에서 수행시간이 100n^2인 알고리즘이 수행시간이 2^n인 알고리즘보다 빨라지는 n의 최솟값은 얼마인가?

    int i = 0;



    while (true) {
        double l1 = 100.0 * i * i;  // 수행 시간이 100n^2인 알고리즘
        double r1 = pow(2.0, i);     // 수행 시간이 2^n인 알고리즘

        std::cout << i << "번 째, 100n^2: " << l1 << ", " << i << "번 째, 2^n: " << r1 << "\n";

        if (l1 < r1 && l1 >0) {
            std::cout << "\n처음으로 100n^2가 2^n보다 빨라지는 정수는 " << i << "번 째입니다." << std::endl;
            break;
        }

        i++;
    }

    std::cout << "\n-----------------------------------------------------" << std::endl;


    std::cout << "\n처음으로 삽입정렬이 병합정렬보다 빨라지는 정수는 " << i << "번 째입니다." << std::endl;

    return 0;


#endif // 0

#if 0
    int num = rand() % 100;
    int arr[100];


    for (int i = 0; i < 100; i++) {
        arr[i] = i;
    }
    std::cout << "배열의 크기는: " << sizeof(arr) << std::endl;
    for (int i = 0; i <sizeof(arr); i++) {
        if (arr[i] == num) {
            std::cout << "찾는 값은:" << i << "입니다." << std::endl;

           

        }
           
    }
    std::cout << "배열의 크기는: " << sizeof(arr) << std::endl;

#endif // 1



    //------------------------ 힙 정렬 -----------------------
    //heapPrint();
    //cout << "힙 정렬 후" << endl;
     //heapPrint();
    randArray(arr);
    heapSort(arr, arr + SORTNUM);
    cout << endl;


}
