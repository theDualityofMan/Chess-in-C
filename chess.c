#include "chess.h"

void setupPieces(piece * pieces[]){

    char defaultWSetup[16] = {'r', 'k', 'b', 'i', 'q', 'b', 'k', 'r', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};
    char defaultBSetup[16] = { 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'R', 'K', 'B', 'Q', 'I', 'B', 'K', 'R'};

    //ALLOCATING MEMORY HERE DUMBASS DONT FUCKING FORGET IT
    for (int i = 0; i < 32; i++){
        pieces[i] = malloc(sizeof(piece));
    }

    for (int i = 0; i < 16; i++){
        pieces[i] -> color = 'w';
        pieces[i] -> pos[0] = i%8;
        pieces[i] -> pos[1] = div(i, 8).quot;
        pieces[i] -> type = defaultWSetup[i];
        pieces[i] -> alive = true;

        //Adding a 2 jump to start for all pawns
        if(pieces[i] -> type == 'p'){
            pieces[i] -> pawnJump = true;
        } else {
            pieces[i] -> pawnJump = false;
        }
    }

    for (int i = 16; i < 32 ; i++){
        pieces[i] -> color = 'b';
        pieces[i] -> pos[0] = i%8;
        pieces[i] -> pos[1] = div(i+32, 8).quot;
        pieces[i] -> type = defaultBSetup[i-16];
        pieces[i] -> alive = true;

        if(pieces[i] -> type == 'P'){
            pieces[i] -> pawnJump = true;
        } else {
            pieces[i] -> pawnJump = false;
        }
    }
}

void freeBoard(piece * pieces[]){
    for (int i = 0; i < 32; i++){
        free(pieces[i]);
    }
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


int ifWithinBounds(int targetPos[2]){
    if(targetPos[0] >= 0 && targetPos[0] <= 7 && targetPos[1] >= 0 && targetPos[1] <= 7){
        return 0;
    }
    return 1;
}

//Determines whether the selected piece has been moved according to its moveset, 
//using the difference between its current position (curPos) and target position (targetPos)
int moveRule(piece * curPiece, int curPos[2], int targetPos[2]){
    switch(curPiece -> type){
        //white pawn
        case 'p':
            if(targetPos[0] == curPos[0] && targetPos[1] == curPos[1] + 1){
                return 0;
            } else if(targetPos[0] == curPos[0] && targetPos[1] == (curPos[1] + 2) && curPiece -> pawnJump == true){
                curPiece -> pawnJump = false;
                return 0;
            }
            break;

        //black pawn
        case 'P':
            if(targetPos[0] == curPos[0] && targetPos[1] == curPos[1] - 1){
                return 0;
            } else if(targetPos[0] == curPos[0] && targetPos[1] == (curPos[1] - 2) && curPiece -> pawnJump == true){
                curPiece -> pawnJump = false;
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
    selectedPiece -> pos[0] = targetPos[0];
    selectedPiece -> pos[1] = targetPos[1];

    return;
}


//BEFORE USING THIS FUNCTION CHECK IF PIECE IS A KNIGHT
bool checkClearPath(piece * pieces[32], int curPos[2], int tarPos[2]){
    int offsetCurPos[2];
    //VERTICAL MOVEMENT
    if(curPos[0] == tarPos[0]){
        for(int y = 1; y < abs(tarPos[1] - curPos[1]); y ++){ //  <--this part gets the absolute distance between the two points
            int offset = y * ((tarPos[1] - curPos[1])/abs(tarPos[1] - curPos[1])); //<-- this part finds which direction the next point to be searched is (toward the target point obviously)
            offsetCurPos[0] = curPos[0];
            offsetCurPos[1] = curPos[1] + offset;
            if(findPiece(pieces, offsetCurPos) != NULL){
                return false;
            }
        }
    } //HORIZONTAL
    else if (curPos[1] == tarPos[1]){
        for(int x = 1; x < abs(tarPos[0] - curPos[0]); x ++){
            int offset = x * ((tarPos[0] - curPos[0])/abs(tarPos[0] - curPos[0]));
            offsetCurPos[0] = curPos[0] + offset;;
            offsetCurPos[1] = curPos[1];
            if(findPiece(pieces, offsetCurPos) != NULL){
                return false;
            }
        }
    } 
    //DIAGONAL
    else if(abs(curPos[0] - tarPos[0]) == abs(curPos[1] - tarPos[1])){
        for(int x = 1; x < abs(tarPos[0] - curPos[0]); x ++){
            int xOffset = x * ((tarPos[0] - curPos[0])/abs(tarPos[0] - curPos[0]));
            int yOffset = x * ((tarPos[1] - curPos[1])/abs(tarPos[1] - curPos[1]));
            offsetCurPos[0] = curPos[0] + xOffset;
            offsetCurPos[1] = curPos[1] + yOffset;
            if(findPiece(pieces, offsetCurPos) != NULL){
                return false;
            }
        }
    }

    return true;

}

int main(){

    int gameOver = 0;

    piece * pieces[32];

    piece * occuPiece;

    int curPos[2], tarPos[2];

    char curPlayer, oppPlayer;

    setupPieces(pieces);

    //TESTING CODE
    while (gameOver != 1){
        printBoard(pieces);
        printf("Enter player (b or w), current x and y coords, and target x and y coords\n");
        scanf(" %c %d %d %d %d", &curPlayer, &curPos[0], &curPos[1], &tarPos[0], &tarPos[1]);

        piece * selected = findPiece(pieces, curPos);

       //First checks if selected position has a valid piece in it and is the same color as the current player, then if its within the target bounds of the board, and then if the movement 
       //is consistent with the chess rules for the selected piece
        if(selected != NULL && selected -> color == curPlayer && 
        ifWithinBounds(tarPos) == 0 && moveRule(selected, curPos, tarPos) == 0){
            occuPiece = findPiece(pieces, tarPos);
            if(occuPiece != NULL){
                if(occuPiece -> color == curPlayer){
                    //ADD CASTLE SCENARIO
                    printf("INVALID MOVE :(( Your piece is in the target space already.\n");
                } else if (checkClearPath(pieces, curPos, tarPos)) {
                    movePiece(curPlayer, pieces, curPos, tarPos);
                    occuPiece -> alive = false;
                    printf("attacked");
                } else{
                    printf("blocked");
                }
            } else if (checkClearPath(pieces, curPos, tarPos)) {
                movePiece(curPlayer, pieces, curPos, tarPos);
            } else {
                printf("blocked");
            }
        } else{
            printf("INVALID MOVE :()\n");
        }
        
    }

    //freeing dynamically allocated memory
    freeBoard(pieces);
    

    /*  TODO:
            ADD COMMENTS
            CHECK AND CHECKMATE
            START ON GAME CODE
            CREATE MOVE PREVIEW FUNCTION TO TEST BLOCKING FUNCTION
        */
    





    
}