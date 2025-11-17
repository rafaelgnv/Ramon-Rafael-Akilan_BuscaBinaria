import time
import sys
from pathlib import Path
import statistics 
import random     


N_EXECUCOES = 50


CAMINHO_DO_SCRIPT = Path(__file__).parent
CAMINHO_DO_PROJETO = CAMINHO_DO_SCRIPT.parent 
PASTA_DADOS = CAMINHO_DO_PROJETO / "dados"
PASTA_RESULTADOS = CAMINHO_DO_PROJETO / "resultados" / "estatisticas"


def busca_binaria(vetor, elemento_procurado):
    """
    Função de Busca Binária (sem alterações)
    """
    inicio = 0
    fim = len(vetor) - 1 
    while inicio <= fim:
        meio = inicio + (fim - inicio) // 2
        if vetor[meio] == elemento_procurado:
            return meio
        if vetor[meio] < elemento_procurado:
            inicio = meio + 1
        else:
            fim = meio - 1
    return -1 

def analisar_arquivo_unico(caminho_completo_dados, tamanho):
    """
    Função que executa UMA busca e retorna o tempo em nanosegundos
    """
    vetor = [] 
    try:
        with open(caminho_completo_dados, 'r') as f:
            linha_inteira = f.readline().strip()
            if not linha_inteira:
                print(f"Erro: Arquivo {caminho_completo_dados.name} está vazio.", file=sys.stderr)
                return -1

            numeros_em_string = linha_inteira.split(',')
            
            vetor = [int(s) for s in numeros_em_string]
            
            if len(vetor) != tamanho:
                 print(f"Aviso: Arquivo {caminho_completo_dados.name} tem {len(vetor)} elementos, esperado {tamanho}.", file=sys.stderr)


    except FileNotFoundError:
        print(f"Erro ao abrir o arquivo: {caminho_completo_dados}.", file=sys.stderr)
        return -1
    except Exception as e:
        print(f"Erro ao processar {caminho_completo_dados.name}: {e}", file=sys.stderr)
        return -1
        
    if not vetor: 
        print(f"Erro: Vetor ficou vazio para {caminho_completo_dados.name}", file=sys.stderr)
        return -1

    if random.random() <= 0.7:
        indice_aleatorio = random.randint(0, tamanho - 1)
        elemento_para_buscar = vetor[indice_aleatorio]
    else:
        elemento_para_buscar = 0

    inicio_t = time.perf_counter_ns()
    
    busca_binaria(vetor, elemento_para_buscar) 
    
    fim_t = time.perf_counter_ns()

    return fim_t - inicio_t

if __name__ == "__main__":
    incremento = 10000
    limite_max = 100000

    arquivo_resultado_py = PASTA_RESULTADOS / "resultados_Python_CasoMedio.csv" 

    try:
        with open(arquivo_resultado_py, 'w', newline='') as f_resultados:
            
            f_resultados.write("n,tempo_ms,desvio\n")
            print(f"Iniciando análise (Python - Caso Médio). Salvando resultados em {arquivo_resultado_py}")

            for tamanho in range(incremento, limite_max + incremento, incremento):
                
                nome_subpasta = f"n{tamanho:06d}"
                caminho_subpasta = PASTA_DADOS / nome_subpasta
                print(f"\nProcessando pasta: {caminho_subpasta} (Tamanho: {tamanho})")
                
                tempos_ns = []

                for i in range(1, N_EXECUCOES + 1):
                    nome_arquivo_dados = f"arquivo_{i:02d}.csv"
                    caminho_completo_dados = caminho_subpasta / nome_arquivo_dados

                    print(f"  -> Analisando {nome_arquivo_dados} ... ", end="")
                    
                    tempo = analisar_arquivo_unico(caminho_completo_dados, tamanho)
                    
                    if tempo >= 0:
                        tempos_ns.append(tempo)
                        print(f"Tempo: {tempo} ns")
                    else:
                        print("Falha na leitura.")
                
                if tempos_ns:
                    media_ns = statistics.mean(tempos_ns)
                    if len(tempos_ns) > 1:
                        desvio_ns = statistics.stdev(tempos_ns)
                    else:
                        desvio_ns = 0.0
                else:
                    media_ns = 0.0
                    desvio_ns = 0.0
                
                media_ms = media_ns / 1_000_000.0
                desvio_ms = desvio_ns / 1_000_000.0
                
                print("--------------------------------------------------")
                print(f"Tamanho {tamanho}: Média: {media_ms:.6f} ms | Desvio: {desvio_ms:.6f} ms")
                print("--------------------------------------------------")
                
                f_resultados.write(f"{tamanho},{media_ms:.8f},{desvio_ms:.8f}\n")
                f_resultados.flush() 

    except IOError as e:
        print(f"Erro ao escrever o arquivo de resultado: {e}", file=sys.stderr)
        print("Verifique se as pastas 'resultados' e 'estatisticas' existem e têm permissão de escrita.", file=sys.stderr)
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}", file=sys.stderr)
    
    print("\nAnálise (Python - Caso Médio) concluída com sucesso.")