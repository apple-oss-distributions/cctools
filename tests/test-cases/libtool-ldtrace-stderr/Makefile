# PLATFORM: MACOS IOS

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	# build some libraries to consume
	${CC} -arch $(ARCH) -o foo.o -c ${TESTROOT}/src/foo.c
	${CC} -arch $(ARCH) -o bar.o -c ${TESTROOT}/src/bar.c
	${LIBTOOL} -static -o libfoo.a foo.o;
	${LIBTOOL} -static -o libbar.a bar.o;

	# verify LD_TRACE_ARCHIVES logs to stderr
	LD_TRACE_ARCHIVES=1 export LD_TRACE_ARCHIVES;			     \
	${LIBTOOL} -static -o libbaz.a libbar.a libfoo.a 2>out.txt
	$(PASS_IFF) cmp out.txt expected.txt

clean:
	rm -rf foo.o bar.o libfoo.a libbar.a libbaz.a out.txt
