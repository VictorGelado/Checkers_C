#include <stdio.h>


#define CHECKERSLEN 8

void clear() {
    // Função ANSI dedicada a limpeza do console.
    
    //printf("\033[2J\033[H");
}

int checkWinner(int ch[CHECKERSLEN][CHECKERSLEN], int *winner) {
    int c, j;
    
    int qtdO = 0;
    int qtdX = 0;
     
    for (c = 0; c < CHECKERSLEN; c++) {
        for (j = 0; c < CHECKERSLEN; j++) {
            if (ch[c][j] == 1) qtdX++;
            else if (ch[c][j] == 2) qtdO++;
        }
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
                printf("  .  |");
            } else if (ch[c][j] == 1) {
                printf("  X  |");
            } else printf("  O  |");
        }
    }
    
    printf("\n--------------------------------------------------\n\n");
}

int capturePiece(int cp, int jp, int cp2, int jp2, int ch[CHECKERSLEN][CHECKERSLEN]) {
    // Verifica se alguma peca foi comida e retira ela.
    
    int cpF = cp - cp2;
    int jpF = jp - jp2;
    
    if (cpF < 0) cpF *= -1;
    if (jpF < 0) jpF *= -1;
    
    if (cpF == 2 && jpF == 2) {
        
        if (cp > cp2 && jp > jp2) {
            ch[cp - 1][jp - 1] = 0;
        } else if (cp > cp2 && jp < jp2) {
            ch[cp - 1][jp + 1] = 0;
        } else if (cp < cp2 && jp < jp2) {
            ch[cp + 1][jp + 1] = 0;
        } else {
            ch[cp + 1][jp - 1] = 0;
        }
        
        return 1; 
    }
    
    return 0; 
    
    
    
}

int checkCaptures(int cp, int jp, int ch[CHECKERSLEN][CHECKERSLEN]){
    // Verifica se existe como o jogador fazer mais uma jogada
    
    // c > 0
    int idx1[2] = {cp-1, jp-1};  // j > 0
    int idx2[2] = {cp-1, jp+1}; // j < 7
    
    // c < 7 
    int idx3[2] = {cp+1, jp+1}; // j < 7
    int idx4[2] = {cp+1, jp-1}; // j > 0
    
    //Verificar se existe uma peca ocupando a posicao cp+2 e jp+2
    
    //cp+1 == 0
    // Verificando se a posicao e menor que o primeiro indice (0)
    if (jp - 1 >= 0 && cp - 1 >= 0) {
        if ((ch[idx1[0]][idx1[1]] != ch[cp][jp] && ch[idx1[0]][idx1[1]] != 0)) {
            return 1;
        }
    }
    
    if (jp + 1 < CHECKERSLEN && cp - 1 >= 0) {
        if ((ch[idx2[0]][idx2[1]] != ch[cp][jp] && ch[idx2[0]][idx2[1]] != 0)) {
            return 1;
        }
    }
    
    if (jp + 1 < CHECKERSLEN && cp + 1 < CHECKERSLEN) {
        if ((ch[idx3[0]][idx3[1]] != ch[cp][jp] && ch[idx3[0]][idx3[1]] != 0)) {
            return 1;
        }
    }
    
    if (jp - 1 >= 0 && cp + 1 < CHECKERSLEN) {
        if ((ch[idx4[0]][idx4[1]] != ch[cp][jp] && ch[idx4[0]][idx4[1]] != 0)) {
            return 1;
        }
    }
    
    return 0;
}

int verifyValidMove(int cp, int jp, int cp2, int jp2, int ch[CHECKERSLEN][CHECKERSLEN]) {
    int validMove = 0;
    
    if (cp < 0 || cp > CHECKERSLEN || jp < 0 || jp > CHECKERSLEN) return validMove;

    
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

void makeMove(int ch[CHECKERSLEN][CHECKERSLEN], int *whoseTurn) {
    // Faz os movimentos ('jogadas') de cada jogador.
    
    int cp, jp, cp2, jp2;
    
    do {
        // Posicao da peca a ser movida.
        
        displayCheckers(ch);
        printf("Digite a linha e coluna da peça que você quer mover (separado por espaço): ");
        scanf("%d %d", &cp, &jp);
        
        cp--; jp--;
    } while (!(ch[cp][jp] == *whoseTurn));
    
    int capturePieceBool;
    
    do {
        do {
            // Posicao de destino da peca.
            
            displayCheckers(ch);
            printf("%c, digite a linha e coluna para onde você quer mover a peça (separado por espaço): ", *whoseTurn == 1? 'X': 'O');
            scanf("%d %d", &cp2, &jp2);
            
            cp2--; jp2--;
        
        } while (!verifyValidMove(cp, jp, cp2, jp2, ch));
        
        // Move a peca de lugar.
        ch[cp2][jp2] = ch[cp][jp];
        ch[cp][jp] = 0;
        displayCheckers(ch);
        
        // Troca os indices da posicao da peca, pois, ela foi movida de cp para cp2
        capturePieceBool = capturePiece(cp, jp, cp2, jp2, ch);
        
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
    
    ch[4][2] = 2;
    ch[5][1] = 0;

    /*for (c = 5; c < CHECKERSLEN; c++) {
        if (c%2==0) j = 1;   
        else j = 0; 
        
        for (j; j < CHECKERSLEN; j+=2) {
            ch[c][j] = 2;
        }
    }*/
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
    }
    
    /*if (!checkWinner(checkers, &winner)) {
            printf("\n");
        }*/
}
















// main sem nenhuma logica/funcionalidade, apenas para chamar gameloop
int main() {
    
    gameloop();

    return 0;
}