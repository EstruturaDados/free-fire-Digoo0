#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// Estrutura que representa cada item da mochila
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Funções do sistema
void inserirItem(Item mochila[], int *qtd);
void removerItem(Item mochila[], int *qtd);
void listarItens(Item mochila[], int qtd);
void buscarItem(Item mochila[], int qtd);

int main() {
    Item mochila[MAX_ITENS]; // Vetor que armazena até 10 itens
    int qtd = 0;             // Quantidade atual de itens
    int opcao;

    printf("=== SISTEMA DE INVENTARIO - MOCHILA DE LOOT ===\n");

    do {
        printf("\nMenu:\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpa o buffer do teclado

        switch (opcao) {
            case 1:
                inserirItem(mochila, &qtd);
                break;
            case 2:
                removerItem(mochila, &qtd);
                break;
            case 3:
                listarItens(mochila, qtd);
                break;
            case 4:
                buscarItem(mochila, qtd);
                break;
            case 0:
                printf("\nEncerrando o sistema...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ================== FUNÇÕES ==================

// Cadastra um novo item na mochila
void inserirItem(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("\nA mochila esta cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    printf("\n=== Adicionar Item ===\n");
    printf("Nome: ");
    fgets(mochila[*qtd].nome, 30, stdin);
    mochila[*qtd].nome[strcspn(mochila[*qtd].nome, "\n")] = '\0'; // remove \n

    printf("Tipo (arma, municao, cura, ferramenta...): ");
    fgets(mochila[*qtd].tipo, 20, stdin);
    mochila[*qtd].tipo[strcspn(mochila[*qtd].tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &mochila[*qtd].quantidade);
    getchar();

    (*qtd)++;
    printf("\nItem adicionado com sucesso!\n");
    listarItens(mochila, *qtd);
}

// Remove um item pelo nome
void removerItem(Item mochila[], int *qtd) {
    char nomeRemover[30];
    int encontrado = 0;

    if (*qtd == 0) {
        printf("\nA mochila esta vazia!\n");
        return;
    }

    printf("\n=== Remover Item ===\n");
    printf("Digite o nome do item a remover: ");
    fgets(nomeRemover, 30, stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    for (int i = 0; i < *qtd; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            // Desloca os itens seguintes para preencher o espaço
            for (int j = i; j < *qtd - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*qtd)--;
            printf("\nItem '%s' removido com sucesso!\n", nomeRemover);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nItem nao encontrado.\n");
    }

    listarItens(mochila, *qtd);
}

// Lista todos os itens cadastrados
void listarItens(Item mochila[], int qtd) {
    printf("\n=== Itens na Mochila ===\n");

    if (qtd == 0) {
        printf("Nenhum item na mochila.\n");
        return;
    }

    for (int i = 0; i < qtd; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Busca um item pelo nome
void buscarItem(Item mochila[], int qtd) {
    char nomeBusca[30];
    int encontrado = 0;

    if (qtd == 0) {
        printf("\nA mochila esta vazia!\n");
        return;
    }

    printf("\n=== Buscar Item ===\n");
    printf("Digite o nome do item: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < qtd; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nItem nao encontrado.\n");
    }
}
