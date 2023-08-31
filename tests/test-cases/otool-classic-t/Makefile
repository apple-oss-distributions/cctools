# PLATFORM: MACOS

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	# compile a test program
	${CC} -arch x86_64 -o hello hello.s -c
	${CC} -arch i386 -o hello_32 ${TESTROOT}/src/hello.c -c

	# disassemble text of a 32-bit MachO
	${OTOOLC} -xV hello_32 | ${CHECK} -p DIS32
# DIS32: hello_32:
# DIS32-NEXT: (__TEXT,__text) section
# DIS32-NEXT: _main:
# DIS32-NEXT: 00000000	pushl	%ebp

	# disassemble text
	${OTOOLC} -t hello | ${CHECK} -p DIS
# DIS: hello:
# DIS-NEXT: (__TEXT,__text) section
# DIS-NEXT: 0000000000000000 55 48 89 e5 48 83 ec 10 c7 45 fc 00 00 00 00 48

	# disassemble text verbosely
	${OTOOLC} -tv hello | ${CHECK} -p DISV
# DISV: hello:
# DISV-NEXT: (__TEXT,__text) section
# DISV-NEXT: _main:
# DISV-NEXT: 0000000000000000	pushq	%rbp
# DISV-NEXT: 0000000000000001	movq	%rsp, %rbp
# DISV-NEXT: 0000000000000004	subq	$0x10, %rsp
# DISV-NEXT: 0000000000000008	movl	$0x0, -0x4(%rbp)
# DISV-NEXT: 000000000000000f	leaq	0xf(%rip), %rdi
# DISV-NEXT: 0000000000000016	movb	$0x0, %al
# DISV-NEXT: 0000000000000018	callq	0x1d
# DISV-NEXT: 000000000000001d	xorl	%eax, %eax
# DISV-NEXT: 000000000000001f	addq	$0x10, %rsp
# DISV-NEXT: 0000000000000023	popq	%rbp
# DISV-NEXT: 0000000000000024	retq

	# disassemble text Verbosely
	${OTOOLC} -tV hello | ${CHECK} -p DISVV
# DISVV: hello:
# DISVV-NEXT:(__TEXT,__text) section
# DISVV-NEXT:_main:
# DISVV-NEXT: .*pushq	%rbp
# DISVV-NEXT: .*movq	%rsp, %rbp
# DISVV-NEXT: .*subq	$0x10, %rsp
# DISVV-NEXT: .*movl	$_main, -0x4(%rbp)
# DISVV-NEXT: .*leaq	0xf(%rip), %rdi                 ## literal pool for: "hello, world!.*
# DISVV-NEXT: .*movb	$0x0, %al
# DISVV-NEXT: .*callq	_printf
# DISVV-NEXT: .*xorl	%eax, %eax
# DISVV-NEXT: .*addq	$0x10, %rsp
# DISVV-NEXT: .*popq	%rbp
# DISVV-NEXT: .*retq

	echo PASS

clean:
	rm -f hello hello_32
