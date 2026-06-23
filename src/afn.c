#include <stdio.h>
#include <string.h>
#include "afn.h"

void inicializar_afn(AFN* afn) {
    afn->num_estados = 0;
    afn->num_iniciais = 0;
    afn->num_finais = 0;
    afn->num_transicoes = 0;
    afn->dicionario[0] = '\0';
}

void adicionar_estado_afn(AFN* afn, const char* estado) {
    if (afn->num_estados < MAX_ESTADOS) {
        strcpy(afn->estados[afn->num_estados++], estado);
    }
}

void adicionar_inicial_afn(AFN* afn, const char* estado) {
    if (afn->num_iniciais < MAX_ESTADOS) {
        strcpy(afn->estados_iniciais[afn->num_iniciais++], estado);
    }
}

void adicionar_final_afn(AFN* afn, const char* estado) {
    if (afn->num_finais < MAX_FINAIS) {
        strcpy(afn->estados_finais[afn->num_finais++], estado);
    }
}

void adicionar_transicao_afn(AFN* afn, const char* origem, char simbolo, const char* destino) {
    if (afn->num_transicoes < MAX_TRANSICOES) {
        strcpy(afn->transicoes[afn->num_transicoes].origem, origem);
        afn->transicoes[afn->num_transicoes].simbolo = simbolo;
        strcpy(afn->transicoes[afn->num_transicoes].destino, destino);
        afn->num_transicoes++;
    }
}

void definir_dicionario_afn(AFN* afn, const char* dicionario) {
    strncpy(afn->dicionario, dicionario, MAX_DICIONARIO - 1);
    afn->dicionario[MAX_DICIONARIO - 1] = '\0';
}

bool explorar_caminho(AFN* afn, const char* estado_atual, const char* palavra, int indice) {
    
    if (palavra[indice] == '\0') {
        for (int i = 0; i < afn->num_finais; i++) {
            if (strcmp(estado_atual, afn->estados_finais[i]) == 0) {
                return true; 
            }
        }
    }

    char simbolo_atual = palavra[indice];

    for (int i = 0; i < afn->num_transicoes; i++) {
        if (strcmp(estado_atual, afn->transicoes[i].origem) == 0) {
            
            if (simbolo_atual != '\0' && afn->transicoes[i].simbolo == simbolo_atual) {
                if (explorar_caminho(afn, afn->transicoes[i].destino, palavra, indice + 1)) {
                    return true;
                }
            }
            
            if (afn->transicoes[i].simbolo == '\\') {
                if (explorar_caminho(afn, afn->transicoes[i].destino, palavra, indice)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool processar_palavra_afn(AFN* afn, const char* palavra) {
    for (int i = 0; i < afn->num_iniciais; i++) {
        if (explorar_caminho(afn, afn->estados_iniciais[i], palavra, 0)) {
            return true;
        }
    }
    return false;
}