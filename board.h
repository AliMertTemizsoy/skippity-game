/*
board.h macrosunu koymadan çalıştırmadığı için macro ile define ettim.
*/


#ifndef _FILE_DEFINED_BOARD_H_
    #define _FILE_DEFINED_BOARD_H_

    /*
    taşları renklendirmek için kullanılan renk kodları
    */

    #define BLACK "\033[0;30m"
    #define RED "\033[0;31m"
    #define GREEN "\033[0;32m"
    #define YELLOW "\033[0;33m"
    #define BLUE "\033[0;34m"
    #define PURPLE "\033[0;35m"
    #define RESET "\033[0m"

    /* oyuncuların taş sayılarını tutan struct */

    typedef struct {
        int skipperCounter[5];
    } Player;

    /* 
    oyun tahtasını tutan struct 
    bu structta tahta boyutu, tahtadaki taşlar, oyuncular ve sıra bilgisi tutulur
    */

    typedef struct {
        int size;
        char **board;
        Player player1;
        Player player2;
        int turn;
    } Board;

    /*
    pozisyon bilgilerini tutan struct
    */

    typedef struct {
        int row;
        int col;
    } Position;

    /*
    hamle bilgilerini tutan struct
    */

    typedef struct {
        Position from;
        Position to;
        char midPieceType;
    } Move;

    /*
    hamle listesini tutan struct
    */

    typedef struct {
        Move *moves;
        int skipperType; /* 0 = A, 1 = B, 2 = C, 3 = D, 4 = E */
    } MoveList;

    void initBoard(Board *);
    void createBoard(Board *);
    void displayBoard(Board *, bool);
    void displayWinner(Board *, bool );
    bool movePiece(Board *, Position);
    void undoMove(Board *, Move *);
    void playMove(Board *, Move *);
#endif
