// Para executar: gcc projeto_ip.c -o projeto_ip && ./projeto_ip

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define CHECKERSLEN 8

// Verifica se a maquina tem windows ou linux e define a funcao de Sleep (que pausa a execucao do codigo)
#ifdef _WIN32
void pauseExecution(int seconds) {
    Sleep(milliseconds*1000);
}
#else
void pauseExecution(int seconds) {
    sleep(seconds);
}
#endif

void clear() {
    #ifdef _WIN32
    system("cls");
    #else
    // ANSI Escape Sequence para limpar o console
    printf("\033[2J\033[H");
    #endif
}

void resetCheckers(int ch[CHECKERSLEN][CHECKERSLEN]) {
    // Reseta a matriz checkers 

    int c, j;

    for (c = 0; c < CHECKERSLEN; c++) {
        for (j = 0; j < CHECKERSLEN; j++) {
            ch[c][j] = 0;
        }
    }
}

int verifyValidMove(int cp, int jp, int cp2, int jp2, int ch[CHECKERSLEN][CHECKERSLEN]) {
    int validMove = 0;
    
    if (cp < 0 || cp >= CHECKERSLEN || jp < 0 || jp >= CHECKERSLEN) return validMove;

    
    int cpF = cp - cp2;
    int jpF = jp - jp2;
    
    if (cpF < 0) cpF *= -1;
    if (jpF < 0) jpF *= -1;
    
    int previousSquareC;
    int previousSquareJ;
    
    if (cp > cp2 && jp > jp2) {
        previousSquareC = cp-1;
        previousSquareJ = jp-1;
    } else if (cp > cp2 && jp < jp2) {
        previousSquareC = cp-1;
        previousSquareJ = jp+1;
    } else if (cp < cp2 && jp < jp2) {
        previousSquareC = cp+1;
        previousSquareJ = jp+1;
    } else {
        previousSquareC = cp+1;
        previousSquareJ = jp-1;
    }
    
    
    if (ch[cp][jp] == 2) {
        if (
            (cpF == 1 && jpF == 1 && ch[cp2][jp2] == 0 && cp > cp2) ||
            (cpF == 2 && jpF == 2 && ch[previousSquareC][previousSquareJ] == 1 && ch[cp2][jp2] == 0)  
        ) {
            validMove = 1;   
        }
    } else if (ch[cp][jp] == 1) {
        if (
            (cpF == 1 && jpF == 1 && ch[cp2][jp2] == 0 && cp < cp2) ||
            (cpF == 2 && jpF == 2 && ch[previousSquareC][previousSquareJ] == 2 && ch[cp2][jp2] == 0)  
        ) {
            validMove = 1;   
        }
    }
    
    return validMove;
}

int canMovePiece(int cp, int jp, int ch[CHECKERSLEN][CHECKERSLEN], int whoseTurn) {
    // Verifica se a peca selecionada pode ser movida pelo jogador atual (1 ou 2)

    int c, j;

    // Caso a posicao esteja fora dos indices de CHECKERS ele retorna 0
    if (cp < 0 || cp >= CHECKERSLEN || jp < 0 || jp >= CHECKERSLEN) return 0;

    if (ch[cp][jp] != whoseTurn) return 0;

    // Verifica se ha movimentos validos disponiveis para essa peca
    for (c = 0; c < CHECKERSLEN; c++) {
        for (j = 0; j < CHECKERSLEN; j++) {
            if (verifyValidMove(cp, jp, c, j, ch)) {
                return 1;
            }
        }
    }

    return 0;
}


int checkWinner(int ch[CHECKERSLEN][CHECKERSLEN], int *winner, int whoseTurn) {
    // Verifica se ha Vencedor, e executado a cada jogada
    
    int c, j;
    
    int qtdO = 0;
    int qtdX = 0;

    int xPossible = 0;
    int oPossible = 0;
     
    for (c = 0; c < CHECKERSLEN; c++) {
        for (j = 0; j < CHECKERSLEN; j++) {
            // Verifica a quantidade de pecas e se alguma delas e possivel ser movida
            
            if (ch[c][j] == 1) {
                qtdX++;

                if (canMovePiece(c, j, ch, 1)) {
                    xPossible++;
                }
            } else if (ch[c][j] == 2) {
                qtdO++;

                if (canMovePiece(c, j, ch, 2)) {
                    oPossible++;
                }
            }
        }
    }

    // Teste de possibilidades, Verifica se alguem nao consegue mover as pecas e define um Vencedor
    if (xPossible == 0 && oPossible == 0 ||
        xPossible == 0 && oPossible > 0 && whoseTurn != 2 ||
        xPossible > 0 && oPossible == 0 && whoseTurn != 1
    ) {
        if (qtdO > qtdX) {
            *winner = 2;
        } else if (qtdO < qtdX) {
            *winner = 1;
        } else {
            *winner = 0;
        }

        return 1;
    }

    // Caso seja possivel mover pecas
    if (qtdO > 0 && qtdX == 0) {
        *winner = 2;
        return 1;
    } else if (qtdX > 0 && qtdO == 0) {
        *winner = 1;
        return 1;
    }
    
    
    
    return 0;
}


