import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Random;

public class DataGenerator {

    public static void generateAndSave(int size) {
        String fileName = "dados_" + size + ".txt";
        System.out.printf("Gerando dados para o arquivo %s (tamanho %d)... ", fileName, size);

        try {
            // 1. Criar e preencher o vetor com números aleatórios
            int[] array = new int[size];
            Random random = new Random();
            for (int i = 0; i < size; i++) {
                // Gera números aleatórios até o dobro do tamanho para ter variedade
                array[i] = random.nextInt(size * 2);
            }

            // 2. Ordenar o vetor
            Arrays.sort(array);

            // 3. Salvar o vetor ordenado no arquivo
            // O try-with-resources garante que o writer será fechado automaticamente
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName))) {
                for (int number : array) {
                    writer.write(String.valueOf(number));
                    writer.newLine();
                }
            }
            
            System.out.println("Concluído!");

        } catch (IOException e) {
            System.err.println("Ocorreu um erro ao escrever o arquivo: " + e.getMessage());
        } catch (OutOfMemoryError e) {
            System.err.println("\nErro: Sem memória suficiente para criar o vetor de tamanho " + size);
            System.err.println("Tente executar o Java com mais heap space, ex: java -Xmx4g DataGenerator");
        }
    }

    public static void main(String[] args) {
        int[] sizes = {10_000, 100_000, 1_000_000, 10_000_000};

        for (int size : sizes) {
            generateAndSave(size);
        }
        
        System.out.println("\nTodos os arquivos foram gerados com sucesso.");
    }
}