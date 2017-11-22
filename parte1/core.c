/**
 * @file core.c
 *
 * @author Guilherme Bastos de Oliveira
 * @author Mariana Carmin
 *
 * @brief Funções com os principais processos/cálculos
 *
 */

#include "core.h"
#include "likinc.h"
/**
 * @brief      Decompõe matriz em LU utilizando eliminação de Gauss com pivotamento parcial
 *
 * @param      mEntrada matriz a ser decomposta
 * @param      LU  matriz dos coeficientes decomposta pela função decompLU
 * @param      trocas   trocas executadas durante a decomposição
 * @param      N   tamanho da matriz
 *
 */
double decompLU(double *mEntrada, double *LU, int *trocas, int N){
    //copia matriz entrada para LU
    FOR_IT(N*N, LU[IT] = mEntrada[IT])

    //preenche vetor trocas
    double tempo_decomp = timestamp();
    for(int i = 0; i < N; i++){
        trocas[i] = i;
    }

    for(int l = 0; l < N; l++){
        //busca pivo
        int i = l;
        double maior = LU[C(l,l)];
        for(int k = l+1; k < N; k++){
            if(fabs(LU[C(k,l)]) > maior){
                maior = fabs(LU[C(k,l)]);
                i = k;
            }
        }

        //armazena troca de linhas
        int a = trocas[l];
        trocas[l] = trocas[i];
        trocas[i] = a;

        //realiza troca de linhas
        double aux;
        for(int k = 0; k < N; k++){
            aux = LU[C(l, k)];
            LU[C(l, k)] = LU[C(i, k)];
            LU[C(i, k)] = aux;
        }

        //calculo de LU, armazenados na mesma matriz
        double normalizador;
        for(int k = l; k < N-1; k++){
            normalizador = LU[C(k+1, l)]/LU[C(l, l)];
            for(int j = l; j < N; j++){
                LU[C(k+1, j)] -= LU[C(l,j)]*normalizador;
            }
            LU[C(k+1, l)] = normalizador;
        }
    }
    return (timestamp() - tempo_decomp);
}

/**
 * @brief      Resolve sistema linear usando uma matriz decomposta em LU
 *
 * @param      LU  matriz dos coeficientes decomposta pela função decompLU()
 * @param      N   tamanho da matriz
 * @param      b   vetor right hand side do sistema linear
 * @param      x   vetor solução do sistema linear
 * @param      trocas   trocas executadas durante a decomposição
 *
 */
void resolveSistema(double *LU, int N, double *b, double * x, int * trocas){
    LIKWID_MARKER_START("op1");
    extern double *y;
    double aux;

	//Ly = b
	for(int i = 0; i < N; i++){
        y[i] = b[trocas[i]];
        for(int k = 0; k < i; k++){
			y[i] -= LU[C(i, k)] * y[k];
        }
    }

 	//Ux = y
	for(int i = N-1; i >= 0; i--){
        aux = 0;
        for(int k = N-1; k > i; k--){
    		aux += LU[C(i, k)] * x[k];
		}
		x[i] = (y[i] - aux)/LU[C(i, i)];
    }
    LIKWID_MARKER_STOP("op1");
}

/**
 * @brief      Realiza o refinamento na solução de um sistema linear
 *
 * @param fout arquivo para saída de dados
 * @param mEntrada matriz com os coeficientes originais
 * @param inversa primeira solução do sistema, é o que será aprimorado
 * @param LU decomposição de mEntrada
 * @param trocas vetor com as posições originais das linhas pivotadas
 * @param iteracoes quantidade de vezes que o refinamento deve ser realizado
 * @param N tamanho da matriz
 * @param top1 variável que receberá o tempo das iterações
 * @param top2 variável que receberá o tempo do residuo
 *
 */
void refinamento(FILE *fout, double *mEntrada, double *inversa, double *LU, int *trocas, int iteracoes, int N, double *top1, double *top2){
    double *b = (double *)malloc(sizeof(double) * N);
    double *x = (double *)malloc(sizeof(double) * N);
    double *residuo = (double *)malloc(sizeof(double) * N*N);

    if(!b || !x || !residuo) perror("erro na alocação"), exit(-1);

    double tempo_op1;
    double tempo_op2;
    *top1 = 0;
    *top2 = 0;
    for(int iter = 0; iter < iteracoes; iter++){
        double aux;
        //R = I - A*A^-1
        tempo_op2 = timestamp();
        LIKWID_MARKER_START("op2");
        for (int i=0;i<N; i++) {
            for (int j=0; j<N; j++) {
                aux = 0;
                for (int k=0; k<N; k++) {
                    aux += (mEntrada[C(i, k)] * inversa[C(k, j)]);
                }
                residuo[C(i,j)] = -aux;
            }
            residuo[C(i,i)] += 1;
        }
        *top2 += timestamp() - tempo_op2;
        LIKWID_MARKER_STOP("op2");

        //Ad = R; X = X + d
        for(int coluna = 0; coluna < N; coluna++){
            //preenche solução do sistema com residuo
            FOR_IT(N, b[IT] = residuo[C(IT,coluna)])

            //resolve sistema e soma d(x) em X(inversa)
            tempo_op1 = timestamp();
            resolveSistema(LU, N, b, x, trocas);
            *top1 += timestamp() - tempo_op1;
            FOR_IT(N, inversa[C(IT,coluna)] += x[IT])
        }

        //calculo do erro
        double erro = 0;
        FOR_IT(N*N, erro += residuo[IT]*residuo[IT])
        fprintf(fout, "# iter %d: %.17g\n", iter, sqrt(erro));
    }
}
