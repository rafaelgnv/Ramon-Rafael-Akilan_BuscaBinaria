import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.Locale; // Para formatar o CSV com "." em vez de ","

public class AnalisadorDeComplexidade {

    // Quantidade de arquivos por pasta
    private static final int N_EXECUCOES = 50;

    /**
     * Função de Busca Binária (sem alterações)
     */
    public static int buscaBinaria(long[] vetor, int tamanho, long elementoProcurado) {
        int inicio = 0;
        int fim = tamanho - 1; 
        while (inicio <= fim) {
            int meio = inicio + (fim - inicio) / 2;
            if (vetor[meio] == elementoProcurado) return meio;
            if (vetor[meio] < elementoProcurado) inicio = meio + 1;
            else fim = meio - 1;
        }
        return -1; // Não encontrado
    }
    
    /**
     * Função auxiliar para calcular a Média
     */
    public static double calcularMedia(long[] temposNs) {
        long soma = 0;
        for (long tempo : temposNs) {
            soma += tempo;
        }
        return (double)soma / temposNs.length;
    }

    /**
     * Função auxiliar para calcular o Desvio Padrão
     */
    public static double calcularDesvioPadrao(long[] temposNs, double media) {
        double somaDiferencasQuadradas = 0;
        for (long tempo : temposNs) {
            somaDiferencasQuadradas += Math.pow(((double)tempo - media), 2);
        }
        return Math.sqrt(somaDiferencasQuadradas / temposNs.length);
    }

    /**
     * Função que executa UMA busca e retorna o tempo em nanosegundos
     */
    public static long analisarArquivoUnico(String nomeArquivo, int tamanho) {
        long[] vetor = new long[tamanho];
        
        try (Scanner scanner = new Scanner(new File(nomeArquivo))) {
            if (!scanner.hasNextLine()) {
                System.err.println("Erro: Arquivo " + nomeArquivo + " está vazio.");
                return -1;
            }
            String linhaInteira = scanner.nextLine();
            String[] numerosEmString = linhaInteira.split(",");

            int i = 0;
            for (String s : numerosEmString) {
                if (i >= tamanho) break;
                vetor[i++] = Long.parseLong(s.trim()); 
            }
        } catch (FileNotFoundException e) {
            System.err.println("Erro ao abrir o arquivo: " + nomeArquivo);
            return -1; 
        } catch (Exception e) {
            System.err.println("Erro ao processar " + nomeArquivo + ": " + e.getMessage());
            return -1;
        }
        
        // 3. Definir o elemento para buscar (O PIOR CASO: 0)
        long elementoParaBuscar = 0;

        // 4. Medir o tempo de execução
        long inicioT = System.nanoTime();
        
        buscaBinaria(vetor, tamanho, elementoParaBuscar); // A busca é executada
        
        long fimT = System.nanoTime();

        // 6. Retornar o tempo em nanosegundos
        return fimT - inicioT;
    }

    public static void main(String[] args) {

        String basePath = new File("").getAbsolutePath();

        // Usa File.separator para funcionar em qualquer S.O. (Windows "\", Linux/Mac "/")
        String caminhoDadosBase = basePath + File.separator + "dados";
        String caminhoResultadosBase = basePath + File.separator + "resultados" + File.separator + "estatisticas";


        int incremento = 10000;
        int limiteMax = 100000;

        // --- CAMINHO DO NOVO ARQUIVO DE RESULTADOS ---
        // (Assumindo que o .class está em 'codigo/java/')
        String arquivoResultadoJava = caminhoResultadosBase + File.separator + "resultados_Java.csv";

        // Usamos try-with-resources para garantir que o PrintWriter feche
        // Usamos Locale.US para garantir que o separador decimal seja "."
        try (PrintWriter pw = new PrintWriter(new FileWriter(arquivoResultadoJava))) {
            
            pw.println("n,tempo_ms,desvio");
            System.out.println("Iniciando análise (Java). Salvando resultados em " + arquivoResultadoJava);

            // LOOP EXTERNO (Para os tamanhos / pastas)
            for (int tamanho = incremento; tamanho <= limiteMax; tamanho += incremento) {
                
                String nomeSubpasta = String.format("n%06d", tamanho);
                
                String caminhoSubpastaDados = caminhoDadosBase + File.separator + nomeSubpasta;
                System.out.printf("\nProcessando pasta: %s/ (Tamanho: %d)\n", caminhoSubpastaDados, tamanho);
                
                long[] temposNs = new long[N_EXECUCOES];

                // LOOP INTERNO (Para os 50 arquivos)
                for (int i = 0; i < N_EXECUCOES; i++) {
                    String nomeArquivoDados = String.format("arquivo_%02d.csv", i + 1);
                    // Monta o caminho (assumindo que estamos em 'codigo/java/')
                    String caminhoCompletoDados = String.format("dados/%s/%s", nomeSubpasta, nomeArquivoDados);

                    System.out.printf("  -> Analisando %s ... ", nomeArquivoDados);
                    
                    long tempo = analisarArquivoUnico(caminhoCompletoDados, tamanho);
                    
                    if (tempo >= 0) {
                        temposNs[i] = tempo;
                        System.out.printf("Tempo: %d ns\n", tempo);
                    } else {
                        temposNs[i] = 0; // Marca como falha
                        System.out.println("Falha na leitura.");
                    }
                }

                // --- CÁLCULO ESTATÍSTICO ---
                double mediaNs = calcularMedia(temposNs);
                double desvioNs = calcularDesvioPadrao(temposNs, mediaNs);
                
                // --- CONVERSÃO PARA MILISSEGUNDOS (ms) ---
                double mediaMs = mediaNs / 1_000_000.0;
                double desvioMs = desvioNs / 1_000_000.0;

                System.out.println("--------------------------------------------------");
                // Usamos Locale.US para garantir o "." como separador
                System.out.printf(Locale.US, "Tamanho %d: Média: %.6f ms | Desvio: %.6f ms\n", tamanho, mediaMs, desvioMs);
                System.out.println("--------------------------------------------------");

                // Salva a linha de resultado no CSV
                pw.printf(Locale.US, "%d,%.8f,%.8f\n", tamanho, mediaMs, desvioMs);
            }
            
        } catch (IOException e) {
            System.err.println("Erro ao escrever o arquivo de resultado: " + e.getMessage());
            System.err.println("Verifique se as pastas 'resultados' e 'estatisticas' existem.");
        }
        
        System.out.println("\nAnálise (Java) concluída com sucesso.");
    }
}