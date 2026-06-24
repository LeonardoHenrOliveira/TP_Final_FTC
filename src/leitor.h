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
#include "apn.h"
bool ler_configuracao_apn(FILE* arquivo, APN* apn);
void ler_casos_teste_apn(FILE* arquivo, APN* apn, bool exigir_estado_final);

#include "apd.h"
bool ler_configuracao_apd(FILE* arquivo, APD* apd);
void ler_casos_teste_apd(FILE* arquivo, APD* apd);

#endif