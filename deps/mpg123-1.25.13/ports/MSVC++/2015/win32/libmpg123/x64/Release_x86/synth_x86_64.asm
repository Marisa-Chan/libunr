












































































































































































































































































































































































































































































































































































































































































































	.section	.rodata



	.balign 32
maxmin_x86_64:
	.long   32767
	.long   32767
	.long   32767
	.long   32767
	.long   -32769
	.long   -32769
	.long   -32769
	.long   -32769
	.text
	.balign 16
.globl INT123_synth_1to1_x86_64_asm
INT123_synth_1to1_x86_64_asm:

	movq		%rcx, %r10
	subq		$104, %rsp 
	movaps		%xmm6, (%rsp)
	movaps		%xmm7, 16(%rsp)
	movaps		%xmm12, 32(%rsp)
	movaps		%xmm13, 48(%rsp)
	movaps		%xmm14, 64(%rsp)
	movaps		%xmm15, 80(%rsp)


	leaq		maxmin_x86_64(%rip), %rax
	movaps		(%rax), %xmm14
	movaps		16(%rax), %xmm13
	pxor		%xmm15, %xmm15
	pcmpeqd		%xmm12, %xmm12
	
	andq		$0xf, %r9
	shlq		$1, %r9
	leaq		32(%r10), %r10
	subq		%r9, %r10
	
	movl		$4, %ecx
	
	.balign 16
1:
	movups		(%r10), %xmm0
	movups		16(%r10), %xmm1
	movups		64(%r10), %xmm2
	movups		80(%r10), %xmm3
	movups		128(%r10), %xmm4
	movups		144(%r10), %xmm5
	movups		192(%r10), %xmm6
	movups		208(%r10), %xmm7
	pmaddwd		(%rdx), %xmm0
	pmaddwd		16(%rdx), %xmm1
	pmaddwd		32(%rdx), %xmm2
	pmaddwd		48(%rdx), %xmm3
	pmaddwd		64(%rdx), %xmm4
	pmaddwd		80(%rdx), %xmm5
	pmaddwd		96(%rdx), %xmm6
	pmaddwd		112(%rdx), %xmm7
	
	paddd		%xmm1, %xmm0
	paddd		%xmm3, %xmm2
	paddd		%xmm5, %xmm4
	paddd		%xmm7, %xmm6
	
	movaps		%xmm0, %xmm1
	movaps		%xmm4, %xmm3
	punpckldq	%xmm2, %xmm0
	punpckldq	%xmm6, %xmm4
	punpckhdq	%xmm2, %xmm1
	punpckhdq	%xmm6, %xmm3
	movaps		%xmm0, %xmm5
	movaps		%xmm1, %xmm7
	movlhps		%xmm4, %xmm0
	movhlps		%xmm5, %xmm4
	movlhps		%xmm3, %xmm1
	movhlps		%xmm7, %xmm3
	paddd		%xmm4, %xmm0
	paddd		%xmm3, %xmm1
	paddd		%xmm1, %xmm0
	psrad		$13, %xmm0
	
	movups		(%r8), %xmm3
	movaps		%xmm0, %xmm1
	movaps		%xmm0, %xmm2
	packssdw	%xmm0, %xmm0
	pcmpgtd		%xmm14, %xmm1
	pcmpgtd		%xmm13, %xmm2
	movhlps		%xmm3, %xmm4
	pshuflw		$0xdd, %xmm3, %xmm3
	pshuflw		$0xdd, %xmm4, %xmm4
	psrlq		$32, %xmm3
	psllq		$32, %xmm4
	por			%xmm4, %xmm3
	punpcklwd	%xmm3, %xmm0
	movups		%xmm0, (%r8)
	
	pxor		%xmm12, %xmm2
	psrld		$31, %xmm1
	psrld		$31, %xmm2
	paddd		%xmm2, %xmm1
	paddd		%xmm1, %xmm15
	
	leaq		256(%r10), %r10
	leaq		128(%rdx), %rdx
	leaq		16(%r8), %r8
	
	decl		%ecx
	jnz			1b
	
	movl		$4, %ecx
	
	.balign 16
1:
	movups		(%r10), %xmm0
	movups		16(%r10), %xmm1
	movups		64(%r10), %xmm2
	movups		80(%r10), %xmm3
	movups		128(%r10), %xmm4
	movups		144(%r10), %xmm5
	movups		192(%r10), %xmm6
	movups		208(%r10), %xmm7
	pmaddwd		(%rdx), %xmm0
	pmaddwd		16(%rdx), %xmm1
	pmaddwd		-32(%rdx), %xmm2
	pmaddwd		-16(%rdx), %xmm3
	pmaddwd		-64(%rdx), %xmm4
	pmaddwd		-48(%rdx), %xmm5
	pmaddwd		-96(%rdx), %xmm6
	pmaddwd		-80(%rdx), %xmm7
	
	paddd		%xmm1, %xmm0
	paddd		%xmm3, %xmm2
	paddd		%xmm5, %xmm4
	paddd		%xmm7, %xmm6
	
	movaps		%xmm0, %xmm1
	movaps		%xmm4, %xmm3
	punpckldq	%xmm2, %xmm0
	punpckldq	%xmm6, %xmm4
	punpckhdq	%xmm2, %xmm1
	punpckhdq	%xmm6, %xmm3
	movaps		%xmm0, %xmm5
	movaps		%xmm1, %xmm7
	movlhps		%xmm4, %xmm0
	movhlps		%xmm5, %xmm4
	movlhps		%xmm3, %xmm1
	movhlps		%xmm7, %xmm3
	paddd		%xmm4, %xmm0
	paddd		%xmm3, %xmm1
	paddd		%xmm1, %xmm0
	psrad		$13, %xmm0
	
	movups		(%r8), %xmm3
	movaps		%xmm0, %xmm1
	movaps		%xmm0, %xmm2
	packssdw	%xmm0, %xmm0
	pcmpgtd		%xmm14, %xmm1
	pcmpgtd		%xmm13, %xmm2
	movhlps		%xmm3, %xmm4
	pshuflw		$0xdd, %xmm3, %xmm3
	pshuflw		$0xdd, %xmm4, %xmm4
	psrlq		$32, %xmm3
	psllq		$32, %xmm4
	por			%xmm4, %xmm3
	punpcklwd	%xmm3, %xmm0
	movups		%xmm0, (%r8)
	
	pxor		%xmm12, %xmm2
	psrld		$31, %xmm1
	psrld		$31, %xmm2
	paddd		%xmm2, %xmm1
	paddd		%xmm1, %xmm15
	
	leaq		256(%r10), %r10
	leaq		-128(%rdx), %rdx
	leaq		16(%r8), %r8
	
	decl		%ecx
	jnz			1b
	
	pshuflw		$0xee, %xmm15, %xmm0
	movhlps		%xmm15, %xmm1
	pshuflw		$0xee, %xmm1, %xmm2
	paddd		%xmm0, %xmm15
	paddd		%xmm1, %xmm15
	paddd		%xmm2, %xmm15
	
	movd		%xmm15, %eax
	

	movaps		(%rsp), %xmm6
	movaps		16(%rsp), %xmm7
	movaps		32(%rsp), %xmm12
	movaps		48(%rsp), %xmm13
	movaps		64(%rsp), %xmm14
	movaps		80(%rsp), %xmm15
	addq		$104, %rsp

	ret


