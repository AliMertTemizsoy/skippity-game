#include "vsComp.h"

/*
state değişkeni ile oyunun hangi durumda olduğunu belirleriz.
VSCOMP_PLAYER: insan oyuncunun sırası
VSCOMP_COMPUTER: bilgisayar oyuncunun sırası
*/

#define VSCOMP_PLAYER 1
#define VSCOMP_COMPUTER 2

/*
insan vs bilgisayar oyunu için main loop fonksiyonu
*/

void vsCompMainLoop(Board *board, Move *pMove) {
    Move move; /*geçici hamle değişkeni*/
    int returnChoice; /*kullanıcının seçimini tutan değişken*/
    bool isFirstMove = true; /*ilk hamle kontrolü için değişken*/
    int state = VSCOMP_PLAYER; /*ilk olarak insan oyuncunun sırasıdır*/

    if (board->turn == 2) {
        state = VSCOMP_COMPUTER; /* bilgisayarın sırası */
    }

    if (pMove->from.row == -1) {
        isFirstMove = true; /*ilk hamle kontrolü*/
    } else {
        isFirstMove = false;
        move = *pMove; /*geçici hamle değişkenine main fonksiyonundan gelen hamleyi atar*/
    }

    while(1) {
        displayBoard(board, true); /*tahtayı yazdırır*/

        switch (state) {
        case VSCOMP_PLAYER: {
                returnChoice = vsHumanInput(board, &move, isFirstMove, true); /*kullanıcıdan hamle alır*/

                /*0)new move 1) pass turn 2) undo 3) save & exit 4) exit */
                switch (returnChoice) {
                    case 0: {
                        isFirstMove=false;
                        move.from = move.to; /* üst üste hamle için to'yu from'a atar */
                    }
                        break;
                    case 1: {
                        isFirstMove = true; /*ilk hamle kontrolü*/
                        if(isGameEnd(board)) {
                            displayWinner(board, true); /*oyunun bitip bitmediğini kontrol eder bittiyse sonuç ekranını yazdırır */
                            return;
                        }
                        board->turn = 2; /* bilgisayarın sırası */
                        state = VSCOMP_COMPUTER; /* bilgisayar state'i */
                        if(isGameEnd(board)) {
                            displayWinner(board, true); /*oyunun bitip bitmediğini kontrol eder bittiyse sonuç ekranını yazdırır */
                            return;
                        }
                    }
                        break;

                    case 2: 
                        isFirstMove = true; /*ilk hamle kontrolü*/
                        undoMove(board, &move); /*hamleyi geri alır*/

                        displayBoard(board, true); /*tahtayı yazdırır*/

                        int choice; /*kullanıcının seçimini tutan değişken*/
                        printf("what do you want to do: 0) redo 1) continue\n"); /*kullanıcıdan seçim yapmasını ister*/
                        scanf("%d", &choice); /*kullanıcının seçimini alır*/

                        switch (choice) {
                            case 0: /*redo*/
                                playMove(board, &move); /*hamleyi tekrar yapar*/

                                if(isGameEnd(board)) {
                                    displayWinner(board, true); /*oyunun bitip bitmediğini kontrol eder bittiyse sonuç ekranını yazdırır */
                                    return;
                                }
                                board->turn = 2; /* bilgisayarın sırası */
                                state = VSCOMP_COMPUTER; /* bilgisayar state'i */
                                break;

                            default:
                                break;
                        }

                        break;

                    case 3: {
                            if (canPieceMove(board, move.to)) {
                                move.from = move.to; /* üst üste hamle için to'yu from'a atar */
                            } else {
                                move.from.row = -1; /* hamle yapamazsa from ve to'yu -1 yapar */
                                move.from.col = -1; /* hamle yapamazsa from ve to'yu -1 yapar */
                                board->turn = 2; /* bilgisayarın sırası */
                                state = VSCOMP_COMPUTER; /* bilgisayar state'i */
                            }
                            saveGame(board, &move); /* oyunu kaydeder */
                        }
                        return;

                    case 4:
                        return;
                    default:
                        break;
                }
            }
            break;
            
        case VSCOMP_COMPUTER: {
                move = getCompMove(board); /*bilgisayarın hamlesini alır*/

                if (move.from.row != -1) {
                    playMove(board, &move); /*hamleyi yapar*/
                }
                if(isGameEnd(board)) {
                    displayBoard(board, true); /*tahtayı yazdırır*/
                    displayWinner(board, true); /*oyunun bitip bitmediğini kontrol eder bittiyse sonuç ekranını yazdırır */
                    return;
                } else if (move.from.row == -1) {
                    printf("impossible to happen\n");
                }

                move.from.row = -1; 
                move.from.col = -1;

                board->turn = (board->turn == 1) ? 2 : 1; /* insan oyuncunun sırası */
                state = VSCOMP_PLAYER; /* insan state'i */

            }
            break;

        default:
            break;
        }

    }
}

/*
bilgisayarın hamlesini yapan fonksiyon
*/

Move getCompMove(Board *board) {
    Move move; /*geçici hamle değişkeni*/
    Position pos; /*pozisyon değişkeni*/
    int *arrPieces; /*taşların bulunduğu array*/
    char requiredSkipper; /*gereken skipper*/
    int i, j, t, skipperInd=0;

    move.from.row = -1; /*hamle değişkenini sıfırlar*/
    move.from.col = -1; /*hamle değişkenini sıfırlar*/
    move.to.row = -1; /*hamle değişkenini sıfırlar*/
    move.to.col = -1; /*hamle değişkenini sıfırlar*/

    requiredSkipper = getMinSkipper(&board->player2); /*gereken minimum skipper'ı bulur ve değişkene atar*/

    while(move.from.row == -1) {

        for(i=0; i< board->size; i++) {
            for(j=0; j<board->size; j++){
                if(board->board[i][j] != ' '){
                    pos.row = i; /*pozisyon değişkenine i'yi atar*/
                    pos.col = j; /*pozisyon değişkenine j'yi atar*/
                    arrPieces = arrOfSkippersCanGetForPos(board, pos); /*pozisyonun etrafındaki taşları bulur ve array'e atar*/

                    t=0;
                    while (arrPieces[t] != -1 && t < 5) {
                        if (arrPieces[t] == requiredSkipper) {
                            arrPieces = arrOfSkippersCanGetForPos(board, pos); /*pozisyonun etrafındaki taşları bulur ve array'e atar*/
                            move.from = pos; /*hamlenin from'unu pozisyon yapar*/
                            move.to = getPosForSkipper(board, pos, requiredSkipper); /*gereken skipper'ı götürecek pozisyonu bulur ve to'ya atar*/
                            return move; /*hamleyi döndürür*/
                        }
                        t++; /*t'yi bir arttırır*/
                    }
                }
            }
        }

        requiredSkipper = 65 + skipperInd; /* skipperindexteki skipper'ı değişkene atar */
        skipperInd++; /*gereken skipper'ı bir arttırır*/

        if(skipperInd == 6) {
            return move; /*bütün diziyi döndüyse hamleyi döndürür*/
        }
    }
    return move; /*hamleyi döndürür*/
}
