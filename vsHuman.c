#include "vsHuman.h"

/*
insan vs insan oyunu için main loop fonksiyonu
*/

void vsHumanMainLoop(Board *board, Move *pMove) {
    Move move; /*geçici hamle değişkeni*/
    int returnChoice; /*kullanıcının seçimini tutan değişken*/
    bool isFirstMove = true; /*ilk hamle kontrolü için değişken*/

    if (pMove->from.row == -1) {
        isFirstMove = true; /*ilk hamle kontrolü*/
    } else {
        isFirstMove = false;
        move = *pMove; /*geçici hamle değişkenine main fonksiyonundan gelen hamleyi atar*/
    }

    while(1) {
        displayBoard(board, false);
        returnChoice = vsHumanInput(board, &move, isFirstMove, false); /*kullanıcıdan hamle alır*/

        /*0)new move 1) pass turn 2) undo 3) save & exit 4) exit */
        switch (returnChoice) {
        case 0: {
                isFirstMove=false; /*ilk hamle kontrolü*/
                move.from = move.to; /*geçici hamle değişkenine yeni hamleyi atar*/
            }
            break;
        case 1: {
                isFirstMove = true; /*ilk hamle kontrolü*/
                if(isGameEnd(board)) {
                    displayWinner(board, false); /*oyunun bitip bitmediğini kontrol eder bittiyse sonuç ekranını yazdırır */
                    return;
                }
                board->turn = (board->turn == 1) ? 2 : 1; /* oyuncu sırasını değiştirir */
            }
            break;

        case 2: 
            isFirstMove = true; /*ilk hamle kontrolü*/
            undoMove(board, &move); /*hamleyi geri alır*/

            displayBoard(board, false); /*tahtayı yazdırır*/

            int choice; /*kullanıcının seçimini tutan değişken*/
            printf("what do you want to do: 0) redo 1) continue\n"); /*kullanıcıdan seçim yapmasını ister*/
            scanf("%d", &choice); /*kullanıcının seçimini alır*/

            switch (choice) {
            case 0: /*redo*/
                playMove(board, &move); /*hamleyi tekrar yapar*/

                if(isGameEnd(board)) {
                    displayWinner(board, false); /*oyunun bitip bitmediğini kontrol eder bittiyse sonuç ekranını yazdırır */
                    return;
                }
                board->turn = (board->turn == 1) ? 2 : 1; /* oyuncu sırasını değiştirir */
                break;
            
            default:
                break;
            }

            break;

        case 3: {
                if (canPieceMove(board, move.to)) {
                    move.from = move.to; /*hamle yapılabiliyorsa geçici hamle değişkenine yeni hamleyi atar*/
                } else {
                    move.from.row = -1; /*hamle yapılamıyorsa geçici hamle değişkenini sıfırlar*/
                    move.from.col = -1; /*hamle yapılamıyorsa geçici hamle değişkenini sıfırlar*/
                    board->turn = (board->turn == 1) ? 2 : 1; /* oyuncu sırasını değiştirir */
                }
                saveGame(board, &move); /*oyunu kaydeder*/
            }
            return;

        case 4:
            return;
        default:
            break;
        }
    }
}

/*
insan vs insan oyunu için hamle almayı sağlayan fonksiyon
*/

int vsHumanInput(Board *board, Move *move, bool isFirstMove, bool isAgainstComputer) {
    int returnChoice; /*kullanıcının seçimini tutan değişken*/
    bool valid = false; /*geçerli hamle kontrolü için değişken*/

    if(isFirstMove) {
        do {
            printf("select your piece (row, col):\n"); /*kullanıcıdan taş seçmesini ister*/
            scanf("%d", &(move->from.row)); /*kullanıcının seçtiği taşın satırını alır*/
            printf(",\n"); 
            scanf("%d", &(move->from.col)); /*kullanıcının seçtiği taşın sütununu alır*/

            move->from.row--; /*kullanıcının seçtiği taşın satırını düzeltir*/
            move->from.col--; /*kullanıcının seçtiği taşın sütununu düzeltir*/

            /* seçilen poziyon kullanılarak hamle yapılabilir mi diye kontrol edilir */

            valid = isPosValid(move->from, board->size) && board->board[move->from.row][move->from.col] != ' ' && canPieceMove(board, move->from);
            if(!valid) {
                printf("Invalid position!\n");
            }
        } while (!valid);
    }
    vsHumanAskTargetPos(board, move); /*kullanıcıdan hedef pozisyonu alır*/

    playMove(board, move); /*hamleyi yapar*/

    displayBoard(board, isAgainstComputer); /*tahtayı yazdırır*/

    /*
    üste üste hamle yapılabilir mi diye kontrol edilir
    */
    
    if (canPieceMove(board, move->to)) { 
        printf("Final Choice: 0) new move 1) pass turn 2) undo 3) save & exit 4) exit\n"); /*kullanıcıya seçim yapmasını ister*/
        scanf("%d", &returnChoice); /*kullanıcının seçimini alır*/
    } else {
        printf("Final Choice: 1) pass turn 2) undo 3) save & exit 4) exit\n"); /*kullanıcıya seçim yapmasını ister*/
        scanf("%d", &returnChoice); /*kullanıcının seçimini alır*/
    }

    return returnChoice; /*kullanıcının seçimini döndürür*/
}

/*
insandan hedef pozisyon almayı sağlayan fonksiyon
*/

void vsHumanAskTargetPos(Board *board, Move *move) {
    bool valid = false;

    do{
        printf("where to move (row, col):\n"); /*kullanıcıdan hedef pozisyonu alır*/
        scanf("%d", &(move->to.row)); /*kullanıcının hedef pozisyonun satırını alır*/
        printf(",\n");
        scanf("%d", &(move->to.col)); /*kullanıcının hedef pozisyonun sütununu alır*/

        move->to.row--; /*kullanıcının hedef pozisyonun satırını düzeltir*/
        move->to.col--; /*kullanıcının hedef pozisyonun sütununu düzeltir*/

        valid = canPieceMoveTo(board, move->from, move->to); /*hamle yapılabilir mi diye kontrol edilir*/
        if(!valid) {
            printf("Invalid position!\n"); /*hamle yapılamıyorsa hata mesajı verir*/
        }
    } while (!valid);
}