#include "nmpp.h"
#include <time.h>
#include <stdio.h>
#include <crtdbg.h>

#define MAX_DEPTH 4
#define SHOW 
typedef unsigned long long chessbits;

struct  Piece{
	int color;
	int type;
	int pos;
};
/*
struct ChessState {
	int selfRating;

	int moves;
	int takes;
	int ratio;
	//int defenses;

	int whiteMoves;
	int blackMoves;
	int whiteForce;
	int blackForce;
	int ratio;
	int lastColor;
	bool completed;
	bool operator <=(ChessState & state) {
		if (ratio && state.ratio){
			if (ratio<=state.ratio)
				ra
		}
		else {
			return (takes+moves*10<=state.takes*10+moves);
		}
		//if (whiteForce==0 && blackForce==0) return true;
		//int thisRatio  =(whiteForce-blackForce)*10+(whiteMoves-blackMoves);
		//int paramRatio =(state.whiteForce-state.blackForce)*10+(state.whiteMoves-state.blackMoves);
		//int parRatio=state.whiteForce-state.blackForce;
		//int deltaForce = (whiteForce-blackForce)-(state.whiteForce-state.blackForce);
		//int deltaMoves = (whiteMoves-blackMoves)-(state.whiteMoves-state.blackMoves);
		//return thisRatio<=paramRatio;
		//return true;
	}
	bool operator >=(ChessState & state) {
		if (ratio && state.ratio){
			return ratio>=state.ratio;
		}

		//if (whiteForce==0 && blackForce==0) return true;
		//int thisRatio  =(whiteForce-blackForce)*10+(whiteMoves-blackMoves);
		//int paramRatio =(state.whiteForce-state.blackForce)*10+(state.whiteMoves-state.blackMoves);
		//int parRatio=state.whiteForce-state.blackForce;
		//int deltaForce = (whiteForce-blackForce)-(state.whiteForce-state.blackForce);
		//int deltaMoves = (whiteMoves-blackMoves)-(state.whiteMoves-state.blackMoves);
		return thisRatio>=paramRatio;
	}
	//bool operator >=(ChessState & state) {
		//int curRatio=whiteForce-blackForce;
		//int parRatio=state.whiteForce-state.blackForce;
	//	return ratio>=state.ratio;
	//}
};

void MinState(ChessState& minState, ChessState& newState){
	if (minState.ratio && newState.ratio){
		if (minState.ratio>newState.ratio)
			minState.ratio=newState.ratio;
	}
	else {
		if (minState.takes*10+minState.moves>newState.takes*10+newState.moves){
			minState.
		}

	}
}*/
extern "C" {
	void transposeSquares(chessbits* squares, int type, int pos);
	void detransposeSquares(chessbits* squares, int type, int pos);
	void getMoveSquares(chessbits* allTakeSquares,chessbits* takeSquares,chessbits* moveSquares);
	
}


#define QUEEN  1
#define ROOK   2
#define BISHOP 3
#define KNIGHT 4
#define PAWN   5
#define PAWNV  6
#define KING   7


#define WHITE 0
#define BLACK 1

char whiteLook[8]={'.','Q','R','B','K','P','P','K'};
char blackLook[8]={'.','q','r','b','k','p','p','k'};

chessbits  pureMovesBase[2][7][8*8];
nm64u replacementFwdBase[2][6][8*8][64];
nm64u replacementInvBase[2][6][8*8][64];
struct {
	int from;
	int to;
} bestMoves[10];

void nmppsCnv_32s1s(nm32s* src, nm1* dst, int size){
	for(int i=0; i<size; i++){
		nmppsPut_1(dst,i,src[i]);
	}
}

//#define ONBOARD(y,dy,x,dx)  ((y+dy>=0) && (y+dy<8) && (x+dx>=0) && (x+dx<8))
#define ONBOARD(y,x)  ((y>=0) && (y<8) && (x>=0) && (x<8))

