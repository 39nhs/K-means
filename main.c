#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int* Euclidean_distance(int** p, int* q, int row, int col);

int** Euclidean_distance_sum(int* p, int** q, int** r, int k, int d, int n);

int* minimum(int** p, int row, int col);

int k_compare(int** p, int** q, int k, int d);

int main() {

    int data_num;
    int dimension;
    int k;
    char route[100];

    //사용자에게 파일의 경로를 입력받아 k, d, n의 값을 읽어온다.
    printf("파일의 경로를 입력해주세요.(따옴표는 제외하고 입력해주세요)");
    scanf("%s", route);
    FILE* fp = fopen(route, "rt");
    fscanf(fp, "%d %d %d", &k, &dimension, &data_num);

    int** before_k;
    int count = 2;
    int** set;
    int** itr_set;
    int** distance;
    int* compare;

    //set 동적할당
    set = (int**)malloc(sizeof(int) * data_num);
    for (int i = 0; i < data_num; i++) {
        set[i] = (int*)malloc(sizeof(int*) * dimension + 1);
    }
    // 파일에 적힌 데이터들을 set에 저장
    for (int i = 0; i < data_num; i++) {
        for (int j = 0; j < dimension; j++) {
            fscanf(fp, "%d", &set[i][j]);
        }
    }

    // itr_set 동적할당
    itr_set = (int**)malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        itr_set[i] = (int*)malloc(sizeof(int*) * dimension);
    }
    // k의 좌표를 데이터 오브젝트 중에 랜덤하게 지정하여 itr_set에 저장
    srand((unsigned int)time(NULL));
    int n = rand() % (data_num - 1) + 1;
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
    distance = (int**)malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        distance[i] = (int*)malloc(sizeof(int*) * data_num);
    }
    // k좌표들과 set좌표들 사이의 유클리드 거리 구해서 distance 배열에 넣어주기
    for (int i = 0; i < k; i++) {
        distance[i] = Euclidean_distance(set, itr_set[i], data_num, dimension);
    }

    //compare 동적할당
    compare = minimum(distance, k, data_num);

    //현재 디렉터리의 상위 디렉터리에 Output 파일 생성.
    FILE* output_file = fopen("../Output.txt", "w");
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
    before_k = (int**)malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
        before_k[i] = (int*)malloc(sizeof(int*) * dimension);
    }

    itr_set = Euclidean_distance_sum(compare, set, itr_set, k, dimension, data_num);
    while (1) {
        for (int i = 0; i < k; i++) {
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

int* Euclidean_distance(int** p, int* q, int row, int col)
{
    int square = 0;
    int* compare = (int*)malloc(sizeof(int) * row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            square += (int)pow(p[i][j] - q[j], 2);
        }
        compare[i] = square;
        square = 0;
    }

    return compare;
}


int** Euclidean_distance_sum(int* p, int** q, int** r, int k, int d, int n)
{
    int count = 0;
    int c = 0;
    int e = 0;
    /* 전의 k좌표들을 before_k에 저장해두었다가 k업데이트 후 비교하여 같으면 return 0를 통해 프로그램을 종료시킨다.
     * k의 좌표가 하나라도 다르다면 다시 프로그램을 계속 진행시킨다. */
    int** temp;
    //temp 동적할당
    temp = (int**)malloc(sizeof(int) * n);
    for (int i = 0; i < d; i++) {
        temp[i] = (int*)malloc(sizeof(int*) * d + 1);
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


int* minimum(int** p, int row, int col)
{
    int* m;
    int* index_set;
    int index = 0;
    //index_set 동적할당
    index_set = (int*)malloc(sizeof(int) * col);
    //m 동적할당
    m = (int*)malloc(sizeof(int) * col);
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