#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

// ================== STRUCTS ==================

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ================== PROTÓTIPOS ==================
// --- Funções do vetor ---
void inserirItemVetor(Item mochila[], int *qtd);
void removerItemVetor(Item mochila[], int *qtd);
void listarItensVetor(Item mochila[], int qtd);
int buscarSequencialVetor(Item mochila[], int qtd, char nome[], int *comparacoes);
void ordenarVetor(Item mochila[], int qtd);
int buscarBinariaVetor(Item mochila[], int qtd, char nome[], int *comparacoes);

// --- Funções da lista encadeada ---
void inserirItemLista(No **inicio);
void removerItemLista(No **inicio);
void listarItensLista(No *inicio);
int buscarSequencialLista(No *inicio, char nome[], int *comparacoes);

// --- Funções auxiliares ---
void menuVetor();
void menuLista();

// ================== MAIN ==================
int main() {
    int opcao;
    srand(time(NULL));

    do {
        printf("\n=== SISTEMA DE INVENTARIO AVANCADO ===\n");
        printf("1 - Mochila com Vetor\n");
        printf("2 - Mochila com Lista Encadeada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: menuVetor(); break;
            case 2: menuLista(); break;
            case 0: printf("\nEncerrando o sistema...\n"); break;
            default: printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// ================== FUNÇÕES DO VETOR ==================

void inserirItemVetor(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("\nMochila cheia!\n");
        return;
    }

    printf("\n=== Inserir Item (Vetor) ===\n");
    printf("Nome: ");
    fgets(mochila[*qtd].nome, 30, stdin);
    mochila[*qtd].nome[strcspn(mochila[*qtd].nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(mochila[*qtd].tipo, 20, stdin);
    mochila[*qtd].tipo[strcspn(mochila[*qtd].tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &mochila[*qtd].quantidade);
    getchar();

    (*qtd)++;
    printf("Item inserido com sucesso!\n");
}

void removerItemVetor(Item mochila[], int *qtd) {
    char nome[30];
    printf("\n=== Remover Item (Vetor) ===\n");
    printf("Nome do item: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < *qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < *qtd - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*qtd)--;
            printf("Item removido com sucesso!\n");
            return;
        }
    }
    printf("Item nao encontrado.\n");
}

void listarItensVetor(Item mochila[], int qtd) {
    printf("\n=== Itens (Vetor) ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

int buscarSequencialVetor(Item mochila[], int qtd, char nome[], int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < qtd; i++) {
        (*comparacoes)++;
        if (strcmp(mochila[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

void ordenarVetor(Item mochila[], int qtd) {
    Item temp;
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (strcmp(mochila[i].nome, mochila[j].nome) > 0) {
                temp = mochila[i];
                mochila[i] = mochila[j];
                mochila[j] = temp;
            }
        }
    }
}

int buscarBinariaVetor(Item mochila[], int qtd, char nome[], int *comparacoes) {
    int inicio = 0, fim = qtd - 1, meio;
    *comparacoes = 0;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(mochila[meio].nome, nome);

        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

void menuVetor() {
    Item mochila[MAX_ITENS];
    int qtd = 0, opcao, comparacoes;
    char nomeBusca[30];

    do {
        printf("\n--- MENU (Vetor) ---\n");
        printf("1 - Inserir\n2 - Remover\n3 - Listar\n4 - Buscar Sequencial\n5 - Ordenar + Buscar Binaria\n0 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: inserirItemVetor(mochila, &qtd); break;
            case 2: removerItemVetor(mochila, &qtd); break;
            case 3: listarItensVetor(mochila, qtd); break;
            case 4:
                printf("Nome do item: ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                int pos = buscarSequencialVetor(mochila, qtd, nomeBusca, &comparacoes);
                if (pos != -1)
                    printf("Item encontrado em %d comparacoes.\n", comparacoes);
                else
                    printf("Item nao encontrado (%d comparacoes).\n", comparacoes);
                break;
            case 5:
                ordenarVetor(mochila, qtd);
                printf("Itens ordenados!\n");
                printf("Nome do item: ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                pos = buscarBinariaVetor(mochila, qtd, nomeBusca, &comparacoes);
                if (pos != -1)
                    printf("Item encontrado (Busca Binaria) em %d comparacoes.\n", comparacoes);
                else
                    printf("Item nao encontrado (%d comparacoes).\n", comparacoes);
                break;
        }
    } while (opcao != 0);
}

// ================== FUNÇÕES DA LISTA ENCADEADA ==================

void inserirItemLista(No **inicio) {
    No *novo = (No *)malloc(sizeof(No));
    if (!novo) return;

    printf("\n=== Inserir Item (Lista) ===\n");
    printf("Nome: ");
    fgets(novo->dados.nome, 30, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo->dados.tipo, 20, stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    getchar();

    novo->proximo = *inicio;
    *inicio = novo;
    printf("Item inserido!\n");
}

void removerItemLista(No **inicio) {
    char nome[30];
    printf("\n=== Remover Item (Lista) ===\n");
    printf("Nome do item: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = *inicio, *anterior = NULL;
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Item nao encontrado.\n");
        return;
    }

    if (anterior == NULL)
        *inicio = atual->proximo;
    else
        anterior->proximo = atual->proximo;

    free(atual);
    printf("Item removido!\n");
}

void listarItensLista(No *inicio) {
    printf("\n=== Itens (Lista Encadeada) ===\n");
    if (inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    No *aux = inicio;
    while (aux != NULL) {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               aux->dados.nome, aux->dados.tipo, aux->dados.quantidade);
        aux = aux->proximo;
    }
}

int buscarSequencialLista(No *inicio, char nome[], int *comparacoes) {
    *comparacoes = 0;
    No *aux = inicio;
    while (aux != NULL) {
        (*comparacoes)++;
        if (strcmp(aux->dados.nome, nome) == 0)
            return 1;
        aux = aux->proximo;
    }
    return 0;
}

void menuLista() {
    No *inicio = NULL;
    int opcao, comparacoes;
    char nomeBusca[30];

    do {
        printf("\n--- MENU (Lista Encadeada) ---\n");
        printf("1 - Inserir\n2 - Remover\n3 - Listar\n4 - Buscar Sequencial\n0 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: inserirItemLista(&inicio); break;
            case 2: removerItemLista(&inicio); break;
            case 3: listarItensLista(inicio); break;
            case 4:
                printf("Nome do item: ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                if (buscarSequencialLista(inicio, nomeBusca, &comparacoes))
                    printf("Item encontrado em %d comparacoes.\n", comparacoes);
                else
                    printf("Item nao encontrado (%d comparacoes).\n", comparacoes);
                break;
        }
    } while (opcao != 0);

    // Libera memória no final
    No *temp;
    while (inicio != NULL) {
        temp = inicio;
        inicio = inicio->proximo;
        free(temp);
    }
}