// „лЯ каждой позиции длЯ каждой фигуры устанавливает битовую карту возможных ходов 
void initPureMovesBase(chessbits pureMoves[2][7][8*8]){
	// CASTLE
	int board[8][8];
	for(int y=0; y<8; y++){
		for(int x=0; x<8; x++){
			nmppsSet_32s((nm32s*)board,0,64);
			for(int move=0; move<8; move++){
				board[y][move]=-1;
				board[move][x]=-1;
			}
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][ROOK][y*8+x],64);
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][ROOK][y*8+x],64);
		}
	}
	
	// BISHOP
	for(int y=0; y<8; y++){
		for(int x=0; x<8; x++){
			nmppsSet_32s((nm32s*)board,0,64);
			for(int move=0; move<8; move++){
				if (ONBOARD(y+move, x+move))
					board  [y+move][x+move]=-1;
				if (ONBOARD(y+move, x-move))
					board  [y+move][x-move]=-1;
				if (ONBOARD(y-move, x+move))
					board  [y-move][x+move]=-1;
				if (ONBOARD(y-move, x-move))
					board  [y-move][x-move]=-1;
			}
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][BISHOP][y*8+x],64);
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][BISHOP][y*8+x],64);
		}
	}
	
	// KNIGHT
	for(int x=0; x<8; x++){
		for(int y=0;y<8; y++){
			nmppsSet_32s((nm32s*)board,0,64);
			if (ONBOARD(y+2,x+1))
				board[y+2][x+1]=-1;
			if (ONBOARD(y+1,x+2))
				board[y+1][x+2]=-1;
			if (ONBOARD(y-1,x+2))
				board[y-1][x+2]=-1;
			if (ONBOARD(y-2,x+1))
				board[y-2][x+1]=-1;
			if (ONBOARD(y-2,x-1))
				board[y-2][x-1]=-1;
			if (ONBOARD(y-1,x-2))
				board[y-1][x-2]=-1;
			if (ONBOARD(y+1,x-2))
				board[y+1][x-2]=-1;
			if (ONBOARD(y+2,x-1))
				board[y+2][x-1]=-1;
			
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][KNIGHT][y*8+x],64);
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][KNIGHT][y*8+x],64);
		}
	}
	// QUEEN
	for(int x=0; x<8; x++){
		for(int y=0;y<8; y++){
			pureMoves[WHITE][QUEEN][y*8+x]=pureMoves[WHITE][ROOK][y*8+x]|pureMoves[WHITE][BISHOP][y*8+x];
			pureMoves[BLACK][QUEEN][y*8+x]=pureMoves[WHITE][QUEEN][y*8+x];
		}
	}

	//          ^(y)
	//          |
	//          |
	// (x)<-----'

	// KING
	for(int x=0; x<8; x++){
		for(int y=0;y<8; y++){
			nmppsSet_32s((nm32s*)board,0,64);
			if (ONBOARD(y+0, x+1))
				board  [y+0][x+1]=-1;
			if (ONBOARD(y+1, x+1))
				board  [y+1][x+1]=-1;
			if (ONBOARD(y+1, x+0))
				board  [y+1][x+0]=-1;
			if (ONBOARD(y+1, x-1))
				board  [y+1][x-1]=-1;
			if (ONBOARD(y+0, x-1))
				board  [y+0][x-1]=-1;
			if (ONBOARD(y-1, x-1))
				board  [y-1][x-1]=-1;
			if (ONBOARD(y-1, x-0))
				board  [y-1][x-0]=-1;
			if (ONBOARD(y-1, x+1))
				board  [y-1][x+1]=-1;
			
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][KING][y*8+x],64);
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][KING][y*8+x],64);
		}
	}	
	// PAWN WHITE
	for(int x=0; x<8; x++){
		for(int y=0;y<8; y++){
			nmppsSet_32s((nm32s*)board,0,64);
			if (ONBOARD(y+1, x)) {
				board  [y+1][x]=-1;
				if (y==1){
					board  [y+2][x]=-1;
				}
				nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][PAWN][y*8+x],64);
			}
			nmppsSet_32s((nm32s*)board,0,64);
			if (ONBOARD(y+1, x+1)) 
				board  [y+1][x+1]=-1;
			if (ONBOARD(y+1, x-1)) 
				board  [y+1][x-1]=-1;
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[WHITE][PAWNV][y*8+x],64);
		}
	}	
	// PAWN BLACK
	for(int x=0; x<8; x++){
		for(int y=0;y<8; y++){
			nmppsSet_32s((nm32s*)board,0,64);
			if (ONBOARD(y-1, x)) {
				board  [y-1][x]=-1;
				if (y==6){
					board  [y-2][x]=-1;
				}
				nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][PAWN][y*8+x],64);
			}
			nmppsSet_32s((nm32s*)board,0,64);
			if (ONBOARD(y-1, x+1)) 
				board  [y-1][x+1]=-1;
			if (ONBOARD(y-1, x-1)) 
				board  [y-1][x-1]=-1;
			nmppsCnv_32s1s((nm32s*)board,(nm1*)&pureMoves[BLACK][PAWNV][y*8+x],64);
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
	nmppsSet_32s(invOrder,-1,64);
	for(int i=0; i<64; i++){
		if (fwdOrder[i]>=0)
			invOrder[fwdOrder[i]]=i;
	}
	
}


