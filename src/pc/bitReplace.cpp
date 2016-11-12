#include "nmpp.h"
extern "C" {
void nmppsBitReplace(nm64u* src,  nm64u* replacement, nm64u* dst, int size){
	for(int k=0; k<size; k++){
		nm64u* d=dst+k;
		nm64u  s=*(src+k);
		*d=0;
		for(int i=0; i<64; i++){
			int bit=s&1;
			if (bit){
				if (i&1){
					*d|=replacement[32+(i>>1)];
				} else {
					*d|=replacement[i>>1];
				}
			}
			s>>=1;
		}
	}
}
};