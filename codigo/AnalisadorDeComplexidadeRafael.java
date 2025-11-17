import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Locale;
import java.util.Scanner;
import java.util.Random; 


public class AnalisadorDeComplexidadeRafael {

    
    private static final int N_EXECUCOES = 50;
    
    
    private static final Random RND = new Random(); 

    
    public static int buscaBinaria(long[] vetor, int tamanho, long elementoProcurado) {
        int inicio = 0;
        int fim = tamanho - 1; 
        while (inicio <= fim) {
            int meio = inicio + (fim - inicio) / 2;
            if (vetor[meio] == elementoProcurado) return meio;
            if (vetor[meio] < elementoProcurado) inicio = meio + 1;
            else fim = meio - 1;
        }
        return -1; 
    }
    
    
    public static double calcularMedia(long[] temposNs) {
        long soma = 0;
        for (long tempo : temposNs) {
            soma += tempo;
        }
        return (double)soma / temposNs.length;
    }

    
    public static double calcularDesvioPadrao(long[] temposNs, double media) {
        double somaDiferencasQuadradas = 0;
        for (long tempo : temposNs) {
            somaDiferencasQuadradas += Math.pow(((double)tempo - media), 2);
        }
        return Math.sqrt(somaDiferencasQuadradas / temposNs.length);
    }

    
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
            
            if (i != tamanho) {
                 System.err.printf("Aviso: Arquivo %s parece ter %d elementos, mas esperava %d.\n", nomeArquivo, i, tamanho);
            }
            
        } catch (FileNotFoundException e) {
            System.err.println("Erro ao abrir o arquivo: " + nomeArquivo);
            return -1; 
        } catch (Exception e) {
            System.err.println("Erro ao processar " + nomeArquivo + ": " + e.getMessage());
            return -1;
        }
        
        
        long elementoParaBuscar;
        
        if (RND.nextDouble() <= 0.7) {
            
            int indiceAleatorio = RND.nextInt(tamanho);
            elementoParaBuscar = vetor[indiceAleatorio];
        } else {
            
            elementoParaBuscar = 0;
        }
        

        
        long inicioT = System.nanoTime();
        
        buscaBinaria(vetor, tamanho, elementoParaBuscar); 
        
        long fimT = System.nanoTime();

        
        return fimT - inicioT;
    }

    public static void main(String[] args) {
        int incremento = 10000;
        int limiteMax = 100000;

        
        String arquivoResultadoJava = "../resultados/estatisticas/resultados_Java_CasoMedio.csv";

        
        try (PrintWriter pw = new PrintWriter(new FileWriter(arquivoResultadoJava))) {
            
            pw.println("n,tempo_ms,desvio");
            System.out.println("Iniciando análise (Java - Caso Médio). Salvando resultados em " + arquivoResultadoJava);

            
            for (int tamanho = incremento; tamanho <= limiteMax; tamanho += incremento) {
                
                String nomeSubpasta = String.format("n%06d", tamanho);
                System.out.printf("\nProcessando pasta: ../dados/%s/ (Tamanho: %d)\n", nomeSubpasta, tamanho);
                
                long[] temposNs = new long[N_EXECUCOES];

                
                for (int i = 0; i < N_EXECUCOES; i++) {
                    String nomeArquivoDados = String.format("arquivo_%02d.csv", i + 1);
                    
                    
                    String caminhoCompletoDados = String.format("../dados/%s/%s", nomeSubpasta, nomeArquivoDados);

                    System.out.printf("  -> Analisando %s ... ", nomeArquivoDados);
                    
                    long tempo = analisarArquivoUnico(caminhoCompletoDados, tamanho);
                    
                    if (tempo >= 0) {
                        temposNs[i] = tempo;
                        System.out.printf("Tempo: %d ns\n", tempo);
                    } else {
                        temposNs[i] = 0; 
                        System.out.println("Falha na leitura.");
                    }
                }

                
                double mediaNs = calcularMedia(temposNs);
                double desvioNs = calcularDesvioPadrao(temposNs, mediaNs);
                
                
                double mediaMs = mediaNs / 1_000_000.0;
                double desvioMs = desvioNs / 1_000_000.0;

                System.out.println("--------------------------------------------------");
                
                System.out.printf(Locale.US, "Tamanho %d: Média: %.6f ms | Desvio: %.6f ms\n", tamanho, mediaMs, desvioMs);
                System.out.println("--------------------------------------------------");

                
                pw.printf(Locale.US, "%d,%.8f,%.8f\n", tamanho, mediaMs, desvioMs);
            }
            
        } catch (IOException e) {
            System.err.println("Erro ao escrever o arquivo de resultado: " + e.getMessage());
            System.err.println("Verifique se as pastas '../resultados/estatisticas' existem.");
        }
        
        System.out.println("\nAnálise (Java - Caso Médio) concluída com sucesso.");
    }
}