void initOrderFwd(int piece, int Y, int X, int* order)
{
	//          ^(y)
	//          |
	//          |
	// (x)<-----'
	nmppsSet_32s(order,-1,64);
	switch (piece)	{
		case ROOK :
			for(int x=X+1,i=0; x<8;  x++,i++){
				order[8*0+i]=Y*8+x;
			}
			for(int y=Y+1,i=0; y<8;  y++,i++){
				order[8*1+i]=y*8+X;
			}
			for(int x=X-1,i=0; x>=0; x--,i++){
				order[8*2+i]=Y*8+x;
			}
			for(int y=Y-1,i=0; y>=0; y--,i++){
				order[8*3+i]=y*8+X;
			}

			break;
		case BISHOP:
			for(int x=X+1,y=Y+1,i=0; x<8&y<8 ;  x++,y++,i++){
				order[8*0+i]=y*8+x;
			}
			for(int x=X+1,y=Y-1,i=0; x<8&y>=0;  x++,y--,i++){
				order[8*1+i]=y*8+x;
			}
			for(int x=X-1,y=Y+1,i=0; x>=0&y<8;  x--,y++,i++){
				order[8*2+i]=y*8+x;
			}
			for(int x=X-1,y=Y-1,i=0; x>=0&y>=0; x--,y--,i++){
				order[8*3+i]=y*8+x;
			}
			break;
	/*		
		case QUEEN:
			for(int x=X+1,i=0; x<8;  x++,i++){
				order[8*0+i]=Y*8+x;
			}
			for(int y=Y+1,i=0; y<8;  y++,i++){
				order[8*1+i]=y*8+X;
			}
			for(int x=X-1,i=0; x>=0; x--,i++){
				order[8*2+i]=Y*8+x;
			}
			for(int y=Y-1,i=0; y>=0; y--,i++){
				order[8*3+i]=y*8+X;
			}
			for(int x=X+1,y=Y+1,i=0; x<8&y<8 ;  x++,y++,i++){
				order[8*4+i]=y*8+x;
			}
			for(int x=X+1,y=Y-1,i=0; x<8&y>=0;  x++,y--,i++){
				order[8*5+i]=y*8+x;
			}
			for(int x=X-1,y=Y+1,i=0; x>=0&y<8;  x--,y++,i++){
				order[8*6+i]=y*8+x;
			}
			for(int x=X-1,y=Y-1,i=0; x>=0&y>=0; x--,y--,i++){
				order[8*7+i]=y*8+x;
			}
		
		case KNIGHT:
		//	if ((X+1)<8 && (Y+2<8))
		//	order[0]=
			break;*/
		default:
			for(int i=0; i<64; i++)
				order[i]=i;
			break;
	}
}


