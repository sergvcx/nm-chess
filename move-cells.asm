
//------------------------------------------------------------------------
//
//  $Workfile:: And.as $
//
//  Векторно-матричная библиотека
//
//  Copyright (c) RC Module Inc.
//
//  $Revision: 1.1 $      $Date: 2004/11/22 13:50:04 $
//
//! \if file_doc
//!
//! \file   And.asm
//! \author Сергей Мушкаев
//! \brief  Логические функции над векторами.
//!
//! \endif
//!
//------------------------------------------------------------------------


extern vec_And:label;
begin ".text_nmplv"


global _nmppsAnd_8u:label;
<_nmppsAnd_8u>
	ar5 = ar7 - 2	with gr7=false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar2,gr2;
	push ar5,gr5	with gr0=gr7;
	push ar6,gr6	with gr1=gr7;

	ar0 = [--ar5];		//	nm8u*		pSrcVec1,	// Input Buffer					:long Local[nSize]
	ar1 = [--ar5];		//	nm8u*		pSrcVec2,	// Input Buffer					:long Local[nSize]
	ar6 = [--ar5];		//	nm8u*		pDstVec,		// Output Buffer				:long Global[nSize

	rep 32 wfifo= [ar0++],ftw;
	rep 1 data = [ar0++]
	rep 1 ram  = [ar1++] with data and ram;
	rep 
	
	//figtt-cells:
	//not x & (x+1) - right bit
	
	
	// free-cells:
	//not x & (x-1)
	//not (x or -x)
	//(x & -x) -1
	
	
	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
return;
.wait;


end ".text_nmplv";
