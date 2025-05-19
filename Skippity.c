/*
video sunum linki:

https://youtu.be/1OTlBgUmvoY

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
Kod yazarken fonksiyonları ve değişkenleri aynı dosyada tanımlamak yerine ayrı dosyalarda tanımlamak daha düzenli bir kod yazmamızı sağlar. Bu 
yüzden bu dosyada sadece main fonksiyonu ve main fonksiyonunda çağrılan fonksiyonlar tanımlanmıştır. Diğer fonksiyonlar ise ayrı dosyalarda bulunmaktadır.
Tanımlanan main dışındaki fonksiyon ve açıklamaları ilgili dosyalarda bulunmaktadır. 
*/

/*
Kullandığımız yardımcı kaynak dosyalarını include ediyoruz.
*/

#include "helperFuncs.c"
#include "board.c"
#include "vsHuman.c"
#include "vsComp.c"

/*
Ana Menu

Welcome to the game of Skippity!

Please select an option:
1) Human vs Human game
2) Human vs Computer game
3) Load game
4) Exit

Your choice: 1
*/

/*
Oyun durumlarını belirlemek için tanımladığımız makrolar.
*/

#define MAIN_NOSELECTION 0
#define MAIN_VSHUMAN     1
#define MAIN_VSCOMP      2
#define MAIN_LOADGAME    3
#define MAIN_EXIT        4


void initializeBoard(Board *);
int displayMainMenu();

int main() {
    Board board;
    int state = MAIN_NOSELECTION; /* ilk olarak hiçbir seçim yapılmamış durumda başlatılır. */
    bool gameLoaded = false;
    Move move;

    /* Hamle kontrolleri için move structını default duruma getirdik. */
    move.midPieceType = ' ';
    move.from.row = -1;
    move.from.col = -1;

    state = displayMainMenu(); /* Ana menüyü ekrana yazdırır ve kullanıcının seçim yapmasını bekler. */

    while (1) {
        switch (state) {
        case MAIN_NOSELECTION:
            break;
        case MAIN_VSHUMAN: {
                if (!gameLoaded) {
                    initBoard(&board); /* Oyun tahtasını başlatır. */
                }
                vsHumanMainLoop(&board, &move); /* İki oyunculu oyunun ana döngüsü. */

                state = displayMainMenu(); /* Ana menüyü ekrana yazdırır ve kullanıcının seçim yapmasını bekler. */
                gameLoaded = false;
            }
            break;
        case MAIN_VSCOMP: {
                if (!gameLoaded) {
                    initBoard(&board); /* Oyun tahtasını başlatır. */
                }
                vsCompMainLoop(&board, &move); /* Bilgisayara karşı oyunun ana döngüsü. */

                state = displayMainMenu(); /* Ana menüyü ekrana yazdırır ve kullanıcının seçim yapmasını bekler. */
                gameLoaded = false;
            }
            break;
        /* Eğer oyun loadgame alınarak başlanacaksa */
        case MAIN_LOADGAME: {
                int size = loadBoardSize(); /* Oyun tahtasının boyutunu alır. */
                int i;

                /* Oyun tahtasını için dinamik yer açar. */
                board.board = (char **) malloc(size * sizeof(char *));
                
                for (i = 0; i < size; i++){
                    board.board[i] = (char *)malloc(size * sizeof(char));
                }

                if (loadGame(&board, &move)) {
                    printf("Game loaded successfully!\n");

                } else {
                    printf("Failed to load game!\n");
                }

                gameLoaded = true; /* Oyun yüklendiğinde bu değişken true yapılır. */
                state = displayMainMenu(); /* Ana menüyü ekrana yazdırır ve kullanıcının seçim yapmasını bekler. */
            }
            break;

        case MAIN_EXIT: /* Programdan çıkış yapar. */
            return 0;

        default:
            break;
        }
    }

    return 0;
}

/*
Ana menüyü ekrana yazdırır ve kullanıcının seçim yapmasını bekler.
*/
int displayMainMenu() {
    int state;
    printf("Welcome to the game of Skippity!\n\n");
    printf("Please select an option:\n");
    printf("1) Human vs Human game\n");
    printf("2) Human vs Computer game\n");
    printf("3) Load game\n");
    printf("4) Exit\n\n");
    printf("Your choice: ");
    scanf("%d", &state);
    return state;
}
