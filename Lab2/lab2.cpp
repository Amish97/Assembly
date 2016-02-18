// Remember to fill in your name and student ID below.
char *yourName = "Joseph Park";
char *yourStudentID = "24136956";


// Part1) This function takes in a square sized grayscale image and applies thresholding on each pixel.
// Width and height of the image are equal to "dim".
void imageThresholding(unsigned char* image, int dim) {
	__asm {
		push ebx;
		push edi;
		push esi;

// YOUR CODE STARTS HERE
	// ElementAddress = BaseAddress + (ElementSizeInBytes * Offset)
	// Offset = (RowIndex * NumberOfColumns) + ColumnIndex
		mov eax, image

		mov edi, 0
		BEGIN_FOR_ROW:
			cmp edi, dim
			jge END_FOR_ROW
			mov esi, 0

			BEGIN_FOR_COL:
				cmp esi, dim
				jge END_FOR_COL
				mov ebx, 0 //Transfer row to ebx

				mov edx, 0 //Multiply row by dim (add row to row dim times)
				BEGIN_FOR_MUL:
					cmp edx, dim
					jge END_FOR_MUL
					add ebx, edi
					inc edx
					jmp BEGIN_FOR_MUL
				END_FOR_MUL:

				add ebx, esi
				xor edx, edx
				mov dl, [eax + ebx] 
				mov cl, 0x80
				and cl, dl
				cmp cl, 0x00
				jne IF_HIGHER
				xor edx, edx //set to Min
				jmp CONT
				IF_HIGHER:
					or edx, 0xFFFFFFFF
				CONT:
					mov [eax + ebx], dl
					inc esi
					jmp BEGIN_FOR_COL

			END_FOR_COL:
				inc edi
				jmp BEGIN_FOR_ROW

		END_FOR_ROW: 
			mov image, eax
// YOUR CODE ENDS HERE
		
		pop esi;
		pop edi;
		pop ebx;
	}
	
}


// Part2) This function rotates a given square sized color image 90 degrees clockwise.
// Width and height of the image are equal to "dim".
// Four variables are defined to simplify your implementation.
// Note that you cannot define additional variables.
void imageRotation(unsigned int* image, int dim) {
	
	unsigned int i0, i90, i180, i270;

	__asm {
		push ebx;
		push edi;
		push esi;

// YOUR CODE STARTS HERE
		//Load Variables
		mov ecx, image
		mov ebx, 0 //n

		//Begin Loops
		mov esi, 0
		BEGIN_ROW: //From 0 -> (dim / 2) + 1
			mov edx, dim //dim / 2 + 1
			shr edx, 1
			inc edx

			cmp esi, edx
			jge END_ROW

			mov edi, ebx
		BEGIN_COL: //From n -> dim - n - 1
			mov edx, dim
			sub edx, ebx
			dec edx
			cmp edi, edx
			jge END_COL

		//Find Addresses
		//i0: base + 4 * (row * dim + col)
			mov eax, esi
			mov edx, dim
			mul edx
			add eax, edi
			shl eax, 2
			add eax, ecx
			mov i0, eax
		//i90: base + 4 * (col * dim + (dim - row - 1)) = base + 4 * (col * dim + dim - row - 1)
			mov eax, edi
			mov edx, dim
			mul edx
			add eax, dim
			sub eax, esi
			dec eax
			shl eax, 2
			add eax, ecx
			mov i90, eax
		//i180: base + 4 * ((dim - row - 1) * dim + (dim - col - 1)) = base + 4 * (dim * (dim - row) - col - 1)
			mov eax, dim
			sub eax, esi
			mov edx, dim
			mul edx
			sub eax, edi
			dec eax
			shl eax, 2
			add eax, ecx
			mov i180, eax
		//i270: base + 4 * ((dim - col - 1) * dim + row) = base + 4 * (dim * (dim - col) - dim + row)
			mov eax, dim
			sub eax, edi
			mov edx, dim
			mul edx
			sub eax, dim
			add eax, esi
			shl eax, 2
			add eax, ecx
			mov i270, eax
		//Rotato
			mov edx, i0
			mov edx, [edx]

			mov eax, i0
			mov ecx, i270
			mov ecx, [ecx]
			mov [eax], ecx

			mov eax, i270
			mov ecx, i180
			mov ecx, [ecx]
			mov [eax], ecx

			mov eax, i180
			mov ecx, i90
			mov ecx, [ecx]
			mov [eax], ecx

			mov eax, i90
			mov [eax], edx

			mov ecx, image
			inc edi
			jmp BEGIN_COL
		END_COL:
			inc esi
			inc ebx
			jmp BEGIN_ROW
		END_ROW:
			mov image, ecx
// YOUR CODE ENDS HERE
	    
		pop esi;
	    pop edi;
	    pop ebx;
	}
	
}

