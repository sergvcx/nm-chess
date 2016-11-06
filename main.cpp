#include "nmpp.h"
#include <time.h>
#include <stdio.h>



typedef unsigned long long chessbits;

struct  {
	int type;
	int pos;
	int color;
} Piece;

extern "C" {
	void transposeSquares(chessbits* squares, int type, int pos);
	void detransposeSquares(chessbits* squares, int type, int pos);
	void getMoveSquares(chessbits* allTakeSquares,chessbits* takeSquares,chessbits* moveSquares);
	
}



#define QUEEN 0
#define KING 1
#define BISHOP 2
#define CASTLE 3
#define KNIGHT 4
#define ROOK 5
#define PAWN 5

#define WHITE 0
#define BLACK 1



const chessbits pureMovesTable[2][6][8*8];

void initPureMoves(chessbits const pureMovesTable[2][6][8*8]){
	

}

int main()
{
	int   blackPieces[64];
	int	  whitePieces[64];
	chessbits blackSquares;
	chessbits whiteSquares;
	chessbits allSquares=blackSquares|whiteSquares;
	
	int piece=KING;
	int pos=63; 
	
	initPureMoves(pureMovesTable);
	
	const chessbits pureMoves=pureMovesTable[WHITE][KING][63];
	chessbits takeSquares,moveSquares;
	
	chessbits allTakeSquares=allSquares&pureMoves;
	transposeSquares( &allTakeSquares,blackPieces[pos],pos);
	getMoveSquares(&allTakeSquares,&takeSquares,&moveSquares);
	detransposeSquares(&takeSquares,blackPieces[pos],pos);
	
	
	
		  
 	return 1;
}
