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

 /** @brief converte indice bidemensional para unidimensional
 *   @param a linha da matriz
 *   @param b coluna da matriz
 */
#define  C(a, b) ((a)*N+(b))

 /** @brief realiza um laço de 0 até 'lim'
 *   @param lim condição de parada do laço
 *   @param exp expressão a ser repitida no laço
 */
#define FOR_IT(lim, exp) for(int IT = 0; IT < lim; IT++) exp;

void alocaTudo(double **inv, double **LU, double **b, double **x, double **y, int **trocas, int N);

double timestamp(void);

double *generateSquareRandomMatrix(unsigned int n);

#endif
