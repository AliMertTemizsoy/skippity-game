#include <stdbool.h>
#include <stdio.h>

#include "board.h"

/*
Örnek oyun ekranı:

         Turn
player1   <   player2
A B C D E     A B C D E  
0 0 0 0 0     0 0 0 0 0

■ 1 2 3 4 5 6 7 8 9 0 ■ 
1 B A B B D E A C E C 1
2 C C C B B B A B A A 2
3 C E D D D D C A C C 3
4 C E E C B B A E C E 4
5 D A A E     C B D D 5
6 D B D D     A E B C 6
7 E A C C E B B A A D 7
8 C A D C B B D D A B 8
9 A C B E B A E E E A 9
0 B A E E D E D E D D 0
■ 1 2 3 4 5 6 7 8 9 0 ■

select your piece (row, col): 5 , 3         
where to move (row, col): 5 , 5
*/

/*
oyunun başlangıcında board'u oluşturan fonksiyon
*/

void initBoard(Board * board) {
    int size;

    clear_screen();

    printf("\nEnter the size of the board: ");
    scanf("%d", &size);
    board->size = size;
    /* kullanıcıdan alınan size'ı board struct'ına atar. */
        
    createBoard(board);
    /* board'u oluşturan fonksiyonu çağırır. */
}

/*
Oyun tahtasını oluşturan fonksiyon.
*/
void createBoard(Board *board) {
    
    int i,j;
    int skipperChoice;
    int size = board->size;
    int maxEachSkipper = size*size/5; /* her bir skipper için max taş sayısı */
    int middle = size/2; /* orta nokta */
    int maxCounter[5] = {0,0,0,0,0}; /* her bir skipper için max taş sayısı kontrolü yapmak için dizi oluşturduk */

    /*
    Oyun tahtası için dinamik yer açar.
    */

    board->board = (char **) malloc(size * sizeof(char *));
    
    for (i = 0; i < size; i++){
        board->board[i] = (char *)malloc(size * sizeof(char));
    }

    /*
    skippers dizisi oluşturduk
    */
    char skippers[5]= {'A','B','C','D','E'};

    srand(time(NULL));

    /*
    kullanıcıdan girilen size'a göre gerekli kontrolleri yapıyoruz ve taşlar max taş sayısına ulaşana kadar board'a rastgele yerleştiriyoruz.
    son olarak tahtanın ortasını boş bırakıyoruz.
    */
    if(size%2 == 0 && size*size%5 == 0){
        for (i = 0; i < size; i++){
            for (j = 0; j < size; j++){
                    skipperChoice = rand() % 5;
                    while(maxCounter[skipperChoice] == maxEachSkipper){
                        skipperChoice = rand() % 5;
                    }
                board->board[i][j] = skippers[skipperChoice];
                maxCounter[skipperChoice]++;
            }
        }

        board->board[middle][middle] = ' ';
        board->board[middle][middle-1] = ' ';
        board->board[middle-1][middle] = ' ';
        board->board[middle-1][middle-1] = ' ';

    } else if(size%2 == 0 && size*size%5 != 0){
        for (i = 0; i < size; i++){
            for (j = 0; j < size; j++){
                skipperChoice = rand() % 5;
                while(maxCounter[skipperChoice] == maxEachSkipper+1){
                    skipperChoice = rand() % 5;
                }
                board->board[i][j] = skippers[skipperChoice];
                maxCounter[skipperChoice]++;
            }
            board->board[middle][middle] = ' ';
            board->board[middle][middle-1] = ' ';
            board->board[middle-1][middle] = ' ';
            board->board[middle-1][middle-1] = ' ';
        }

    } else if(size%2 == 1 && size*size %5 == 0){
        for (i = 0; i < size; i++){
            for (j = 0; j < size; j++){
                skipperChoice = rand() % 5;
                while(maxCounter[skipperChoice] == maxEachSkipper){
                    skipperChoice = rand() % 5;
                }
                board->board[i][j] = skippers[skipperChoice];
                maxCounter[skipperChoice]++;
            }
            board->board[middle][middle] = ' ';
            board->board[middle][middle-1] = ' ';
            board->board[middle-1][middle] = ' ';
            board->board[middle-1][middle-1] = ' ';
        }

    } else {
            for (i = 0; i < size; i++){
                for (j = 0; j < size; j++){
                    skipperChoice = rand() % 5;
                    while(maxCounter[skipperChoice] == maxEachSkipper+1){
                        skipperChoice = rand() % 5;
                    }
                    board->board[i][j] = skippers[skipperChoice];
                    maxCounter[skipperChoice]++;
                }
                board->board[middle][middle] = ' ';
            }     
    }

    /* oyuncu sırasını 1'den başlatır */

    board->turn = 1;

    /* her iki oyuncunun skorlarını sıfırlar */
    
    for(i = 0; i < 5; i++) {
        board->player1.skipperCounter[i] = 0;
        board->player2.skipperCounter[i] = 0;
    }
}

