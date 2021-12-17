#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
//두 좌표사이의 유클리드 거리를 구해주는 함수
//p에는 Input 텍스트 파일의 좌표, q에는 k의 좌표가 저장되어있다.
int * Euclidean_distance(int** p, int* q, int row, int col);
//k좌표와 set 좌표 사이의 유클리드 거리의 전체 합을 구하는 함수.
double Euclidean_distance_sum(int** p, int* q);
//더 작은 거리 값을 리턴하는 함수
//p는 distance
int** retrocede(int** p, int row, int col);
//배열에서 제일 작은 값을 리턴해주는 함수
int* minimum(int** p, int row, int col);

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

    printf("%d \n", k);
    printf("%d \n", dimension);
    printf("%d \n", data_num);

    //배열 안에 데이터의 좌표들을 저장
    int **set;
    int **itr_set;
    int **distance;
    int **compare;
    int * cluster;

    //set 동적할당
    set = (int **) malloc(sizeof(int) * data_num);
    for (int i = 0; i < data_num; i++) {
        set[i] = (int *) malloc(sizeof(int *) * dimension);
    }
    // 파일에 적힌 데이터들을 set에 저장
    for (int i = 0; i < data_num; i++) {
        for (int j = 0; j < dimension; j++) {
            fscanf(fp, "%d", &set[i][j]);
        }
    }

    //set의 원소값 출력
    for (int i = 0; i < data_num; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("%d ", set[i][j]);
        }
        printf("\n");
    }
    // itr_set 동적할당
    itr_set = (int **) malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        itr_set[i] = (int *) malloc(sizeof(int *) * dimension);
    }
    // k의 좌표를 랜덤하게 생성하여 itr_set에 저장
    srand(time(NULL));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < dimension; j++) {
            itr_set[i][j] = rand() % 9 + 1;
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
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < data_num; j++) {
            printf("%d ", distance[i][j]);
        }
        printf("\n");
    }
    //compare 비교해서 군집화 시켜서 저장시키는 배열.
    //즉, compare[0]에는 itr_1 compare[1]에는 itr_2 compare[2]에는 ...
    //compare 동적할당
    /*
    compare = (int **) malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        compare[i] = (int *) malloc(sizeof(int *) * data_num);
    }
    compare = retrocede(distance, k, data_num);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < data_num; j++) {
            printf("%d ", compare[i][j]);
        }
        printf("\n");
    }*/

    /* cluster 동적할당
     */
    cluster = (int *) malloc(sizeof(int) * data_num);
    cluster = minimum(distance, k, data_num);
    for (int i = 0; i < data_num; i++) {
        cluster[i] += 1;
        printf("%d ", cluster[i]);
    }

    FILE * output_file = fopen("../Output.txt", "w");
    fprintf(output_file, "%d", 1);

    return 0;

}
//k 좌표와 set 좌표 사이의 유클리드 거리를 구해주는 함수
int * Euclidean_distance(int** p, int* q, int row, int col)
{
    double square = 0;
    int distance;
    int * compare = (int *) malloc(sizeof(int) * row);
    for (int i = 0; i < row; i++){//p의 사이즈만큼 반복 3번 = data_num
        for (int j = 0; j < col; j++) {//2번 = dimension
            square += pow(p[i][j] - q[j], 2);
            distance = square;
        }
        square = 0;
        compare[i] = distance;
    }

    return compare;
}

// k좌표와 전체 set 좌표 사이의 유클리드 거리 합을 구해주는 함수
double Euclidean_distance_sum(int** p, int* q)
{
    double square = 0;
    double distance;
    for (int i = 0; i < _msize(p) / sizeof(int); i++){//p의 사이즈만큼 반복 3번 = data_num
        for (int j = 0; j < _msize(p[0]) / sizeof(int); j++) { //2번 = dimension
            square += pow(p[i][j] - q[j], 2);
        }
    }
    distance = sqrt(square);
    return distance;
}

//min에서 인덱스 값을 받아서 restore에 저장후 반환 하는 함수
//일단 만들어 놓은 것이니 더 좋은 방법이 있다면 그걸로 대체
/*
int** retrocede(int** p, int row, int col)
{
    int** restore;
    int* m;
    //m 동적할당
    m = (int *) malloc(sizeof(int) * col);
    m = minimum(p, row, col);
    //retore 동적할당
    restore = (int **) malloc(sizeof(int) * row);
    for (int i = 0; i < row; i++) {
        restore[i] = (int *) malloc(sizeof(int *) * col);
    }

    for (int i = 0; i < col; i++) {
        restore[m[i]][i] = m[i];
    }
    return restore;
}*/

//비교해서 최소값의 인덱스를 포인터에 저장해서 반환해줌
//예를들어 [4 , 7, 8]
//        [6, 4, 9]
//일때 열끼리 비교해서 더 작은 값의 인덱스를 반환.
//[0, 1, 0]
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
            printf("%d ", m[j]);
        }
        for (int k = 1; k < row; k++) {
            if (m[k] < m[index]) index = k;
        }
        index_set[i] = index;
        index = 0;
        printf("\n");
    }
    return index_set;
}