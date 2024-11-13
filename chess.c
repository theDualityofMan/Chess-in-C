#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct chesspiece {
    char type;
    int pos[2];
    char color;
    bool alive;
    
} typedef piece;

char defaultWSetup[16] = {'r', 'k', 'b', 'i', 'q', 'b', 'k', 'r', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};
char defaultBSetup[16] = { 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'R', 'K', 'B', 'Q', 'I', 'B', 'K', 'R'};


void setupPieces(piece * pieces[]){
    for (int i = 0; i < 16; i++){
        pieces[i] -> color = 'w';
        pieces[i] -> pos[0] = i%8;
        pieces[i] -> pos[1] = div(i, 8).quot;
        pieces[i] -> type = defaultWSetup[i];
        pieces[i] -> alive = true;
    }

    for (int i = 16; i < 32 ; i++){
        pieces[i] -> color = 'b';
        pieces[i] -> pos[0] = i%8;
        pieces[i] -> pos[1] = div(i+32, 8).quot;
        pieces[i] -> type = defaultBSetup[i-16];
        pieces[i] -> alive = true;
    }
}

piece * findPiece(piece * pieces[], int tarPos[2]){
    for(int c = 0; c < 32; c++){
        if(pieces[c] -> alive && pieces[c] -> pos[0] == tarPos[0] && pieces[c] -> pos[1] == tarPos[1]){
            return pieces[c];
        }
    }
    return NULL;
}

char findPieceType(piece * pieces[], int searchPos[2]){
    if (findPiece(pieces, searchPos) != NULL){
        return findPiece(pieces, searchPos) -> type;
    }
    return 1;
}

void printBoard(piece * pieces[]){
    int foundPiece = 0;
    printf("\n    0  1  2  3  4  5  6  7\n\n");
    for (int i = 0; i < 8; i++){
        printf("%d   ", i);
        for (int j = 0; j < 8; j++){
            for(int c = 0; c < 32; c++){
                if(pieces[c] -> pos[0] == j && pieces[c] -> pos[1] == i && pieces[c] -> alive){
                    foundPiece = 1;
                    printf("%c  ", pieces[c] -> type);
                }
            }
            if (!foundPiece){
                printf("0  ");
            } else {
                foundPiece = 0;
            }
        }
        printf("\n");
    }
    return;
}

int ifWithinBounds(int targetPos[2]){
    if(targetPos[0] >= 0 && targetPos[0] <= 7 && targetPos[1] >= 0 && targetPos[1] <= 7){
        return 0;
    }
    return 1;
}
//Determines whether the selected piece has been moved according to its moveset, 
//using the difference between its current position (curPos) and target position (targetPos)
int moveRule(char type, int curPos[2], int targetPos[2]){
    switch(type){
        //white pawn
        case 'p':
            if(targetPos[0] == curPos[0] && targetPos[1] == curPos[1] + 1){
                return 0;
            }
            break;

        //black pawn
        case 'P':
            if(targetPos[0] == curPos[0] && targetPos[1] == curPos[1] - 1){
                return 0;
            }
            break;

        //rooks
        case 'r':
        case 'R':
            if(targetPos[0] == curPos[0] || targetPos[1] == curPos[1]){
                return 0;
            }
            break;

        //knights
        case 'k':
        case 'K':
            if((abs(targetPos[0] - curPos[0]) == 1 && abs(targetPos[1] - curPos[1]) == 2) || 
            (abs(targetPos[0] - curPos[0]) == 2 && abs(targetPos[1] - curPos[1]) == 1) ){
                return 0;
            }
            break;

        //bishops
        case 'b': 
        case 'B':
            if(abs(targetPos[0] - curPos[0]) == abs(targetPos[1] - curPos[1])){
                return 0;
            }
            break;

        //kings
        case 'i': 
        case 'I':
            if((abs(targetPos[0] - curPos[0]) == 1 && targetPos[1] == curPos[1]) || //movement on x axis
            (abs(targetPos[1] - curPos[1]) == 1 && targetPos[0] == curPos[0]) || //movement on y axis
            (abs(targetPos[0] - curPos[0]) == abs(targetPos[1] - curPos[1]) == 1)){ //diagonal movement
                return 0;
            }
            break;

        //queens
        case 'q':
        case 'Q':
            if((abs(targetPos[0] - curPos[0]) == abs(targetPos[1] - curPos[1])) || //diagonal movement
            (targetPos[0] == curPos[0] || targetPos[1] == targetPos[1])){ //linear movement
                return 0;
            }
            break;
        
        
    }
    return 1;
}

void  movePiece(char curPlayer, piece * pieces[32], int curPos[2], int targetPos[2]){
    piece * selectedPiece = NULL;

    for (int i = 0; i < 32; i++){
        if(curPlayer == pieces[i] -> color && curPos[0] == pieces[i] -> pos[0] && curPos[1] == pieces[i] -> pos[1]){
            selectedPiece = pieces[i];
        }
    }

    if(selectedPiece == NULL){
        printf("INVALID MOVE :(( You have no pieces in this selected space.");
    } else {
        selectedPiece -> pos[0] = targetPos[0];
        selectedPiece -> pos[1] = targetPos[1];
    }

    return;
}

int main(){

    int gameOver = 0;

    piece * pieces[32];

    piece * occuPiece;

    int curPos[2], tarPos[2];

    char curPlayer, oppPlayer;

    for (int i = 0; i < 32; i++){
        pieces[i] = malloc(sizeof(piece));
    }

    setupPieces(pieces);

    //TESTING CODE
    while (gameOver != 1){
        printBoard(pieces);
        printf("Enter x and y coords\n");
        scanf(" %c %d %d %d %d", &curPlayer, &curPos[0], &curPos[1], &tarPos[0], &tarPos[1]);

        //NEED FUNC TO DETERMINE WHETHER SELECTED POSITION HAS A PIECE IN IT
        if(findPiece(pieces, curPos) && ifWithinBounds(tarPos) == 0 && moveRule(findPieceType(pieces, curPos), curPos, tarPos) == 0){
            occuPiece = findPiece(pieces, tarPos);
            if(occuPiece != NULL){
                if(occuPiece -> color == curPlayer){
                    //ADD CASTLE SCENARIO
                    printf("INVALID MOVE :(( Your piece is in the target space already.");
                } else {
                    movePiece(curPlayer, pieces, curPos, tarPos);
                    occuPiece -> alive = false;
                    printf("attacked");
                }
            } else{
                movePiece(curPlayer, pieces, curPos, tarPos);
            }
        }
        
    }
    

    /*  TODO:
            ADD COMMENTS
            OPTIMIZE USE OF findPiece()
            CREATE BLOCKING FUNCTION
        */
    





    
}