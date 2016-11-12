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



#define QUEEN 1
#define KING 2
#define BISHOP 3
#define CASTLE 4
#define KNIGHT 5
#define ROOK 6
#define PAWN 7

#define WHITE 0
#define BLACK 1



chessbits pureMovesBase[2][6][8*8];

void nmppsCnv_32s1s(nm32s* src, nm1* dst, int size){
	for(int i=0; i<size; i++){
		nmppsPut_1(dst,i,src[i]);
	}
}

#define ONBOARD(y,dy,x,dx)  ((y+dy>=0) && (y+dy<8) && (x+dx>=0) && (x+dx<8))

void initPureMoves(chessbits pureMoves[2][6][8*8]){
	// CASTLE
	int board[8][8];
	for(int x=0; x<8; x++){
		for(int y=0;y<8; y++){
			nmppsSet_32s((nm32s*)board,0,64);
			for(int move=0; move<8; move++){
				board[y][move]=-1;
				board[move][x]=-1;
			}
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][CASTLE][y*8+x],64);
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][CASTLE][y*8+x],64);
		}
	}
	// BISHOP
	
	for(int x=0; x<8; x++){
		for(int y=0;y<8; y++){
			nmppsSet_32s((nm32s*)board,0,64);
			for(int move=0; move<8; move++){
				if (ONBOARD(y,move,x,move))
					board[y+move][x+move]=-1;
				if (ONBOARD(y,move,x,-move))
					board[y+move][x-move]=-1;
				if (ONBOARD(y,-move,x,move))
					board[y-move][x+move]=-1;
				if (ONBOARD(y,-move,x,-move))
					board[y-move][x-move]=-1;
			}
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][BISHOP][y*8+x],64);
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][BISHOP][y*8+x],64);
		}
	}
}

void pieces2chessbits(int* pieces, chessbits* bits){
	for (int i=0; i<64; i++){
		if (pieces[i])
			nmppsPut_1((nm1*)bits,i,1);
		else 
			nmppsPut_1((nm1*)bits,i,0);
	}
}



extern "C" {
	void nmppsBitReplace(nm64u* src, nm64u* replacement, nm64u* dst, int size);
	void nmppsInitBitReplace(int* newOrder, nm64u* replacement);
	void getMoveBits(chessbits* bitsT, chessbits* takeBitsT, chessbits* moveBitsT);
}
	
void inverseOrder(int* fwdOrder, int* invOrder)	{
	for(int i=0; i<64; i++){
		invOrder[fwdOrder[i]]=i;
	}
}
/*
	int newOrderFwd[64]={ 	01,02,03,04,05,06,07,8,9,
						10,11,12,13,14,15,16,17,18,19,
						20,21,22,23,24,25,26,27,28,29,
						30,31,32,33,34,35,36,37,38,39,
						40,41,42,43,44,45,46,47,48,49,
						50,51,52,53,54,55,56,57,58,59,
						60,61,62,63};
*/
int newOrderFwd[64]={ 	00,01,02,03,04,05,06,07,
8,16,32,40,48,52,60 };

	int newOrderInv[64];
int main()
{
	inverseOrder(newOrderFwd,newOrderInv);

	int   blackPieces[8][8];
	int	  whitePieces[8][8];
	nmppsSet_32s((nm32s*)blackPieces,0,64);
	nmppsSet_32s((nm32s*)whitePieces,0,64);
	
	blackPieces[0][7]=PAWN;
	blackPieces[0][6]=PAWN;
	blackPieces[0][5]=PAWN;
	blackPieces[0][4]=PAWN;
	blackPieces[4][0]=PAWN;
	blackPieces[5][0]=PAWN;
	blackPieces[6][0]=PAWN;
	blackPieces[7][0]=PAWN;
	
	chessbits blackBits;
	chessbits whiteBits;
	pieces2chessbits((int*)blackPieces,&blackBits);
	pieces2chessbits((int*)whitePieces,&whiteBits);
	chessbits allBits=blackBits|whiteBits;
	
	int piece=KING;
	int pos=63; 
	
	initPureMoves(pureMovesBase);
	
	chessbits pureMoves=pureMovesBase[WHITE][CASTLE][0];
	//return (pureMoves);
	chessbits takeSquares,moveSquares;
	
	//return (allBits);
	chessbits allTakeBits=allBits&pureMoves;
	chessbits allTakeBitsT;
	chessbits takeBitsT;
	chessbits moveBitsT;
	chessbits takeBits;
	chessbits moveBits;

	nm64u replacementFwd[64];
	nm64u replacementInv[64];
	nmppsInitBitReplace(newOrderFwd,replacementFwd);
	nmppsInitBitReplace(newOrderInv,replacementInv);
	nmppsBitReplace(&allTakeBits,replacementFwd,&allTakeBitsT,1);
	getMoveBits(&allTakeBitsT,&takeBitsT,&moveBitsT);
	nmppsBitReplace(&allTakeBitsT,replacementInv,&takeBits,1);
	nmppsBitReplace(&moveBitsT,replacementInv,&moveBits,1);
//	detransposeSquares(&takeSquares,blackPieces[pos],pos);
	
	
	
		  
 	return 1;
}
