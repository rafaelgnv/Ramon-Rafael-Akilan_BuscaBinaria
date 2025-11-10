#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

// Constantes para os saltos aleatórios (sem alteração)
#define MAX_SALTO_INTERNO 10
#define MAX_SALTO_ENTRE_ARQUIVOS 100

/**
 * Gera um arquivo CSV com 'tamanho' elementos, em ordem crescente e com saltos aleatórios.
 * Esta função permanece a mesma, pois ela é modular.
 *
 * @param nome_arquivo O NOME COMPLETO DO ARQUIVO (incluindo o caminho) a ser criado.
 * @param tamanho O número de elementos que este arquivo deve conter.
 * @param valor_inicial_base O último valor gerado pelo arquivo anterior.
 * @return O último valor numérico gerado e salvo neste arquivo.
 */
long long gerar_arquivo_csv(const char* nome_arquivo, int tamanho, long long valor_inicial_base) {
    
    // O 'printf' agora é mais curto para não poluir o terminal 500 vezes
    printf("  -> Gerando arquivo %s ... ", nome_arquivo);
    
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        // Se a pasta (ex: n010000) não existir, o erro aparecerá aqui
        fprintf(stderr, "\nErro ao abrir o arquivo %s para escrita. (A pasta existe?)\n", nome_arquivo);
        exit(1);
    }

    // 1. Define o primeiro valor para ESTE arquivo
    long long valor_atual = valor_inicial_base + (rand() % MAX_SALTO_ENTRE_ARQUIVOS + 1);

    // 2. Loop para gerar os 'tamanho' elementos
    for (int i = 1; i <= tamanho; i++) {
        fprintf(arquivo, "%lld", valor_atual);
        if (i < tamanho) {
            fprintf(arquivo, ",");
        }
        // Prepara o próximo valor (incremento interno)
        valor_atual += (rand() % MAX_SALTO_INTERNO + 1);
    }
    
    fprintf(arquivo, "\n"); 
    fclose(arquivo);

    printf("Concluído!\n");

    // 4. Retorna o último valor gerado para manter a cadeia
    return valor_atual; 
}

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // --- NOVAS VARIÁVEIS DE CONTROLE ---
    int incremento = 10000;
    int limite_max = 100000;
    int arquivos_por_pasta = 50; // Novo requisito: 50 arquivos por pasta

    // Buffers para construir os nomes das pastas e arquivos
    char nome_subpasta[20];
    char nome_arquivo[30];
    char caminho_completo[100]; // Buffer para o caminho final (ex: ../dados/n010000/arquivo_01.csv)

    // A cadeia de valores é a mesma, começa em 0 e é atualizada 500 vezes
    long long ultimo_valor_gerado = 0;

    printf("Iniciando geração de 500 arquivos de dados...\n");
    
    // === LOOP EXTERNO (Para os tamanhos / pastas) ===
    // (tamanho = 10000, 20000, ..., 100000)
    for (int tamanho = incremento; tamanho <= limite_max; tamanho += incremento) {
        
        // 1. Cria o nome da subpasta (ex: "n010000")
        // %06d = formata o int com 6 dígitos, preenchendo com zeros à esquerda
        sprintf(nome_subpasta, "n%06d", tamanho);
        
        printf("\nProcessando pasta: ../dados/%s/ (Tamanho: %d elementos)\n", nome_subpasta, tamanho);
        
        // === LOOP INTERNO (Para os 50 arquivos dentro de cada pasta) ===
        // (i = 1, 2, ..., 50)
        for (int i = 1; i <= arquivos_por_pasta; i++) {
            
            // 2. Cria o nome do arquivo (ex: "arquivo_01.csv")
            // %02d = formata o int com 2 dígitos
            sprintf(nome_arquivo, "arquivo_%02d.csv", i);

            // 3. Monta o caminho completo
            sprintf(caminho_completo, "../dados/%s/%s", nome_subpasta, nome_arquivo);

            // 4. Chama a função de geração e ATUALIZA o 'ultimo_valor_gerado'
            // Esta é a linha que garante a cadeia contínua
            ultimo_valor_gerado = gerar_arquivo_csv(caminho_completo, tamanho, ultimo_valor_gerado);
        }
    }

    printf("\nTodos os 500 arquivos CSV foram gerados com sucesso.\n");
    return 0;
}