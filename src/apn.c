#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "apn.h"

void inicializar_apn(APN* apn) {
    apn->num_estados = 0;
    apn->num_finais = 0;
    apn->num_transicoes = 0;
    apn->estado_inicial[0] = '\0';
    apn->alfabeto_pilha[0] = '\0';
}

void adicionar_estado_apn(APN* apn, const char* estado) {
    if (apn->num_estados < MAX_ESTADOS_APN) {
        strcpy(apn->estados[apn->num_estados], estado);
        apn->num_estados++;
    }
}

void definir_alfabeto_pilha_apn(APN* apn, const char* alfabeto) {
    strncpy(apn->alfabeto_pilha, alfabeto, MAX_ALFABETO_PILHA - 1);
    apn->alfabeto_pilha[MAX_ALFABETO_PILHA - 1] = '\0';
}

void definir_inicial_apn(APN* apn, const char* estado) {
    strcpy(apn->estado_inicial, estado);
}

void adicionar_final_apn(APN* apn, const char* estado) {
    if (apn->num_finais < MAX_FINAIS_APN) {
        strcpy(apn->estados_finais[apn->num_finais], estado);
        apn->num_finais++;
    }
}

void adicionar_transicao_apn(APN* apn, const char* origem, char entrada, char desempilha, const char* destino, const char* empilha) {
    if (apn->num_transicoes < MAX_TRANSICOES_APN) {
        strcpy(apn->transicoes[apn->num_transicoes].origem, origem);
        apn->transicoes[apn->num_transicoes].entrada = entrada;
        apn->transicoes[apn->num_transicoes].desempilha = desempilha;
        strcpy(apn->transicoes[apn->num_transicoes].destino, destino);
        strncpy(apn->transicoes[apn->num_transicoes].empilha, empilha, MAX_EMPILHA - 1);
        apn->transicoes[apn->num_transicoes].empilha[MAX_EMPILHA - 1] = '\0';
        apn->num_transicoes++;
    }
}

typedef struct Config {
    char estado_atual[TAM_NOME_APN];
    int indice;
    char pilha[MAX_PILHA_EXEC];
    int top; 
    struct Config* next;
} Config;

typedef struct {
    char estado_atual[TAM_NOME_APN];
    int indice;
    char pilha[MAX_PILHA_EXEC];
    int top;
} VisitedConfig;

#define HASH_SIZE 10007
typedef struct HashNode {
    VisitedConfig config;
    struct HashNode* next;
} HashNode;


unsigned long hash_config(const VisitedConfig* c) {
    unsigned long hash = 5381;
    for (int i = 0; c->estado_atual[i] != '\0'; i++) hash = ((hash << 5) + hash) + c->estado_atual[i];
    hash = ((hash << 5) + hash) + c->indice;
    for (int i = 0; i <= c->top; i++) hash = ((hash << 5) + hash) + c->pilha[i];
    return hash;
}

bool is_visited(HashNode** hash_table, const VisitedConfig* config) {
    unsigned long h = hash_config(config) % HASH_SIZE;
    HashNode* node = hash_table[h];
    while (node != NULL) {
        if (strcmp(node->config.estado_atual, config->estado_atual) == 0 &&
            node->config.indice == config->indice &&
            node->config.top == config->top) {
            
            bool equal_stack = true;
            for (int i = 0; i <= config->top; i++) {
                if (node->config.pilha[i] != config->pilha[i]) {
                    equal_stack = false;
                    break;
                }
            }
            if (equal_stack) return true;
        }
        node = node->next;
    }
    return false;
}

void mark_visited(HashNode** hash_table, const VisitedConfig* config) {
    unsigned long h = hash_config(config) % HASH_SIZE;
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    new_node->config = *config;
    new_node->next = hash_table[h];
    hash_table[h] = new_node;
}

