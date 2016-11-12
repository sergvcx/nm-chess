
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



begin ".text"

global _replaceBits:label;
<_replaceBits>
	ar5 = ar7 - 2	with gr7=false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar5,gr5	with gr0=gr7;
	push ar6,gr6	with gr1=gr7;

	ar0 = [--ar5];		//	nm64* srcBits
	ar1 = [--ar5];		//	nm64* replace table
	ar6 = [--ar5];		//	nm64* dstBits

	nb1 = 0ffffffffh;
	sb  = 0aaaaaaaah;
	
	rep 32 wfifo= [ar0++],ftw,wtw;
	rep 1 data = [ar0++] with vsum ,data ,0;
	
	rep 32 wfifo= [ar0++],ftw,wtw;
	rep 1 data = [ar0++] with vsum ,shift data,afifo;
	
	rep 1 [ar6++]=afifo;
	
	
	//figtt-cells:
	//not x & (x+1) - right bit
	
	
	// free-cells:
	//not x & (x-1)
	//not (x or -x)
	//(x & -x) -1
	
	
	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar1,gr1;
	pop ar0,gr0;
return;
.wait;


global _transposeSquares:label;
<_transposeSquares>
	ar5 = ar7 - 2	with gr7=false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar5,gr5	with gr0=gr7;
	push ar6,gr6	with gr1=gr7;

	ar0 = [--ar5];		//	nm64* srcBits
	ar1 = [--ar5];		//	nm64* replace table
	ar6 = [--ar5];		//	nm64* dstBits

	nb1 = 0ffffffffh;
	sb  = 0aaaaaaaah;
	
	rep 32 wfifo= [ar0++],ftw,wtw;
	rep 1  data = [ar0++] with vsum ,data ,0;
	rep 32 wfifo= [ar0++],ftw,wtw;
	rep 1  data = [ar0++] with vsum ,shift data,afifo;
	rep 1 [ar6++]=afifo;
	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar1,gr1;
	pop ar0,gr0;
return;
.wait;


global _detransposeSquares:label;
<_detransposeSquares>
	ar5 = ar7 - 2	with gr7=false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar5,gr5	with gr0=gr7;
	push ar6,gr6	with gr1=gr7;

	ar0 = [--ar5];		//	nm64* srcBits
	ar1 = [--ar5];		//	nm64* replace table
	ar6 = [--ar5];		//	nm64* dstBits

	nb1 = 0ffffffffh;
	sb  = 0aaaaaaaah;
	
	rep 32 wfifo= [ar0++],ftw,wtw;
	rep 1  data = [ar0++] with vsum ,data ,0;
	rep 32 wfifo= [ar0++],ftw,wtw;
	rep 1  data = [ar0++] with vsum ,shift data,afifo;
	rep 1 [ar6++]=afifo;
	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar1,gr1;
	pop ar0,gr0;
return;
.wait;


end ".text";
