import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class BinarySearchAnalyzer {

    /**
     * Realiza uma busca binária em um vetor de inteiros.
     * @param array O vetor ordenado onde a busca será realizada.
     * @param target O elemento a ser procurado.
     * @return O índice do elemento se encontrado, caso contrário -1.
     */
    public static int binarySearch(int[] array, int target) {
        int left = 0;
        int right = array.length - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2; // Previne overflow

            if (array[mid] == target) {
                return mid; // Elemento encontrado
            }

            if (array[mid] < target) {
                left = mid + 1; // Busca na metade direita
            } else {
                right = mid - 1; // Busca na metade esquerda
            }
        }
        return -1; // Elemento não encontrado
    }

    public static void analyzeFile(int size) {
        String fileName = "dados_" + size + ".txt";
        System.out.println("---------------------------------------------------");
        System.out.printf("Analisando arquivo: %s (Tamanho: %d elementos)\n", fileName, size);

        try {
            // 1. Carregar os dados do arquivo para o vetor
            int[] array = new int[size];
            // O try-with-resources garante que o reader será fechado automaticamente
            try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
                String line;
                int index = 0;
                while ((line = reader.readLine()) != null && index < size) {
                    array[index++] = Integer.parseInt(line);
                }
            }

            // 2. Definir um elemento para buscar (um que certamente existe)
            int target = array[size / 2]; // Vamos procurar pelo elemento do meio
            System.out.printf("  Procurando pelo elemento: %d\n", target);

            // 3. Medir o tempo de execução da busca
            long startTime = System.nanoTime();
            int foundIndex = binarySearch(array, target);
            long endTime = System.nanoTime();

            long durationInNanos = endTime - startTime;

            if (foundIndex != -1) {
                System.out.printf("    -> Sucesso! Elemento encontrado no índice %d.\n", foundIndex);
            } else {
                System.out.printf("    -> Falha! Elemento não foi encontrado.\n");
            }
            System.out.printf("    -> Tempo de execução: %d nanosegundos\n", durationInNanos);

        } catch (IOException e) {
            System.err.println("Erro ao ler o arquivo: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("Erro: O arquivo contém dados que não são números inteiros.");
        } catch (OutOfMemoryError e) {
            System.err.println("\nErro: Sem memória suficiente para carregar o vetor de tamanho " + size);
            System.err.println("Tente executar o Java com mais heap space, ex: java -Xmx4g BinarySearchAnalyzer");
        }
    }

    public static void main(String[] args) {
        int[] sizes = {10_000, 100_000, 1_000_000, 10_000_000};
        for (int size : sizes) {
            analyzeFile(size);
        }
        System.out.println("---------------------------------------------------");
    }
}