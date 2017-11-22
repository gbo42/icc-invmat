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
 * @param L matriz triangular inferior
 * @param U matriz triangular superior
 * @param b vetor de N
 * @param x vetor de N
 * @param y vetor de N
 * @param trocas vetor de N
 * @param N tamanho da matriz
 *
 */
void alocaTudo(double **inv, double **L, double **U, double **b, double **x, double **y, int **trocas, int N){
    if(alocaAlinhado(inv,N*N)
    || alocaAlinhado(L,N*(N-1)/2)
    || alocaAlinhado(U,N*(N+1)/2)
    || alocaAlinhado(b,N)
    || alocaAlinhado(x,N)
    || alocaAlinhado(y,N)) perror("erro na alocação"), exit(-1);
    *trocas = (int *)malloc(sizeof(int) * N);
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
    // mat = (double *)malloc(n*n*sizeof(double));
    if(alocaAlinhado(&mat,n*n)) perror("erro na alocação"), exit(-1);

    double *ptr = mat;
    double *end = mat + n*n;
    double invRandMax = 1.0/(double)RAND_MAX;

    while( ptr != end ) {
        *ptr++ = (double)rand() * invRandMax;
    }

    return (mat);
}
