# PLATFORM: MACOS

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	# compile a library function
	${CC} -arch $(ARCH) -o libfoo.o $(LOCAL_CC_FLAGS) -c libfoo.c

	${FAIL_IF_ERROR} ${LIBTOOL} -static -o libfoo.a -filelist "filelist, dir/filelist"
	${PASS_IFF} true

clean:
	rm -rf libfoo.o libfoo.a
