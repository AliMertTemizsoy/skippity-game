#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "helperFuncs.h"

/*
istersek terminali temizleyen fonksiyon ama şuan kapalı durumda bıraktım
*/

void clear_screen() {
#ifdef _WIN32
    /*system("cls");*/
#else
    /*system("clear");*/
#endif
}

/*
seçilen iki pozisyon arasındaki orta noktayı döndüren fonksiyon
*/

Position getMiddlePosBetween(Position from, Position to) {
    Position middle;
    middle.row = (from.row + to.row) / 2;
    middle.col = (from.col + to.col) / 2;
    return middle;
}

/*
pozisyonun geçerli olup olmadığını kontrol eden fonksiyon
*/

bool isPosValid(Position pos, int size) {
    if (pos.row >= 0 && pos.row < size && pos.col >= 0 && pos.col < size) {
        return true;
    }
    return false;
}

/*
hamle o noktaya yapılabilir mi kontrol eden fonksiyon
*/

bool canPieceMoveTo(Board *board, Position piecePos, Position targetPos) {
    if (isPosValid(targetPos, board->size) && board->board[targetPos.row][targetPos.col] == ' ') {
        Position mid = getMiddlePosBetween(piecePos, targetPos); /* aradaki pozisyonu bulduk */
        int diff = abs(targetPos.row - piecePos.row) + abs(targetPos.col - piecePos.col); 
        /* sadece 2 farklı alt üst sağ sol hareketleri yapsın diye kontrol ettik */
        if(board->board[mid.row][mid.col] != ' ' && diff == 2) {
            return true;
        } else {
            return false;
        }
    }
    
    return false;
}

/*
taş hareket edebilir mi diye kontrol eden fonksiyon
*/

bool canPieceMove(Board *board, Position piecePos) {
    bool retVal = false; /* taşın hareket edip edemeyeceğini tutan değişken */

    /* taşın tahtada olup olmadığını ve taşın boş olup olmadığını kontrol ediyoruz */

    if (!isPosValid(piecePos, board->size) || board->board[piecePos.row][piecePos.col] == ' ') {
        return false;
    }

    Position top = piecePos; /* taşın üstünü tuttuk */
    Position bottom = piecePos; /* taşın altını tuttuk */
    Position left = piecePos; /* taşın solunu tuttuk */
    Position right = piecePos; /* taşın sağı tuttuk */

    top.row = piecePos.row - 2; 
    bottom.row = piecePos.row + 2;
    left.col = piecePos.col - 2;
    right.col = piecePos.col + 2;

    if (canPieceMoveTo(board, piecePos, top)) {
        retVal = true; /* taşın üstüne hareket edebiliyorsa true döner */
    } else if (canPieceMoveTo(board, piecePos, bottom)) {
        retVal = true; /* taşın altına hareket edebiliyorsa true döner */
    } else if (canPieceMoveTo(board, piecePos, left)) {
        retVal = true; /* taşın soluna hareket edebiliyorsa true döner */
    } else if (canPieceMoveTo(board, piecePos, right)) {
        retVal = true; /* taşın sağına hareket edebiliyorsa true döner */
    }

    return retVal;
}

/*
bilgisayar oyunu için taşın gidebileceği pozisyonları döndüren fonksiyon
*/

int *arrOfSkippersCanGetForPos(Board *board, Position piecePos) {
    int *retVal; /* taşın gidebileceği pozisyonları tutan dizi */
    int ind = 0; /* dizi için indis */
    int i;

    /* retVal için dinamik yer açıp diziyi -1 ile dolduruyoruz */

    retVal = (int *)malloc(5 * sizeof(int));
    for(i=0; i<5; i++){
        retVal[i] = -1;
    }

    /* taşın tahtada olup olmadığını ve taşın boş olup olmadığını kontrol ediyoruz */

    if (!isPosValid(piecePos, board->size) || board->board[piecePos.row][piecePos.col] == ' ') {
        return false;
    }

    Position top = piecePos; /* taşın üstünü tuttuk */
    Position bottom = piecePos; /* taşın altını tuttuk */
    Position left = piecePos; /* taşın solunu tuttuk */
    Position right = piecePos; /* taşın sağı tuttuk */

    top.row = piecePos.row - 2;
    bottom.row = piecePos.row + 2;
    left.col = piecePos.col - 2;
    right.col = piecePos.col + 2;

    /* taşın gidebileceği pozisyonları kontrol ediyoruz ve diziye ekliyoruz */

    if (canPieceMoveTo(board, piecePos, top)) {
        Position mid = getMiddlePosBetween(piecePos, top);
        retVal[ind]=board->board[mid.row][mid.col];
        ind++;
    }

    if (canPieceMoveTo(board, piecePos, bottom)) {
        Position mid = getMiddlePosBetween(piecePos, bottom);
        retVal[ind]=board->board[mid.row][mid.col];
        ind++;
    }
    if (canPieceMoveTo(board, piecePos, left)) {
        Position mid = getMiddlePosBetween(piecePos, left);
        retVal[ind]=board->board[mid.row][mid.col];
        ind++;
    }
    if (canPieceMoveTo(board, piecePos, right)) {
        Position mid = getMiddlePosBetween(piecePos, right);
        retVal[ind]=board->board[mid.row][mid.col];
    }

    return retVal; /* taşın gidebileceği pozisyonları döndürüyoruz */
}

