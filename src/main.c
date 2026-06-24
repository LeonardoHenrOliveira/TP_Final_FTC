#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"
#include "afd.h"
#include "mt.h"
#include "afn.h"
int main() {
    char nome_arquivo[100];
    char caminho_arquivo[100];
    printf("Digite o nome do arquivo de teste (ex: teste1.txt): ");
    if (scanf("%99s", nome_arquivo) != 1) {
        fprintf(stderr, "Erro ao ler o nome do arquivo.\n");
        return 1;
    }

    sprintf(caminho_arquivo, "../testes/%s", nome_arquivo);

    FILE* arquivo = fopen(caminho_arquivo, "r");
    
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo.\n");
        return 1;
    }

    char primeira_linha[20];
    
    if (fgets(primeira_linha, sizeof(primeira_linha), arquivo) != NULL) {
        
        primeira_linha[strcspn(primeira_linha, "\n")] = '\0';
        primeira_linha[strcspn(primeira_linha, "\r")] = '\0';
        
        if (strcmp(primeira_linha, "@AF") == 0) {
            AFD meu_afd;
            inicializar_afd(&meu_afd);

            if (ler_configuracao(arquivo, &meu_afd)) {
                ler_casos_teste(arquivo, &meu_afd);
            } else {
                fprintf(stderr, "Erro de formatacao do AF.\n");
            }
            
        } else if (strcmp(primeira_linha, "@MT") == 0) {
            MaquinaTuring minha_mt;
            inicializar_mt(&minha_mt);

            if (ler_configuracao_mt(arquivo, &minha_mt)) {
                ler_casos_teste_mt(arquivo, &minha_mt, false);
            } else {
                fprintf(stderr, "Erro de formatacao da MT.\n");
            }
            
        } else if (strcmp(primeira_linha, "@ALL") == 0) {
            MaquinaTuring minha_all;
            inicializar_mt(&minha_all);

            if (ler_configuracao_mt(arquivo, &minha_all)) {
                ler_casos_teste_mt(arquivo, &minha_all, true);
            } else {
                fprintf(stderr, "Erro de formatacao da MT.\n");
            }
        } else if (strcmp(primeira_linha, "@AFN") == 0) {
            AFN meu_afn;
            inicializar_afn(&meu_afn);

            if (ler_configuracao_afn(arquivo, &meu_afn)) {
                ler_casos_teste_afn(arquivo, &meu_afn);
            } else {
                fprintf(stderr, "Erro de formatacao do AFN.\n");
            }
        } else if (strcmp(primeira_linha, "@APN") == 0 || strcmp(primeira_linha, "@AP") == 0) {
            printf("Tipo detectado: Automato de Pilha Nao-Deterministico (%s)\n", primeira_linha);
            
            APN meu_apn;
            inicializar_apn(&meu_apn);

            if (ler_configuracao_apn(arquivo, &meu_apn)) {
                ler_casos_teste_apn(arquivo, &meu_apn, true); // padrao python é true
            } else {
                fprintf(stderr, "Erro de formatacao do APN.\n");
            }
        } else if (strcmp(primeira_linha, "@APD") == 0) {
            printf("Tipo detectado: Automato de Pilha Deterministico (@APD)\n");
            
            APD meu_apd;
            inicializar_apd(&meu_apd);

            if (ler_configuracao_apd(arquivo, &meu_apd)) {
                ler_casos_teste_apd(arquivo, &meu_apd);
            } else {
                fprintf(stderr, "Erro de formatacao do APD.\n");
            }
        } else {
            fprintf(stderr, "Erro: Tipo de automato desconhecido.\n");
        }
        
    }

    fclose(arquivo);
    return 0;
}