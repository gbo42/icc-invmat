/**
 * @file utils.c
 *
 * @author Guilherme Bastos de Oliveira
 * @author Mariana Carmin
 *
 * @brief Funções e macros auxiliares
 *
 */

#include "utils.h"

/**
 * @brief Aloca a maioria das estruturas de dados
 *
 * @param inv matriz de N*N
 * @param LU matriz de N*N
 * @param b vetor de N
 * @param x vetor de N
 * @param y vetor de N
 * @param trocas vetor de N
 * @param N tamanho da matriz
 *
 */
void alocaTudo(double **inv, double **LU, double **b, double **x, double **y, int **trocas, int N){
    *inv = (double *)malloc(sizeof(double)*N*N);
    *LU = (double *)malloc(sizeof(double)*N*N);
    *b = (double *)malloc(sizeof(double) * N);
    *x = (double *)malloc(sizeof(double) * N);
    *y = (double *)malloc(sizeof(double) * N);
    *trocas = (int *)malloc(sizeof(int) * N);

    if(!inv || !LU || !b || !x || !y || !trocas) perror("erro na alocação"), exit(-1);
}

/**
* @brief      Calcula horário atual
*
* @return horário atual
*
*/
double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}

/**
 * @brief      Gera matriz aleatória
 *
 * @param      N   tamanho da matriz
 *
 * @return ponteiro para área de memória com a matriz aleatória
 */
double *generateSquareRandomMatrix(unsigned int n){
    double *mat = NULL;
    mat = (double *)malloc(n*n*sizeof(double));
    if(!mat) perror("erro na alocação"), exit(-1);

    double *ptr = mat;
    double *end = mat + n*n;
    double invRandMax = 1.0/(double)RAND_MAX;

    while( ptr != end ) {
        *ptr++ = (double)rand() * invRandMax;
    }

    return (mat);
}
