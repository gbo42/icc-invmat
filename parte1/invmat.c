/**
 * @file invmat.c
 *
 * @author Guilherme Bastos de Oliveira
 * @author Mariana Carmin
 *
 * @brief Programa principal
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "entrada_saida.h"
#include "core.h"
#include "utils.h"

#include "likinc.h"
double *y; /**< vetor temporário para funções */
/**
 * @brief      Programa principal, decompõe matriz, encontra a inversa e realiza refinamento
 *
 * @param      argc quantidade de argumentos passados para o programa
 * @param      argv vetor de strings dos argumentos
 *
 * @return inteiro indicando sucesso (0)
 */
int main(int argc, char *argv[]){
    LIKWID_MARKER_INIT;
    double *x; /*!< vetor que armazena a solução de um sistema */
    double *b; /**< vetor que armazena o LFH de um sistema */
    double *inversa; /**< matriz que contém a solução do sistema, ou seja, a matriz inversa */
    double *LU; /**< matriz que contém a decomposição LU */
    double *m_entrada; /**< matriz de coeficientes do sistema, pode ser lida ou gerada aleatoriamente */

    int *trocas; /**< vetor que armazena posição das linhas pivotadas */

    FILE *fin = stdin, *fout = stdout;

    int N, iteracoes = 0, aleatorio = 0;

    double tempo_decomp, tempo_op1, tempo_op2;

    //trata os argumentos
    obtemParametros(argc, argv, &fin, &fout, &N, &iteracoes, &aleatorio);

    //le matriz caso tenha entrada ou gera aleatória
    m_entrada = leMatriz(fin, aleatorio, &N);

    //aloca matrizes e veores
    alocaTudo(&inversa, &LU, &b, &x, &y, &trocas, N);

    //calcula L e U
    tempo_decomp = decompLU(m_entrada, LU, trocas, N);

    //primeira solução do sistema
    for(int coluna = 0; coluna < N; coluna++){
        //preenche b de acordo com a I
        FOR_IT(N, b[IT] = 0)
        b[coluna] = 1;

        //resolve sistema para uma coluna da I e armazena
        resolveSistema(LU, N, b, x, trocas);
        FOR_IT(N, inversa[C(IT,coluna)] = x[IT])
    }

    fprintf(fout, "#\n");

    //refinamento
    refinamento(fout, m_entrada, inversa, LU, trocas, iteracoes, N, &tempo_op1, &tempo_op2);

    fprintf(fout, "# Tempo LU: %lf\n", tempo_decomp);
    fprintf(fout, "# Tempo op1: %lf\n", tempo_op1/(iteracoes?iteracoes:1));
    fprintf(fout, "# Tempo op2: %lf\n#\n", tempo_op2/(iteracoes?iteracoes:1));
    imprimeMatriz(fout, N, inversa);
    LIKWID_MARKER_CLOSE;
    return 0;
}
