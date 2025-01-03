#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct chesspiece {
    char type;
    int pos[2];
    char color;
    bool alive;
    bool pawnJump;

} typedef piece;

void setupPieces(piece * pieces[]);

void freeBoard(piece * pieces[]);

void printBoard(piece * pieces[]);

piece * findPiece(piece * pieces[], int tarPos[2]);

char findPieceType(piece * pieces[], int searchPos[2]);

int ifWithinBounds(int targetPos[2]);

int moveRule(piece * curPiece, int curPos[2], int targetPos[2]);

void movePiece(char curPlayer, piece * pieces[32], int curPos[2], int targetPos[2]);

bool checkClearPath(piece * pieces[32], int curPos[2], int tarPos[2]);

void printPreview(piece * pieces[32], int curPos[2], int tarPos[2]);
