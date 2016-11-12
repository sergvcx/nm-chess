#include "nmpp.h"
typedef unsigned long long chessbits;
extern "C"  {



void getMoveBits(chessbits* bitsT, chessbits* takeBitsT, chessbits* moveBitsT){
	nm8u* bitsT8=(nm8u*)bitsT;
	nm8u* takeBitsT8=(nm8u*)takeBitsT;
	nm8u* moveBitsT8=(nm8u*)moveBitsT;

	for (int i=0; i<8; i++){
		takeBitsT8[i]=bitsT8[i]&(-bitsT8[i]);
		moveBitsT8[i]=(bitsT8[i]-1)&(~bitsT8[i]);
	}
	
}

};