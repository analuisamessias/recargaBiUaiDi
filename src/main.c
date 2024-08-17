#include "pontos_recarga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s -b <arquivo_base> -e <arquivo_eventos>\n", argv[0]);
        return 1;
    }

    char *arquivo_base_nome = NULL;
    char *arquivo_ev_nome = NULL;

    // Processar os argumentos
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-b") == 0) {
            arquivo_base_nome = argv[i + 1];
        } else if (strcmp(argv[i], "-e") == 0) {
            arquivo_ev_nome = argv[i + 1];
        } else {
            printf("Parâmetro desconhecido: %s\n", argv[i]);
            printf("Uso: %s -b <arquivo_base> -e <arquivo_eventos>\n", argv[0]);
            return 1;
        }
    }

    // Verificar se ambos os arquivos foram especificados
    if (arquivo_base_nome == NULL || arquivo_ev_nome == NULL) {
        printf("Parâmetros -b e -e são obrigatórios.\n");
        return 1;
    }

    // Carregar endereços do arquivo base
    Endereco *enderecos = (Endereco *)malloc(MAX_ENDERECOS * sizeof(Endereco));
    if (enderecos == NULL) {
        printf("Erro ao alocar memória para endereços.\n");
        return 1;
    }

    int num_enderecos = 0;
    carregar_enderecos(arquivo_base_nome, enderecos, &num_enderecos);

    // Abrir o arquivo de eventos
    FILE *arquivo_ev = fopen(arquivo_ev_nome, "r");
    if (arquivo_ev == NULL) {
        printf("Erro ao abrir o arquivo %s\n", arquivo_ev_nome);
        free(enderecos);
        return 1;
    }

    // Ler a quantidade de comandos
    char linha[MAX_STR_LEN];

    // Ignora a primeira linha (o número de comandos)
    fgets(linha, sizeof(linha), arquivo_ev);

    // Lê o restante das linhas até o final do arquivo
    while (fgets(linha, sizeof(linha), arquivo_ev) != NULL) {
        processar_comando(enderecos, num_enderecos, linha);
    }

    // Fechar o arquivo de eventos
    fclose(arquivo_ev);

    // Liberar memória alocada
    for (int i = 0; i <= num_enderecos; i++) {
        free(enderecos[i].idend);
        free(enderecos[i].sigla_tipo);
        free(enderecos[i].nome_logra);
        free(enderecos[i].nome_bairr);
        free(enderecos[i].nome_regio);
    }
    free(enderecos);

    return 0;
}