void free_hash_table(HashNode** hash_table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* node = hash_table[i];
        while (node != NULL) {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
}

bool processar_palavra_apn(APN* apn, const char* palavra, bool exigir_estado_final) {
    if (apn->estado_inicial[0] == '\0') {
        printf("O automato nao possui estado inicial\n");
        return false;
    }

    Config* fronteira = (Config*)malloc(sizeof(Config));
    strcpy(fronteira->estado_atual, apn->estado_inicial);
    fronteira->indice = 0;
    fronteira->top = -1;
    fronteira->next = NULL;

    HashNode** visitados = (HashNode**)calloc(HASH_SIZE, sizeof(HashNode*));
    int configuracoes_testadas = 0;
    int len_palavra = strlen(palavra);
    bool aceito = false;

    while (fronteira != NULL) {
        // Pop do topo da fronteira
        Config* atual = fronteira;
        fronteira = fronteira->next;

        configuracoes_testadas++;
        if (configuracoes_testadas > LIMITE_COMPUTACOES) {
            free(atual);
            break; // Evita loop infinito
        }

        VisitedConfig vc;
        strcpy(vc.estado_atual, atual->estado_atual);
        vc.indice = atual->indice;
        vc.top = atual->top;
        for (int i = 0; i <= vc.top; i++) vc.pilha[i] = atual->pilha[i];

        if (is_visited(visitados, &vc)) {
            free(atual);
            continue;
        }
        mark_visited(visitados, &vc);

        bool palavra_lida = (atual->indice == len_palavra);
        bool pilha_vazia = (atual->top == -1);
        bool estado_final_ok = !exigir_estado_final;

        if (!estado_final_ok) {
            for (int i = 0; i < apn->num_finais; i++) {
                if (strcmp(atual->estado_atual, apn->estados_finais[i]) == 0) {
                    estado_final_ok = true;
                    break;
                }
            }
        }

        if (palavra_lida && pilha_vazia && estado_final_ok) {
            aceito = true;
            free(atual);
            break;
        }

        // Gera proximas configuracoes usando as transicoes aplicaveis
        for (int i = apn->num_transicoes - 1; i >= 0; i--) {
            if (strcmp(apn->transicoes[i].origem, atual->estado_atual) != 0) {
                continue;
            }

            TransicaoAPN t = apn->transicoes[i];

            // Verifica simbolo de entrada
            int novo_indice = atual->indice;
            if (t.entrada != '\\') {
                if (atual->indice >= len_palavra) continue;
                if (palavra[atual->indice] != t.entrada) continue;
                novo_indice++;
            }

            // Verifica e executa desempilhamento
            int novo_top = atual->top;
            if (t.desempilha != '\\') {
                if (novo_top == -1) continue;
                if (atual->pilha[novo_top] != t.desempilha) continue;
                novo_top--;
            }

            // Cria nova pilha copiando da atual
            char nova_pilha[MAX_PILHA_EXEC];
            for (int k = 0; k <= novo_top; k++) nova_pilha[k] = atual->pilha[k];

            // Executa empilhamento (ordem reversa ja que a string da definicao eh inserida char a char)
            bool overflow = false;
            if (strcmp(t.empilha, "\\") != 0) {
                int len_empilha = strlen(t.empilha);
                for (int j = len_empilha - 1; j >= 0; j--) {
                    novo_top++;
                    if (novo_top >= MAX_PILHA_EXEC) {
                        overflow = true;
                        break;
                    }
                    nova_pilha[novo_top] = t.empilha[j];
                }
            }
            if (overflow) continue;

            // Insere nova configuracao na fronteira
            Config* novo_config = (Config*)malloc(sizeof(Config));
            strcpy(novo_config->estado_atual, t.destino);
            novo_config->indice = novo_indice;
            novo_config->top = novo_top;
            for (int k = 0; k <= novo_top; k++) novo_config->pilha[k] = nova_pilha[k];
            
            novo_config->next = fronteira;
            fronteira = novo_config;
        }

        free(atual);
    }

    // Limpa memoria
    while (fronteira != NULL) {
        Config* temp = fronteira;
        fronteira = fronteira->next;
        free(temp);
    }
    free_hash_table(visitados);
    free(visitados);

    return aceito;
}