void initReplacementBase(nm64u replacementFwdBase[2][6][8*8][64], nm64u replacementInvBase[2][6][8*8][64])
{
	int orderFwd[64];
	int orderInv[64];
	for(int pieceColor=0; pieceColor<2; pieceColor++){
		for(int pieceType=0; pieceType<6; pieceType++){
			for(int y=0; y<8; y++){
				for(int x=0; x<8; x++){
					initOrderFwd(pieceType,y,x,orderFwd);
					inverseOrder(orderFwd,orderInv);
					nmppsInitBitReplace(orderFwd,replacementFwdBase[pieceColor][pieceType][y*8+x]);
					nmppsInitBitReplace(orderInv,replacementInvBase[pieceColor][pieceType][y*8+x]);
				}
			}
		}
	}
}

int newOrderInv[64];
void showBits(chessbits bits, char *text, char symbol='*')
{
	printf("%s\n",text);
	for(int y=7;y>=0;y--){
		for(int x=7; x>=0; x--){
			int bit=nmppsGet_1((nm1*)&bits,y*8+x);
			if (bit)
				printf("%c",symbol);
			else 
				printf(".");
		}
		printf("\n");
	}
}

void showChess(int* white, int* black, char* text)
{
	char w,b;
	printf("%s\n",text);
	for(int y=7;y>=0;y--){
		for(int x=7; x>=0; x--){
			if  (w=white[y*8+x])
				printf("%c",whiteLook[w]);
			else
				printf("%c",blackLook[black[y*8+x]]);
		}
		printf("\n");
	}

}
void showBitsWB(chessbits whiteBits, chessbits blackBits, char *text)
{
	printf("%s\n",text);
	for(int y=7;y>=0;y--){
		for(int x=7; x>=0; x--){
			int whiteBit=nmppsGet_1((nm1*)&whiteBits,y*8+x);
			int blackBit=nmppsGet_1((nm1*)&blackBits,y*8+x);
			_ASSERTE( (whiteBits & blackBits)==0);
			if (whiteBit)
				printf("%c",'w');
			else if (blackBit)
				printf("%c",'b');
			else
				printf(".");
		}
		printf("\n");
	}
}

void showMoves(int count)
{
	for(int i=0; i<count; i++){
		printf("%d,%d => %d,%d\n",bestMoves[i].from/8,bestMoves[i].from%8,bestMoves[i].to/8,bestMoves[i].to%8);

	}

}
void whatCanPieceDo(Piece* piece , chessbits allBits,  chessbits whiteBits,  chessbits blackBits, chessbits* takeBits, chessbits* moveBits)
{
	chessbits& pureMoves=pureMovesBase[piece->color][piece->type][piece->pos];
	chessbits allTakeBits=allBits&pureMoves;
	chessbits allTakeBitsT;
	chessbits takeBitsT;
	chessbits moveBitsT;

	if (piece->type<=BISHOP){
		nmppsBitReplace(&allTakeBits,replacementFwdBase[piece->color][piece->type][piece->pos],&allTakeBitsT,1);
		getMoveBits(&allTakeBitsT,&takeBitsT,&moveBitsT);
		nmppsBitReplace(&moveBitsT,replacementInvBase[piece->color][piece->type][piece->pos],moveBits,1);
		nmppsBitReplace(&takeBitsT,replacementInvBase[piece->color][piece->type][piece->pos],takeBits,1);
		if (piece->color==WHITE)
			(*takeBits)&=blackBits;
		else 
			(*takeBits)&=whiteBits;
	}
	else {
		*moveBits=(~allBits)&pureMoves;
		if (piece->color==WHITE)
			(*takeBits)=pureMoves&blackBits;
		else 
			(*takeBits)=pureMoves&whiteBits;

	}
}

