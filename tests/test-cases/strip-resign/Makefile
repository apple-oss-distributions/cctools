# PLATFORM: MACOS
# <rdar://problem/51911417> ER: Change "strip" tool to ad-hoc sign

PLATFORM = MACOS
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile
SRC = ${TESTROOT}/src
DATA = ${TESTROOT}/data

.PHONY: all clean

all:
	# test re-signing an apple signed binary
	${STRIP} ${DATA}/echo.fat -o echo 2>&1 | ${CHECK} -p APPLE
# APPLE: .*strip: warning: changes being made to the file will invalidate the code signature in: .* (for architecture x86_64)
# APPLE: .*strip: warning: changes being made to the file will invalidate the code signature in: .* (for architecture arm64e)

	# test re-signing a linker-signed binary
	${CC} -o hello ${SRC}/hello.c
	${STRIP} hello 2>&1 | ${CHECK} -p ADHOC
# ADHOC-NOT: .*strip: warning: changes being made to the file will invalidate the code signature.*
	./hello | ${CHECK} -p HELLO
# HELLO: hello, world!

	echo PASS

clean:
	rm -f echo hello

