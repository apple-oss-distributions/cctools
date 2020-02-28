# PLATFORM: IOS
#
# verify otool-classic -bind_info prints the bind information for an
# arm64e binary. The trick is to find an interesting binary...

PLATFORM = IOS
TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	${CC} -arch arm64e -o verstool ${TESTROOT}/src/verstool.c
	${OTOOLC} -bind_info verstool | ${CHECK}
# CHECK: verstool:
# CHECK-NEXT: bind information:
# CHECK-NEXT: segment      section    address     value              type       \[\[:xdigit:\]\]+nd   dylib     symbol
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem ___error (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem ___memcpy_chk (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem ___memset_chk (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem ___snprintf_chk (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem ___stack_chk_fail (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _calloc (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _chmod (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _close (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _exit (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _fprintf (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _free (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _fstat (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _memcpy (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _mkstemp (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _open (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _perror (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _printf (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _read (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _reallocf (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _rename (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _strchr (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _strcmp (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _strdup (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _strerror (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _strlen (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _strrchr (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _strtoul (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_build_version_command (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_load_command (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_mach_h\[\[:xdigit:\]\]+r_64 (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_section (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_section_64 (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_segment_command (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_segment_command_64 (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _swap_version_min_command (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem _write (JOP: diversity 0, address true, IA)
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem ___stack_chk_guard
# CHECK-NEXT: __DATA_CONST __auth_got 0x\[\[:xdigit:\]\]+ 0x\[\[:xdigit:\]\]+ pointer    0x0      libSystem ___stderrp

	echo PASS

clean:
	rm -f verstool
