import random

def gerar_e_salvar(tamanho):
    """
    Gera uma lista de números aleatórios, ordena e salva em um arquivo de texto.
    """
    nome_arquivo = f"dados_{tamanho}.txt"
    print(f"Gerando dados para {nome_arquivo} (tamanho {tamanho})... ", end="", flush=True)

    try:
        # 1. Gera uma lista de números aleatórios
        # Usamos uma faixa maior para garantir diversidade
        numeros = [random.randint(0, tamanho * 2) for _ in range(tamanho)]

        # 2. Ordena a lista (o Timsort do Python é muito eficiente)
        numeros.sort()

        # 3. Salva os números no arquivo, um por linha
        with open(nome_arquivo, 'w') as f:
            # Escrever em blocos é mais rápido do que linha por linha
            f.write('\n'.join(map(str, numeros)))
        
        print("Concluído!")

    except MemoryError:
        print(f"\nErro: Memória insuficiente para criar a lista de tamanho {tamanho}.")
        print("Esta máquina pode não ter RAM suficiente para a lista de 10 milhões de inteiros.")
    except Exception as e:
        print(f"\nOcorreu um erro: {e}")

if __name__ == "__main__":
    tamanhos = [10_000, 100_000, 1_000_000, 10_000_000]
    for tam in tamanhos:
        gerar_e_salvar(tam)
    print("\nTodos os arquivos foram gerados com sucesso.")