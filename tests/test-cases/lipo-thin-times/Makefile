# PLATFORM: MACOS
#
# test thining a fat file
#
# Regression tests include:
#
# rdar://49604334 verify lipo -thin doesn't zero out fractional seconds

.PHONY: all clean

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	# build a tool for reporting fractional file times
	${CC} -o times ${TESTROOT}/src/times.c

	# compile a fat file, any fat file, yours
	${CC} -arch i386 -arch x86_64 -o hello ${TESTROOT}/src/hello.c

	# thin the file
	${LIPO} -thin i386 -output hello.i386 hello

	# verify the file fractional seconds
	#
	# as a race condition, there is a sliiiight chance we'll get unlucky
	# and write a file exactly on a microsecond or nanosecond binary. not
	# really sure how to defend against that.
	./times hello.i386 | ${CHECK} -p TIMES
# TIMES: hello.i386
# TIMES-NOT:    atime: \d+	0
# TIMES-NEXT:   atime: \d+	\d+
# TIMES-NOT:    mtime: \d+	0
# TIMES-NEXT:   mtime: \d+	\d+

	echo PASS

clean:
	rm -rf times hello hello.i386
