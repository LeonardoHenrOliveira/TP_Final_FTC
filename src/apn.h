#ifndef APN_H
#define APN_H

#include <stdbool.h>

#define MAX_ESTADOS_APN 100
#define MAX_FINAIS_APN 100
#define MAX_TRANSICOES_APN 500
#define TAM_NOME_APN 8 
#define MAX_ALFABETO_PILHA 30
#define MAX_EMPILHA 20
#define MAX_PILHA_EXEC 10000
#define LIMITE_COMPUTACOES 100000

typedef struct {
    char origem[TAM_NOME_APN];
    char entrada;
    char desempilha;
    char destino[TAM_NOME_APN];
    char empilha[MAX_EMPILHA];
} TransicaoAPN;

typedef struct {
    char estados[MAX_ESTADOS_APN][TAM_NOME_APN];
    int num_estados;

    char alfabeto_pilha[MAX_ALFABETO_PILHA];

    char estado_inicial[TAM_NOME_APN];

    char estados_finais[MAX_FINAIS_APN][TAM_NOME_APN];
    int num_finais;
    
    TransicaoAPN transicoes[MAX_TRANSICOES_APN];
    int num_transicoes;
} APN;

void inicializar_apn(APN* apn);
void adicionar_estado_apn(APN* apn, const char* estado);
void definir_alfabeto_pilha_apn(APN* apn, const char* alfabeto);
void definir_inicial_apn(APN* apn, const char* estado);
void adicionar_final_apn(APN* apn, const char* estado);
void adicionar_transicao_apn(APN* apn, const char* origem, char entrada, char desempilha, const char* destino, const char* empilha);
bool processar_palavra_apn(APN* apn, const char* palavra, bool exigir_estado_final);

#endif
