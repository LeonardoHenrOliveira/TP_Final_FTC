#ifndef APD_H
#define APD_H

#include <stdbool.h>

#define MAX_ESTADOS_APD 100
#define MAX_FINAIS_APD 100
#define MAX_TRANSICOES_APD 500
#define TAM_NOME_APD 8 
#define MAX_ALFABETO_PILHA 30
#define MAX_EMPILHA 20
#define MAX_PILHA_EXEC 10000

typedef struct {
    char origem[TAM_NOME_APD];
    char entrada;
    char desempilha;
    char destino[TAM_NOME_APD];
    char empilha[MAX_EMPILHA];
} TransicaoAPD;

typedef struct {
    char estados[MAX_ESTADOS_APD][TAM_NOME_APD];
    int num_estados;

    char alfabeto_pilha[MAX_ALFABETO_PILHA];

    char estado_inicial[TAM_NOME_APD];

    char estados_finais[MAX_FINAIS_APD][TAM_NOME_APD];
    int num_finais;
    
    TransicaoAPD transicoes[MAX_TRANSICOES_APD];
    int num_transicoes;
} APD;

void inicializar_apd(APD* apd);
void adicionar_estado_apd(APD* apd, const char* estado);
void definir_alfabeto_pilha_apd(APD* apd, const char* alfabeto);
void definir_inicial_apd(APD* apd, const char* estado);
void adicionar_final_apd(APD* apd, const char* estado);
void adicionar_transicao_apd(APD* apd, const char* origem, char entrada, char desempilha, const char* destino, const char* empilha);
bool processar_palavra_apd(APD* apd, const char* palavra);

#endif
