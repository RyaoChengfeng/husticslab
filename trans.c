/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";

void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    if (M == 32 && N == 32) {
        int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
        for (int i = 0; i < N; i += 8) {
            // 第j列为起点的块
            for (int j = 0; j < M; j += 8) {
                // 遍历块内的元素
                for (int k = i; k < i + 8; k++) {
                    tmp1 = A[k][j];
                    tmp2 = A[k][j + 1];
                    tmp3 = A[k][j + 2];
                    tmp4 = A[k][j + 3];
                    tmp5 = A[k][j + 4];
                    tmp6 = A[k][j + 5];
                    tmp7 = A[k][j + 6];
                    tmp8 = A[k][j + 7];
                    B[j][k] = tmp1;
                    B[j + 1][k] = tmp2;
                    B[j + 2][k] = tmp3;
                    B[j + 3][k] = tmp4;
                    B[j + 4][k] = tmp5;
                    B[j + 5][k] = tmp6;
                    B[j + 6][k] = tmp7;
                    B[j + 7][k] = tmp8;
                }
            }
        }
    } else if (M == 64 && N == 64) {
        int i, j, x, y;
        int x1, x2, x3, x4, x5, x6, x7, x8;
        for (i = 0; i < N; i += 8)
            for (j = 0; j < M; j += 8) {
                for (x = i; x < i + 4; ++x) {
                    x1 = A[x][j];
                    x2 = A[x][j + 1];
                    x3 = A[x][j + 2];
                    x4 = A[x][j + 3];
                    x5 = A[x][j + 4];
                    x6 = A[x][j + 5];
                    x7 = A[x][j + 6];
                    x8 = A[x][j + 7];

                    B[j][x] = x1;
                    B[j + 1][x] = x2;
                    B[j + 2][x] = x3;
                    B[j + 3][x] = x4;
                    B[j][x + 4] = x5;
                    B[j + 1][x + 4] = x6;
                    B[j + 2][x + 4] = x7;
                    B[j + 3][x + 4] = x8;
                }
                for (y = j; y < j + 4; ++y) {
                    x1 = A[i + 4][y];
                    x2 = A[i + 5][y];
                    x3 = A[i + 6][y];
                    x4 = A[i + 7][y];
                    x5 = B[y][i + 4];
                    x6 = B[y][i + 5];
                    x7 = B[y][i + 6];
                    x8 = B[y][i + 7];

                    B[y][i + 4] = x1;
                    B[y][i + 5] = x2;
                    B[y][i + 6] = x3;
                    B[y][i + 7] = x4;
                    B[y + 4][i] = x5;
                    B[y + 4][i + 1] = x6;
                    B[y + 4][i + 2] = x7;
                    B[y + 4][i + 3] = x8;
                }
                for (x = i + 4; x < i + 8; ++x) {
                    x1 = A[x][j + 4];
                    x2 = A[x][j + 5];
                    x3 = A[x][j + 6];
                    x4 = A[x][j + 7];
                    B[j + 4][x] = x1;
                    B[j + 5][x] = x2;
                    B[j + 6][x] = x3;
                    B[j + 7][x] = x4;
                }
            }
    } else if (M == 61 && N == 67) {
        // 第i行为起点的块
        for (int i = 0; i < N; i += 16) {
            // 第j列为起点的块
            for (int j = 0; j < M; j += 16) {
                // 遍历块内的元素
                for (int k = i; k < i + 16 && k < N; k++) {
                    for (int l = j; l < j + 16 && l < M; l++) {
                        B[l][k] = A[k][l];
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";

void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

