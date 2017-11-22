#ifndef __UTILS
#define  __UTILS
/**
* @file utils.h
*
* @author Guilherme Bastos de Oliveira
* @author Mariana Carmin
*
* @brief macros utilizadas para facilitar a vida
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "entrada_saida.h"
#include "core.h"

/** @brief calcula indice de acesso de matriz quadrada
*   @param i linha
*   @param j coluna
*/
#define alocaAlinhado(ptr,t) (posix_memalign((void **)(ptr),32,sizeof(double)*(t)))

/** @brief calcula indice de acesso de matriz quadrada
*   @param i linha
*   @param j coluna
*/
#define  C(i,j) ((i)*N+(j))

/** @brief calcula indice de acesso da matriz L para decomposição
*   @param i linha
*   @param j coluna
*/
#define indiceL(i,j) ((((i)*((i)+1)/2)+(j))-(i))

/** @brief calcula indice de acesso da matriz U para decomposição
*   @param i linha
*   @param j coluna
*/
#define indiceU(i,j) (((N-1-(i))*(N-(i))/2)+N-1-(j))

/** @brief calcula indice de acesso da matriz L para resolver sistema
*   @param i linha
*   @param j coluna
*/
#define indiceLT(i,j) (((N-1+N-(i))*i/2)+((j)-1-(i)))

/** @brief calcula indice de acesso da matriz U para resolver sistema
*   @param i linha
*   @param j coluna
*/
#define indiceUT(i,j) ((N+(i))*(N-1-(i))/2+(N-1-(j)))

 /** @brief realiza um laço de 0 até 'lim'
 *   @param lim condição de parada do laço
 *   @param exp expressão a ser repitida no laço
 */
#define FOR_IT(lim, exp) for(int IT = 0; IT < lim; IT++) exp;

void alocaTudo(double **inv, double **L,double **U, double **b, double **x, double **y, int **trocas, int N);

void liberaTudo(double *inv, double *L, double *U, double *b, double *x, double *y, int *trocas);

double timestamp(void);

double *generateSquareRandomMatrix(unsigned int n);

#endif
