












































































































































































































































































































































































































































































































































































































































































































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
.globl INT123_synth_1to1_s_x86_64_asm
INT123_synth_1to1_s_x86_64_asm:

	movl		40(%rsp), %eax 
	subq		$168, %rsp 
	movaps		%xmm6, (%rsp)
	movaps		%xmm7, 16(%rsp)
	movaps		%xmm8, 32(%rsp)
	movaps		%xmm9, 48(%rsp)
	movaps		%xmm10, 64(%rsp)
	movaps		%xmm11, 80(%rsp)
	movaps		%xmm12, 96(%rsp)
	movaps		%xmm13, 112(%rsp)
	movaps		%xmm14, 128(%rsp)
	movaps		%xmm15, 144(%rsp)

	

	shlq		$32, %rax
	shrq		$31, %rax
	movq		%rcx, %r10






	leaq		32(%r10), %r10
	subq		%rax, %r10
	
	leaq		maxmin_x86_64(%rip), %rax
	movaps		(%rax), %xmm14
	movaps		16(%rax), %xmm13
	pxor		%xmm15, %xmm15
	pcmpeqd		%xmm12, %xmm12
	
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
	movaps		%xmm0, %xmm8
	movaps		%xmm1, %xmm9
	movaps		%xmm2, %xmm10
	movaps		%xmm3, %xmm11
	pmaddwd		(%rdx), %xmm0
	pmaddwd		16(%rdx), %xmm1
	pmaddwd		32(%rdx), %xmm2
	pmaddwd		48(%rdx), %xmm3
	pmaddwd		(%r8), %xmm8
	pmaddwd		16(%r8), %xmm9
	pmaddwd		32(%r8), %xmm10
	pmaddwd		48(%r8), %xmm11
	paddd		%xmm1, %xmm0
	paddd		%xmm3, %xmm2
	paddd		%xmm9, %xmm8
	paddd		%xmm11, %xmm10
	movaps		%xmm4, %xmm1
	movaps		%xmm5, %xmm9
	movaps		%xmm6, %xmm3
	movaps		%xmm7, %xmm11
	pmaddwd		64(%rdx), %xmm4
	pmaddwd		80(%rdx), %xmm5
	pmaddwd		96(%rdx), %xmm6
	pmaddwd		112(%rdx), %xmm7
	pmaddwd		64(%r8), %xmm1
	pmaddwd		80(%r8), %xmm9
	pmaddwd		96(%r8), %xmm3
	pmaddwd		112(%r8), %xmm11
	paddd		%xmm5, %xmm4
	paddd		%xmm7, %xmm6
	paddd		%xmm1, %xmm9
	paddd		%xmm3, %xmm11
	
	movaps		%xmm0, %xmm1
	movaps		%xmm4, %xmm3
	movaps		%xmm8, %xmm5
	movaps		%xmm9, %xmm7
	punpckldq	%xmm2, %xmm0
	punpckldq	%xmm6, %xmm4
	punpckhdq	%xmm2, %xmm1
	punpckhdq	%xmm6, %xmm3
	punpckldq	%xmm10, %xmm8
	punpckldq	%xmm11, %xmm9
	punpckhdq	%xmm10, %xmm5
	punpckhdq	%xmm11, %xmm7
	movaps		%xmm0, %xmm2
	movaps		%xmm1, %xmm6
	movaps		%xmm8, %xmm10
	movaps		%xmm5, %xmm11
	movlhps		%xmm4, %xmm0
	movhlps		%xmm2, %xmm4
	movlhps		%xmm3, %xmm1
	movhlps		%xmm6, %xmm3
	movlhps		%xmm9, %xmm8
	movhlps		%xmm10, %xmm9
	movlhps		%xmm7, %xmm5
	movhlps		%xmm11, %xmm7
	paddd		%xmm4, %xmm0
	paddd		%xmm3, %xmm1
	paddd		%xmm9, %xmm8
	paddd		%xmm7, %xmm5
	paddd		%xmm1, %xmm0
	paddd		%xmm5, %xmm8
	psrad		$13, %xmm0
	psrad		$13, %xmm8
	
	movaps		%xmm0, %xmm1
	movaps		%xmm0, %xmm2
	movaps		%xmm0, %xmm3
	movaps		%xmm8, %xmm4
	punpckldq	%xmm8, %xmm0
	punpckhdq	%xmm8, %xmm1
	packssdw	%xmm1, %xmm0
	movups		%xmm0, (%r9)
	
	pcmpgtd		%xmm14, %xmm2
	pcmpgtd		%xmm13, %xmm3
	pcmpgtd		%xmm14, %xmm4
	pcmpgtd		%xmm13, %xmm8
	packssdw	%xmm4, %xmm2
	packssdw	%xmm8, %xmm3
	pxor		%xmm12, %xmm3
	psrlw		$15, %xmm2
	psrlw		$15, %xmm3
	paddw		%xmm3, %xmm2
	paddw		%xmm2, %xmm15
	
	leaq		256(%r10), %r10
	leaq		128(%rdx), %rdx
	leaq		128(%r8), %r8
	leaq		16(%r9), %r9
	
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
	movaps		%xmm0, %xmm8
	movaps		%xmm1, %xmm9
	movaps		%xmm2, %xmm10
	movaps		%xmm3, %xmm11
	pmaddwd		(%rdx), %xmm0
	pmaddwd		16(%rdx), %xmm1
	pmaddwd		-32(%rdx), %xmm2
	pmaddwd		-16(%rdx), %xmm3
	pmaddwd		(%r8), %xmm8
	pmaddwd		16(%r8), %xmm9
	pmaddwd		-32(%r8), %xmm10
	pmaddwd		-16(%r8), %xmm11
	paddd		%xmm1, %xmm0
	paddd		%xmm3, %xmm2
	paddd		%xmm9, %xmm8
	paddd		%xmm11, %xmm10
	movaps		%xmm4, %xmm1
	movaps		%xmm5, %xmm9
	movaps		%xmm6, %xmm3
	movaps		%xmm7, %xmm11
	pmaddwd		-64(%rdx), %xmm4
	pmaddwd		-48(%rdx), %xmm5
	pmaddwd		-96(%rdx), %xmm6
	pmaddwd		-80(%rdx), %xmm7
	pmaddwd		-64(%r8), %xmm1
	pmaddwd		-48(%r8), %xmm9
	pmaddwd		-96(%r8), %xmm3
	pmaddwd		-80(%r8), %xmm11
	paddd		%xmm5, %xmm4
	paddd		%xmm7, %xmm6
	paddd		%xmm1, %xmm9
	paddd		%xmm3, %xmm11
	
	movaps		%xmm0, %xmm1
	movaps		%xmm4, %xmm3
	movaps		%xmm8, %xmm5
	movaps		%xmm9, %xmm7
	punpckldq	%xmm2, %xmm0
	punpckldq	%xmm6, %xmm4
	punpckhdq	%xmm2, %xmm1
	punpckhdq	%xmm6, %xmm3
	punpckldq	%xmm10, %xmm8
	punpckldq	%xmm11, %xmm9
	punpckhdq	%xmm10, %xmm5
	punpckhdq	%xmm11, %xmm7
	movaps		%xmm0, %xmm2
	movaps		%xmm1, %xmm6
	movaps		%xmm8, %xmm10
	movaps		%xmm5, %xmm11
	movlhps		%xmm4, %xmm0
	movhlps		%xmm2, %xmm4
	movlhps		%xmm3, %xmm1
	movhlps		%xmm6, %xmm3
	movlhps		%xmm9, %xmm8
	movhlps		%xmm10, %xmm9
	movlhps		%xmm7, %xmm5
	movhlps		%xmm11, %xmm7
	paddd		%xmm4, %xmm0
	paddd		%xmm3, %xmm1
	paddd		%xmm9, %xmm8
	paddd		%xmm7, %xmm5
	paddd		%xmm1, %xmm0
	paddd		%xmm5, %xmm8
	psrad		$13, %xmm0
	psrad		$13, %xmm8
	
	movaps		%xmm0, %xmm1
	movaps		%xmm0, %xmm2
	movaps		%xmm0, %xmm3
	movaps		%xmm8, %xmm4
	punpckldq	%xmm8, %xmm0
	punpckhdq	%xmm8, %xmm1
	packssdw	%xmm1, %xmm0
	movups		%xmm0, (%r9)
	
	pcmpgtd		%xmm14, %xmm2
	pcmpgtd		%xmm13, %xmm3
	pcmpgtd		%xmm14, %xmm4
	pcmpgtd		%xmm13, %xmm8
	packssdw	%xmm4, %xmm2
	packssdw	%xmm8, %xmm3
	pxor		%xmm12, %xmm3
	psrlw		$15, %xmm2
	psrlw		$15, %xmm3
	paddw		%xmm3, %xmm2
	paddw		%xmm2, %xmm15
	
	leaq		256(%r10), %r10
	leaq		-128(%rdx), %rdx
	leaq		-128(%r8), %r8
	leaq		16(%r9), %r9
	
	decl		%ecx
	jnz			1b
	
	movhlps		%xmm15, %xmm0
	paddw		%xmm15, %xmm0
	pshuflw		$0x55, %xmm0, %xmm1
	pshuflw		$0xaa, %xmm0, %xmm2
	pshuflw		$0xff, %xmm0, %xmm3
	paddw		%xmm1, %xmm0
	paddw		%xmm2, %xmm0
	paddw		%xmm3, %xmm0
	
	movd		%xmm0, %eax
	andl		$0xffff, %eax
	

	movaps		(%rsp), %xmm6
	movaps		16(%rsp), %xmm7
	movaps		32(%rsp), %xmm8
	movaps		48(%rsp), %xmm9
	movaps		64(%rsp), %xmm10
	movaps		80(%rsp), %xmm11
	movaps		96(%rsp), %xmm12
	movaps		112(%rsp), %xmm13
	movaps		128(%rsp), %xmm14
	movaps		144(%rsp), %xmm15
	addq		$168, %rsp

	ret


