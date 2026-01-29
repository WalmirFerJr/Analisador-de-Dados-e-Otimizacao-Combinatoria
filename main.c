#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

int dfs(int linha, int col, int max_linhas, int max_colunas, int **matriz);
bool encontrarCombinacao(int alvo, int profundidades[], int n, int solucao[], int *sol_count);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Erro: Numero incorreto de argumentos.\n");
        printf("Uso: %s <profundidade_container> <linhas> <colunas> <nome_arquivo>\n", argv[0]);
        return 1;
    }

    int profundidade_container = atoi(argv[1]);
    int linhas = atoi(argv[2]);
    int colunas = atoi(argv[3]);
    char *nome_arquivo = argv[4];

    int **matriz = (int **)malloc(linhas * sizeof(int *));
    if (matriz == NULL) {
        printf("Erro de alocacao de memoria.\n");
        return 1;
    }
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        if (matriz[i] == NULL) {
            printf("Erro de alocacao de memoria.\n");
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return 1;
        }
    }

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", nome_arquivo);
        for (int i = 0; i < linhas; i++) {
            free(matriz[i]);
        }
        free(matriz);
        return 1;
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (fscanf(arquivo, "%d", &matriz[i][j]) != 1) {
                printf("Erro ao ler dados do arquivo.\n");
                fclose(arquivo);
                return 1;
            }
        }
    }
    fclose(arquivo);

    int profundidades_ilhas[linhas * colunas];
    int num_ilhas = 0;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] > 0) {
                int soma_alturas = dfs(i, j, linhas, colunas, matriz);

                double profundidade_calculada = (double)soma_alturas / 6.0;

                int profundidade_final;
                if (profundidade_calculada > 0 && profundidade_calculada < 1) {
                    profundidade_final = 1;
                } else {
                    profundidade_final = (int)profundidade_calculada;
                }
                
                profundidades_ilhas[num_ilhas] = profundidade_final;
                num_ilhas++;
            }
        }
    }

    printf("%d\n", num_ilhas);

    for (int i = 0; i < num_ilhas; i++) {
        printf("%d%s", profundidades_ilhas[i], (i == num_ilhas - 1) ? "" : " ");
    }
    printf("\n");

    int solucao[num_ilhas];
    int sol_count = 0;
    if (encontrarCombinacao(profundidade_container, profundidades_ilhas, num_ilhas, solucao, &sol_count)) {
        for (int i = 0; i < sol_count; i++) {
            printf("%d%s", solucao[i], (i == sol_count - 1) ? "" : " ");
        }
        printf("\n");
    } else {
        printf("Nao ha resposta valida!\n");
    }

    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}

int dfs(int linha, int col, int max_linhas, int max_colunas, int **matriz) {
    if (linha < 0 || linha >= max_linhas || col < 0 || col >= max_colunas || matriz[linha][col] == 0) {
        return 0; 
    }

    int altura_atual = matriz[linha][col];

    matriz[linha][col] = 0;

    int soma_total = altura_atual;
    soma_total += dfs(linha + 1, col, max_linhas, max_colunas, matriz);
    soma_total += dfs(linha - 1, col, max_linhas, max_colunas, matriz);
    soma_total += dfs(linha, col + 1, max_linhas, max_colunas, matriz);
    soma_total += dfs(linha, col - 1, max_linhas, max_colunas, matriz);

    return soma_total;
}

bool encontrarCombinacao(int alvo, int profundidades[], int n, int solucao[], int *sol_count) {
    if (alvo == 0) {
        return true;
    }

    if (alvo < 0 || n == 0) {
        return false;
    }

    solucao[*sol_count] = profundidades[n-1];
    (*sol_count)++;
    
    if (encontrarCombinacao(alvo - profundidades[n-1], profundidades, n - 1, solucao, sol_count)) {
        return true;
    }

    (*sol_count)--;
    
    return encontrarCombinacao(alvo, profundidades, n - 1, solucao, sol_count);
}
