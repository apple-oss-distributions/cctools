# PLATFORM: MACOS
# <rdar://problem/57839745> ctf_insert can't find space for load commands when __TEXT has no sections

PLATFORM = MACOS
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

.PHONY: all clean

all:
	$(PASS_IFF) $(CTF_INSERT) in.exe -o in.out -arch x86_64 in.txt

clean:
	rm -rf in.out