/*
oyuncu vs oyuncu mu yoksa oyuncu vs bilgisayar mı kontrolü yapar.
ona göre oyun ekranını ekrana yazdırır.
*/

void displayBoard(Board *board, bool isCompGame) {
    int i;
    char turnChar;

    clear_screen();

    if(board->turn == 1) { /* player 1 */
        turnChar = '<';
    } else { /* player 2 */
        turnChar = '>';
    }
    printf("         Turn\n");
    if (isCompGame) {
        printf("Player    %c   Computer\n", turnChar);
    } else {
        printf("Player1   %c   Player2\n", turnChar);
    }
    printf("A B C D E     A B C D E  \n");
    printf("%d %d %d %d %d     %d %d %d %d %d\n", board->player1.skipperCounter[0], board->player1.skipperCounter[1], board->player1.skipperCounter[2], board->player1.skipperCounter[3], board->player1.skipperCounter[4],
                                                  board->player2.skipperCounter[0], board->player2.skipperCounter[1], board->player2.skipperCounter[2], board->player2.skipperCounter[3], board->player2.skipperCounter[4]);
    printf("\n");

    /*
    önceki fonksiyonlarda tanımını yaptığımız renkler ve taşları ekrana yazdırır.
    */

    for (i = 0; i <= board->size+1; i++) {
        int j;
        for (j = 0; j <= board->size+1; j++){
            if(i == 0 || i == board->size+1)
                if(j == 0 || j == board->size+1)
                    printf( "%c ", 254);
                else
                    printf("%d ",j%10);
            else if(j == 0 || j == board->size+1) 
                printf("%d ",i%10);
            else if(board->board[i-1][j-1] == 'A')
                printf(BLUE "%c " RESET ,board->board[i-1][j-1]);
            else if(board->board[i-1][j-1] == 'B')
                printf(YELLOW "%c " RESET ,board->board[i-1][j-1]);
            else if(board->board[i-1][j-1] == 'C')
                printf(GREEN "%c " RESET ,board->board[i-1][j-1]);
            else if(board->board[i-1][j-1] == 'D')
                printf(PURPLE "%c " RESET ,board->board[i-1][j-1]);
            else if(board->board[i-1][j-1] == 'E')
                printf(RED "%c " RESET ,board->board[i-1][j-1]);
            else
                printf("  ");
        }
        printf("\n");
    }
    printf("\n");
}

/*
oyun sonunda kazananı belirleyen ve kazananı ekrana yazdıran fonksiyon.
*/

