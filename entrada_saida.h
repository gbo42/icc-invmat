#ifndef __ENTRADASAIDA
#define  __ENTRADASAIDA

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void imprimeMatriz(FILE *arq, int N, double * matriz);

double *leMatriz(FILE *fin, int aleatorio, int *N);

void obtemParametros(int argc, char *argv[], FILE **fin, FILE **fout, int* N, int* iteracoes, int* aleatorio);

#endif
