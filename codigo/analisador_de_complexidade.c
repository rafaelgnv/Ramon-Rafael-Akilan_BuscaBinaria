#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Para clock_gettime E time()
#include <math.h>   // Para sqrt() e pow()
#include <string.h> // Para strcpy, strcat, etc. (apesar de usarmos sprintf)

// Quantidade de arquivos por pasta
#define N_EXECUCOES 50

/**
 * Função de Busca Binária (sem alterações)
 * Procura por um 'long long' em um vetor de 'long long'.
 */
int busca_binaria(long long* vetor, int tamanho, long long elemento_procurado) {
    int inicio = 0;
    int fim = tamanho - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2; 
        if (vetor[meio] == elemento_procurado) return meio;
        if (vetor[meio] < elemento_procurado) inicio = meio + 1; 
        else fim = meio - 1; 
    }
    return -1; // Não encontrado
}

/**
 * Função auxiliar para calcular a Média
 */
double calcular_media(long long tempos_ns[], int n) {
    long long soma = 0;
    for (int i = 0; i < n; i++) {
        soma += tempos_ns[i];
    }
    return (double)soma / n;
}

/**
 * Função auxiliar para calcular o Desvio Padrão
 */
double calcular_desvio_padrao(long long tempos_ns[], int n, double media) {
    double soma_diferencas_quadradas = 0;
    for (int i = 0; i < n; i++) {
        // pow( (tempo_atual - media), 2 )
        soma_diferencas_quadradas += pow(((double)tempos_ns[i] - media), 2);
    }
    // Raiz quadrada da (soma_diferencas / n)
    return sqrt(soma_diferencas_quadradas / n);
}

/**
 * Função que executa UMA busca e retorna o tempo em nanosegundos
 */
long long analisar_arquivo_unico(const char* nome_arquivo, int tamanho) {
    
    // 1. Alocar memória
    long long *vetor = (long long*) malloc(tamanho * sizeof(long long));
    if (vetor == NULL) {
        fprintf(stderr, "Erro de alocação de memória para %s.\n", nome_arquivo);
        return -1;
    }

    // 2. Abrir e ler o arquivo
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", nome_arquivo);
        free(vetor);
        return -1;
    }

    int i = 0;
    while (i < tamanho && fscanf(arquivo, "%lld", &vetor[i]) == 1) {
        i++;
        char separador = fgetc(arquivo);
        // Tratamento simples para a vírgula
        if (separador == '\n' || separador == EOF) break; 
    }
    fclose(arquivo);
    
    if (i != tamanho) {
        fprintf(stderr, "Aviso: Arquivo %s parece ter %d elementos, mas esperava %d.\n", nome_arquivo, i, tamanho);
    }

    // --- LÓGICA DE BUSCA 70/30 (MODIFICADO) ---
    long long elemento_para_buscar;
    
    // Gera um float entre 0.0 e 1.0
    double prob = (double)rand() / RAND_MAX; 
    
    if (prob <= 0.7) {
        // 70% SUCESSO: Pega um índice aleatório
        // rand() % tamanho gera um número de 0 até (tamanho-1)
        int indice_aleatorio = rand() % tamanho;
        elemento_para_buscar = vetor[indice_aleatorio];
    } else {
        // 30% FALHA: Pega o 0 (que sabemos que não existe)
        elemento_para_buscar = 0;
    }
    // --- FIM DA MODIFICAÇÃO ---


    // 4. Medir o tempo de execução
    struct timespec inicio_t, fim_t;
    clock_gettime(CLOCK_MONOTONIC, &inicio_t);
    
    busca_binaria(vetor, tamanho, elemento_para_buscar); // A busca é executada
    
    clock_gettime(CLOCK_MONOTONIC, &fim_t);

    // 5. Limpar a memória
    free(vetor);

    // 6. Retornar o tempo em nanosegundos
    return (fim_t.tv_sec - inicio_t.tv_sec) * 1000000000LL + (fim_t.tv_nsec - inicio_t.tv_nsec);
}

int main() {
    // --- ADICIONADO: Inicializa o gerador de números aleatórios ---
    // Deve ser chamado apenas UMA VEZ
    srand(time(NULL)); 
    
    int incremento = 10000;
    int limite_max = 100000;
    
    // --- CAMINHO DO NOVO ARQUIVO DE RESULTADOS ---
    // (Nome do arquivo alterado)
    const char* arquivo_resultado_c = "../resultados/estatisticas/resultados_C_CasoMedio.csv";
    
    FILE* f_resultados = fopen(arquivo_resultado_c, "w");
    if (f_resultados == NULL) {
        fprintf(stderr, "Erro ao criar arquivo de resultado em %s.\n", arquivo_resultado_c);
        fprintf(stderr, "Verifique se as pastas 'resultados' e 'estatisticas' existem.\n");
        return 1;
    }
    
    // Escreve o cabeçalho do CSV de resultados
    fprintf(f_resultados, "n,tempo_ms,desvio\n");
    printf("Iniciando análise (C - Caso Médio). Salvando resultados em %s\n", arquivo_resultado_c);

    // Buffers para os nomes de pastas e arquivos
    char nome_subpasta[20];
    char nome_arquivo_dados[30];
    char caminho_completo_dados[100];

    // LOOP EXTERNO (Para os tamanhos / pastas)
    for (int tamanho = incremento; tamanho <= limite_max; tamanho += incremento) {
        
        sprintf(nome_subpasta, "n%06d", tamanho);
        printf("\nProcessando pasta: ../dados/%s/ (Tamanho: %d)\n", nome_subpasta, tamanho);
        
        // Array para guardar os 50 tempos de cada lote
        long long tempos_ns[N_EXECUCOES];
        
        // LOOP INTERNO (Para os 50 arquivos)
        for (int i = 0; i < N_EXECUCOES; i++) {
            
            sprintf(nome_arquivo_dados, "arquivo_%02d.csv", i + 1); // (i+1) para ir de 01 a 50
            
            // Monta o caminho (assumindo que estamos em 'codigo/c/')
            sprintf(caminho_completo_dados, "../dados/%s/%s", nome_subpasta, nome_arquivo_dados);
            
            printf("  -> Analisando %s ... ", nome_arquivo_dados);
            
            long long tempo = analisar_arquivo_unico(caminho_completo_dados, tamanho);
            
            if (tempo >= 0) {
                tempos_ns[i] = tempo;
                printf("Tempo: %lld ns\n", tempo);
            } else {
                tempos_ns[i] = 0; // Marca como falha
                printf("Falha na leitura.\n");
            }
        }

        // --- CÁLCULO ESTATÍSTICO ---
        double media_ns = calcular_media(tempos_ns, N_EXECUCOES);
        double desvio_ns = calcular_desvio_padrao(tempos_ns, N_EXECUCOES, media_ns);

        // --- CONVERSÃO PARA MILISSEGUNDOS (ms) ---
        // (Conforme solicitado: n,tempo_ms,desvio)
        double media_ms = media_ns / 1000000.0;
        double desvio_ms = desvio_ns / 1000000.0;

        printf("--------------------------------------------------\n");
        printf("Tamanho %d: Média: %.6f ms | Desvio: %.6f ms\n", tamanho, media_ms, desvio_ms);
        printf("--------------------------------------------------\n");
        
        // Salva a linha de resultado no CSV
        fprintf(f_resultados, "%d,%.8f,%.8f\n", tamanho, media_ms, desvio_ms);
        fflush(f_resultados); // Garante que a linha seja escrita imediatamente
    }
    
    fclose(f_resultados);
    printf("\nAnálise (C - Caso Médio) concluída com sucesso.\n");
    return 0;
}