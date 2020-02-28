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
dst=${DSTROOT}/System/DriverKit/Runtime/usr/include/mach-o

install_files ${src} ${dst} ldsyms.h nlist.h
install_files ${src}/arm ${dst}/arm reloc.h
install_files ${src}/arm64 ${dst}/arm64 reloc.h
install_files ${src}/x86_64 ${dst}/x86_64 reloc.h