int countBits(chessbits bits)
{
	int count=0;
	for (int i=0; i<64; i++){
		count+=(bits&1);
		bits>>=1;
	}
	return count;
}

int estimatePosition(chessbits takeBits, chessbits moveBits)
{

	return 1;
}

int   blackPieces[64];//8][8];
int	  whitePieces[64];//8][8];

int whiteMove(int& );

//ChessState chessState={0,0,0,0,-1,false};
int moveDepth=0;
int  blackMove(int& blackSelfRating)
{
	int whiteSelfRating;
	int minWhiteSelfRating=999;
	int minRatio=999;
	int ratio;
	//ChessState minChessState={0,0,0,0,999,-1,false};
	moveDepth++;
	chessbits blackBits;
	chessbits whiteBits;
	pieces2chessbits(blackPieces,&blackBits);
	pieces2chessbits(whitePieces,&whiteBits);
	chessbits allBits=blackBits|whiteBits;

	char out[100];
	
	//showChess(whitePieces,blackPieces, out);
	

	Piece piece;
	piece.color=BLACK;
	int totalMoves=0;
	int totalTakes=0;
	int totalForce=0;
	for(int i=0; i<64; i++)	{
		if (blackPieces[i]){
			piece.type=blackPieces[i];
			piece.pos=i;
			chessbits takeBits;
			chessbits moveBits;

			whatCanPieceDo(&piece,allBits,whiteBits,blackBits,&takeBits,&moveBits);
			
			totalForce++;
			totalMoves+=countBits(moveBits);
			if (moveDepth<MAX_DEPTH)
				for(int move=0; move<64; move++){
					if ((moveBits>>move)&1){
						
						blackPieces[i]=0;
						blackPieces[move]=piece.type;
#ifdef SHOW						
						sprintf(out,"black move.. %d",moveDepth);
						showChess(whitePieces,blackPieces,out);
#endif
						int ratio=whiteMove(whiteSelfRating);
						if (ratio>-999){
							if (minRatio>=ratio){
								minRatio=ratio;
								bestMoves[moveDepth].from=i;
								bestMoves[moveDepth].to=move;
							}
						}
						else {
							if (minWhiteSelfRating>=whiteSelfRating)
								minWhiteSelfRating =whiteSelfRating;
						}


						blackPieces[move]=0;
  						blackPieces[i]=piece.type;
					}
				}
			int countTakeBits=countBits(takeBits);
			totalTakes+=countTakeBits;
			if (countTakeBits)
				for (int take=0; take<64; take++){
					if ((takeBits>>take)&1){
						totalTakes++;
						blackPieces[i]=0;
						blackPieces[take]=piece.type;
						int whiteDead=whitePieces[take];
						whitePieces[take]=0;
						_ASSERTE(whiteDead);
#ifdef SHOW
						sprintf(out,"black take.. %d",moveDepth);
						showChess(whitePieces,blackPieces,out);
#endif
						int ratio=whiteMove(whiteSelfRating);
						if (ratio>-999){
							if (minRatio>=ratio){
								minRatio =ratio;
								bestMoves[moveDepth].from=i;
								bestMoves[moveDepth].to=take;
							}
						}
						else {
							if (minWhiteSelfRating>=whiteSelfRating)
								minWhiteSelfRating =whiteSelfRating;
						}


						blackPieces[take]=0;
						blackPieces[i]=piece.type;
						whitePieces[take]=whiteDead;
					}
				}
			//if (!minChessState.completed){
			//	if (minChessState.blackMoves==0)
			//		minChessState.blackMoves+=countBits(moveBits);
			//}
			//if (minChessState.blackForce==0)
			//	minChessState.blackForce++;

		}
	}
	blackSelfRating=totalMoves+totalTakes+totalForce*10;
	if (minWhiteSelfRating!=999){
		ratio=minWhiteSelfRating-blackSelfRating;
		if (minRatio>=ratio)
			minRatio =ratio;
	}

	//if (!minChessState.completed){
	//	minChessState.whiteForce=countBits(whiteBits);
	//	minChessState.blackForce=countBits(blackBits);
	//	minChessState.ratio=minChessState.whiteForce-minChessState.blackForce;
	//	minChessState.completed=true;
	//}

	moveDepth--;
	return minRatio;
}


