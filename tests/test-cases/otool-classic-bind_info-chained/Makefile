# PLATFORM: MACOS IOS WATCHOS TVOS

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

VALID_ARCHS := $(filter-out i386 armv7k,$(VALID_ARCHS))

all:
	for arch in ${VALID_ARCHS};					     \
	do								     \
	  ${CC} -arch $$arch -Wl,-fixup_chains -o verstool		     \
	    ${TESTROOT}/src/verstool.c;					     \
	  ${OTOOLC} -bind_info verstool > output.txt;			     \
	  if [ "$$arch" == "arm64e" ]; then				     \
	    ${CHECK} -p CHECK64E -i output.txt;				     \
	  else								     \
	    ${CHECK} -p CHECK -i output.txt;				     \
	  fi								     \
	done

	echo PASS

################################################################################
#
# CHECK: verstool:
# CHECK-NEXT: bind information:
# CHECK-NEXT: segment\s* section address\s* value\s* type       \[\[:xdigit:\]\]+nd   dylib     symbol
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem ___error
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem ___memcpy_chk
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem ___memset_chk
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem ___snprintf_chk
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem ___stack_chk_fail
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem ___stack_chk_guard
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem ___stderrp
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _calloc
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _chmod
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _close
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _exit
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _fprintf
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _free
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _fstat\($INO\[\[:xdigit:\]\]+\)?
# SKIP Not always present: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _memcpy
# CHECK: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _mkstemp
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _open
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _perror
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _printf
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _read
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _reallocf
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _rename
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _strchr
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _strcmp
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _strdup
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _strerror
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _strlen
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _strrchr
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _strtoul
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_build_version_command
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_load_command
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_mach_header_64
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_section
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_section_64
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_segment_command
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_segment_command_64
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _swap_version_min_command
# CHECK-NEXT: __DATA\(_CONST\)?\s* __got   0x\[\[:xdigit:\]\]+\s* bind ptr   0x0      libSystem _write
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK-NEXT: __DATA\(_CONST\)?\s* __const 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0

################################################################################
#
# CHECK64E: verstool:
# CHECK64E-NEXT: bind information:
# CHECK64E-NEXT: segment\s* section    address     value              type       \[\[:xdigit:\]\]+nd   dylib     symbol
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem ___error (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem ___memcpy_chk (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem ___memset_chk (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem ___snprintf_chk (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem ___stack_chk_fail (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _calloc (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _chmod (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _close (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _exit (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _fprintf (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _free (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _fstat (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _memcpy (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _mkstemp (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _open (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _perror (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _printf (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _read (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _reallocf (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _rename (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _strchr (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _strcmp (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _strdup (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _strerror (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _strlen (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _strrchr (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _strtoul (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_build_version_command (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_load_command (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_mach_header_64 (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_section (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_section_64 (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_segment_command (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_segment_command_64 (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _swap_version_min_command (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ bind aptr  0x0      libSystem _write (JOP: diversity 0, address true, IA)
# CHECK64E-NEXT: __DATA_CONST __got      0x\[\[:xdigit:\]\]+                    bind ptr   0x0      libSystem ___stack_chk_guard
# CHECK64E-NEXT: __DATA_CONST __got      0x\[\[:xdigit:\]\]+                    bind ptr   0x0      libSystem ___stderrp
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0
# CHECK64E-NEXT: __DATA_CONST __const    0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ rebase ptr 0x0

clean:
	rm -f verstool output.txt
