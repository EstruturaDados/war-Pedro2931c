// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define MAX_NOME 32
#define MAX_COR  16

// IDs de missão
#define MISSAO_DESTRUIR_VERDE 1
#define MISSAO_CONQUISTAR_3   2

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
Territorio* alocarMapa(size_t n);
void inicializarTerritorios(Territorio *mapa, size_t n);
void liberarMemoria(Territorio *mapa);

void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa, size_t n);
void exibirMissao(int missao);

void faseDeAtaque(Territorio *mapa, size_t n, const char *corJogador);
void simularAtaque(Territorio *mapa, size_t n, int idxAtacante, int idxDefensor);

int  sortearMissao(const char *corJogador);
int  verificarVitoria(const Territorio *mapa, size_t n, const char *corJogador, int missao);

void limparBufferEntrada(void);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main(void) {
    setlocale(LC_ALL, "");
    srand((unsigned)time(NULL));

    Territorio *mapa = alocarMapa(NUM_TERRITORIOS);
    if (!mapa) {
        fprintf(stderr, "Erro: falha ao alocar memoria para o mapa.\n");
        return 1;
    }
    inicializarTerritorios(mapa, NUM_TERRITORIOS);

    const char *corJogador = "Azul";
    int missao = sortearMissao(corJogador);

    int opcao;
    int venceu = 0;
    do {
        system("clear||cls");
        printf("=========== WAR ESTRUTURADO ===========\n");
        printf("Jogador: %s\n", corJogador);
        exibirMapa(mapa, NUM_TERRITORIOS);
        printf("\n");
        exibirMissao(missao);
        printf("\n");
        exibirMenuPrincipal();
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            limparBufferEntrada();
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS, corJogador);
                venceu = verificarVitoria(mapa, NUM_TERRITORIOS, corJogador, missao);
                if (venceu) {
                    printf("\n>>> MISSAO CUMPRIDA! Voce venceu! <<<\n");
                }
                printf("\nPressione ENTER para continuar...");
                limparBufferEntrada(); getchar();
                break;
            case 2:
                if (verificarVitoria(mapa, NUM_TERRITORIOS, corJogador, missao)) {
                    printf("\n>>> MISSAO CUMPRIDA! Voce venceu! <<<\n");
                } else {
                    printf("\nAinda nao cumpriu a missao. Continue lutando!\n");
                }
                printf("\nPressione ENTER para continuar...");
                limparBufferEntrada(); getchar();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
                printf("Pressione ENTER para continuar...");
                limparBufferEntrada(); getchar();
                break;
        }
    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(size_t n) {
    return (Territorio*) calloc(n, sizeof(Territorio));
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio *mapa, size_t n) {
    const char *nomes[NUM_TERRITORIOS] = {
        "Alfa", "Bravo", "Charlie", "Delta", "Echo"
    };
    const char *cores[NUM_TERRITORIOS] = {
        "Verde", "Vermelho", "Verde", "Amarelo", "Azul"
    };
    const int tropasIni[NUM_TERRITORIOS] = { 3, 3, 3, 3, 3 };

    for (size_t i = 0; i < n && i < NUM_TERRITORIOS; ++i) {
        strncpy(mapa[i].nome, nomes[i], MAX_NOME-1);
        mapa[i].nome[MAX_NOME-1] = '\0';
        strncpy(mapa[i].cor, cores[i], MAX_COR-1);
        mapa[i].cor[MAX_COR-1] = '\0';
        mapa[i].tropas = tropasIni[i];
    }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal(void) {
    printf("------------- MENU -------------\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
    printf("--------------------------------\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio *mapa, size_t n) {
    printf("\n------ MAPA ------\n");
    printf("%-3s %-12s %-10s %-6s\n", "#", "Territorio", "Exercito", "Tropas");
    for (size_t i = 0; i < n; ++i) {
        printf("%-3zu %-12s %-10s %-6d\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int missao) {
    printf("Missao atual: ");
    if (missao == MISSAO_DESTRUIR_VERDE) {
        printf("Destruir o exercito Verde (todos os territorios Verdes devem deixar de existir).\n");
    } else if (missao == MISSAO_CONQUISTAR_3) {
        printf("Conquistar 3 territorios (ter 3 territorios sob sua cor).\n");
    } else {
        printf("Desconhecida.\n");
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio *mapa, size_t n, const char *corJogador) {
    (void)corJogador;
    int a, d;
    exibirMapa(mapa, n);
    printf("\nEscolha territorio ATACANTE (1-%zu): ", n);
    if (scanf("%d", &a) != 1) { limparBufferEntrada(); printf("Entrada invalida.\n"); return; }
    printf("Escolha territorio DEFENSOR (1-%zu): ", n);
    if (scanf("%d", &d) != 1) { limparBufferEntrada(); printf("Entrada invalida.\n"); return; }

    a--; d--;
    if (a < 0 || d < 0 || (size_t)a >= n || (size_t)d >= n || a == d) {
        printf("Indices invalidos.\n");
        return;
    }

    if (mapa[a].tropas < 2) {
        printf("O atacante precisa de ao menos 2 tropas para atacar.\n");
        return;
    }

    simularAtaque(mapa, n, a, d);
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio *mapa, size_t n, int idxAtacante, int idxDefensor) {
    (void)n;
    Territorio *A = &mapa[idxAtacante];
    Territorio *D = &mapa[idxDefensor];

    if (D->tropas <= 0) {
        printf("Defensor nao possui tropas. Nada a fazer.\n");
        return;
    }

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\nBATALHA: %s (%s, %d) ATACA %s (%s, %d)\n",
           A->nome, A->cor, A->tropas, D->nome, D->cor, D->tropas);
    printf("Dados -> Atacante: %d | Defensor: %d\n", dadoA, dadoD);

    if (dadoA >= dadoD) {
        D->tropas -= 1;
        printf("Atacante vence o confronto! Tropas do defensor -1 (agora %d)\n", D->tropas);

        if (D->tropas <= 0) {
            strncpy(D->cor, A->cor, MAX_COR-1);
            D->cor[MAX_COR-1] = '\0';
            D->tropas = 1;
            A->tropas -= 1;
            printf(">>> TERRITORIO CONQUISTADO! %s agora domina %s.\n", A->cor, D->nome);
            printf("Uma tropa foi movida do atacante (agora %d) para o novo territorio (1).\n", A->tropas);
        }
    } else {
        A->tropas -= 1;
        printf("Defensor resiste! Tropas do atacante -1 (agora %d)\n", A->tropas);
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao(const char *corJogador) {
    int m;
    do {
        m = (rand() % 2) ? MISSAO_DESTRUIR_VERDE : MISSAO_CONQUISTAR_3;
    } while (m == MISSAO_DESTRUIR_VERDE && strcmp(corJogador, "Verde") == 0);
    return m;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio *mapa, size_t n, const char *corJogador, int missao) {
    if (missao == MISSAO_DESTRUIR_VERDE) {
        for (size_t i = 0; i < n; ++i) {
            if (strcmp(mapa[i].cor, "Verde") == 0 && mapa[i].tropas > 0) {
                return 0;
            }
        }
        return 1;
    } else if (missao == MISSAO_CONQUISTAR_3) {
        int count = 0;
        for (size_t i = 0; i < n; ++i) {
            if (strcmp(mapa[i].cor, corJogador) == 0) count++;
        }
        return (count >= 3);
    }
    return 0;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
