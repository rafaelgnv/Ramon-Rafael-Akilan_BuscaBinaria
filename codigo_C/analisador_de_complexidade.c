#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função de Busca Binária - Complexidade O(log n)
// Retorna o índice do elemento se encontrado, caso contrário retorna -1.
int busca_binaria(int* vetor, int tamanho, int elemento_procurado) {
    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2; // Evita overflow para 'inicio + fim' muito grandes

        // Se o elemento está no meio
        if (vetor[meio] == elemento_procurado) {
            return meio;
        }

        // Se o elemento é maior, ignora a metade da esquerda
        if (vetor[meio] < elemento_procurado) {
            inicio = meio + 1;
        } 
        // Se o elemento é menor, ignora a metade da direita
        else {
            fim = meio - 1;
        }
    }

    // Retorna -1 se o elemento não foi encontrado
    return -1;
}

// Função principal para carregar o vetor e analisar
void analisar_arquivo(const char* nome_arquivo, int tamanho) {
    printf("---------------------------------------------------\n");
    printf("Analisando arquivo: %s (Tamanho: %d elementos)\n", nome_arquivo, tamanho);

    int *vetor = (int*) malloc(tamanho * sizeof(int));
    if (vetor == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return;
    }

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s. Execute o gerador primeiro.\n", nome_arquivo);
        free(vetor);
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        if (fscanf(arquivo, "%d", &vetor[i]) != 1) {
            fprintf(stderr, "Erro ao ler dados do arquivo.\n");
            break;
        }
    }
    fclose(arquivo);

    // Para garantir que a busca seja justa, vamos procurar por um elemento
    // que sabemos que existe, por exemplo, o que está na posição 3/4 do vetor.
    int elemento_para_buscar = vetor[(tamanho * 3) / 4];

    printf("  Procurando pelo elemento: %d\n", elemento_para_buscar);

    clock_t inicio, fim;
    double tempo_gasto;

    inicio = clock(); // Marca o tempo de início
    int indice_encontrado = busca_binaria(vetor, tamanho, elemento_para_buscar);
    fim = clock(); // Marca o tempo de fim

    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (indice_encontrado != -1) {
        printf("    -> Sucesso! Elemento encontrado no índice %d.\n", indice_encontrado);
    } else {
        printf("    -> Falha! Elemento não foi encontrado.\n");
    }
    printf("    -> Tempo de execução: %f segundos\n", tempo_gasto);

    free(vetor);
}

int main() {
    int tamanhos[] = {10000, 100000, 1000000};
    char nome_arquivo[50];

    for (int i = 0; i < 3; i++) {
        sprintf(nome_arquivo, "dados_%d.txt", tamanhos[i]);
        analisar_arquivo(nome_arquivo, tamanhos[i]);
    }
    printf("---------------------------------------------------\n");

    return 0;
}