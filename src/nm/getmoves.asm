
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

global _getMoveSquares:label;
<_getMoveSquares>
	ar5 = ar7 - 2	with gr7=false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar5,gr5	with gr0=gr7;
	push ar6,gr6	with gr1=gr7;

	ar0 = [--ar5];		//	nm64* srcBitsT
	ar1 = [--ar5];		//	nm64* takeBits
	ar6 = [--ar5];		//	nm64* moveBits

	nb1 = 0;
	rep 1 data,ram  = [ar0] with data + 1;
	rep 1 	 with afifo and not ram;
	rep 1 [ar1++]=afifo;
	
	rep 1 with ram - 1 ;
	rep 1 with afifo and not ram;
	rep 1 [ar6++] = afifo;
	
	//fight-cells:
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


end ".text";
