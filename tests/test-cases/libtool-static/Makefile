# PLATFORM: MACOS IOS

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	${CC} -arch $(ARCH) -o libfoo.o  $(LOCAL_CC_FLAGS) -c libfoo.c
	${LIBTOOL} -static -o libfoo.a libfoo.o
	${OTOOL} -av libfoo.a | wc -l | grep -w 3 | ${FAIL_IF_EMPTY}
	ls -l libfoo.a | sed 's/ .*//' | grep -- '-rw-r--r--' | ${FAIL_IF_EMPTY}
	${PASS_IFF_SUCCESS} true

clean:
	rm -rf libfoo.o libfoo.a
