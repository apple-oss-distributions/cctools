# PLATFORM: MACOS
# <rdar://problem/51911417> ER: Change "strip" tool to ad-hoc sign

PLATFORM = MACOS
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile
SRC = ${TESTROOT}/src

.PHONY: all clean

all:
	# Codesigning support for bitcode_strip was disabled to reduce risk
	# in the GlacierPointB release. This "echo XFAIL" can be removed in
	# order to renable the re-signing logic
	echo XFAIL

	${CC} -o hello -fembed-bitcode ${SRC}/hello.c
	codesign -s - hello
	codesign --verify hello | ${CHECK} -p VERIFY
# VERIFY: .*valid on disk

	${BITCODE_STRIP} -r hello -o hello -keep_cs
	codesign --verify hello | ${CHECK} -p VERIFY

	echo PASS

clean:
	rm -f hello
