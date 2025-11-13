#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define STRLEN_NAME 30
#define STRLEN_TYPE 20

// -------------------------------------------------
// Struct que representa um componente da torre
// -------------------------------------------------
typedef struct {
    char nome[STRLEN_NAME];
    char tipo[STRLEN_TYPE];
    int prioridade; // 1 (mais baixa) .. 10 (mais alta)
} Componente;

// -------------------------------------------------
// Funções utilitárias
// -------------------------------------------------
void limparNovaLinha(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

void mostrarComponentes(Componente vet[], int n) {
    printf("\n--- Componentes (total: %d) ---\n", n);
    if (n == 0) { printf("Nenhum componente cadastrado.\n"); return; }
    for (int i = 0; i < n; i++) {
        printf("%2d) Nome: %-28s | Tipo: %-10s | Prioridade: %2d\n",
               i + 1, vet[i].nome, vet[i].tipo, vet[i].prioridade);
    }
}

// -------------------------------------------------
// Bubble sort por NOME (string) - mede comparações e tempo
// -------------------------------------------------
void bubbleSortNome(Componente vet[], int n, long *comparacoes, double *tempoSeg) {
    *comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(vet[j].nome, vet[j + 1].nome) > 0) {
                Componente tmp = vet[j];
                vet[j] = vet[j + 1];
                vet[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }

    clock_t fim = clock();
    *tempoSeg = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// -------------------------------------------------
// Insertion sort por TIPO (string) - mede comparações e tempo
// -------------------------------------------------
void insertionSortTipo(Componente vet[], int n, long *comparacoes, double *tempoSeg) {
    *comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 1; i < n; i++) {
        Componente chave = vet[i];
        int j = i - 1;
        // Comparações com base em tipo; se igual, manter ordem estável por nome
        while (j >= 0) {
            (*comparacoes)++;
            int cmp = strcmp(vet[j].tipo, chave.tipo);
            if (cmp > 0 || (cmp == 0 && strcmp(vet[j].nome, chave.nome) > 0)) {
                vet[j + 1] = vet[j];
                j--;
            } else break;
        }
        vet[j + 1] = chave;
    }

    clock_t fim = clock();
    *tempoSeg = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// -------------------------------------------------
// Selection sort por PRIORIDADE (int) - mede comparações e tempo
// Ordena do maior para o menor (prioridade alta primeiro)
// -------------------------------------------------
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes, double *tempoSeg) {
    *comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < n - 1; i++) {
        int idxMax = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (vet[j].prioridade > vet[idxMax].prioridade) idxMax = j;
            else if (vet[j].prioridade == vet[idxMax].prioridade) {
                // critério secundário: ordenar por nome ascendente
                (*comparacoes)++;
                if (strcmp(vet[j].nome, vet[idxMax].nome) < 0) idxMax = j;
            }
        }
        if (idxMax != i) {
            Componente tmp = vet[i];
            vet[i] = vet[idxMax];
            vet[idxMax] = tmp;
        }
    }

    clock_t fim = clock();
    *tempoSeg = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// -------------------------------------------------