int whiteMove(int& whiteSelfRating)
{
	int blackSelfRating;
	int minBlackSelfRating=999;
	int maxRatio=-999;
	int ratio;

	moveDepth++;
	chessbits blackBits,whiteBits;
	pieces2chessbits((int*)blackPieces,&blackBits);
	pieces2chessbits((int*)whitePieces,&whiteBits);
	chessbits allBits=blackBits|whiteBits;

	char out[100];
	sprintf(out,"white thinking.. %d",moveDepth);
	//showBitsWB(whiteBits,blackBits, out);
	//showChess(whitePieces,blackPieces,out);
	
	Piece piece={WHITE,0,0};
	int totalMoves=0;
	int totalTakes=0;
	int totalForce=0;
	
	//int minBlackRatio=9999;
	//int bestWhiteMove=9999;
	//int bestWhitePiece=9999;

	//ChessState maxChessState={0,0,0,0,-999,-1,false};

	for(int i=0; i<64; i++)	{
		if (whitePieces[i]){
			piece.type=whitePieces[i];
			piece.pos=i;
			chessbits takeBits,moveBits;

			
			whatCanPieceDo(&piece,allBits,whiteBits,blackBits,&takeBits,&moveBits);
			totalForce++;
			totalMoves+=countBits(moveBits);
			if (moveDepth<MAX_DEPTH)
				for(int move=0; move<64; move++){
					if ((moveBits>>move)&1){
						whitePieces[i]=0;
						whitePieces[move]=piece.type;
#ifdef SHOW
						sprintf(out,"WHITE MOVE.. %d",moveDepth);
						showChess(whitePieces,blackPieces,out);
#endif
						ratio=blackMove(blackSelfRating);
						if (ratio<999){	// Если рейтинг вернулся, запоминаем ход который дает максимум рейтинга
							if (maxRatio<=ratio){
								maxRatio =ratio;
								bestMoves[moveDepth].from=i;
								bestMoves[moveDepth].to=move;
							}
						}
						else {	// Запоминаем минимальный селф-рейтинг черных
							if (minBlackSelfRating>=blackSelfRating){
								minBlackSelfRating =blackSelfRating;
								//bestMoves[moveDepth].from=i;
								//bestMoves[moveDepth].to=move;
							}
						}


						//if (blackRatio<minBlackRatio){
						//	minBlackRatio=blackRatio;
						//	bestWhiteMove=move;
						//	bestWhitePiece=i;
						//}
						whitePieces[move]=0;
						whitePieces[i]=piece.type;

					}
				}
			int countTakes=countBits(takeBits);
			totalTakes+=countTakes;
			if (countTakes)
				for (int take=0; take<64; take++){
					if ((takeBits>>take)&1){
						whitePieces[i]=0;
						whitePieces[take]=piece.type;
						int blackDead=blackPieces[take];
						blackPieces[take]=0;
						_ASSERTE(blackDead);
#ifdef SHOW
						sprintf(out,"WHITE MOVE take.. %d",moveDepth);
						showChess(whitePieces,blackPieces,out);
#endif
						ratio=blackMove(blackSelfRating);
						if (ratio<999){
							if (maxRatio<=ratio){
								maxRatio =ratio;
								bestMoves[moveDepth].from=i;
								bestMoves[moveDepth].to=take;
							}
						}
						else {
							if (minBlackSelfRating>=blackSelfRating)
								minBlackSelfRating =blackSelfRating;
						}


						//if (blackRatio<minBlackRatio){
						//	minBlackRatio=blackRatio;
						//	bestWhiteMove=take;
						//	bestWhitePiece=i;
						//}
						whitePieces[take]=0;
						whitePieces[i]=piece.type;
						blackPieces[take]=blackDead;
					}
				}
			//if (!maxChessState.completed){
			//	if (maxChessState.whiteMoves==0)
			//		maxChessState.whiteMoves+=countBits(moveBits);
			//}

		}
	}
	//if (!maxChessState.completed){
	//	maxChessState.whiteForce=countBits(whiteBits);
	//	maxChessState.blackForce=countBits(blackBits);
	//	maxChessState.ratio=maxChessState.whiteForce-maxChessState.blackForce;
	//	maxChessState.completed=true;
	//}
	whiteSelfRating=totalMoves+totalTakes+totalForce*10;
	if (minBlackSelfRating!=999){
		ratio=whiteSelfRating-minBlackSelfRating;
		if (maxRatio<=ratio)
			maxRatio =ratio;
	}

	moveDepth--;
	return maxRatio; // return -999 if not ready
}

