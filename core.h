#ifndef __CORE
#define  __CORE

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "entrada_saida.h"
#include "utils.h"

double decompLU(double *mEntrada, double *Lt, double *Ut, int *trocas, int N);

void resolveSistema(double *L, double *U, int N, double *b, double *x, int *trocas);

void refinamento(FILE *fout, double *mEntrada, double *inversa, double *L, double *U, int *trocas, int iteracoes, int N, double *titer, double *tresiduo);


#endif
