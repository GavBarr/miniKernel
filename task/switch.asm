global context_switch

context_switch:
	mov edx, [esp + 8] ;getting the *new arg
	mov eax, [esp + 4] ;getting the *old arg
	
	;store the current vals into the old task cpu_context struct
	mov [eax + 0], edi ;0 = 0 bytes
	mov [eax + 4], esi ;4 = 4 bytes
	mov [eax + 8], ebx
	mov [eax + 12],ebp
	mov ecx, [esp]
	mov [eax + 16], ecx

	lea ecx, [esp + 12]
	mov [eax + 20], ecx


	;now load the new task cpu_context into the designated registers
	mov edi, [edx + 0]
	mov esi, [edx + 4]
	mov ebx, [edx + 8]
	mov ebp, [edx + 12]
	mov ecx, [edx + 16]
	mov esp, [edx + 20] ;switch to the new stack
	push ecx ;push new eip first
	sti
		
	ret	