Position getPosForSkipper(Board *board, Position piecePos, char skipper) {
    Position retVal = {-1,-1};
    int row, col;

    /*check left*/
    row = piecePos.row; 
    col = piecePos.col - 1;

    if (col >= 0 && board->board[row][col] == skipper) {
        retVal.row = row;
        retVal.col = col-1;

        if( board->board[retVal.row][retVal.col] == ' ') {
            return retVal;
        }
    }

    /*check right*/
    row = piecePos.row;
    col = piecePos.col+1;

    if (col < board->size && board->board[row][col] == skipper) {
        retVal.row = row;
        retVal.col = col+1;

        if( board->board[retVal.row][retVal.col] == ' ') {
            return retVal;
        }
    }
    
    /*check top*/
    row = piecePos.row - 1;
    col = piecePos.col;

    if (row >= 0 && board->board[row][col] == skipper) {
        retVal.row = row-1;
        retVal.col = col;

        if( board->board[retVal.row][retVal.col] == ' ') {
            return retVal;
        }
    }

    /*check bottom*/
    row = piecePos.row + 1;
    col = piecePos.col;

    if (row < board->size && board->board[row][col] == skipper) {
        retVal.row = row+1;
        retVal.col = col;
        
        if( board->board[retVal.row][retVal.col] == ' ') {
            return retVal;
        }
    }

    return retVal;
}

/*
oyunun bitip bitmediğini kontrol eden fonksiyon
*/

bool isGameEnd(Board *bigBoard) {
    char **board = bigBoard->board;
    int size = bigBoard->size;
    int i,j;
    Position pos;

    /* tahtadaki konumda taş yoksa veya o taş hamle yapamıyorsa false döndürür */

    for(i=0; i<size; i++){
        for(j=0; j<size; j++){
            if(board[i][j] != ' '){
                pos.row = i;
                pos.col = j;
                if(canPieceMove(bigBoard, pos)){
                    return false;
                }
            }
        }
    }
    return true;
}

/*
oyunu save eden fonksiyon
*/

void saveGame(Board *board, Move *move) {
    FILE *file = fopen("save.dat", "w"); /* save.dat adında bir dosya açıyoruz. aynı zamanda yazacağımız için w modunda açtık */
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    int i, j;
    
    fprintf(file, "%d\n", board->size); /* size'ı kaydettik */
    fprintf(file, "%d\n", board->turn); /* sırayı kaydettik */
    fprintf(file, "%d\n", move->from.row); /* from'un row'unu kaydettik */
    fprintf(file, "%d\n", move->from.col); /* from'un col'unu kaydettik */
    fprintf(file, "%d\n", board->player1.skipperCounter[0]); /* player1'in A tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player1.skipperCounter[1]); /* player1'in B tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player1.skipperCounter[2]); /* player1'in C tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player1.skipperCounter[3]); /* player1'in D tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player1.skipperCounter[4]); /* player1'in E tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player2.skipperCounter[0]); /* player2'in A tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player2.skipperCounter[1]); /* player2'in B tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player2.skipperCounter[2]); /* player2'in C tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player2.skipperCounter[3]); /* player2'in D tipi taş sayısını kaydettik */
    fprintf(file, "%d\n", board->player2.skipperCounter[4]); /* player2'in E tipi taş sayısını kaydettik */
    
    /* tahtayı dosyaya yazdık */
    for (i = 0; i < board->size; i++) {
        for (j = 0; j < board->size; j++) {
            fprintf(file, "%c", board->board[i][j]);
        }
        fprintf(file, "\n");
    }
    /* dosyayı kapattık */
    fclose(file);
}

