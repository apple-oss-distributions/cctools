# PLATFORM: MACOS
#
# smoke test for as, just to exercise some code
#
# This test uses the old, old gas assemblers for code generation. These
# assemblers are frozen in time, and do not support many modernisms that
# exist in the FSF code or in LLVM. For the purposes of just building some
# code, this test converts a C program into ASM, then removes unrecognized
# opcodes from the assembler.

PLATFORM = MACOS
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

.PHONY: all clean

all:
	# convert a non-trivial test binary into assembly
	${CC} -arch ${ARCH} ${TESTROOT}/src/verstool.c -S -o verstool.s -O2

	# just um remove the op codes that cctools as does not recognize :(
	cat verstool.s							     \
		| sed 's/\.build_version.*//g'				     \
		| sed 's/\.cfi_startproc.*//g'				     \
		| sed 's/\.cfi_endproc.*//g'				     \
		| sed 's/\.cfi_def_cfa_offset.*//g'			     \
		| sed 's/\.cfi_offset.*//g'				     \
		| sed 's/\.cfi_def_cfa_register.*//g'			     \
		> verstool2.s

	# build the binary
	${FAIL_IF_ERROR} ${AS} -arch ${ARCH} -Q -o verstool.o verstool2.s
	${FAIL_IF_ERROR} ${CC} -o verstool verstool.o -lsystem 2>&1

	# run the binary, if applicable
	if [ "$(ARCH)" == "x64_64" ];					     \
	then								     \
	    ./verstool 2>&1 | ${CHECK};					     \
	fi
# CHECK: usage: verstool <input> list
# CHECK:                 <input> write [(vm|bv) <platform> <minos> <sdk>] ... <output>

	echo PASS
clean:
	rm -rf verstool.s verstool2.s verstool.o verstool
