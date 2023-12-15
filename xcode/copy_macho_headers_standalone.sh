#!/bin/sh

set -x

if [ "${DRIVERKIT}" == 1 ]; then
    mkdir -p ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}
    cp ${SRCROOT}/include/standalone/getsect.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/
    cp ${SRCROOT}/include/mach-o/nlist.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/
    cp ${SRCROOT}/include/mach-o/loader.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/
    cp ${SRCROOT}/include/mach-o/reloc.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/
    cp ${SRCROOT}/include/mach-o/ldsyms.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/
    cp ${SRCROOT}/include/mach-o/fat.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/

    mkdir -p ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/arm
    cp ${SRCROOT}/include/mach-o/arm/reloc.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/arm/
    mkdir -p ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/arm64
    cp ${SRCROOT}/include/mach-o/arm64/reloc.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/arm64/
    mkdir -p ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/x86_64
    cp ${SRCROOT}/include/mach-o/x86_64/reloc.h ${DSTROOT}/${PUBLIC_HEADERS_FOLDER_PATH}/x86_64/
elif [ "${SYSTEM_PREFIX}" != "" ]; then # Exclaves
    mkdir -p ${DSTROOT}${SYSTEM_PREFIX}/usr/include/mach-o
    mkdir -p ${DSTROOT}${SYSTEM_PREFIX}/usr/local/include/mach-o

    cp ${SRCROOT}/include/mach-o/fat.h ${DSTROOT}${SYSTEM_PREFIX}/usr/include/mach-o
    cp ${SRCROOT}/include/mach-o/loader.h ${DSTROOT}${SYSTEM_PREFIX}/usr/include/mach-o
    cp ${SRCROOT}/include/mach-o/nlist.h ${DSTROOT}${SYSTEM_PREFIX}/usr/include/mach-o
    cp ${SRCROOT}/include/standalone/getsect.h ${DSTROOT}${SYSTEM_PREFIX}/usr/include/mach-o

    cp ${SRCROOT}/include/modules/mach-o.exclaves.modulemap ${DSTROOT}${SYSTEM_PREFIX}/usr/include/mach-o/module.modulemap
    cp ${SRCROOT}/include/modules/mach-o.private.modulemap ${DSTROOT}${SYSTEM_PREFIX}/usr/local/include/mach-o/module.modulemap
fi
