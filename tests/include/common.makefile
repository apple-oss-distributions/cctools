# stuff to include in every test Makefile

SHELL = /bin/sh

# unless overridden by the Makefile, command-line, or environment, assume we 
# are building for macOS. This aids developing/debugging tests, as you can run
# a test just by typing "make" at the command shell.
PLATFORM ?= MACOS

# the test name is the directory name
TEST = $(shell basename `pwd`)

# configure platform-specific settings, including the default SDKROOT, default
# arch, and list of valid archs for this platform.
#
# This configuration will change over time as the build trains add and remove
# Mach-O slices.
ifeq ($(PLATFORM), MACOS)
	ARCH        = x86_64
	VALID_ARCHS = i386 x86_64 x86_64h
	SDKROOT     = $(shell xcodebuild -sdk macosx.internal -version Path 2>/dev/null)
endif
ifeq ($(PLATFORM), IOS)
	ARCH        = arm64
	VALID_ARCHS = arm64 arm64e
	SDKROOT     = $(shell xcodebuild -sdk iphoneos.internal -version Path 2>/dev/null)
endif
ifeq ($(PLATFORM), WATCHOS)
	ARCH=arm64_32
	VALID_ARCHS=armv7k arm64_32 arm64e
	SDKROOT     = $(shell xcodebuild -sdk watchos.internal -version Path 2>/dev/null)
endif
ifeq ($(PLATFORM), TVOS)
	ARCH=arm64
	VALID_ARCHS=arm64
	SDKROOT     = $(shell xcodebuild -sdk tvos.internal -version Path 2>/dev/null)
endif

# set the command invocations for cctools. If CCTOOLS_ROOT is set and exists
# in the filesystem use cctools from that root. Otherwise, fall back to the
# xcode toolchain.
ifneq ("$(wildcard ${CCTOOLS_ROOT})","")
	BITCODE_STRIP = $(CCTOOLS_ROOT)/usr/bin/bitcode_strip
	CS_ALLOC =	$(CCTOOLS_ROOT)/usr/bin/codesign_allocate
	LIBTOOL	=	$(CCTOOLS_ROOT)/usr/bin/libtool
	LIPO	=	$(CCTOOLS_ROOT)/usr/bin/lipo
#	OTOOL	=	$(CCTOOLS_ROOT)/usr/bin/otool
#	OTOOLC	=	$(CCTOOLS_ROOT)/usr/bin/otool-classic
	OTOOL	=	xcrun otool
	OTOOLC = 	xcrun otool-classic
	RANLIB	=	$(CCTOOLS_ROOT)/usr/bin/ranlib
	SEGEDIT	=	$(CCTOOLS_ROOT)/usr/bin/segedit
	STRINGS	= 	$(CCTOOLS_ROOT)/usr/bin/strings
	STRIP	= 	$(CCTOOLS_ROOT)/usr/bin/strip
else
	BITCODE_STRIP = xcrun bitcode_strip
	CS_ALLOC =	xcrun codesign_allocate
	LIBTOOL	=	xcrun libtool
	LIPO	=	xcrun lipo
	OTOOL	=	xcrun otool
	OTOOLC	=	xcrun otool-classic
	RANLIB	=	xcrun ranlib
	SEGEDIT	=	xcrun segedit
	STRINGS	= 	xcrun strings
	STRIP	= 	xcrun strip
endif

# set other common tool commands
CC		=	cc -isysroot $(SDKROOT)
MKDIRS		=	mkdir -p
# MACHOCHECK	=	xcrun machocheck

# utilites for Makefiles
MYDIR=$(shell cd ../../bin;pwd)
PASS_IFF		= ${MYDIR}/pass-iff-exit-zero.pl
PASS_IFF_SUCCESS	= ${PASS_IFF}
PASS_IFF_EMPTY		= ${MYDIR}/pass-iff-no-stdin.pl
PASS_IFF_STDIN		= ${MYDIR}/pass-iff-stdin.pl
FAIL_IFF		= ${MYDIR}/fail-iff-exit-zero.pl
FAIL_IFF_SUCCESS	= ${FAIL_IFF}
PASS_IFF_ERROR		= ${MYDIR}/pass-iff-exit-non-zero.pl
FAIL_IF_ERROR		= ${MYDIR}/fail-if-exit-non-zero.pl
FAIL_IF_SUCCESS     	= ${MYDIR}/fail-if-exit-zero.pl
FAIL_IF_EMPTY		= ${MYDIR}/fail-if-no-stdin.pl
FAIL_IF_STDIN		= ${MYDIR}/fail-if-stdin.pl
# PASS_IFF_GOOD_MACHO	= ${PASS_IFF} ${MACHOCHECK}
# FAIL_IF_BAD_MACHO	= ${FAIL_IF_ERROR} ${MACHOCHECK}
# FAIL_IF_BAD_OBJ		= ${FAIL_IF_ERROR} ${OBJECTDUMP} >/dev/null
VERIFY_ALIGN_16K	= $(MYDIR)/verify-align.pl -a 0x4000
VERIFY_ALIGN_4K		= $(MYDIR)/verify-align.pl -a 0x1000
