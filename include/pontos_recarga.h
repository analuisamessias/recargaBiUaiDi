#ifndef PONTOS_RECARGA_H
#define PONTOS_RECARGA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ENDERECOS 1000
#define MAX_STR_LEN 512

typedef struct {
    char* idend;
    long id_logrado;
    char* sigla_tipo;
    char* nome_logra;
    int numero_imo;
    char* nome_bairr;
    char* nome_regio;
    int cep;
    double x;
    double y;
    int ativo; // 0 para desativado, 1 para ativo
} Endereco;

typedef struct {
    double distancia;
    int indice;
} Distancia;

double distancia_euclidiana(double x1, double y1, double x2, double y2);

int compara_distancia(const void *a, const void *b);

void processar_comando(Endereco *enderecos, int num_enderecos, char *comando);

#endif // PONTOS_RECARGA_H
