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

		//Code in C++
		/*
		int threshold = 0x80;
		int min = 0x00;
		int max = 0xFF;
		for (int i = 0; i < dim; ++i) {
			for (int j = 0; j < dim; ++j) {
				if (image[i][j] < threshold) {
					image[i][j] = min;
				}
				else {
					image[i][j] = max;
				}
			}
		}
		*/
		//Parameters
		mov eax, image
		//mov ebx, dim;

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
				//mov ecx, 0x80000000
				mov cl, 0x80
				//and ecx, edx
				and cl, dl
				//cmp ecx, 0x00000000
				cmp cl, 0x00
				jne IF_HIGHER
				//cmp esi, 0x80
				//jae IF_HIGHER  //Instead of jge
				//mov [eax + ebx], 0x00
				xor edx, edx //set to Min
				jmp CONT
				IF_HIGHER:
					//Set to Max
					//mov esi, [eax + ebx]
					//mov edi, esi
					//xor edi, 0xFFFFFFFF
					//xor esi, edi
					or edx, 0xFFFFFFFF
					//mov [eax + ebx], 0xFF
				//xor [eax + ebx], 0xFFFFFFFF
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
		//Variables: 
		//dim / 2 = row
		//col = dim - 1
		//i , j
		//temp for i0-270
		//start
		//temp for rotating
		/*int row = dim / 2 + 1, col = dim - 1;
			int start = 0;
		for (int i = 0; i < row; ++i) {

			for (int j = start; j < col; ++j) {
				//find p90
				int x90 = j, y90 = dim - i;
				//find p180
				int x180 = dim - i, y180 = dim - j;
				//find p270
				int x270 = dim - j, y270 = i;
				int temp = image[i][j];
				image[i][j] = image[x270][y270];
				image[x270][y270] = image[x180][y180];
				image[x180][y180] = image[x90][y90];
				image[x90][y90] = temp;
			}
			++start;
			--col;
		}*/

		//REDO: Two For Loops, Rotation = 3 way XOR swap, Finding vals, Use 4 byte (32 bit) 

		//How to Find i:
		//i0: base + 4 * (row * dim + col)
		//i90: base + 4 * (col * dim + (dim - row - 1)) = base + 4 * (col * dim + dim - row - 1)
		//i180: base + 4 * ((dim - row - 1) * dim + (dim - col - 1)) = base + 4 * (dim * (dim - row) - col - 1)
		//i270: base + 4 * ((dim - col - 1) * dim + row) = base + 4 * (dim * (dim - col) - dim + row)

		//How to ROTATO: 
		//Given i0, i90, i180, i270:
		//XOR swap (i0, i270)
		//XOR swap (i270, i180)
		//XOR swap (i180, i90)
		//XOR swap (n, m) = 
			//xor n, m
			//xor m, n
			//xor n, m

		//How to iterate over Red Triangle:
		//For each row: from 0 to dim / 2 (exclusive)
		//For each column: from n to m (exclusive)
			//n begins at 0, and increases per row
			//m begins at dim - 1, and decreases per row
			//Then m = (dim - 1)-- = dim - n + 1, n++. 
			//Thus, it goes from n = 0 to dim - n + 1, n++ (exclusive)

		//Variables and Assignments:
		//esi : row index
		//edi : col index
		//eax : image
		//i0, i90, i180, i270 : Address of pixel p#
		//ebx : n & m
		//ecx, edx : Temp values

		//Load Variables
		mov eax, image //Store base in eax
		mov ebx, 0 //Store 0 (n) in ebx

		// Iterate over the red triangle (row by row starting from top left)
		mov esi, 0 //int i = 0
		BEGIN_ROW_FOR: //Row Loop
			mov edi, dim //Temporarily store dim / 2 in edi
			shr edi, 1
			cmp esi, edi //From 0 to dim / 2 (exclusive)
			jge END_ROW_FOR

			xor edi, edi //Clear edi
			mov edi, ebx //ebx -> edi
			mov edx, dim //Store dim in dedx
			sub edx, ebx //edx - ebx = dim - n + 1
			inc edx
			BEGIN_COL_FOR: //Col Loop
				cmp edi, edx // From n to dim - n + 1 (exclusive)
				jge END_COL_FOR
				
				// compute index of pixel p0, find the corresponding memory address and store it in i0
				//i0: base + 4 * (row * dim + col)
				mov i0, 0
				mov ecx, 0
				BEGIN_FOR_MUL0:
					cmp ecx, dim
					jge END_FOR_MUL0
					add i0, esi
					inc ecx
					jmp BEGIN_FOR_MUL0
				END_FOR_MUL0 :
					add i0, edi
				// compute index of pixel p90, find the corresponding memory address and store it in i90
				//i90: base + 4 * (col * dim + (dim - row - 1)) = base + 4 * (col * dim + dim - row - 1)
				mov i90, 0
				mov ecx, 0
				BEGIN_FOR_MUL90 :
					cmp ecx, dim
					jge END_FOR_MUL90
					add i90, edi
					inc ecx
					jmp BEGIN_FOR_MUL90
				END_FOR_MUL90 :
					xor ecx, ecx
					mov ecx, i90
					add ecx, dim
					sub ecx, esi
					dec ecx
					mov i90, ecx
				// compute index of pixel p180, find the corresponding memory address and store it in i180
				//i180: base + 4 * ((dim - row - 1) * dim + (dim - col - 1)) = base + 4 * (dim * (dim - row) - col - 1)
				mov i180, 0
				mov ecx, 0
				mov edx, dim
				BEGIN_FOR_MUL180dim:
					cmp ecx, dim
					jge END_FOR_MUL180dim
					add i180, edx
					inc ecx
					jmp BEGIN_FOR_MUL180dim
				END_FOR_MUL180dim :
				mov ecx, 0
				BEGIN_FOR_MUL180row :
					cmp ecx, dim
					jge END_FOR_MUL180row
					sub i180, esi
					inc ecx
					jmp BEGIN_FOR_MUL180row
				END_FOR_MUL180row :
					sub i180, edi
					dec i180
				// compute index of pixel p270, find the corresponding memory address and store it in i270
				//i270: base + 4 * ((dim - col - 1) * dim + row) = base + 4 * (dim * (dim - col) - dim + row)
				mov i270, 0
				mov ecx, 0
				BEGIN_FOR_MUL270dim:
					cmp ecx, dim
					jge END_FOR_MUL270dim
					add i270, edx
					inc ecx
					jmp BEGIN_FOR_MUL270dim
				END_FOR_MUL270dim :
				mov ecx, 0
				BEGIN_FOR_MUL270col :
					cmp ecx, dim
					jge END_FOR_MUL270col
					sub i270, edi
					inc ecx
					jmp BEGIN_FOR_MUL270col
				END_FOR_MUL270col :
					xor ecx, ecx
					mov ecx, i270
					sub ecx, dim
					add ecx, esi
					mov i270, ecx
				// rotate pixel values: p0 -> p90 -> p180 -> p270 -> p0
				//XOR swap (i0, i270)
				//XOR swap (i270, i180)
				//XOR swap (i180, i90)
				//XOR swap (n, m) = 
				//xor n, m
				//xor m, n
				//xor n, m

				mov edx, dim
				sub edx, ebx
				inc edx
				///*
				xor ecx, ecx
				mov ecx, dword ptr [eax + 4 * i0]
				//XOR swap i0, i270
				xor ecx, [eax + 4*i270]
				xor [eax + 4*i270], ecx
				xor ecx, [eax + 4*i270]
				mov dword ptr [eax + 4*i0], ecx

				xor ecx, ecx
				mov ecx, dword ptr [eax + 4 * i270]
				//XOR swap i270, i180
				xor ecx, [eax + 4 * i180]
				xor [eax + 4 * i180], ecx
				xor ecx, [eax + 4 * i180]
				mov dword ptr [eax + 4 * i270], ecx
				
				xor ecx, ecx
				mov ecx, dword ptr [eax + 4 * i180]
				//XOR swap i180, i90
				xor ecx, [eax + 4 * i90]
				xor [eax + 4 * i90], ecx
				xor ecx, [eax + 4 * i90]
				mov dword ptr [eax + 4 * i180], ecx
				//*/
				inc edi
				jmp BEGIN_COL_FOR
			END_COL_FOR: //End of Col Loop
				inc esi
				inc ebx
				jmp BEGIN_ROW_FOR
		END_ROW_FOR: //The end
			//mov image, eax
// YOUR CODE ENDS HERE
	    
		pop esi;
	    pop edi;
	    pop ebx;
	}
	
}

