void clear_screen();
Position getMiddlePosBetween(Position, Position);
bool isPosValid(Position, int);
bool canPieceMoveTo(Board *, Position, Position);
bool canPieceMove(Board *, Position);
int *arrOfSkippersCanGetForPos(Board *, Position);
Position getPosForSkipper(Board *, Position, char);
bool isGameEnd(Board *);

void saveGame(Board *, Move *);
bool loadGame(Board *, Move *);
int getPlayerPoint(Player *);
int getTotalSkipperCount(Player *);
char getMinSkipper(Player *);
