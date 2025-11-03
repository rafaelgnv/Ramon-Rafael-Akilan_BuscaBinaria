import time

def busca_binaria(lista_ordenada, alvo):
    """
    Realiza uma busca binária em uma lista ordenada.
    Retorna o índice do alvo se encontrado, caso contrário, -1.
    Esta função é implementada manualmente para fins de estudo.
    """
    inicio = 0
    fim = len(lista_ordenada) - 1

    while inicio <= fim:
        meio = (inicio + fim) // 2  # Divisão inteira

        # Se o valor do meio é o alvo, retorna o índice
        if lista_ordenada[meio] == alvo:
            return meio
        
        # Se o alvo é maior, ignora a metade esquerda
        elif lista_ordenada[meio] < alvo:
            inicio = meio + 1
        
        # Se o alvo é menor, ignora a metade direita
        else:
            fim = meio - 1
            
    return -1 # Retorna -1 se o elemento não for encontrado

def analisar_arquivo(tamanho):
    """
    Carrega um vetor de um arquivo, executa a busca binária e mede o tempo.
    """
    nome_arquivo = f"dados_{tamanho}.txt"
    print("---------------------------------------------------")
    print(f"Analisando arquivo: {nome_arquivo} (Tamanho: {tamanho} elementos)")

    try:
        # 1. Carrega os dados do arquivo para a lista
        with open(nome_arquivo, 'r') as f:
            # List comprehension é uma forma rápida e "pythônica" de ler os dados
            numeros = [int(line.strip()) for line in f]

        # 2. Define um alvo que sabemos que existe para garantir uma busca justa
        # Vamos procurar por um elemento próximo do final da lista
        alvo = numeros[(tamanho * 3) // 4]
        print(f"  Procurando pelo elemento: {alvo}")

        # 3. Mede o tempo de execução da busca
        # time.perf_counter() é ideal para medir intervalos curtos de tempo
        inicio_tempo = time.perf_counter()
        indice_encontrado = busca_binaria(numeros, alvo)
        fim_tempo = time.perf_counter()

        duracao_em_segundos = fim_tempo - inicio_tempo

        if indice_encontrado != -1:
            print(f"    -> Sucesso! Elemento encontrado no índice {indice_encontrado}.")
        else:
            print(f"    -> Falha! Elemento não foi encontrado.")
        
        # Exibindo em microssegundos (mais legível para tempos tão pequenos)
        print(f"    -> Tempo de execução: {duracao_em_segundos * 1_000_000:.4f} microssegundos")

    except FileNotFoundError:
        print(f"Erro: Arquivo '{nome_arquivo}' não encontrado. Execute o gerador primeiro.")
    except MemoryError:
        print(f"Erro: Memória insuficiente para carregar o arquivo de tamanho {tamanho}.")
    except Exception as e:
        print(f"Ocorreu um erro: {e}")


if __name__ == "__main__":
    tamanhos = [10_000, 100_000, 1_000_000, 10_000_000]
    for tam in tamanhos:
        analisar_arquivo(tam)
    print("---------------------------------------------------")