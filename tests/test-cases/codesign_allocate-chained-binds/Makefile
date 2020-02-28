# PLATFORM: MACOS
#
# when replacing an existing code signature with a page-aligned code signature
# codesign_allocate will rewrite linkedit in order to elminate the padding
# between the string table and the code signature. If the calculation is wrong,
# the Mach-O will be corrupt:
#
# checksyms: object: hello2 malformed object (code signature data at offset 12288 with a size of 32, overlaps string table at offset 8456 with a size of 3840)
#

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	${CC} -Wl,-fixup_chains -o hello ${TESTROOT}/src/hello.c
	codesign -s - hello
	${FAIL_IF_ERROR} ${CHECKSYMS} hello
	${CS_ALLOC} -p -i hello -o hello2 -a x86_64 32
	${PASS_IFF} ${CHECKSYMS} hello2

clean:
	rm -f hello hello2
