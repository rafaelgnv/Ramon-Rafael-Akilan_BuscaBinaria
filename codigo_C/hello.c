#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função de comparação necessária para o qsort
int comparar(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Função para gerar, ordenar e salvar um vetor em um arquivo
void gerar_e_salvar_vetor(const char* nome_arquivo, int tamanho) {
    printf("Gerando dados para o arquivo %s (tamanho %d)... ", nome_arquivo, tamanho);
    
    // Aloca memória para o vetor
    int *vetor = (int*) malloc(tamanho * sizeof(int));
    if (vetor == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(1);
    }

    // Preenche o vetor com números aleatórios
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand(); // Usando rand() para simplicidade
    }

    // Ordena o vetor usando a função qsort da biblioteca padrão
    qsort(vetor, tamanho, sizeof(int), comparar);

    // Abre o arquivo para escrita
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita.\n");
        free(vetor);
        exit(1);
    }

    // Salva os elementos ordenados no arquivo, um por linha
    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%d\n", vetor[i]);
    }

    // Fecha o arquivo e libera a memória
    fclose(arquivo);
    free(vetor);
    
    printf("Concluído!\n");
}

int main() {
    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    // Define os tamanhos dos vetores
    int tamanhos[] = {10000, 100000, 1000000};
    char nome_arquivo[50];

    for (int i = 0; i < 3; i++) {
        // Cria o nome do arquivo dinamicamente
        sprintf(nome_arquivo, "dados_%d.txt", tamanhos[i]);
        gerar_e_salvar_vetor(nome_arquivo, tamanhos[i]);
    }
    
    printf("\nTodos os arquivos foram gerados com sucesso.\n");

    return 0;
}
