#ifndef AFD_H
#define AFD_H

#include <stdbool.h>

#define MAX_ESTADOS 100
#define MAX_FINAIS 100
#define MAX_TRANSICOES 500
#define TAM_NOME 8 

typedef struct {
    char origem[TAM_NOME];
    char simbolo;
    char destino[TAM_NOME];
} Transicao;

typedef struct {
    char estados[MAX_ESTADOS][TAM_NOME];
    int num_estados;

    char estado_inicial[TAM_NOME];

    char estados_finais[MAX_FINAIS][TAM_NOME];
    int num_finais;
    
    Transicao transicoes[MAX_TRANSICOES];
    int num_transicoes;
} AFD;

void inicializar_afd(AFD* afd);
void adicionar_estado(AFD* afd, const char* estado);
void definir_inicial(AFD* afd, const char* estado);
void adicionar_final(AFD* afd, const char* estado);
void adicionar_transicao(AFD* afd, const char* origem, char simbolo, const char* destino);
bool processar_palavra(AFD* afd, const char* palavra);

#endif