void displayCheckers(int ch[CHECKERSLEN][CHECKERSLEN]) {
    // Exibe a dama e suas pecas em suas determinadas posicoes.
    
    int c, j;
    
    clear();
    
    printf("    1     2     3     4     5     6     7     8 \n");
    printf("--------------------------------------------------");
    
    for (c = 0; c < CHECKERSLEN; c++) {
        printf("\n%d|", c+1);
        
        for (j = 0; j < CHECKERSLEN; j++) {
            if (ch[c][j] == 0) {
                printf("     |");
            } else if (ch[c][j] == 1) {
                printf("  X  |");
            } else printf("  O  |");
        }
    }
    
    printf("\n--------------------------------------------------\n\n");
}

int capturePiece(int cp, int jp, int cp2, int jp2, int ch[CHECKERSLEN][CHECKERSLEN]) {
    // Faz a captura da peca
    
    int cpF = cp - cp2;
    int jpF = jp - jp2;

    if (abs(cpF) == 2 && abs(jpF) == 2) {
        int midC = (cp + cp2) / 2;
        int midJ = (jp + jp2) / 2;

        ch[midC][midJ] = 0; // Remove a peca capturada
        return 1; // Indica que houve captura
    }

    return 0; // Nao houve captura
}


int checkCaptures(int cp, int jp, int ch[CHECKERSLEN][CHECKERSLEN]) {
    int player = ch[cp][jp];

    // Verifica as quatro possiveis posicoes de captura
    if (cp >= 2 && jp >= 2 && ch[cp - 1][jp - 1] != 0 && ch[cp - 2][jp - 2] == 0)
        return 1;
    
    if (cp >= 2 && jp < CHECKERSLEN - 2 && ch[cp - 1][jp + 1] != 0 && ch[cp - 2][jp + 2] == 0)
        return 1;
    
    if (cp < CHECKERSLEN - 2 && jp < CHECKERSLEN - 2 && ch[cp + 1][jp + 1] != 0 && ch[cp + 2][jp + 2] == 0)
        return 1;
    
    if (cp < CHECKERSLEN - 2 && jp >= 2 && ch[cp + 1][jp - 1] != 0 && ch[cp + 2][jp - 2] == 0)
        return 1;

    return 0;
}

void canCapturePiece(int player, int c2, int j2,  int ch[CHECKERSLEN][CHECKERSLEN]) {
    // Verifica se depois de uma jogada o jogador poderia ter capturado uma peca e nao capturou
    
    int c, j;

    for (c = 0; c < CHECKERSLEN; c++) {
        for (j = 0; j < CHECKERSLEN; j++) {
            if (player == ch[c][j] && (c2 != c || j2 != j)) {
                int v[4][4] = { 
                    // Definicao das 4 posicoes em que poderia comer uma peca. +2 ou -2 é para onde a peca vai (tem q ser igual a 0)
                    // -1 ou +1 é a peca que pode ser capturada tem q ser != 0 && != player
                    {c+1, j+1, c+2, j+2},
                    {c+1, j-1, c+2, j-2},
                    {c-1, j+1, c-2, j+2},
                    {c-1, j-1, c-2, j-2}
                };

                for (int k = 0; k < 4; k++) {
                    int cp1 = v[k][0];
                    int jp1 = v[k][1];
                    int cp2 = v[k][2];
                    int jp2 = v[k][3];

                    if (cp1 >= 0 && cp1 < CHECKERSLEN && jp1 >= 0 && jp1 < CHECKERSLEN &&
                        cp2 >= 0 && cp2 < CHECKERSLEN && jp2 >= 0 && jp2 < CHECKERSLEN) {

                        int opponent = (player == 1) ? 2 : 1;

                        if (ch[cp1][jp1] == opponent && ch[cp2][jp2] == 0) {

                            ch[c][j] = 0;

                            printf("Peca (%d, %d) removida por nao comer.\n", c+1, j+1);
                            pauseExecution(6); // Pausa por 4 segundos

                            return;
                        }
                    }
                }
            }
        }
    }
}


