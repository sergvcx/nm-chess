
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

global _nmppsBitReplace:label;
<_nmppsBitReplace>
	ar5 = ar7 - 2	with gr7=false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar5,gr5	with gr0=gr7;
	push ar6,gr6	with gr1=gr7;

	ar0 = [--ar5];		//	nm64* srcBits
	ar1 = [--ar5];		//	nm64* replace table wfifo-format
	ar6 = [--ar5];		//	nm64* dstBits
	gr7 = [--ar5];		//  int   size
	nb1 = 0ffffffffh;
	sb  = 0aaaaaaaah;
	gr0 = ar0;
	<next>
		rep 32 wfifo= [ar1++],ftw,wtw;
		rep 1  data,ram  = [ar0++] with vsum ,data ,0;
		
		rep 32 wfifo= [ar1++],ftw,wtw;
	
		ar0 = gr0 with gr7--;
	if <>0 delayed goto next;		
		rep 1       with vsum ,shift ram,afifo;
		rep 1 [ar6++]=afifo;
	
		
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar1,gr1;
	pop ar0,gr0;
return;
.wait;



end ".text";
