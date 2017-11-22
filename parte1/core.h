#ifndef __CORE
#define  __CORE

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "entrada_saida.h"
#include "utils.h"

double decompLU(double *mEntrada, double *LU, int *trocas, int N);

void resolveSistema(double *LU, int N, double *b, double *x, int *trocas);

void refinamento(FILE *fout, double *mEntrada, double *inversa, double *LU, int *trocas, int iteracoes, int N, double *titer, double *tresiduo);


#endif
