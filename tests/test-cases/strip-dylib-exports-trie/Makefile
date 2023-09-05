# PLATFORM: MACOS
#
# rdar://99566424 (SEED: Web: strip `-r -u` is inconsistent with default invocation)
# Test dylib's exports trie isn't pruned even when using -ru.

.PHONY: all clean

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all: hello
	# verify the unstripped binary has a function named "foo_impl"
	# and the stripped binary does not in the symbol table
	${NMC} libfoo.dylib.sym | ${CHECK} -p HASFOO 
	${NMC} libfoo.dylib | ${CHECK} -p NOFOO 
	# verify the unstripped and stripped binary have a function named "foo_impl"
	# in the exports trie
	echo ${DYLD_INFO}
	${DYLD_INFO} -exports libfoo.dylib.sym | ${CHECK} -p HASFOO 
	${DYLD_INFO} -exports libfoo.dylib | ${CHECK} -p HASFOO
# HASFOO:    .*foo_impl.*
# NOFOO-NOT: .*foo_impl.*

	echo PASS

hello:
	clang -shared -o libfoo.dylib.sym foo.c
	${STRIP} -ru -o libfoo.dylib libfoo.dylib.sym

clean:
	rm -rf libfoo.dylib.sym libfoo.dylib
