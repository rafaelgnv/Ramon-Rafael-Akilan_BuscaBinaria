import time
import sys
from pathlib import Path
import statistics # Para calcular média e desvio padrão
import random     # <--- ADICIONADO

# Quantidade de arquivos por pasta
N_EXECUCOES = 50

# --- LÓGICA DE CAMINHOS ---
# (O script está em 'codigo/')
CAMINHO_DO_SCRIPT = Path(__file__).parent
CAMINHO_DO_PROJETO = CAMINHO_DO_SCRIPT.parent # Sobe 1 nível
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
    return -1 # Não encontrado

def analisar_arquivo_unico(caminho_completo_dados, tamanho):
    """
    Função que executa UMA busca e retorna o tempo em nanosegundos
    """
    vetor = [] # Inicializa como lista vazia
    try:
        with open(caminho_completo_dados, 'r') as f:
            linha_inteira = f.readline().strip()
            if not linha_inteira:
                print(f"Erro: Arquivo {caminho_completo_dados.name} está vazio.", file=sys.stderr)
                return -1

            numeros_em_string = linha_inteira.split(',')
            # Corrigido: Garantir que o vetor tenha o tamanho esperado (embora o split já deva fazer isso)
            vetor = [int(s) for s in numeros_em_string]
            
            if len(vetor) != tamanho:
                 print(f"Aviso: Arquivo {caminho_completo_dados.name} tem {len(vetor)} elementos, esperado {tamanho}.", file=sys.stderr)


    except FileNotFoundError:
        print(f"Erro ao abrir o arquivo: {caminho_completo_dados}.", file=sys.stderr)
        return -1
    except Exception as e:
        print(f"Erro ao processar {caminho_completo_dados.name}: {e}", file=sys.stderr)
        return -1
        
    if not vetor: # Checagem extra
        print(f"Erro: Vetor ficou vazio para {caminho_completo_dados.name}", file=sys.stderr)
        return -1

    # --- LÓGICA DE BUSCA 70/30 (MODIFICADO) ---
    if random.random() <= 0.7:
        # 70% SUCESSO: Pega um índice aleatório
        # Usamos 'tamanho - 1' porque o índice é 0-based
        indice_aleatorio = random.randint(0, tamanho - 1)
        elemento_para_buscar = vetor[indice_aleatorio]
    else:
        # 30% FALHA: Pega o 0 (que sabemos que não existe)
        elemento_para_buscar = 0
    # --- FIM DA MODIFICAÇÃO ---

    # 4. Medir o tempo de execução
    inicio_t = time.perf_counter_ns()
    
    busca_binaria(vetor, elemento_para_buscar) # A busca é executada
    
    fim_t = time.perf_counter_ns()

    # 6. Retornar o tempo em nanosegundos
    return fim_t - inicio_t

# Bloco principal
if __name__ == "__main__":
    incremento = 10000
    limite_max = 100000

    # --- CAMINHO DO NOVO ARQUIVO DE RESULTADOS ---
    arquivo_resultado_py = PASTA_RESULTADOS / "resultados_Python_CasoMedio.csv" # Nome do arquivo alterado

    try:
        # Abre o arquivo de resultados para escrita
        with open(arquivo_resultado_py, 'w', newline='') as f_resultados:
            
            # (Não precisamos de um 'writer' CSV, pois o formato é simples)
            f_resultados.write("n,tempo_ms,desvio\n")
            print(f"Iniciando análise (Python - Caso Médio). Salvando resultados em {arquivo_resultado_py}")

            # LOOP EXTERNO (Para os tamanhos / pastas)
            for tamanho in range(incremento, limite_max + incremento, incremento):
                
                nome_subpasta = f"n{tamanho:06d}"
                caminho_subpasta = PASTA_DADOS / nome_subpasta
                print(f"\nProcessando pasta: {caminho_subpasta} (Tamanho: {tamanho})")
                
                tempos_ns = [] # Lista para guardar os 50 tempos

                # LOOP INTERNO (Para os 50 arquivos)
                for i in range(1, N_EXECUCOES + 1): # Vai de 1 a 50
                    nome_arquivo_dados = f"arquivo_{i:02d}.csv"
                    caminho_completo_dados = caminho_subpasta / nome_arquivo_dados

                    print(f"  -> Analisando {nome_arquivo_dados} ... ", end="")
                    
                    # Passamos o 'tamanho' para a função
                    tempo = analisar_arquivo_unico(caminho_completo_dados, tamanho)
                    
                    if tempo >= 0:
                        tempos_ns.append(tempo)
                        print(f"Tempo: {tempo} ns")
                    else:
                        print("Falha na leitura.")
                
                # --- CÁLCULO ESTATÍSTICO ---
                # Garante que temos dados antes de calcular
                if tempos_ns:
                    media_ns = statistics.mean(tempos_ns)
                    # Precisa de pelo menos 2 pontos para desvio padrão
                    if len(tempos_ns) > 1:
                        desvio_ns = statistics.stdev(tempos_ns)
                    else:
                        desvio_ns = 0.0
                else:
                    media_ns = 0.0
                    desvio_ns = 0.0
                
                # --- CONVERSÃO PARA MILISSEGUNDOS (ms) ---
                media_ms = media_ns / 1_000_000.0
                desvio_ms = desvio_ns / 1_000_000.0
                
                print("--------------------------------------------------")
                print(f"Tamanho {tamanho}: Média: {media_ms:.6f} ms | Desvio: {desvio_ms:.6f} ms")
                print("--------------------------------------------------")
                
                # Salva a linha de resultado no CSV
                f_resultados.write(f"{tamanho},{media_ms:.8f},{desvio_ms:.8f}\n")
                f_resultados.flush() # Garante que a linha seja escrita

    except IOError as e:
        print(f"Erro ao escrever o arquivo de resultado: {e}", file=sys.stderr)
        print("Verifique se as pastas 'resultados' e 'estatisticas' existem e têm permissão de escrita.", file=sys.stderr)
    except Exception as e:
        print(f"Ocorreu um erro inesperado: {e}", file=sys.stderr)
    
    print("\nAnálise (Python - Caso Médio) concluída com sucesso.")