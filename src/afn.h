#ifndef AFN_H
#define AFN_H

#include <stdbool.h>

#define MAX_ESTADOS 100
#define MAX_FINAIS 100
#define MAX_TRANSICOES 500
#define TAM_NOME 8 
#define MAX_DICIONARIO 30

typedef struct {
    char origem[TAM_NOME];
    char simbolo;
    char destino[TAM_NOME];
} TransicaoAFN;

typedef struct {
    char estados[MAX_ESTADOS][TAM_NOME];
    int num_estados;

    char estados_iniciais[MAX_ESTADOS][TAM_NOME];
    int num_iniciais;

    char estados_finais[MAX_FINAIS][TAM_NOME];
    int num_finais;

    char dicionario[MAX_DICIONARIO];
    
    TransicaoAFN transicoes[MAX_TRANSICOES];
    int num_transicoes;
} AFN;

void inicializar_afn(AFN* afn);
void adicionar_estado_afn(AFN* afn, const char* estado);
void adicionar_inicial_afn(AFN* afn, const char* estado);
void adicionar_final_afn(AFN* afn, const char* estado);
void adicionar_transicao_afn(AFN* afn, const char* origem, char simbolo, const char* destino);
void definir_dicionario_afn(AFN* afn, const char* dicionario);

bool explorar_caminho(AFN* afn, const char* estado_atual, const char* palavra, int indice);
bool processar_palavra_afn(AFN* afn, const char* palavra);

#endif