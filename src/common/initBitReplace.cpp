#include "nmpp.h"
extern "C" {
	void nmppsInitBitReplace(int* newOrder, nm64u* replacement){
		
		nmppsSet_64u(replacement,0,64);
		for(int i=0; i<64; i++){
			if (i&1)
				nmppsPut_1((nm1*)(replacement+32+(newOrder[i]>>1)),i,1);
			else 
				nmppsPut_1((nm1*)(replacement+(newOrder[i]>>1)),i,1);
		}
	}
};
