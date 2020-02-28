#!/bin/sh
#
# install the mach-o header files that can't be installed by the libmacho
# dynamic library target. This is specifically all of the architecture specific
# header files backed by libmacho, some private header files, and some public
# files that we don't want indexed by TAPI

install_files() {
  src_dir=$1; shift
  dst_dir=$1; shift
  files=$*
  
  (
    cd ${src_dir}
    mkdir -p ${dst_dir}
    install -c -m 444 ${files} ${dst_dir}
  )
}

src=${SRCROOT}/include/mach-o
dst=${DSTROOT}/usr/include/mach-o

install_files ${src} ${dst} ldsyms.h nlist.h
install_files ${src}/arm ${dst}/arm reloc.h
install_files ${src}/arm64 ${dst}/arm64 reloc.h
install_files ${src}/i386 ${dst}/i386 swap.h
install_files ${src}/x86_64 ${dst}/x86_64 reloc.h

dst=${DSTROOT}/usr/local/include/mach-o

install_files ${src}/hppa ${dst}/hppa reloc.h swap.h
install_files ${src}/i860 ${dst}/i860 reloc.h swap.h
install_files ${src}/m68k ${dst}/m68k swap.h
install_files ${src}/m88k ${dst}/m88k reloc.h swap.h
install_files ${src}/sparc ${dst}/sparc reloc.h swap.h
install_files ${src}/ppc ${dst}/ppc reloc.h swap.h

src=${SRCROOT}/include/stuff
dst=${DSTROOT}/usr/local/include/dyld

install_files ${src} ${dst} bool.h
