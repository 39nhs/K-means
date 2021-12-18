#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
//두 좌표사이의 유클리드 거리를 구해주는 함수
//p에는 Input 텍스트 파일의 좌표, q에는 k의 좌표가 저장되어있다.
int* Euclidean_distance(int** p, int* q, int row, int col);
//k좌표와 set 좌표 사이의 유클리드 거리의 전체 합을 구하는 함수.
int** Euclidean_distance_sum(int** p, int** q, int** r,int k, int d, int n);
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
    int **cluster;

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
    }

    FILE * output_file = fopen("../Output.txt", "w");
    fprintf(output_file, "%d", 1);
    fprintf(output_file, "\n");
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < data_num; j++) {
            if (compare[i][j] >= 0) {
                fprintf(output_file, "%d ", j + 1);
            }
        }
        fprintf(output_file, "\n");
    }
    /* k값을 갱신하고 k값의 변화가 없다면 반복을 종료한다.
     * 우선 k값을 갱신하기 위해 군집화 시킨 좌표들을 따로 저장해두자. */
    /* cluster 동적 할당
     * cluster 에는 각 번호에 해당하는 좌표 값들을 저장.
     * cluster[0]에는 cluster1에 모인 좌표들 cluster[1]에는 cluster2에 모인 좌표들... */
    cluster = (int **) malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        cluster[i] = (int *) malloc(sizeof(int) * data_num);
    }
    itr_set = Euclidean_distance_sum(compare, set, itr_set, k, dimension, data_num);


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

/* K의 좌표 갱신을 위한 함수.
 * 군집화 된 좌표들 사이에서 k값을 갱신하여 좌표가 담긴 포인터를 리턴한다.
 * 그 후 itr_set에 저장되어 있는 k의 좌표를 업데이트 하여 작업을 반복한다.
 * 만약 바뀐 k의 좌표가 원래 값과 같다면 return 0을 통해 프로그램을 종료한다.
 * 다르다면 업데이트 된 itr_set을 리턴한다. */
// p = compare q = set r = itr_set
int** Euclidean_distance_sum(int** p, int** q, int** r, int k, int d, int n)
{
    int row = 0;
    int set_sum = 0;
    int count = 0;
    /* 전의 k좌표들을 before_k에 저장해두었다가 k업데이트 후 비교하여 같으면 return 0를 통해 프로그램을 종료시킨다.
     * k의 좌표가 하나라도 다르다면 다시 프로그램을 계속 진행시킨다. */
    int ** before_k = r;
    int ** stay;
    /* stay 동적할당
     * stay는 좌표를 열 단위로 읽어서 저장해놓은 배열 */
    stay = (int **) malloc(sizeof (int) * n);
    for (int i = 0; i < n; i ++) {
        stay[i] = (int *) malloc(sizeof (int*) * d);
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            if (p[i][j] >= 0) {
                stay[row] = q[j];
                row += 1;
            }
        }
        for (int a = 0; a < d; a++) {
            for (int b = 0; b < n; b++) {
                printf("%d \n", stay[b][a]);
                if (stay[b][a] >= 0) {
                    set_sum += stay[b][a];
                    count += 1;
                    printf("%d \n", set_sum);
                }
            }
            set_sum = set_sum / count;
            r[i][a] = set_sum;
            set_sum = 0;
            count = 0;
        }
        set_sum = 0;
        row = 0;
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            printf("%d ", r[i][j]);
        }
        printf("\n");
    }

    return r;
}

//min에서 인덱스 값을 받아서 restore에 저장후 반환 하는 함수
//일단 만들어 놓은 것이니 더 좋은 방법이 있다면 그걸로 대체

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
}

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