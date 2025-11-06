#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// Função que gera uma peça nova
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Função para iniciar a fila
void iniciarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

// Função para iniciar a pilha
void iniciarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->qtd == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Adiciona uma peça na fila (enqueue)
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->itens[f->fim] = nova;
    f->qtd++;
}

// Remove uma peça da fila (dequeue)
Peca desenfileirar(Fila *f) {
    Peca removida = {' ', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

// Adiciona uma peça na pilha (push)
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = nova;
}

// Remove uma peça da pilha (pop)
Peca desempilhar(Pilha *p) {
    Peca removida = {' ', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// Mostra o estado atual da fila e pilha
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n\n");

    printf("Fila de peças\t");
    if (filaVazia(f))
        printf("(vazia)");
    else {
        int i, pos;
        for (i = 0; i < f->qtd; i++) {
            pos = (f->inicio + i) % TAM_FILA;
            printf("[%c %d] ", f->itens[pos].nome, f->itens[pos].id);
        }
    }

    printf("\nPilha de reserva\t(Topo -> base): ");
    if (pilhaVazia(p))
        printf("(vazia)");
    else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }

    printf("\n");
}

// Troca a peça da frente da fila com o topo da pilha
void trocarUma(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nNão é possível trocar: fila ou pilha vazia.\n");
        return;
    }

    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\nAção: troca realizada entre a peça da frente da fila e o topo da pilha.\n");
}

// Troca as três primeiras peças da fila com as três da pilha
void trocarTres(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("\nNão é possível fazer a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int posFila = (f->inicio + i) % TAM_FILA;
        Peca temp = f->itens[posFila];
        f->itens[posFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\nAção: troca realizada entre as 3 primeiras peças da fila e as 3 da pilha.\n");
}

int main() {
    Fila fila;
    Pilha pilha;
    int opcao, contadorID = 0;

    srand(time(NULL));
    iniciarFila(&fila);
    iniciarPilha(&pilha);

    // Preenche a fila inicialmente
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorID++));
    }

    do {
        printf("\n--- MENU ---\n");
        printf("1. Jogar peça da frente da fila\n");
        printf("2. Enviar peça da fila para a pilha de reserva\n");
        printf("3. Usar peça da pilha de reserva\n");
        printf("4. Trocar peça da frente da fila com o topo da pilha\n");
        printf("5. Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("6. Visualizar estado atual\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (filaVazia(&fila))
                printf("\nFila vazia!\n");
            else {
                Peca usada = desenfileirar(&fila);
                printf("\nAção: peça [%c %d] jogada.\n", usada.nome, usada.id);
                enfileirar(&fila, gerarPeca(contadorID++));
            }
        }

        else if (opcao == 2) {
            if (filaVazia(&fila))
                printf("\nFila vazia!\n");
            else if (pilhaCheia(&pilha))
                printf("\nPilha cheia! Não é possível reservar mais peças.\n");
            else {
                Peca movida = desenfileirar(&fila);
                empilhar(&pilha, movida);
                printf("\nAção: peça [%c %d] enviada para a pilha.\n", movida.nome, movida.id);
                enfileirar(&fila, gerarPeca(contadorID++));
            }
        }

        else if (opcao == 3) {
            if (pilhaVazia(&pilha))
                printf("\nPilha vazia! Nenhuma peça reservada para usar.\n");
            else {
                Peca usada = desempilhar(&pilha);
                printf("\nAção: peça [%c %d] usada da pilha.\n", usada.nome, usada.id);
            }
        }

        else if (opcao == 4) {
            trocarUma(&fila, &pilha);
        }

        else if (opcao == 5) {
            trocarTres(&fila, &pilha);
        }

        else if (opcao == 6) {
            mostrarEstado(&fila, &pilha);
        }

        else if (opcao == 0) {
            printf("\nEncerrando o programa...\n");
        }

        else {
            printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}