	.data	
	.text	
	.globl		main
main:
	lw		$t0, x
	li		$v0, 5
	syscall	
	move		$t0, $v0
	lw		$t0, x
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
x:	.word		0