/*
oyunu yükleyen fonksiyon
*/
bool loadGame(Board *board, Move *move) {
    FILE *file = fopen("save.dat", "r"); /* save.dat adında bir dosya açıyoruz. aynı zamanda okuyacağımız için r modunda açtık */

    if (file == NULL) {
        printf("Error opening file!\n");
        return false;
    }

    fscanf(file, "%d\n", &board->size); /* size'ı okuduk */
    fscanf(file, "%d\n", &board->turn); /* sırayı okuduk */
    fscanf(file, "%d\n", &move->from.row); /* from'un row'unu okuduk */
    fscanf(file, "%d\n", &move->from.col); /* from'un col'unu okuduk */
    fscanf(file, "%d\n", &board->player1.skipperCounter[0]); /* player1'in A tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player1.skipperCounter[1]); /* player1'in B tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player1.skipperCounter[2]); /* player1'in C tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player1.skipperCounter[3]); /* player1'in D tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player1.skipperCounter[4]); /* player1'in E tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player2.skipperCounter[0]); /* player2'in A tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player2.skipperCounter[1]); /* player2'in B tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player2.skipperCounter[2]); /* player2'in C tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player2.skipperCounter[3]); /* player2'in D tipi taş sayısını okuduk */
    fscanf(file, "%d\n", &board->player2.skipperCounter[4]); /* player2'in E tipi taş sayısını okuduk */
    
    int i=0, j=0;
    char c;

    while (feof(file) == 0)
    {
        fscanf(file, "%c", &c);
        /* new line gördüğünde aşağı kaydırıyoruz ve j=-1 yapıyoruz yoksa tahta sağa kayıyor */
        if (c == '\n') {
            j = -1;
            i++;
        } else {
            if ( i < board->size && j < board->size ) {
                board->board[i][j] = c; /* tahtayı sırayla okuduk */
            } else {
                printf("Broken save file!\n");
                return false;
            }
        }
        j++;
    }

    /* dosyayı kapattık */
    fclose(file);

    return true;
}

/*
tahtanın boyutunu kaydeden fonksiyon
*/

int loadBoardSize() {
    int size=0;
    FILE *file = fopen("save.dat", "r"); /* save.dat adında bir dosya açıyoruz. aynı zamanda okuyacağımız için r modunda açtık */

    if (file == NULL) {
        printf("Error opening file!\n");
        return false;
    }

    fscanf(file, "%d", &size); /* size'ı okuduk */

    /* dosyayı kapattık */

    fclose(file);

    return size; /* size'ı döndürdük */
}

/*
oyuncunun set puanını döndüren fonksiyon
*/

int getPlayerPoint(Player *player) {
    int i;
    int retVal = 99; /* oyuncunun puanını tutan değişken başta çok büyük tanımladık min yapacağımız için */

    for (i =0; i < 5; i++) {
        if (player->skipperCounter[i] < retVal) {
            retVal = player->skipperCounter[i]; /* en küçük set puanını bulduk */
        }
    }

    return retVal; /* set puanını döndürdük */
}

/*
total skipper sayısını döndüren fonksiyon
*/
int getTotalSkipperCount(Player *player) {
    int i;
    int retVal = 0;

    for (i =0; i < 5; i++) {
        retVal += player->skipperCounter[i]; /* toplam skipper sayısını dizi elemanlarını sırayla toplayarak bulduk */
    }

    return retVal; /* toplam skipper sayısını döndürdük */
}

/*
minimum olan skipper'ı döndüren fonksiyon
*/

char getMinSkipper(Player *player) {
    int i;
    int min = 99; /* en küçük set puanını tutan değişken min olacağı için büyük tanımladık */
    char retVal = ' '; 

    for (i =0; i < 5; i++) {
        if (player->skipperCounter[i] < min) {
            min = player->skipperCounter[i]; /* en küçük skipper sayısını bulduk */
            retVal = 65 + i; /* en küçük skipper'ın tipini bulduk */
        }
    }

    return retVal; /* en küçük skipper'ı döndürdük */
}
