.text
.global insertL
.global printL
.type insertL, %function
.type printL, %function

@InsertL
insertL:
	push {r4-r6,lr}		@ save registers
	mov r4,r0		@ r4 = **L
	mov r5,r1		@ r5 = data
	ldr r6,[r4]             @ r6 = *L

	mov r0,#8		@ byte for malloc
	bl malloc		@ r0 points to struct
	str r5,[r0]		@ r0->data = r5
	mov r1,#0		@ r1 = NULL
	str r1,[r0,#4]		@ r0->next = NULL

	cmp r6,#0		@ if L == NULL
	beq headInsert		@ headInsert

	ldr r1,[r6]		@ r1 = L->data
	cmp r1,r5		@ if r1 - r5
	bgt headInsert		@ headInsert

 sortedInsert:
	ldr r1,[r6,#4]		@ r1 = *L->next
	cmp r1,#0		@ if r1 == NULL
	beq tailInsert		@ tailInsert

	ldr r2,[r1]		@ r2 = r1->data
	cmp r2,r5		@ if r2 - r5
	movlt r6,r1		@ r6 = r1
	blt sortedInsert	@ continue

	str r1,[r0,#4]		@ r0->next = r1
	str r0,[r6,#4]		@ r6->next = r0
	pop {r4-r6,pc}		@ return

 headInsert:
	str r6,[r0,#4]		@ r0->next = *L
	str r0,[r4]		@ **L = r0
	pop {r4-r6,pc}		@ return

 tailInsert:
	str r0,[r6,#4]		@ r6->next = r0
	pop {r4-r6,pc}

@PrintL
printL:
        push {r4, lr}           @ save registers
        mov r4, r0              @ r4 = *L
 loop:
        cmp r4,#0               @ if r4 == NULL
	beq endprint
        ldr r1,[r4]             @ r1 = *L->data
        ldr r0,=s               @ r0 = s
        bl printf               @ print r1
        ldr r1,[r4,#4]          @ r1 = *L->next
        mov r4,r1		@ r4 = r1
	b loop

 endprint:
	pop {r4,pc}           @ return
.data
s: .string "%d "
