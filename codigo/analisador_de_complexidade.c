#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
#include <math.h>   
#include <string.h> 


#define N_EXECUCOES 50

int busca_binaria(long long* vetor, int tamanho, long long elemento_procurado) {
    int inicio = 0;
    int fim = tamanho - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2; 
        if (vetor[meio] == elemento_procurado) return meio;
        if (vetor[meio] < elemento_procurado) inicio = meio + 1; 
        else fim = meio - 1; 
    }
    return -1; 
}


double calcular_media(long long tempos_ns[], int n) {
    long long soma = 0;
    for (int i = 0; i < n; i++) {
        soma += tempos_ns[i];
    }
    return (double)soma / n;
}


double calcular_desvio_padrao(long long tempos_ns[], int n, double media) {
    double soma_diferencas_quadradas = 0;
    for (int i = 0; i < n; i++) {
        
        soma_diferencas_quadradas += pow(((double)tempos_ns[i] - media), 2);
    }
    
    return sqrt(soma_diferencas_quadradas / n);
}


long long analisar_arquivo_unico(const char* nome_arquivo, int tamanho) {
    
    
    long long *vetor = (long long*) malloc(tamanho * sizeof(long long));
    if (vetor == NULL) {
        fprintf(stderr, "Erro de alocação de memória para %s.\n", nome_arquivo);
        return -1;
    }

    
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
        
        if (separador == '\n' || separador == EOF) break; 
    }
    fclose(arquivo);
    
    if (i != tamanho) {
        fprintf(stderr, "Aviso: Arquivo %s parece ter %d elementos, mas esperava %d.\n", nome_arquivo, i, tamanho);
    }

    
    long long elemento_para_buscar;
    
    
    double prob = (double)rand() / RAND_MAX; 
    
    if (prob <= 0.7) {
        
        int indice_aleatorio = rand() % tamanho;
        elemento_para_buscar = vetor[indice_aleatorio];
    } else {
        
        elemento_para_buscar = 0;
    }
    


    
    struct timespec inicio_t, fim_t;
    clock_gettime(CLOCK_MONOTONIC, &inicio_t);
    
    busca_binaria(vetor, tamanho, elemento_para_buscar); // 
    
    clock_gettime(CLOCK_MONOTONIC, &fim_t);

    
    free(vetor);

    
    return (fim_t.tv_sec - inicio_t.tv_sec) * 1000000000LL + (fim_t.tv_nsec - inicio_t.tv_nsec);
}

int main() {
    
    srand(time(NULL)); 
    
    int incremento = 10000;
    int limite_max = 100000;
    
    
    const char* arquivo_resultado_c = "../resultados/estatisticas/resultados_C_CasoMedio.csv";
    
    FILE* f_resultados = fopen(arquivo_resultado_c, "w");
    if (f_resultados == NULL) {
        fprintf(stderr, "Erro ao criar arquivo de resultado em %s.\n", arquivo_resultado_c);
        fprintf(stderr, "Verifique se as pastas 'resultados' e 'estatisticas' existem.\n");
        return 1;
    }
    
    
    fprintf(f_resultados, "n,tempo_ms,desvio\n");
    printf("Iniciando análise (C - Caso Médio). Salvando resultados em %s\n", arquivo_resultado_c);

    
    char nome_subpasta[20];
    char nome_arquivo_dados[30];
    char caminho_completo_dados[100];

    
    for (int tamanho = incremento; tamanho <= limite_max; tamanho += incremento) {
        
        sprintf(nome_subpasta, "n%06d", tamanho);
        printf("\nProcessando pasta: ../dados/%s/ (Tamanho: %d)\n", nome_subpasta, tamanho);
        
        
        long long tempos_ns[N_EXECUCOES];
        
        
        for (int i = 0; i < N_EXECUCOES; i++) {
            
            sprintf(nome_arquivo_dados, "arquivo_%02d.csv", i + 1); 
            
            
            sprintf(caminho_completo_dados, "../dados/%s/%s", nome_subpasta, nome_arquivo_dados);
            
            printf("  -> Analisando %s ... ", nome_arquivo_dados);
            
            long long tempo = analisar_arquivo_unico(caminho_completo_dados, tamanho);
            
            if (tempo >= 0) {
                tempos_ns[i] = tempo;
                printf("Tempo: %lld ns\n", tempo);
            } else {
                tempos_ns[i] = 0; 
                printf("Falha na leitura.\n");
            }
        }

        
        double media_ns = calcular_media(tempos_ns, N_EXECUCOES);
        double desvio_ns = calcular_desvio_padrao(tempos_ns, N_EXECUCOES, media_ns);

        
        double media_ms = media_ns / 1000000.0;
        double desvio_ms = desvio_ns / 1000000.0;

        printf("--------------------------------------------------\n");
        printf("Tamanho %d: Média: %.6f ms | Desvio: %.6f ms\n", tamanho, media_ms, desvio_ms);
        printf("--------------------------------------------------\n");
        
        
        fprintf(f_resultados, "%d,%.8f,%.8f\n", tamanho, media_ms, desvio_ms);
        fflush(f_resultados); 
    }
    
    fclose(f_resultados);
    printf("\nAnálise (C - Caso Médio) concluída com sucesso.\n");
    return 0;
}