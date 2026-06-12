#include <stdio.h>
#include <string.h>
#include "afd.h"

void inicializar_afd(AFD* afd) {
    afd->num_estados = 0;
    afd->num_finais = 0;
    afd->num_transicoes = 0;
    afd->estado_inicial[0] = '\0';
}

void adicionar_estado(AFD* afd, const char* estado) {
    if (afd->num_estados < MAX_ESTADOS) {
        strcpy(afd->estados[afd->num_estados], estado);
        afd->num_estados++;
    }
}

void definir_inicial(AFD* afd, const char* estado) {
    strcpy(afd->estado_inicial, estado);
}

void adicionar_final(AFD* afd, const char* estado) {
    if (afd->num_finais < MAX_FINAIS) {
        strcpy(afd->estados_finais[afd->num_finais], estado);
        afd->num_finais++;
    }
}

void adicionar_transicao(AFD* afd, const char* origem, char simbolo, const char* destino) {
    if (afd->num_transicoes < MAX_TRANSICOES) {
        strcpy(afd->transicoes[afd->num_transicoes].origem, origem);
        afd->transicoes[afd->num_transicoes].simbolo = simbolo;
        strcpy(afd->transicoes[afd->num_transicoes].destino, destino);
        afd->num_transicoes++;
    }
}

bool processar_palavra(AFD* afd, const char* palavra) {
    char estado_atual[TAM_NOME];
    strcpy(estado_atual, afd->estado_inicial);

    for (int i = 0; palavra[i] != '\0'; i++) {
        char simbolo = palavra[i];
        bool encontrou_transicao = false;

        for (int j = 0; j < afd->num_transicoes; j++) {
            if (strcmp(estado_atual, afd->transicoes[j].origem) == 0 && 
                afd->transicoes[j].simbolo == simbolo) {
                
                strcpy(estado_atual, afd->transicoes[j].destino);
                encontrou_transicao = true;
                break;
            }
        }

        if (!encontrou_transicao) {
            return false;
        }
    }

    for (int i = 0; i < afd->num_finais; i++) {
        if (strcmp(estado_atual, afd->estados_finais[i]) == 0) {
            return true;
        }
    }

    return false;
}