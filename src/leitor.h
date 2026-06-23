#ifndef LEITOR_H
#define LEITOR_H

#include <stdio.h>
#include <stdbool.h>
#include "afd.h" 
#include "mt.h"
#include "afn.h"

bool ler_configuracao(FILE* arquivo, AFD* afd);
void ler_casos_teste(FILE* arquivo, AFD* afd);
void ler_casos_teste_mt(FILE* arquivo, MaquinaTuring* mt, bool is_all);
bool ler_configuracao_mt(FILE* arquivo, MaquinaTuring* mt);
bool ler_configuracao_afn(FILE* arquivo, AFN* afn);
void ler_casos_teste_afn(FILE* arquivo, AFN* afn);
#endif