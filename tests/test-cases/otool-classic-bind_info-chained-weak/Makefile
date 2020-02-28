# PLATFORM: MACOS IOS WATCHOS TVOS
#
# Test various kinds of weak chained binds, as these require different
# codepaths through otool, etc.

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

VALID_ARCHS := $(filter-out i386 armv7k,$(VALID_ARCHS))

# test_weak compiles and runs the test with dyldinfo bind opcodes
#   $1 is the arch
#   $2 is the src file
#   $3 is the check prefix
test_weak =								     \
	  ${CPP} -w -arch $(1) -Wl,-fixup_chains $(2);			     \
	  ${OTOOLC} -bind_info a.out | ${CHECK} -p $(3)

.PHONY : all clean

all:
################################################################################
#
# 	for arch in ${VALID_ARCHS};					     \
# 	do								     \
# 	  ${CC} -arch $$arch -Wl,-fixup_chains -o verstool		     \
# 	    ${TESTROOT}/src/verstool.c;					     \
# 	  ${OTOOLC} -bind_info verstool > output.txt;			     \
# 	  if [ "$$arch" == "arm64e" ]; then				     \
# 	    ${CHECK} -p CHECK64E -i output.txt;				     \
# 	  else								     \
# 	    ${CHECK} -p CHECK -i output.txt;				     \
# 	  fi								     \
# 	done
#
# x86_64
ifneq (,$(filter x86_64,$(VALID_ARCHS)))
	$(call test_weak,x86_64,weak-large-addends.cpp,CHECK)
	$(call test_weak,x86_64,weak-no-addends.cpp,CHECK)
	$(call test_weak,x86_64,weak-small-addends.cpp,CHECK)
endif
#
# x86_64h
ifneq (,$(filter x86_64h,$(VALID_ARCHS)))
	$(call test_weak,x86_64h,weak-large-addends.cpp,CHECK)
	$(call test_weak,x86_64h,weak-no-addends.cpp,CHECK)
	$(call test_weak,x86_64h,weak-small-addends.cpp,CHECK)
endif
#
# armv7k
ifneq (,$(filter armv7k,$(VALID_ARCHS)))
	$(call test_weak,armv7k,weak-large-addends.cpp,CHECK)
	$(call test_weak,armv7k,weak-no-addends.cpp,CHECK)
	$(call test_weak,armv7k,weak-small-addends.cpp,CHECK)
endif
#
# armv64_32
ifneq (,$(filter armv64_32,$(VALID_ARCHS)))
	$(call test_weak,armv64_32,weak-large-addends.cpp,CHECK)
	$(call test_weak,armv64_32,weak-no-addends.cpp,CHECK)
	$(call test_weak,armv64_32,weak-small-addends.cpp,CHECK)
endif
#
# arm64
ifneq (,$(filter arm64,$(VALID_ARCHS)))
	$(call test_weak,arm64,weak-large-addends.cpp,CHECK)
	$(call test_weak,arm64,weak-no-addends.cpp,CHECK)
	$(call test_weak,arm64,weak-small-addends.cpp,CHECK)
endif
#
# arm64e
ifneq (,$(filter arm64e,$(VALID_ARCHS)))
	$(call test_weak,arm64e,weak-large-addends.cpp,CHECK64E)
	$(call test_weak,arm64e,weak-no-addends.cpp,CHECK64E)
	$(call test_weak,arm64e,weak-small-addends.cpp,CHECK64E)
endif

	echo PASS

clean:
	rm -f a.out

################################################################################
#
# CHECK: a.out:
# CHECK-NEXT: bind information:
# CHECK-NEXT: segment\s+section         address     value              type       \[\[:xdigit:\]\]+nd   dylib     symbol
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      weak      __ZdlPv
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      weak      __Znwm
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      libc\+\+    ___cxa_call_unexpected
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      libSystem ___error
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      libc\+\+    ___gxx_personality_v0
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      weak      _a
# CHECK-NEXT: __DATA\(_CONST\)?\s+__got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      libSystem _malloc
# CHECK-NEXT: __DATA\(_CONST\)?\s+__mod_init_func 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s+__const         0x\[\[:xdigit:\]\]+                    bind ptr   0x10     libc\+\+    __ZTVN10__cxxabiv117__class_type_infoE
# CHECK-NEXT: __DATA\(_CONST\)?\s+__const         0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s+__data          0x\[\[:xdigit:\]\]+                    bind ptr   0x\[\[:xdigit:\]\]+\s+weak      _b
# CHECK-NEXT: __DATA\(_CONST\)?\s+__data          0x\[\[:xdigit:\]\]+                    bind ptr   0x\[\[:xdigit:\]\]+\s+weak      _b

################################################################################
#
# CHECK64E: a.out:
# CHECK64E-NEXT: bind information:
# CHECK64E-NEXT: segment      section         address     value              type       \[\[:xdigit:\]\]+nd   dylib     symbol
# CHECK64E-NEXT: __DATA_CONST __auth_got      0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      weak      __ZdlPv (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got      0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      weak      __Znwm (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got      0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libc\+\+    ___cxa_call_unexpected (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got      0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem ___error (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got      0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _malloc (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __got           0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __got           0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libc\+\+    ___gxx_personality_v0 (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __got           0x\[\[:xdigit:\]\]+                    bind ptr   0x0      weak      _a
# CHECK64E-NEXT: __DATA_CONST __mod_init_func 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const         0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libc\+\+    __ZTVN10__cxxabiv117__class_type_infoE (JOP: diversity 0, address false, DA)
# CHECK64E-NEXT: __DATA_CONST __const         0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA       __data          0x\[\[:xdigit:\]\]+                    bind ptr   0x\[\[:xdigit:\]\]+\s* weak      _b
# CHECK64E-NEXT: __DATA       __data          0x\[\[:xdigit:\]\]+                    bind ptr   0x\[\[:xdigit:\]\]+\s* weak      _b
