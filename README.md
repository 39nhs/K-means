K-means clustering
=============
Code review
-------------
# 1. 변수
## 1.1 int 변수
```C
int data_num; //데이터 오브젝트의 총 개수.
int dimension; //데이터 오브젝트의 차원 수.
int k; //클러스터의 수.
int count = 2; // while문으로 반복 시, itr수를 출력하기 위한 변수. 
```
## 1.2 char[] 변수
```C
char route[100]; //사용자의 Input 파일 경로를 담기 위한 char 배열.
```
## 1.3 포인터 변수
```C
int **set; //Iuput 파일에서 데이터 오브젝트의 좌표들을 저장해두는 이차원 포인터.
int **itr_set; //set에 저장된 데이터 오브젝트 중 랜덤하게 골라, 좌표를 저장해두는 이차원 포인터.
int **distance; //k좌표와 각 데이터 오브젝트 사이의 유클리드 거리를 저장해두는 이차원 포인터. 
int **before_k; //k좌표가 바뀌었는지 확인하기 위해 전의 k좌표들을 저장해두는 이차원 포인터.
int *compare; // 각각의 k들과 데이터 오브젝트 사이의 거리 중, 더 작은 경우의 인덱스 값을 저장해두는 포인터. 
```
# 2.함수
## 2.1 Euclidean_distance
```C
int * Euclidean_distance(int** p, int* q, int row, int col)
{
    int square = 0;
    int distance;
    int * compare = (int *) malloc(sizeof(int) * row);
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++) {
            square += pow(p[i][j] - q[j], 2);
            distance = square;
        }
        square = 0;
        compare[i] = distance;
    }

    return compare;
}
```
k좌표와 각 데이터 오브젝트 사이의 유클리드 거리를 구해주는 함수이다.   
p는 set, q = itr_set[i], row는 data_num, col은 dimension이다.   
예를들어, set은 [ [10, 4], [5, 6] ] itr_set[i]는 [4, 3] row는 2 col은 2라고 가정해보자.   
이 경우 우선 for문에서 p[0][0] - q[0]의 값을 제곱하여 square에 저장한다.   
그 다음은 p[0][1] - q[1]를 제곱하여 square에 더해서 저장한다.   
그리고 이 더해진 값을 compare[0]에다 저장한다.    
즉 compare[0]에는 첫번째 좌표와 첫번째 k 좌표 사이의 유클리드 거리가 저장된 것이다.   
## 2.2 minimum
```C
int* minimum(int** p, int row, int col)
{
    int* m;
    int* index_set;
    int index = 0;
    //index_set 동적할당
    index_set = (int *) malloc(sizeof(int) * col);
    //m 동적할당
    m = (int *) malloc(sizeof(int) * col);
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            m[j] = p[j][i];
        }
        for (int k = 1; k < row; k++) {
            if (m[k] < m[index]) index = k;
        }
        index_set[i] = index;
        index = 0;
    }
    return index_set;
}
```
minimum 함수는 열끼리 비교하여 더 작은 값의 인덱스들을 모아 포인터로 반환해주는 함수이다.   
우린 먼저 2.1의 Euclidean_distance 함수를 이용하여 main 함수의 distance 포인터에 각각의 k좌표와 데이터 오브젝트 사이의 거리를 저장해두었다.   
여기서 p는 distance, row는 k, col은 data_num이다.   
예를 들어, distance가   
[[4, 7, 7]   
[6, 4, 9]]   
이면 열끼리 읽어서, [4, 6]중 4가 더 작으므로 4의 인덱스 값인 0을 index_set[0]에 저장한다.   
그 다음 [7, 4]중 4가 더 작으므로 4의 인덱스 값인 1을 index_set[1]에 저장한다...   
이 과정을 반복한 후 index_set을 반환하여 main 함수의 compare에 저장한다.   
## 2.3 Euclidean_distance_sum
```C
int** Euclidean_distance_sum(int* p, int** q, int** r, int k, int d, int n)
{
    int count = 0;
    int c = 0;
    int e = 0;
    int ** temp;
    //temp 동적할당
    temp = (int **) malloc(sizeof (int) * n);
    for (int i = 0; i < d; i++) {
        temp[i] = (int *) malloc(sizeof (int *) * d + 1);
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            if (q[j][d] == i) {
                temp[e] = q[j];
                e += 1;
            }
        }
    }
    e = 0;
    for (int i = 0; i < k; i++) {
        for (int g = 0; g < d; g++) {
            for (int j = 0; j < n; j++) {
                if (temp[j][d] == i) {
                    c += temp[j][g];
                    e += 1;
                    count += 1;
                }
            }
            c = c / count;
            r[i][g] = c;
            count = 0;
            c = 0;
        }
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            printf("%d ", r[i][j]);
        }
        printf("\n");
    }

    return r;
}
```
Euclidean_distance_sum 함수는 k값의 갱신을 위해 같은 cluster에 모인 좌표들끼리 평균값을 구해주는 함수이다.   
p는 compare q는 set r은 itr_set이다.   
temp에 cluster가 같은 순서대로 오름차순으로 정렬한다.   
예를 들어, 원래 set 함수가     
[[10, 4, 0]   
[5 , 6, 1]   
[4, 7, 0]]   
이라면 맨 뒤의 클러스터(예시에선 0과 1)에 따라 다음과 같이 정렬한다.   
[[10, 4, 0]   
[4, 7, 0]   
[5 , 6, 1]]   
그 후 같은 클러스트끼리 열끼리 더해 더해준 만큼 나눈뒤, r 즉 k좌표들이 모인 itr_set을 업데이트 해서 반환한다.   
## 2.4 k_compare
```C
int k_compare(int** p, int** q, int k, int d)
{
    int count = 0;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            if (p[i][j] == q[i][j])
                count += 1;
        }
    }
    if (count == k * d) {
        return 2;
    }
    else
        return 1;
}
```
k_compare 함수는 함수의 이름 그대로 전의 k의 좌표들과 새로 업데이트 된 k좌표들을 비교하여 모두 같다면   
k의 값이 업데이트 되지 않은 것이므로   
2를 반환하여 main 함수의 while문을 빠져나오고, 그렇지 않다면 1을 반환하여 main 함수의 while문을 계속 진행한다.   
## 2.5 main
```C
int main() {

    int data_num;
    int dimension;
    int k;
    char route[100];

    //사용자에게 파일의 경로를 입력받아 k, d, n의 값을 읽어온다.
    printf("파일의 경로를 입력해주세요.(따옴표는 제외하고 입력해주세요)");
    scanf("%s", route);
    FILE *fp = fopen(route, "rt");
    fscanf(fp, "%d %d %d", &k, &dimension, &data_num);

    int **before_k;
    int count = 2;
    int **set;
    int **itr_set;
    int **distance;
    int *compare;

    //set 동적할당
    set = (int **) malloc(sizeof(int) * data_num);
    for (int i = 0; i < data_num; i++) {
        set[i] = (int *) malloc(sizeof(int *) * dimension + 1);
    }
    // 파일에 적힌 데이터들을 set에 저장
    for (int i = 0; i < data_num; i++) {
        for (int j = 0; j < dimension; j++) {
            fscanf(fp, "%d", &set[i][j]);
        }
    }

    // itr_set 동적할당
    itr_set = (int **) malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        itr_set[i] = (int *) malloc(sizeof(int *) * dimension);
    }
    // k의 좌표를 데이터 오브젝트 중에 랜덤하게 지정하여 itr_set에 저장
    srand(time(NULL));
    int n  = rand() % (data_num - 1) + 1;
    for (int i = 0; i < k; i++) {
        int m = rand() % (data_num - 1) + 1;
        if (i == 0) {
            itr_set[i] = set[n];
        }
        else {
            if (n != m) {
                itr_set[i] = set[m];
            }
            else
                i--;
        }
    }
    //itr_set의 원소값 출력
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("%d ", itr_set[i][j]);
        }
        printf("\n");
    }
    //distance 동적할당
    distance = (int **) malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        distance[i] = (int *) malloc(sizeof(int *) * data_num);
    }
    // k좌표들과 set좌표들 사이의 유클리드 거리 구해서 distance 배열에 넣어주기
    for (int i = 0; i < k; i++) {
        distance[i] = Euclidean_distance(set, itr_set[i], data_num, dimension);
    }

    //compare 동적할당
    compare = minimum(distance, k, data_num);

    FILE * output_file = fopen("../Output.txt", "w");
    fprintf(output_file, "%d", 1);
    fprintf(output_file, "\n");
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < data_num; j++) {
            if (compare[j] == i) {
                fprintf(output_file, "%d ", j + 1);
            }
        }
        fprintf(output_file, "\n");
    }
    for (int i = 0; i < data_num; i++) {
        set[i][dimension] = compare[i];
    }
    //원래 k 좌표들을 저장해두는 배열
    before_k = (int **) malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        before_k[i] = (int *) malloc(sizeof(int *) * dimension);
    }

    itr_set = Euclidean_distance_sum(compare, set, itr_set, k, dimension, data_num);
    while (1) {
        for (int i = 0; i < k; i ++) {
            for (int j = 0; j < dimension; j++) {
                before_k[i][j] = itr_set[i][j];
            }
        }
        for (int i = 0; i < k; i++) {
            distance[i] = Euclidean_distance(set, itr_set[i], data_num, dimension);
        }

        compare = minimum(distance, k, data_num);
        fprintf(output_file, "%d", count);
        fprintf(output_file, "\n");
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < data_num; j++) {
                if (compare[j] == i) {
                    fprintf(output_file, "%d ", j + 1);
                }
            }
            fprintf(output_file, "\n");
        }
        for (int i = 0; i < data_num; i++) {
            set[i][dimension] = compare[i];
        }
        count += 1;
        itr_set = Euclidean_distance_sum(compare, set, itr_set, k, dimension, data_num);
        if (k_compare(before_k, itr_set, k, dimension) == 2)
            break;
        else if (k_compare(before_k, itr_set, k, dimension) == 1)
            continue;
    }

    return 0;

}
```
main 함수의 전체적인 흐름은 다음과 같다.   
1. 우선 Input 파일의 데이터 오브젝트들을 읽어서 set에 저장한다.   
2. 데이터 오브젝트들중 랜덤하게 골라, itr_set에 저장한다.   
3. Euclidean_distance 함수를 이용하여 itr_set의 첫번째부터 행의 수인 k만큼 반복하여 각 k의 좌표와 데이터 오브젝트 사이의 거리를 distance에 저장한다.   
4. minimum 함수를 이용하여 distance에 저장된 거리를 열끼리 읽어서 더 작은 값이 저장되어 있는 인덱스 값을 저장한 포인터를 반환하여 compare에 저장한다.   
5. 우선 compare를 이용하여 output 파일에 출력을 한번 한뒤, Euclidean_distance_sum 함수를 이용하여 k좌표를 업데이트 한다.   
6. k의 좌표를 업데이트 하고 before_k에 k의 값들을 복사해둔뒤 while문을 이용하여 3~5의 과정을 반복한다.   
7. while문을 한번 끝내고 반복하기 전에 if문과 k_compare 함수를 이용하여 k_compare 함수가 1을 반환했다면 break를, 2를 반환했다면 continue한다.     
