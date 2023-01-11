# PLATFORM: MACOS

PLATFORM = MACOS
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

.PHONY: all clean

CC_ARM := ${CC} -arch arm64

all:
	# Build a dylib
	${CC_ARM} -dynamiclib -o one.dylib ${TESTSRC}/foo.c -DFOO=one

	# Verify the LC_ID_DYLIB load command is set.
	${OTOOLC} -lv one.dylib | grep LC_ID_DYLIB -A2 | ${CHECK} -p ONE
	# ONE:          cmd LC_ID_DYLIB
	# ONE:      cmdsize 40
	# ONE:         name one.dylib (offset 24)

	# Verify the code signature
	codesign --verify --verbose one.dylib 2>&1

	# Move the dylib
	cp one.dylib one_a.dylib
	${INSTALL_NAME_TOOL} -id one_a.dylib one_a.dylib
	${OTOOLC} -lv one_A.dylib | grep LC_ID_DYLIB -A2 | ${CHECK} -p ONEA
	# ONEA:          cmd LC_ID_DYLIB
	# ONEA:      cmdsize 40
	# ONEA:         name one_a.dylib (offset 24)

	# Verify the code signature
	codesign --verify --verbose one_a.dylib 2>&1

	echo PASS

clean:
	rm -f one.dylib one_a.dylib
