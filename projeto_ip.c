#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define CHECKERSLEN 8

#ifdef _WIN32
void pauseExecution(int milliseconds) {
    Sleep(milliseconds);
}
#else
void pauseExecution(int seconds) {
    sleep(seconds);
}
#endif


#define CHECKERSLEN 8

void clear() {
    // Função ANSI dedicada a limpeza do console.
    
    printf("\033[2J\033[H");
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
    // Verifica se a peça selecionada pode ser movida pelo jogador atual (1 ou 2)

    int c, j;


    if (cp < 0 || cp >= CHECKERSLEN || jp < 0 || jp >= CHECKERSLEN) return 0;

    if (ch[cp][jp] != whoseTurn) return 0;

    // Verifica se há movimentos válidos disponíveis para essa peça
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
    int c, j;
    
    int qtdO = 0;
    int qtdX = 0;

    int xPossible = 0;
    int oPossible = 0;
     
    for (c = 0; c < CHECKERSLEN; c++) {
        for (j = 0; j < CHECKERSLEN; j++) {
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
    // Exibe a dama e suas peças em suas determinadas posicoes.
    
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
    int cpF = cp - cp2;
    int jpF = jp - jp2;

    if (abs(cpF) == 2 && abs(jpF) == 2) {
        int midC = (cp + cp2) / 2;
        int midJ = (jp + jp2) / 2;

        ch[midC][midJ] = 0; // Remove a peça capturada
        return 1; // Indica que houve captura
    }

    return 0; // Não houve captura
}


int checkCaptures(int cp, int jp, int ch[CHECKERSLEN][CHECKERSLEN]) {
    int player = ch[cp][jp];
    int forwardDir = (player == 1) ? 1 : -1; // Direção para frente do jogador

    // Verifica as quatro possíveis posições de captura
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
    int c, j;

    for (c = 0; c < CHECKERSLEN; c++) {
        for (j = 0; j < CHECKERSLEN; j++) {
            if (player == ch[c][j] && (c2 != c || j2 != j)) {
                int v[4][4] = {
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
        printf("%c, digite a linha e coluna da peça que você quer mover (separado por espaço): ", *whoseTurn == 1? 'X': 'O');
        scanf("%d %d", &cp, &jp);
        
        cp--; jp--;
        
        if (!canMovePiece(cp, jp, ch, *whoseTurn)) {
            printf("A peça selecionada não pode ser movida. Escolha outra peça.\n");
            pauseExecution(3); // Pausa por 3 segundos antes de mostrar o tabuleiro novamente
        }
    } while (!canMovePiece(cp, jp, ch, *whoseTurn));
    
    int capturePieceBool;
    
    do {
        do {
            // Posicao de destino da peca.
            
            displayCheckers(ch);
            printf("Voce esta movendo a peca (%d, %d).\n", cp+1, jp+1);
            printf("%c, digite a linha e coluna para onde você quer mover a peça (separado por espaço): ", *whoseTurn == 1? 'X': 'O');
            scanf("%d %d", &cp2, &jp2);
            
            cp2--; jp2--;
        
        } while (!verifyValidMove(cp, jp, cp2, jp2, ch));

        
        // Move a peca de lugar.
        ch[cp2][jp2] = ch[cp][jp];
        ch[cp][jp] = 0;      

        
        capturePieceBool = capturePiece(cp, jp, cp2, jp2, ch);

        if (!capturePieceBool) canCapturePiece(*whoseTurn, cp2, jp2, ch);

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
    // Instancia do checkers (dama), alem do laço de repeticao que mantem o jogo rodando.
    
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
    int newWinner = 0;
    
    fillCheckers(checkers);
    int whoseTurn = winner;
    
    while (1) {
        makeMove(checkers, &whoseTurn);
        
        if (checkWinner(checkers, &winner, whoseTurn)) {
            int con = 0;
            
            if (winner == 0) {
                printf("Deu empate!");
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