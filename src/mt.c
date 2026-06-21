#include <stdio.h>
#include <string.h>
#include "mt.h"

void inicializar_mt(MaquinaTuring* mt) {
    mt->num_estados = 0;
    mt->num_finais = 0;
    mt->num_transicoes = 0;
}

bool processar_palavra_mt(MaquinaTuring* mt, const char* palavra, char* fita_saida, bool is_all) {
    char fita[TAM_FITA];
    memset(fita, '_', TAM_FITA);
    
    int cabecote = 1;
    fita[0] = '<';
    int len = strlen(palavra);
    for (int i = 0; i < len; i++) {
        fita[i + 1] = palavra[i];
    }
    if (is_all) {
        fita[len + 1] = '>'; 
    }
    fita[TAM_FITA - 1] = '\0';

    char estado_atual[TAM_NOME];
    strcpy(estado_atual, mt->estado_inicial);

    bool em_execucao = true;
    while (em_execucao) {
        em_execucao = false;
        char simbolo_lido = fita[cabecote];

        for (int i = 0; i < mt->num_transicoes; i++) {
            if (strcmp(estado_atual, mt->transicoes[i].origem) == 0 && 
                mt->transicoes[i].lido == simbolo_lido) {
                
                // Aplica transição: a/bd
                fita[cabecote] = mt->transicoes[i].escrito;
                strcpy(estado_atual, mt->transicoes[i].destino);
                
                if (mt->transicoes[i].direcao == 'D') cabecote++;
                else if (mt->transicoes[i].direcao == 'E') cabecote--;
                
                em_execucao = true;
                break;
            }
        }
    }

    int ultimo_valido = TAM_FITA - 2;
    while (ultimo_valido > 0 && fita[ultimo_valido] == '_') {
        ultimo_valido--;
    }
    strncpy(fita_saida, fita, ultimo_valido + 1);
    fita_saida[ultimo_valido + 1] = '\0';

    for (int i = 0; i < mt->num_finais; i++) {
        if (strcmp(estado_atual, mt->estados_finais[i]) == 0) {
            return true;
        }
    }
    return false;
}