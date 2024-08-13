#include "pontos_recarga.h"

// Calcula a distancia entre dois pontos
double distancia_euclidiana(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Compara duas distâncias para encontrar menor caminho
int compara_distancia(const void *a, const void *b) {
    Distancia *da = (Distancia *)a;
    Distancia *db = (Distancia *)b;
    return (da->distancia > db->distancia) - (da->distancia < db->distancia);
}

void processar_comando(Endereco *enderecos, int num_enderecos) {
    char comando[MAX_STR_LEN];
    double x, y;
    int n, i;

    while (1) {
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            break;
        }

      // Comando "C" retorna as n estações de recarga mais próximas de um ponto (x, y)
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
        } else if (comando[0] == 'A') {
            char id[MAX_STR_LEN];
            sscanf(comando, "A %s", id);
            for (i = 0; i < num_enderecos; i++) {
                if (strcmp(enderecos[i].idend, id) == 0) {
                    if (enderecos[i].ativo) {
                        printf("Ponto de recarga %s ja estava ativo.\n", id);
                    } else {
                        enderecos[i].ativo = 1;
                        printf("Ponto de recarga %s ativado.\n", id);
                    }
                    break;
                }
            }
        } else if (comando[0] == 'D') {
            char id[MAX_STR_LEN];
            sscanf(comando, "D %s", id);
            for (i = 0; i < num_enderecos; i++) {
                if (strcmp(enderecos[i].idend, id) == 0) {
                    if (!enderecos[i].ativo) {
                        printf("Ponto de recarga %s ja estava desativado.\n", id);
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
}
