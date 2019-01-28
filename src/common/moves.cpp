

//typedef  long long Bitmap;
//typedef  long long Beam;

class Bitmap{
	union{
		long long bits;
		unsigned char byte[8];
	} map;
	
	Bitmap operator +(int c) {
		return *this;
	}
	Bitmap operator -(int c) {
		return *this;
	}
	Bitmap operator & (Bitmap& c) {

		return *this;
	}
	Bitmap operator | (Bitmap& c) {

		return *this;
	}
	Bitmap operator ~ () {

		return *this;
	}
}
//Bitmap whitePawns;
//bitmap whitePawns;

void findRightMoves(Beam beam, Bitmap ownBits, Bitmap oppBits, Bitmap allBits, Bitmap& moveBits, Bitmap &fightBit)
{
	//Bitmap oppAllFigures){
	Bitmap x=allBits&beam;
	Bitmap y=~x&(x-1);  		// 01011000=>00000111 
	moveBits=y&beam; 		// все только ходы  
	
	fightBit = oppBits&(moveBits + 1);
	//Bitmap fightBit;
	//Bitmap bits=beam&oppAllFigures;
	//Bitmap notBits=~bits;
	//fightBit= bits&(notBits+1); // ~x & (x+1)    10100111=>00001000
	//return fightBit;
}


void findDiagFight(Beam beam, Bitmap ownBits, Bitmap oppBits, Bitmap allBits, Bitmap& moveBits, Bitmap &fightBit)
{
	//Bitmap oppAllFigures){
	Bitmap x=allBits&beam;
	Bitmap y=~x&(x-1);  		// 01011000=>00000111 
	moveBits=y&beam; 		// все только ходы  
	
	fightBit = oppBits&(moveBits + 1);
	//Bitmap fightBit;
	//Bitmap bits=beam&oppAllFigures;
	//Bitmap notBits=~bits;
	//fightBit= bits&(notBits+1); // ~x & (x+1)    10100111=>00001000
	//return fightBit;
}


// [Normal Order]:
//int leftOrder[]={
//  7  6  5  4  3  2  1  0
// 15 14 13 12 11 10  9  8 
// 23 22 21 20 19 18 17 16
// 31 30 29 28 27 26 25 24 
// 39 38 37 36 35 34 33 32
// 
//// 63 62 61 60 59 58 57 56
//
//// [Right Order]
//int rightOrder[]=
//    0  1  2  3  4  5  6  7 
//    8  9 10 11 12 13 14 15
//   16 17 18 19 20 21 22 23
//   24 25 26 27 28 29 30 31
//   32 33 34 25 26 27 28 39
//   40 41 42 43 44 45 46 47 
//   48 
////

// DownOrder
// 
void rightReverse(Bitmap* bits, Bitmap *reverseBits, int  count){
	
}
void upReverse(Bitmap* bits, Bitmap *reverseBits, int  count){
	
}
void downReverse(Bitmap* bits, Bitmap *reverseBits, int  count){
	
}


/*
struct DirBits{
	Bitmap LBits;
	Bitmap RBits;
	Bitmap UBits;
	Bitmap DBits;
	Bitmap ULBits;
	Bitmap URBits;
	Bitmap DLBits;
	Bitmap DRBits;
	
	
};*/

struct DirBits{
	Bitmap dir[8];
	int count;
};
struct Position{
	DirBits figure[16];
	DirBits beam[16];
	DirBits all;
	
	//int alive[16];
	//Bitmap LBits;
	//Bitmap RBits;
	//Bitmap UBits;
	//Bitmap DBits;
	//Bitmap ULBits;
	//Bitmap URBits;
	//Bitmap DLBits;
	//Bitmap DRBits;
};

void  push(DirBits& bits, int fig) {

}
void  pop(DirBits* bits) {

}

void reverseDirs(Bitmap bits, DirBits& dirBits) {

}
void createBeam(Bitmap bit, int fig, DirBits& dirBits) {

}
void swap(Position& ownPos, Position& oppPos) {

}
void move(int fig, Bitmap ownMove, int isFight, Position& ownPos, Position& oppPos, DirBits&  bwFigures){
	
	push(ownPos.figure[fig],fig);
	push(ownPos.beam[fig],fig);
	push(ownPos.all,0);
	push(oppPos.all,0);
	push(bwFigures,0);
	DirBits ownMoveDirs;
	reverseDirs(ownMove,ownMoveDirs);
	createBeam(ownMove,fig,ownPos.beam[fig]);
	for(int d=0; d<ownPos.figure[fig].count; d++){
		bwFigures.dir[d]^= ownPos.figure[fig].dir[d];
		bwFigures.dir[d]|= ownMoveDirs.dir[d];
		ownPos.all.dir[d]^=ownPos.figure[fig].dir[d];
		ownPos.all.dir[d]|=ownMoveDirs.dir[d];
		oppPos.all.dir[d]^=ownMoveDirs.dir[d];
		ownPos.figure[fig].dir[d]=ownMoveDirs.dir[d];
		ownPos.beam[fig].dir[d]=0;
	}
	if (isFight){
		for(int f=0; f<16; f++){
			if (oppPos.figure[f].dir[0]){
				for(int d=0;d<oppPos.figure[f].count; d++){
					oppPos.figure[f].dir[d]&=~ownMoveDirs.dir[d];
					if (oppPos.figure[f].dir[d]!=0){
						push(ownMoveDirs,f);
						push(oppPos.beam[f],f);
						oppPos.beam[f].dir[d]=0;
					}
				}
			}
		}
	}
	// завершили ход , переключились на ход оппонента
	swap(ownPos,oppPos);
	
	Bitmap ownMoves;
	Bitmap ownFight;
	
	// castle 
	for(int f=0; f<2; f++){
		if (ownPos.figure[f].dir[0]==0) 
			continue;

		for(int d=0;d<4;d++){
			findRightMoves(ownPos.beam[f].dir[d],ownPos.all.dir[d],oppPos.all.dir[d],bwFigures.dir[d], ownMoves,ownFight);
			if (ownFight)
				move(fig, ownFight,1, ownPos, oppPos, bwFigures);
			
			while(ownMoves){
				ownMove=ownMoves&~(ownMoves-1); // 01011000=>00001000
				move(f,ownMove,0,ownPos,oppPos,bwFigures)	;
				ownMoves&=(ownMoves-1); // 01011000=>01010000
			}
		}
	}
	if (isFight){
		pop(oppPos.beam);
		pop(oppPos.figure);
	}
	pop(&bwFigures);
	pop(&oppPos.all);
	pop(&ownPos.all);
	pop(ownPos.beam);
	pop(ownPos.figure);
}