void displayWinner(Board *board, bool isCompGame) {
    clear_screen();

    int winner = 0;
    int player1Score = getPlayerPoint(&board->player1); /* player1'in skorunu alır */
    int player2Score = getPlayerPoint(&board->player2); /* player2'nin skorunu alır */
    int player1TotalSkipper = getTotalSkipperCount(&board->player1); /* player1'in toplam skipper sayısını alır */
    int player2TotalSkipper = getTotalSkipperCount(&board->player2); /* player2'nin toplam skipper sayısını alır */

    /* bilgisayar mı oyuncu ekranı mı diye bakıp ona göre sonuç ekranını yazdırır. */

    if(isCompGame){
        printf("Player       Computer\n");
    }
    else{
        printf("player1       player2\n");
    }
    printf("A B C D E     A B C D E  \n");
    printf("%d %d %d %d %d     %d %d %d %d %d\n", board->player1.skipperCounter[0], board->player1.skipperCounter[1], board->player1.skipperCounter[2], board->player1.skipperCounter[3], board->player1.skipperCounter[4],
                                                  board->player2.skipperCounter[0], board->player2.skipperCounter[1], board->player2.skipperCounter[2], board->player2.skipperCounter[3], board->player2.skipperCounter[4]);
    printf("\n");

    if(isCompGame){
        printf("Player's set score: %d  Total skipper: %d\n", player1Score, player1TotalSkipper);
        printf("Computer's set score: %d  Total skipper: %d\n", player2Score, player2TotalSkipper);
    }
    else{
        printf("Player 1's set score: %d  Total skipper: %d\n", player1Score, player1TotalSkipper);
        printf("Player 2's set score: %d  Total skipper: %d\n", player2Score, player2TotalSkipper);
    }
    printf("\n");

    /*
    set sayısı fazla olan oyuncu oyunu kazanır. 
    eğer set sayısı eşitse toplam skipper sayısı fazla olan oyuncu oyunu kazanır.
    ikisi de eşitse berabere biter.
    */

    if (player1Score > player2Score) {
        winner = 1;
    } else if (player1Score < player2Score) {
        winner = 2;
    } else {
        if (player1TotalSkipper > player2TotalSkipper) {
            winner = 1;
        } else if (player1TotalSkipper < player2TotalSkipper) {
            winner = 2;
        } else {
            printf("IT IS A DRAW\n");
            return;
        }
    }
    if(isCompGame){
        if(winner == 1){
            printf("WINNER IS PLAYER\n");
        }
        else{
            printf("WINNER IS COMPUTER\n");
        }
    }
    else{
        printf("WINNER IS PLAYER%d\n", winner);
    }
}

/*
hamle yapılmasını sağlayan fonksiyon.
*/

void playMove(Board *board, Move *move) {
    int fromRow = move->from.row; /* taşın başlangıç satırı */
    int fromCol = move->from.col; /* taşın başlangıç sütunu */
    int toRow = move->to.row; /* taşın gideceği satır */
    int toCol = move->to.col; /* taşın gideceği sütun */
    Position middle = getMiddlePosBetween(move->from, move->to); /* taşın gideceği konum ile taşın başlangıç konumu arasındaki konumu alır */
    
    move->midPieceType = board->board[middle.row][middle.col]; /* taşın gideceği konum ile taşın başlangıç konumu arasındaki taşı alır */

    /* ortadaki taşın türüne göre o oyuncunun puanı arttırılır */
    if(board->turn == 1) {
        board->player1.skipperCounter[board->board[middle.row][middle.col] - 65]++;
    } else {
        board->player2.skipperCounter[board->board[middle.row][middle.col] - 65]++;
    }

    /* taşın gideceği konuma taşı yerleştirir ve gerekli düzenlemeleri yapar */
    board->board[toRow][toCol] = board->board[fromRow][fromCol];
    board->board[fromRow][fromCol] = ' ';
    board->board[middle.row][middle.col] = ' ';
}

/* 
hamleyi geri almak için kullanılan fonksiyon.
*/

void undoMove(Board *board, Move *move) {
    int fromRow = move->from.row; /* taşın başlangıç satırı */
    int fromCol = move->from.col; /* taşın başlangıç sütunu */
    int toRow = move->to.row; /* taşın gideceği satır */
    int toCol = move->to.col; /* taşın gideceği sütun */

    /* eğer ortadaki taşın türü 65'ten küçükse o taşın ortada taşınacak bir taşı yoktur */
    if (move->midPieceType < 65 ) {
        return;
    }

    /* taşın gideceği konum ile taşın başlangıç konumu arasındaki konumu alır */
    Position middle = getMiddlePosBetween(move->from, move->to);

    /* oyuncunun puanını azaltır */
    if(board->turn == 1) {
        board->player1.skipperCounter[move->midPieceType - 65]--;
    } else {
        board->player2.skipperCounter[move->midPieceType - 65]--;
    }

    /*tahtayı eski haline getirir */
    board->board[fromRow][fromCol] = board->board[toRow][toCol];
    board->board[toRow][toCol] = ' ';
    board->board[middle.row][middle.col] = move->midPieceType;
}