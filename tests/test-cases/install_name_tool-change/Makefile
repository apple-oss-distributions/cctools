# PLATFORM: MACOS

PLATFORM = MACOS
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

.PHONY: all clean

CC_ARM := ${CC} -arch arm64

all:
	# Build some dylibs and client
	${CC_ARM} -dynamiclib -o one.dylib ${TESTSRC}/foo.c -DFOO=one
	${CC_ARM} -dynamiclib -o two.dylib ${TESTSRC}/foo.c -DFOO=one
	${CC_ARM} -o client client.c one.dylib

	# Verify the LC_ID_DYLIB load commands are set.
	${OTOOLC} -lv one.dylib | grep LC_ID_DYLIB -A2 | ${CHECK} -p ONE
	# ONE:          cmd LC_ID_DYLIB
	# ONE:      cmdsize 40
	# ONE:         name one.dylib (offset 24)
	${OTOOLC} -lv two.dylib | grep LC_ID_DYLIB -A2 | ${CHECK} -p TWO
	# TWO:          cmd LC_ID_DYLIB
	# TWO:      cmdsize 40
	# TWO:         name two.dylib (offset 24)

	# Verify the LC_LOAD_DYLIB load command is set.
	${OTOOLC} -lv client | grep LC_LOAD_DYLIB -A2 | ${CHECK} -p ONE_LOAD
	# ONE_LOAD:          cmd LC_LOAD_DYLIB
	# ONE_LOAD:      cmdsize 40
	# ONE_LOAD:         name one.dylib (offset 24)

	# Verify the code signature
	codesign --verify --verbose client 2>&1

	# Switch the dylib from one to two (note they have the same symbols).
	${INSTALL_NAME_TOOL} -change one.dylib two.dylib client
	${OTOOLC} -lv client | grep LC_LOAD_DYLIB -A2 | ${CHECK} -p TWO_LOAD
	# TWO_LOAD:          cmd LC_LOAD_DYLIB
	# TWO_LOAD:      cmdsize 40
	# TWO_LOAD:         name two.dylib (offset 24)

	# Verify the code signature
	codesign --verify --verbose client 2>&1

	echo PASS

clean:
	rm -f one.dylib two.dylib client

