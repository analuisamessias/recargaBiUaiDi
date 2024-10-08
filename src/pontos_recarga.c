#include "pontos_recarga.h"

// Função para calcular distância entre dois pontos
double distancia_euclidiana(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Função para comparar as distâncias calculadas e ver qual é maior e qual menor
int compara_distancia(const void *a, const void *b) {
    Distancia *da = (Distancia *)a;
    Distancia *db = (Distancia *)b;
    return (da->distancia > db->distancia) - (da->distancia < db->distancia);
}

// Função para carregar os endereços do arquivo de base
void carregar_enderecos(const char* nome_arquivo, Endereco* enderecos, int* num_enderecos) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    char linha[MAX_STR_LEN];
    *num_enderecos = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL && *num_enderecos < MAX_ENDERECOS) {
        enderecos[*num_enderecos].idend = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[*num_enderecos].sigla_tipo = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[*num_enderecos].nome_logra = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[*num_enderecos].nome_bairr = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[*num_enderecos].nome_regio = (char*)malloc(MAX_STR_LEN * sizeof(char));

        sscanf(linha, "%[^;];%ld;%[^;];%[^;];%d;%[^;];%[^;];%d;%lf;%lf",
               enderecos[*num_enderecos].idend, &enderecos[*num_enderecos].id_logrado,
               enderecos[*num_enderecos].sigla_tipo, enderecos[*num_enderecos].nome_logra,
               &enderecos[*num_enderecos].numero_imo, enderecos[*num_enderecos].nome_bairr,
               enderecos[*num_enderecos].nome_regio, &enderecos[*num_enderecos].cep,
               &enderecos[*num_enderecos].x, &enderecos[*num_enderecos].y);
        enderecos[*num_enderecos].ativo = 1;
        (*num_enderecos)++;
    }

    fclose(arquivo);
}


// Função para processar os comandos do arquivo geracarga.ev
void processar_comando(Endereco *enderecos, int num_enderecos, char *comando) {
    double x, y;
    int n, i;
    
    printf("%s", comando);
    
    // Comando "C" retorna as n estaçõs de recarga mais próximas
    if (comando[0] == 'C') {
        sscanf(comando, "C %lf %lf %d", &x, &y, &n);
        Distancia *distancias = (Distancia *)malloc(num_enderecos * sizeof(Distancia));

        for (i = 0; i < num_enderecos; i++) {
            if (enderecos[i].ativo) {
                distancias[i].distancia = distancia_euclidiana(x, y, enderecos[i].x, enderecos[i].y);
                distancias[i].indice = i;
            } else {
                distancias[i].distancia = __DBL_MAX__; // Valor máximo para distâncias de endereços inativos
            }
        }

        qsort(distancias, num_enderecos, sizeof(Distancia), compara_distancia);

        for (i = 0; i < n && i < num_enderecos; i++) {
            int idx = distancias[i].indice;
            if (distancias[i].distancia != __DBL_MAX__) { // Ignorar endereços inativos
                printf("%s %s, %d, %s, %s, %d (%.3f)\n",
                       enderecos[idx].sigla_tipo, enderecos[idx].nome_logra, enderecos[idx].numero_imo,
                       enderecos[idx].nome_bairr, enderecos[idx].nome_regio, enderecos[idx].cep,
                       distancias[i].distancia);
            }
        }

        free(distancias);
    } 
    // Comando "A" ativa um ponto de recarga
    else if (comando[0] == 'A') {
        char id[MAX_STR_LEN];
        sscanf(comando, "A %s", id);
        for (i = 0; i < num_enderecos; i++) {
            if (strcmp(enderecos[i].idend, id) == 0) {
                if (enderecos[i].ativo) {
                    printf("Ponto de recarga %s já estava ativo.\n", id);
                } else {
                    enderecos[i].ativo = 1;
                    printf("Ponto de recarga %s ativado.\n", id);
                }
                break;
            }
        }
    } 
    // Comando "D" desativa um ponto de recarga
    else if (comando[0] == 'D') {
        char id[MAX_STR_LEN];
        sscanf(comando, "D %s", id);
        for (i = 0; i < num_enderecos; i++) {
            if (strcmp(enderecos[i].idend, id) == 0) {
                if (!enderecos[i].ativo) {
                    printf("Ponto de recarga %s já estava desativado.\n", id);
                } else {
                    enderecos[i].ativo = 0;
                    printf("Ponto de recarga %s desativado.\n", id);
                }
                break;
            }
        }
    } else {
        printf("Comando inválido!\n");
    }
}