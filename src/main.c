#include <stdio.h>
#include <stdlib.h>
#include "pontos_recarga.h"

int main() {
    Endereco *enderecos = (Endereco *)malloc(MAX_ENDERECOS * sizeof(Endereco));
    if (enderecos == NULL) {
        printf("Erro ao alocar memoria para enderecos.\n");
        return 1;
    }

  // Lendo o arquivo com os endereços dos pontos de recarga
    FILE *arquivo = fopen("enderecos_recarga.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo geracarga.txt\n");
        free(enderecos);
        return 1;
    }

    char linha[MAX_STR_LEN];
    int num_enderecos = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL && num_enderecos < MAX_ENDERECOS) {
        enderecos[num_enderecos].idend = malloc(MAX_STR_LEN);
        enderecos[num_enderecos].sigla_tipo = malloc(MAX_STR_LEN);
        enderecos[num_enderecos].nome_logra = malloc(MAX_STR_LEN);
        enderecos[num_enderecos].nome_bairr = malloc(MAX_STR_LEN);
        enderecos[num_enderecos].nome_regio = malloc(MAX_STR_LEN);

        sscanf(linha, "%[^;];%ld;%[^;];%[^;];%d;%[^;];%[^;];%d;%lf;%lf",
               enderecos[num_enderecos].idend, &enderecos[num_enderecos].id_logrado,
               enderecos[num_enderecos].sigla_tipo, enderecos[num_enderecos].nome_logra,
               &enderecos[num_enderecos].numero_imo, enderecos[num_enderecos].nome_bairr,
               enderecos[num_enderecos].nome_regio, &enderecos[num_enderecos].cep,
               &enderecos[num_enderecos].x, &enderecos[num_enderecos].y);
        enderecos[num_enderecos].ativo = 1; // Inicialmente, todos os pontos estão ativados
        num_enderecos++;
    }

    fclose(arquivo);

    // Chama a função para processar os comandos do usuário
    processar_comando(enderecos, num_enderecos);

    for (int i = 0; i < num_enderecos; i++) {
        free(enderecos[i].idend);
        free(enderecos[i].sigla_tipo);
        free(enderecos[i].nome_logra);
        free(enderecos[i].nome_bairr);
        free(enderecos[i].nome_regio);
    }

    // Libera a memória alocada para o vetor de endereços
    free(enderecos);
    return 0;
}
