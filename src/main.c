#include "pontos_recarga.h"

int main() {
    // Lê o arquivo com as informações dos pontos de recarga
    FILE *arquivo_base = fopen("geracarga.base", "r");
    if (arquivo_base == NULL) {
        printf("Erro ao abrir o arquivo geracarga.base\n");
        return 1;
    }

    // Lê o arquivo com os comandos para o funcionamento do programa
    FILE *arquivo_ev = fopen("geracarga.ev", "r");
    if (arquivo_ev == NULL) {
        printf("Erro ao abrir o arquivo geracarga.ev\n");
        fclose(arquivo_base);
        return 1;
    }

    Endereco *enderecos = (Endereco *)malloc(MAX_ENDERECOS * sizeof(Endereco));
    if (enderecos == NULL) {
        printf("Erro ao alocar memória para endereços.\n");
        fclose(arquivo_base);
        fclose(arquivo_ev);
        return 1;
    }

    char linha[MAX_STR_LEN];
    int num_enderecos = 0;

    // Ler endereços do arquivo geracarga.base
    while (fgets(linha, sizeof(linha), arquivo_base) != NULL && num_enderecos < MAX_ENDERECOS) {
        enderecos[num_enderecos].idend = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[num_enderecos].sigla_tipo = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[num_enderecos].nome_logra = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[num_enderecos].nome_bairr = (char*)malloc(MAX_STR_LEN * sizeof(char));
        enderecos[num_enderecos].nome_regio = (char*)malloc(MAX_STR_LEN * sizeof(char));

        sscanf(linha, "%[^;];%ld;%[^;];%[^;];%d;%[^;];%[^;];%d;%lf;%lf",
               enderecos[num_enderecos].idend, &enderecos[num_enderecos].id_logrado,
               enderecos[num_enderecos].sigla_tipo, enderecos[num_enderecos].nome_logra,
               &enderecos[num_enderecos].numero_imo, enderecos[num_enderecos].nome_bairr,
               enderecos[num_enderecos].nome_regio, &enderecos[num_enderecos].cep,
               &enderecos[num_enderecos].x, &enderecos[num_enderecos].y);
        enderecos[num_enderecos].ativo = 1; // Inicialmente, todos os pontos estão ativados
        num_enderecos++;
    }

    fclose(arquivo_base);

    // Ler comandos do arquivo geracarga.ev
    int num_comandos;
    if (fgets(linha, sizeof(linha), arquivo_ev) != NULL) {
        sscanf(linha, "%d", &num_comandos);
    } else {
        printf("Erro ao ler a quantidade de comandos.\n");
        fclose(arquivo_ev);
        free(enderecos);
        return 1;
    }

    // Processar cada comando do arquivo geracarga.ev
    for (int i = 0; i <= num_comandos; i++) {
        if (fgets(linha, sizeof(linha), arquivo_ev) != NULL) {
            processar_comando(enderecos, num_enderecos, linha);
        } else {
            printf("Erro ao ler comando %d.\n", i + 1);
        }
    }

    fclose(arquivo_ev);

    // Liberação de memória
    for (int i = 0; i < num_enderecos; i++) {
        free(enderecos[i].idend);
        free(enderecos[i].sigla_tipo);
        free(enderecos[i].nome_logra);
        free(enderecos[i].nome_bairr);
        free(enderecos[i].nome_regio);
    }
    free(enderecos);

    return 0;
}
