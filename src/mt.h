#ifndef MT_H
#define MT_H

#include <stdbool.h>

#define MAX_ESTADOS_MT 100
#define MAX_FINAIS_MT 100
#define MAX_TRANSICOES_MT 500
#define TAM_NOME 8
#define TAM_FITA 10000

typedef struct {
    char origem[TAM_NOME];
    char lido;
    char destino[TAM_NOME];
    char escrito;
    char direcao;
} TransicaoMT;

typedef struct {
    char estados[MAX_ESTADOS_MT][TAM_NOME];
    int num_estados;

    char estado_inicial[TAM_NOME];
    
    char estados_finais[MAX_FINAIS_MT][TAM_NOME];
    int num_finais;

    char alfabeto_fita[30];
    
    TransicaoMT transicoes[MAX_TRANSICOES_MT];
    int num_transicoes;
} MaquinaTuring;

void inicializar_mt(MaquinaTuring* mt);
bool processar_palavra_mt(MaquinaTuring* mt, const char* palavra, char* fita_saida, bool is_all);
#endif