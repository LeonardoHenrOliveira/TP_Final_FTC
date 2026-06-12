#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"
#include "afd.h"

int main() {
    char nome_arquivo[100];
    char caminho_arquivo[100];

    printf("============================================\n");
    printf("Digite o nome do arquivo de teste: ");
    
    if (scanf("%99s", nome_arquivo) != 1) {
        printf("Erro ao ler o nome do arquivo.\n");
        return 1;
    }

    sprintf(caminho_arquivo, "../testes/%s", nome_arquivo);

    FILE* arquivo = fopen(caminho_arquivo, "r");
    
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo no caminho '%s'.\n", caminho_arquivo);
        return 1;
    }

    char primeira_linha[20];
    
    if (fgets(primeira_linha, sizeof(primeira_linha), arquivo) != NULL) {
        
        primeira_linha[strcspn(primeira_linha, "\n")] = '\0';
        primeira_linha[strcspn(primeira_linha, "\r")] = '\0';
        
        if (strcmp(primeira_linha, "@AF") == 0) {
            printf("Tipo detectado: Automato Finito (@AF)\n");
            
            AFD meu_afd;
            inicializar_afd(&meu_afd);

            if (ler_configuracao(arquivo, &meu_afd)) {
                ler_casos_teste(arquivo, &meu_afd);
            } else {
                printf("Erro de formatacao ao ler a configuracao do AF.\n");
            }
            
        } else if (strcmp(primeira_linha, "@MT") == 0) {
            printf("Tipo detectado: Maquina de Turing (@MT)\n");
            
        } else if (strcmp(primeira_linha, "@AP") == 0) {
            printf("Tipo detectado: Automato de Pilha (@AP)\n");
            
        } else {
            printf("Erro: Tipo de automato desconhecido ou tag faltando (%s).\n", primeira_linha);
        }
        
    } else {
        printf("Erro: Arquivo vazio.\n");
    }

    fclose(arquivo);
    return 0;
}