void makeMove(int ch[CHECKERSLEN][CHECKERSLEN], int *whoseTurn) {
    // Faz os movimentos ('jogadas') de cada jogador.
    
    int cp, jp, cp2, jp2;
    
    do {
        // Posicao da peca a ser movida.
        displayCheckers(ch);
        printf("%c, digite a linha e coluna da peca que voce quer mover (separado por espaco): ", *whoseTurn == 1? 'X': 'O');
        scanf("%d %d", &cp, &jp);
        
        cp--; jp--;
        
        if (!canMovePiece(cp, jp, ch, *whoseTurn)) {
            printf("A peca selecionada noo pode ser movida. Escolha outra peca.\n");
            pauseExecution(2); // Pausa por 2 segundos antes de mostrar o tabuleiro novamente
        }
    } while (!canMovePiece(cp, jp, ch, *whoseTurn));
    
    int capturePieceBool;
    
    do {
        do {
            // Posicao de destino da peca.
            
            displayCheckers(ch);
            printf("Voce esta movendo a peca (%d, %d).\n", cp+1, jp+1);
            printf("%c, digite a linha e coluna para onde voce quer mover a peca (separado por espaco): ", *whoseTurn == 1? 'X': 'O');
            scanf("%d %d", &cp2, &jp2);
            
            cp2--; jp2--;
        
        } while (!verifyValidMove(cp, jp, cp2, jp2, ch));
        
        // Verifica se houve captura
        capturePieceBool = capturePiece(cp, jp, cp2, jp2, ch); 

        // Caso nao tenha ocorrido captura, Verifica se havia a possibilida e assopra uma peca aleatoria caso haja mais de duas
        if (!capturePieceBool) canCapturePiece(*whoseTurn, cp2, jp2, ch); 

        
        // Move a peca de lugar.
        ch[cp2][jp2] = ch[cp][jp];
        ch[cp][jp] = 0;      
        

        displayCheckers(ch);
        

        // Troca os indices da posicao da peca, pois, ela foi movida de cp para cp2.
        cp = cp2;
        jp = jp2;
    } while (checkCaptures(cp, jp, ch) && capturePieceBool);
    
    
    *whoseTurn = *whoseTurn == 1? 2: 1; 
}


void fillCheckers(int ch[CHECKERSLEN][CHECKERSLEN]) {
    // Posiciona as pecas de forma inicial.
    
    int c, j;
    
    for (c = 0; c < CHECKERSLEN; c++) {
        if (c < 3) {
            if (c%2==0) j = 0;   
            else j = 1; 
        } else if (c > 4) {
            if (c%2==0) j = 0;   
            else j = 1; 
        }
        
        for (j; j < CHECKERSLEN; j+=2) {
            ch[c][j] = c > 4? 2: 1;
        }
    }

}

void gameloop() {
    // Instancia do checkers (dama), alem do laco de repeticao que mantem o jogo rodando.
    
    int checkers[CHECKERSLEN][CHECKERSLEN] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    
    int winner = 1; // Vencedor anterior.
    
    fillCheckers(checkers);
    int whoseTurn = winner;
    
    while (1) {
        makeMove(checkers, &whoseTurn);
        
        if (checkWinner(checkers, &winner, whoseTurn)) {
            int con = 0;
            
            if (winner == 0) {
                printf("Deu empate!\n");
                winner = 1;
            }
            else printf("O vencedor foi o %c.\n", winner == 1? 'X': 'O');

            printf("Gostaria de jogar mais uma?\n[1] SIM.\n[2] NAO.\n");
            
            do {
                scanf("%d", &con);
            } while (con != 1 && con != 2);
            
            if (con == 2) break;
            else {
                whoseTurn = winner;
                resetCheckers(checkers);
                fillCheckers(checkers);
            }
        }
    }
}





// main sem nenhuma logica/funcionalidade, apenas para chamar gameloop
int main() {
    
    gameloop();

    return 0;
}