void init2Kinights()
{
	whitePieces[4*8+4]=KNIGHT;
	whitePieces[5*8+2]=ROOK;
	blackPieces[5*8+5]=KNIGHT;
}

void int2Bishops2Rooks(){
	whitePieces[0*8+5]=ROOK;
	whitePieces[0*8+4]=ROOK;
	whitePieces[1*8+0]=BISHOP;
	whitePieces[0*8+1]=BISHOP;
	blackPieces[6*8+2]=ROOK;
	blackPieces[6*8+3]=ROOK;
	blackPieces[7*8+1]=BISHOP;
	blackPieces[7*8+7]=BISHOP;
}

int main()
{
	
	initPureMovesBase(pureMovesBase);
	initReplacementBase(replacementFwdBase, replacementInvBase);


	nmppsSet_32s((nm32s*)blackPieces,0,64);
	nmppsSet_32s((nm32s*)whitePieces,0,64);
	
/*	whitePieces[1*8+0]=PAWN;
                 
	blackPieces[0*8+7]=PAWN;
	blackPieces[0*8+6]=PAWN;
	blackPieces[1*8+5]=PAWN;
	blackPieces[0*8+4]=PAWN;
	blackPieces[0*8+3]=PAWN;
	//blackPieces[0][2]=PAWN;
	//blackPieces[0][1]=PAWN;
	blackPieces[0*8+0]=PAWN;
	//blackPieces[1][0]=PAWN;
	//blackPieces[2][0]=PAWN;
	blackPieces[3*8+0]=PAWN;
	blackPieces[4*8+0]=PAWN;
	//blackPieces[5][0]=PAWN;
	blackPieces[6*8+0]=PAWN;
	blackPieces[7*8+0]=PAWN;
*/

	init2Kinights();

	showChess(whitePieces,blackPieces,"0");
	int whiteSelfRating;
	int ratio=whiteMove(whiteSelfRating);
	showChess(whitePieces,blackPieces,"0");
	showMoves(10);
/*
	chessbits blackBits;
	chessbits whiteBits;
	chessbits takeBits;
	chessbits moveBits;

	pieces2chessbits((int*)blackPieces,&blackBits);
	pieces2chessbits((int*)whitePieces,&whiteBits);
	chessbits allBits=blackBits|whiteBits;
	
	
	
	
	
	Piece fig={WHITE,ROOK,63};
	whatCanPieceDo( &fig, allBits,  whiteBits,  blackBits, &takeBits, &moveBits);

	showBits(allBits,"\nallBits",'o');
	showBits(whiteBits,"\nwhiteBits",'w');
	showBits(blackBits,"\nblackBits",'b');
	showBits(takeBits,"\ntakeBits",'*');
	showBits(moveBits,"\nmoveBits",'#');

*/
	
		  
 	return 0;//takeBits;
}
