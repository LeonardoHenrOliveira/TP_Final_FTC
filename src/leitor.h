#ifndef LEITOR_H
#define LEITOR_H

#include <stdio.h>
#include <stdbool.h>
#include "afd.h" 

bool ler_configuracao(FILE* arquivo, AFD* afd);
void ler_casos_teste(FILE* arquivo, AFD* afd);

#endif