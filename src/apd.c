#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "apd.h"

void inicializar_apd(APD* apd) {
    apd->num_estados = 0;
    apd->num_finais = 0;
    apd->num_transicoes = 0;
    apd->estado_inicial[0] = '\0';
    apd->alfabeto_pilha[0] = '\0';
}

void adicionar_estado_apd(APD* apd, const char* estado) {
    if (apd->num_estados < MAX_ESTADOS_APD) {
        strcpy(apd->estados[apd->num_estados], estado);
        apd->num_estados++;
    }
}

void definir_alfabeto_pilha_apd(APD* apd, const char* alfabeto) {
    strncpy(apd->alfabeto_pilha, alfabeto, MAX_ALFABETO_PILHA - 1);
    apd->alfabeto_pilha[MAX_ALFABETO_PILHA - 1] = '\0';
}

void definir_inicial_apd(APD* apd, const char* estado) {
    strcpy(apd->estado_inicial, estado);
}

void adicionar_final_apd(APD* apd, const char* estado) {
    if (apd->num_finais < MAX_FINAIS_APD) {
        strcpy(apd->estados_finais[apd->num_finais], estado);
        apd->num_finais++;
    }
}

void adicionar_transicao_apd(APD* apd, const char* origem, char entrada, char desempilha, const char* destino, const char* empilha) {
    if (apd->num_transicoes < MAX_TRANSICOES_APD) {
        strcpy(apd->transicoes[apd->num_transicoes].origem, origem);
        apd->transicoes[apd->num_transicoes].entrada = entrada;
        apd->transicoes[apd->num_transicoes].desempilha = desempilha;
        strcpy(apd->transicoes[apd->num_transicoes].destino, destino);
        strncpy(apd->transicoes[apd->num_transicoes].empilha, empilha, MAX_EMPILHA - 1);
        apd->transicoes[apd->num_transicoes].empilha[MAX_EMPILHA - 1] = '\0';
        apd->num_transicoes++;
    }
}

bool processar_palavra_apd(APD* apd, const char* palavra) {
    char estado_atual[TAM_NOME_APD];
    strcpy(estado_atual, apd->estado_inicial);

    char pilha[MAX_PILHA_EXEC];
    int top = -1;

    int i = 0;
    int len_palavra = strlen(palavra);

    while (true) {
        char simbolo_entrada = (i < len_palavra) ? palavra[i] : '\\';
        char simbolo_topo = (top >= 0) ? pilha[top] : '\\';
        
        int transicao_idx = -1;
        bool consumiu = false;

        // Try exact match with current input symbol
        if (simbolo_entrada != '\\') {
            for (int j = 0; j < apd->num_transicoes; j++) {
                if (strcmp(estado_atual, apd->transicoes[j].origem) == 0 &&
                    apd->transicoes[j].entrada == simbolo_entrada &&
                    apd->transicoes[j].desempilha == simbolo_topo) {
                    transicao_idx = j;
                    consumiu = true;
                    break;
                }
            }
        }

        // Try epsilon transition if no exact match
        if (transicao_idx == -1) {
            for (int j = 0; j < apd->num_transicoes; j++) {
                if (strcmp(estado_atual, apd->transicoes[j].origem) == 0 &&
                    apd->transicoes[j].entrada == '\\' &&
                    apd->transicoes[j].desempilha == simbolo_topo) {
                    transicao_idx = j;
                    break;
                }
            }
        }

        if (transicao_idx == -1) {
            break; // No valid transition
        }

        if (consumiu) {
            i++;
        }

        strcpy(estado_atual, apd->transicoes[transicao_idx].destino);
        const char* empilha = apd->transicoes[transicao_idx].empilha;

        if (simbolo_topo != '\\') {
            top--; // desempilha
        }

        if (strcmp(empilha, "\\") != 0) {
            // Empilha the string in reverse so the first char becomes the new top
            int len_empilha = strlen(empilha);
            for (int j = len_empilha - 1; j >= 0; j--) {
                top++;
                pilha[top] = empilha[j];
            }
        }
    }

    if (i == len_palavra && top == -1) {
        bool estado_final_ok = false;
        
        // Se a especificação não tiver estados finais (F: vazio), consideramos ok? 
        // Vamos forçar a checagem se houver estados finais definidos.
        if (apd->num_finais == 0) {
            estado_final_ok = true; 
        } else {
            for (int k = 0; k < apd->num_finais; k++) {
                if (strcmp(estado_atual, apd->estados_finais[k]) == 0) {
                    estado_final_ok = true;
                    break;
                }
            }
        }
        
        return estado_final_ok;
    } else {
        return false;
    }
}
