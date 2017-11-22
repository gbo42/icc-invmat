/**
 * @file entrada_saida.c
 *
 * @author Guilherme Bastos de Oliveira
 * @author Mariana Carmin
 *
 * @brief Funções de entrada/saída
 *
 */

#include "entrada_saida.h"

/**
 * @brief      Imprime matriz
 *
 * @param      arq    arquivo onde deve ser impressa a matriz
 * @param      N      tamanho da matriz
 * @param      matriz matriz a ser impressa
 *
 * @return ponteiro para área de memória com a matriz aleatória
 */
void imprimeMatriz(FILE *arq, int N, double * matriz){
    fprintf(arq, "%d\n", N);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N-1; j++){
            fprintf(arq, "%.17g ", matriz[C(i,j)]);
        }
        fprintf(arq, "%.17g\n", matriz[C(i,N-1)]);
    }
}

/**
 * @brief      Lê matriz
 *
 * @param      fin    arquivo de onde deve ser lida a matriz
 * @param      aleatorio informa se deve ser usada matriz aleatoria (1 -> sim, 0 -> não)
 * @param      N      tamanho da matriz
 *
 * @return ponteiro para área de memória com a matriz lida/gerada
 */
double *leMatriz(FILE *fin, int aleatorio, int *tMatriz){
    double *matriz;
    int N = *tMatriz;
    if(aleatorio == 0){
        if(!fscanf(fin, "%d", &N)) perror("erro ao ler tamanho da mariz"), exit(-1);
        *tMatriz = N;
        matriz = (double *)malloc(sizeof(double)*N*N);
        if(!matriz) perror("erro na alocação"), exit(-1);
        for(int i = 0; i < N; i++) FOR_IT(N, if(!fscanf(fin, "%lf", &matriz[C(i,IT)])) (perror("erro ao ler matriz"), exit(-1)))
    } else {
        srand(20172);
        matriz = generateSquareRandomMatrix(N);
    }

    return matriz;
}

/**
 * @brief      Trata argumentos/parametros do terminal
 *
 * @param      argc quantidade de argumentos passados para o programa
 * @param      argv vetor de strings dos argumentos
 * @param      fin arquivo de entrada
 * @param      fout arquivo de saida
 * @param      N tamanho da matriz
 * @param      iteracoes quantidade de iterações do refinamento
 * @param      aleatorio informa se será usada uma matriz aleatória
 *
 */
void obtemParametros(int argc, char *argv[], FILE **fin, FILE **fout, int* N, int* iteracoes, int* aleatorio){
    for(int i = 1; i < argc; i++){
        switch (argv[i][1]) {
            case 'i' :
                if(!argv[++i]){
                    perror("Especifique o número de iterações.");
                    exit(-1);
                }
                *iteracoes = atoi(argv[i]);
            break;
            case 'e':
                *fin = fopen(argv[++i], "r");
                if(!*fin){
                    perror("Erro ao abrir arquivo de entrada");
                    exit(-1);
                }
            break;
            case 'o':
                *fout = fopen(argv[++i], "w");
                if(!*fout){
                    perror("Erro ao abrir arquivo de saida");
                    exit(-1);
                }
            break;
            case 'r':
                if(!argv[++i]){
                    perror("Especifique o tamanho da matriz.");
                    exit(-1);
                }
                *N = atoi(argv[i]);
                *aleatorio = 1;
            break;
        }
    }
}
