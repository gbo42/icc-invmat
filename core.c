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
#include "entrada_saida.h"
#include "likinc.h"

/**
 * @brief      Decompõe matriz em LU utilizando eliminação de Gauss com pivotamento parcial
 *
 * @param      entrada matriz a ser decomposta
 * @param      Lt  matriz triangular inferior da decomposição
 * @param      Ut  matriz triangular inferior da decomposição
 * @param      trocas   trocas executadas durante a decomposição
 * @param      N   tamanho da matriz
 *
 *@return   double contendo o tempo gasto neste processo
 */
double decompLU(double *entrada, double *Lt, double *Ut, int *trocas, int N){
    int i, j;
    double *L = NULL, *U = NULL;

    if(alocaAlinhado(&L,(N*(N-1))/2) || alocaAlinhado(&U,(N*(N+1)/2))) perror("Erro na alocação");

    //copia matriz entrada para L e U
    for(i=0; i<N; i++){
      for(j = 0; j < i; j++){
        L[indiceL(i,j)]= entrada[C(i,j)];
      }
      for(; j < N; j++){
        U[indiceU(i,j)]= entrada[C(i,j)];
      }
    }


    //preenche vetor trocas
    double tempo_decomp = timestamp();
    for(i = 0; i < N; i++){
        trocas[i] = i;
    }

    //laço que repete processo para todas as linhas
    for(int l = 0; l < N; l++){
        //busca pivo
        i = l;
        double maior = U[indiceU(l,l)];
        for(int k = l+1; k < N; k++){
            if(fabs(L[indiceL(k,l)]) > maior){
                maior = fabs(L[indiceL(k,l)]);
                i = k;
            }
        }

        //armazena troca de linhas
        int a = trocas[l];
        trocas[l] = trocas[i];
        trocas[i] = a;

        //realiza troca de linhas
        double aux;
        int k;
        for(k = 0; k < l; k++){
          aux = L[indiceL(l, k)];
          L[indiceL(l, k)] = L[indiceL(i, k)];
          L[indiceL(i, k)] = aux;
        }

        //realiza troca de linhas
        for(; k < i; k++){
          aux = U[indiceU(l, k)];
          U[indiceU(l, k)] = L[indiceL(i, k)];
          L[indiceL(i, k)] = aux;
        }

        //realiza troca de linhas
        for(; k < N; k++){
          aux = U[indiceU(l, k)];
          U[indiceU(l, k)] = U[indiceU(i, k)];
          U[indiceU(i, k)] = aux;
        }

        //calculo de LU
        double normalizador;
        for(int k = l; k < N-1; k++){
            normalizador = L[indiceL(k+1, l)]/U[indiceU(l, l)];
            for(j = l; j < k+1; j++){
                L[indiceL(k+1, j)] -= U[indiceU(l,j)]*normalizador;
            }
            for(; j < N; j++){
              U[indiceU(k+1, j)] -= U[indiceU(l,j)]*normalizador;
            }
            L[indiceL(k+1, l)] = normalizador;
        }
    }

    //monta L com os indices para acesso linear
    for(int i = 0; i < N; i++){
        for(int k = i+1; k < N; k++){
            Lt[indiceLT(i,k)] = L[indiceL(k,i)];
        }
    }

    //monta U com os indices para acesso linear
    for(int i = N-1; i >= 0; i--){
        Ut[indiceUT(i,i)] = U[indiceU(i,i)];
        for(int k = i-1; k >= 0; k--){
        	Ut[indiceUT(i,k)] = U[indiceU(k, i)];
		}
    }
    return (timestamp() - tempo_decomp);
}

/**
 * @brief      Resolve sistema linear usando uma matriz decomposta em LU
 *
 * @param      L  matriz triangular inferior da decomposição
 * @param      U  matriz triangular superior da decomposição
 * @param      N   tamanho da matriz
 * @param      b   vetor right hand side do sistema linear
 * @param      x   vetor solução do sistema linear
 * @param      trocas   trocas executadas durante a decomposição
 *
 */
void resolveSistema(double *L, double *U, int N, double *b, double * x, int * trocas){
    LIKWID_MARKER_START("op1");
    extern double * y;
    double xx, yy;

    //inicia valores de Y
    FOR_IT(N, y[IT] = b[trocas[IT]])

    //Ly = b
    for(int i = 0; i < N; i++){
        yy = y[i];
        for(int k = i+1; k < N; k++){
            y[k] -= L[indiceLT(i,k)] * yy;
		}
    }

 	//Ux = y
    for(int i = N-1; i >= 0; i--){
        xx = y[i]/U[indiceUT(i, i)];
        for(int k = i-1; k >= 0; k--){
      		y[k] -= U[indiceUT(i, k)] * xx;
		}
        x[i] = y[i]/U[indiceUT(i, i)];
    }
    LIKWID_MARKER_STOP("op1");
}

/**
 * @brief      Realiza o refinamento na solução de um sistema linear
 *
 * @param fout arquivo para saída de dados
 * @param entrada matriz com os coeficientes originais
 * @param inversa primeira solução do sistema, é o que será aprimorado
 * @param L matriz triangular inferior da decomposição de entrada
 * @param U matriz triangular superior da decomposição de entrada
 * @param trocas vetor com as posições originais das linhas pivotadas
 * @param iteracoes quantidade de vezes que o refinamento deve ser realizado
 * @param N tamanho da matriz
 * @param op1 variável que receberá o tempo das iterações
 * @param op2 variável que receberá o tempo do residuo
 *
 */
void refinamento(FILE *fout, double *entrada, double *inversa, double *L, double *U, int *trocas, int iteracoes, int N, double *top1, double *top2){
    double *b = NULL, *x = NULL, *residuo = NULL;
    if(alocaAlinhado(&b,N) || alocaAlinhado(&x,N) || alocaAlinhado(&residuo,N*N)) perror("Erro na alocação");

    double tempo_op1;
    double tempo_op2;
    *top1 = 0;
    *top2 = 0;
    int i, j, k;

    //transpõe matriz entrada
    for(i = 0; i < N-1; i++){
        for(j = i+1; j < N; j++){
            double swap = entrada[C(i,j)];
            entrada[C(i,j)] = entrada[C(j,i)];
            entrada[C(j,i)] = swap;
        }
    }

    for(int iter = 0; iter < iteracoes; iter++){
        //limpa matriz de residuos
        FOR_IT(N*N, residuo[IT] = 0)

        double aux;

        //R = I - A*A^-1
        tempo_op2 = timestamp();
        LIKWID_MARKER_START("op2");
        for (i=0;i<N; i++) {
            residuo[C(i,i)] += 1;
            for (j=0; j<N; j++) {
                aux = inversa[C(i,j)];
                for (k=0; k<N; k++) {
                    residuo[C(i,k)] -= (entrada[C(j, k)] * aux);
                }
            }
        }
        LIKWID_MARKER_STOP("op2");
        *top2 += timestamp() - tempo_op2;

        //Ad = R; X = X + d
        for(int coluna = 0; coluna < N; coluna++){
            //preenche solução do sistema com residuo
            FOR_IT(N, b[IT] = residuo[C(coluna,IT)])

            //resolve sistema
            tempo_op1 = timestamp();
            resolveSistema(L, U, N, b, x, trocas);
            *top1 += timestamp() - tempo_op1;

            //soma d em X
	        FOR_IT(N, inversa[C(coluna,IT)] += x[IT])
        }

        //calculo do erro
        double erro = 0;
        FOR_IT(N*N, erro += residuo[IT]*residuo[IT])
        fprintf(fout, "# iter %d: %.17g\n", iter, sqrt(erro));
    }
}
