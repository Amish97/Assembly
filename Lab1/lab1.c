#include <stdio.h>
#include <ctype.h>

/*
	ICS 51, Lab #1
	
	-----------------------------------------------------------------

	PART I: 

	Given a 32-bit integer, swap all odd bits with even bits. 
	For example, if the given number is 23 (00010111), it should be 
	converted to 43 (00101011). Every even position bit is swapped 
	with adjacent bit on right side, and every odd position bit is 
	swapped with adjacent on left side.

	Implementation details:
	The input integer is stored in registers ebx. You need to store the 
	answer into register eax.

	Hint: 
	You only need a half-dozen instructions to do this. 
	Think about "clearing" odd (even) bits and using shift (shl, shr) 
	instructions.
	
	If we have an 8-bit number 00010111, the expected output is 00101011.
	Clearing (zeroing) odd bits (7, 5, 3, 1) of 00010111 would make it 
	00010101. Now if we shift it to the left by one bit, it becomes 
	00101010. The odd bits of 00101010 are equal to the odd bits of the 
	expected output. They are "0 1 1 1".

	Using the same technique, generate the even bits of the expected 
	output and then figure out a way for combining generated odd and 
	even bits. 

	-----------------------------------------------------------------

	PART II: 
	
	You are given three integers. You need to add the smallest 
	number to the largest number and multiply that sum by two. You 
	cannot use mul/imul instructions.

	Implementation details:
	The three integers are stored in registers eax, ebx, and ecx. You 
	need to store the answer into register eax.

	Hint:
	One way of finding minimum and maximum number is to sort the numbers.

	-----------------------------------------------------------------

	IMPORTATNT NOTES:
	
	- To test your code see the "testcode" function in lab1-testing.c
	
	- Write your assembly code only in the marked blocks.
	
	- Do NOT change anything outside the marked blocks.
	
	- For part II, you are NOT allowed to use any version of the 
      MULTIPLY instruction!
	
	- Remember to fill in your name, student ID below.

	- This lab has to be done individually.
	
	- Submit ONLY lab1.c file for grading.

*/

char *yourName = "Joseph Park";
char *yourStudentID = "241369456";

/* Implement the body of this function for part I*/
int swapBits(int x)
{
	int result;

	__asm 
	{
		push ebx
		push edi
		push esi

		mov ebx, x

// YOUR CODE STARTS HERE
		mov ecx, ebx //Copy Input for modification
		and ecx, 0x55555555 //Zero out Odds
		shl ecx, 1 //Shift Left 

		//mov edx, ebx //Copy Input for modification
		and ebx, 0xAAAAAAAA //Zero out Evens
		shr ebx, 1 //Shift Right

		or ecx, ebx //Combine the Odds and Evens
		mov eax, ecx //Copy to Result
// YOUR CODE ENDS HERE
 
        mov result, eax

        pop esi
		pop edi
		pop ebx
	}
	return result;

}

/* Implement the body of this function for part II*/
void doubleMax(int a, int b, int c, int *result)
{
	__asm 
	{
		push ebx
		push edi
		push esi

		mov esi, result

		mov eax, a
		mov ebx, b
		mov ecx, c

// YOUR CODE STARTS HERE
		/*cmp eax, ebx // a < b
		jge ElseOuter

			cmp ebx, ecx //b < c
			jge ElseInner1
			//a < b < c
			add ecx, eax
			shl ecx, 1
			mov eax, ecx
			jmp EndOfIf

			ElseInner1: //c < b
				cmp eax, ecx //a < c
				jge ElseInnerInner1
				//a < c < b
				add ebx, eax
				shl ebx, 1
				mov eax, ebx
				jmp EndOfIf

				ElseInnerInner1: //c < a
				//c < a < b
				add ebx, ecx
				shl ebx, 1
				mov eax, ebx
				jmp EndOfIf

		ElseOuter://b < a
			cmp eax, ecx //a < c
			jge ElseInner2
			////b < a < c
			add ecx, ebx
			shl ecx, 1
			mov eax, ecx
			jmp EndOfIf

			ElseInner2: //c < a
				cmp ecx, ebx //c < b
				jge ElseInnerInner2
				//c < b < a
				add eax, ecx
				shl eax, 1
				jmp EndOfIf

				ElseInnerInner2: //b < c
				// b < c < a
				add eax, ebx
				shl eax, 1
				jmp EndOfIf

		EndOfIf:*/
		// Find larger of a and b
		mov edx, eax
		cmp ebx, edx
		jle ElseMax1
		mov edx, ebx
		jmp Next1

		ElseMax1:
			jmp Next1

		Next1: //Find larger of a or b and c; Max is stored in edx
			cmp ecx, edx
			jle ElseMax2
			mov edx, ecx
			jmp Min

			ElseMax2:
				jmp Min

		Min: //Find smaller of a and b
			cmp ebx, eax
			jge ElseMin1
			mov eax, ebx
			jmp Next2

			ElseMin1:
				jmp Next2

		Next2: //Find smaller of a or b and c; Min is stored in eax
			cmp ecx, eax
			jge ElseMin2
			mov eax, ecx
			jmp End

			ElseMin2:
				jmp End

		End: //Add Min and Max and store in eax; multiply eax by 2
			add eax, edx
			shl eax, 1




	
// YOUR CODE ENDS HERE

		mov [esi][0], eax

        pop esi
		pop edi
		pop ebx
	}

	return;
}