// Busca binária por NOME (após ordenação por nome)
// Retorna índice (0..n-1) ou -1 se não encontrado.
// Também conta comparações.
// -------------------------------------------------
int buscaBinariaPorNome(Componente vet[], int n, const char chave[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(vet[meio].nome, chave);
        if (cmp == 0) return meio;
        if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// -------------------------------------------------
// Leitura / cadastro de componentes (até MAX_COMPONENTES)
// -------------------------------------------------
int cadastrarComponentes(Componente vet[]) {
    int n = 0;
    char buf[100];
    printf("\nQuantos componentes deseja cadastrar? (max %d): ", MAX_COMPONENTES);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    n = atoi(buf);
    if (n < 0) n = 0;
    if (n > MAX_COMPONENTES) n = MAX_COMPONENTES;

    for (int i = 0; i < n; i++) {
        printf("\n--- Componente %d ---\n", i + 1);
        printf("Nome: ");
        fgets(vet[i].nome, STRLEN_NAME, stdin);
        limparNovaLinha(vet[i].nome);

        printf("Tipo (controle/suporte/propulsao/...): ");
        fgets(vet[i].tipo, STRLEN_TYPE, stdin);
        limparNovaLinha(vet[i].tipo);

        printf("Prioridade (1 a 10): ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) vet[i].prioridade = 1;
        else vet[i].prioridade = atoi(buf);
        if (vet[i].prioridade < 1) vet[i].prioridade = 1;
        if (vet[i].prioridade > 10) vet[i].prioridade = 10;
    }
    return n;
}

// -------------------------------------------------
// Menu principal do módulo de montagem da torre
// -------------------------------------------------
int main() {
    Componente componentes[MAX_COMPONENTES];
    int nComponentes = 0;
    int opcao;
    int ordenadoPor = 0; // 0 = none, 1 = nome, 2 = tipo, 3 = prioridade

    printf("=== Módulo: Montagem da Torre de Resgate ===\n");

    // Cadastro inicial
    nComponentes = cadastrarComponentes(componentes);
    mostrarComponentes(componentes, nComponentes);

    do {
        printf("\n--- Menu ---\n");
        printf("1 - Bubble sort por NOME\n");
        printf("2 - Insertion sort por TIPO\n");
        printf("3 - Selection sort por PRIORIDADE\n");
        printf("4 - Busca binaria por NOME (requer estar ordenado por NOME)\n");
        printf("5 - Mostrar componentes\n");
        printf("6 - Re-cadastrar (substitui lista atual)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) { // tratamento simples para EOF/erro
            while (getchar() != '\n'); opcao = 0;
        }
        getchar(); // limpar newline

        if (opcao == 1) {
            long comps; double tempo;
            bubbleSortNome(componentes, nComponentes, &comps, &tempo);
            ordenadoPor = 1;
            printf("\n== Resultado: Bubble sort por NOME ==\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n", comps, tempo);
            mostrarComponentes(componentes, nComponentes);
        } else if (opcao == 2) {
            long comps; double tempo;
            insertionSortTipo(componentes, nComponentes, &comps, &tempo);
            ordenadoPor = 2;
            printf("\n== Resultado: Insertion sort por TIPO ==\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n", comps, tempo);
            mostrarComponentes(componentes, nComponentes);
        } else if (opcao == 3) {
            long comps; double tempo;
            selectionSortPrioridade(componentes, nComponentes, &comps, &tempo);
            ordenadoPor = 3;
            printf("\n== Resultado: Selection sort por PRIORIDADE (desc) ==\n");
            printf("Comparacoes: %ld | Tempo: %.6f s\n", comps, tempo);
            mostrarComponentes(componentes, nComponentes);
        } else if (opcao == 4) {
            if (ordenadoPor != 1) {
                printf("\nAviso: a busca binária exige que os componentes estejam ordenados por NOME.\n");
                printf("Deseja aplicar agora o Bubble sort por NOME e prosseguir? (s/n): ");
                char resp = getchar(); getchar(); // ler e consumir newline
                if (resp == 's' || resp == 'S') {
                    long comps; double tempo;
                    bubbleSortNome(componentes, nComponentes, &comps, &tempo);
                    ordenadoPor = 1;
                    printf("Ordenado por NOME: Comparacoes=%ld | Tempo=%.6f s\n", comps, tempo);
                } else {
                    printf("Busca binaria cancelada.\n");
                    continue;
                }
            }
            char chave[STRLEN_NAME];
            printf("\nDigite o NOME do componente chave a buscar: ");
            fgets(chave, STRLEN_NAME, stdin); limparNovaLinha(chave);
            long compsBin;
            int idx = buscaBinariaPorNome(componentes, nComponentes, chave, &compsBin);
            if (idx >= 0) {
                printf("\nComponente encontrado (indice %d) | Comparacoes (binaria) = %ld\n", idx, compsBin);
                printf(" -> Nome: %s | Tipo: %s | Prioridade: %d\n",
                       componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
            } else {
                printf("\nComponente nao encontrado. (Comparacoes=%ld)\n", compsBin);
            }
        } else if (opcao == 5) {
            mostrarComponentes(componentes, nComponentes);
        } else if (opcao == 6) {
            printf("\nRe-cadastro selecionado. Informe nova lista:\n");
            nComponentes = cadastrarComponentes(componentes);
            ordenadoPor = 0;
            mostrarComponentes(componentes, nComponentes);
        } else if (opcao == 0) {
            printf("\nFinalizando módulo.\n");
        } else {
            printf("\nOpcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}
