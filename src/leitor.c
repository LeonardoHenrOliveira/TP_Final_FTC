#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "leitor.h"
#include "afd.h"
#include "mt.h"

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

bool ler_configuracao_mt(FILE* arquivo, MaquinaTuring* mt) {
    char linha[MAX_LINHA];

    // Lendo Q:
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    char* token = strtok(linha + 3, " "); 
    while (token != NULL) {
        strcpy(mt->estados[mt->num_estados++], token);
        token = strtok(NULL, " ");
    }

    // =======================================================
    // BLOCO DE LEITURA FLEXÍVEL (S:, G: e I:)
    // =======================================================
    
    // Lê a próxima linha após o Q:
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
    remover_quebra_linha(linha);

    // Verifica se a linha atual é o S: (Alfabeto de entrada)
    if (strncmp(linha, "S:", 2) == 0) {
        // Pula o S: e lê a próxima linha
        if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
        remover_quebra_linha(linha);
    }

    // Verifica se a linha atual é o G: (Alfabeto da Fita)
    if (strncmp(linha, "G:", 2) == 0) {
        if (strlen(linha) > 3) { // Garante que tem algo após o "G: "
            char* alfabeto_fita = linha + 3;
            while (*alfabeto_fita == ' ' || *alfabeto_fita == '\t') {
                alfabeto_fita++; 
            }
            strncpy(mt->alfabeto_fita, alfabeto_fita, sizeof(mt->alfabeto_fita) - 1);
        } else {
            mt->alfabeto_fita[0] = '\0'; // Fita sem alfabeto restrito se a linha for só "G:"
        }
        
        // Se leu o G:, a próxima linha obrigatoriamente deve ser o I:
        if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
        remover_quebra_linha(linha);
    }

    // Agora, a variável 'linha' com certeza contém o I: (Estado Inicial)
    if (strncmp(linha, "I:", 2) == 0) {
        char* inicio = strchr(linha, ':') + 1;
        while (*inicio == ' ' || *inicio == '\t') inicio++;
        sscanf(inicio, "%7s", mt->estado_inicial);
    } else {
        // Se chegou aqui e não é o I:, o arquivo está mal formatado
        return false;
    }
    // =======================================================

    // Lendo F:
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) return false;
    remover_quebra_linha(linha);
    token = strtok(linha + 3, " ");
    while (token != NULL) {
        strcpy(mt->estados_finais[mt->num_finais++], token);
        token = strtok(NULL, " ");
    }

    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        remover_quebra_linha(linha);

        if (strcmp(linha, "---") == 0) {
            break; 
        }

        char origem[8], destino[8];
        int lidos = sscanf(linha, "%7s -> %7s", origem, destino);
        
        if (lidos == 2) {
            char* ptr_regras = strchr(linha, '|');
            if (ptr_regras != NULL) {
                ptr_regras++; 
            } else {
                ptr_regras = strstr(linha, destino) + strlen(destino);
            }

            char* token_regra = strtok(ptr_regras, " \t");
            while (token_regra != NULL) {
                if (strlen(token_regra) >= 4 && token_regra[1] == '/') {
                    int idx = mt->num_transicoes;
                    strcpy(mt->transicoes[idx].origem, origem);
                    mt->transicoes[idx].lido = token_regra[0];
                    strcpy(mt->transicoes[idx].destino, destino);
                    mt->transicoes[idx].escrito = token_regra[2];
                    mt->transicoes[idx].direcao = token_regra[3];
                    mt->num_transicoes++;
                }
                token_regra = strtok(NULL, " \t");
            }
        }
    }
    return true;
}

void ler_casos_teste_mt(FILE* arquivo, MaquinaTuring* mt, bool is_all) {
    char linha[256];
    char fita_saida[10000];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        remover_quebra_linha(linha);
        
        // Repassa a flag para o processador
        if (processar_palavra_mt(mt, linha, fita_saida, is_all)) {
            printf("OK %s\n", fita_saida);
        } else {
            printf("X %s\n", fita_saida); 
        }
    }
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