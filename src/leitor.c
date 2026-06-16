#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "leitor.h"
#include "afd.h"

#define MAX_LINHA 256

void remover_quebra_linha(char* string) {
    string[strcspn(string, "\n")] = '\0';
    string[strcspn(string, "\r")] = '\0';
}

bool ler_configuracao(FILE* arquivo, AFD* afd) {
    char linha[MAX_LINHA];

    // Lendo Q:
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    char* token = strtok(linha + 3, " "); 
    while (token != NULL) {
        adicionar_estado(afd, token);
        token = strtok(NULL, " ");
    }

    //lendo S:
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
    remover_quebra_linha(linha);

    char* dicionario = linha;
    if(strncmp(linha, "S: ", 3) == 0) {
        dicionario = linha + 3;
        while (*dicionario == ' ' || *dicionario == '\t') {
            dicionario++; 
        }
    }
    definir_dicionario(afd, dicionario);

    // Lendo I:
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    char estado_ini[8];
    sscanf(linha, "I: %7s", estado_ini);
    definir_inicial(afd, estado_ini);

    // Lendo F:
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    token = strtok(linha + 3, " ");
    while (token != NULL) {
        adicionar_final(afd, token);
        token = strtok(NULL, " ");
    }

    // Lendo transições
    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        remover_quebra_linha(linha);

        if (strcmp(linha, "---") == 0) {
            break; 
        }

        char origem[8], destino[8], barra[2], seta[3];
        int lidos = sscanf(linha, "%7s %2s %7s %1s", origem, seta, destino, barra);
        
        if (lidos == 4 && strcmp(seta, "->") == 0 && strcmp(barra, "|") == 0) {
            char* ptr_simbolos = strchr(linha, '|');
            if (ptr_simbolos != NULL) {
                ptr_simbolos++;
                char* token_simbolo = strtok(ptr_simbolos, " ");
                while (token_simbolo != NULL) {
                    adicionar_transicao(afd, origem, token_simbolo[0], destino);
                    token_simbolo = strtok(NULL, " ");
                }
            }
        }
    }
    return true;
}

void ler_casos_teste(FILE* arquivo, AFD* afd) {
    char linha[MAX_LINHA];

    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        remover_quebra_linha(linha);
        
        // Testa a palavra no AFD e imprime o resultado exigido
        if (processar_palavra(afd, linha)) {
            printf("OK\n"); 
        } else {
            printf("X\n"); 
        }
    }
}