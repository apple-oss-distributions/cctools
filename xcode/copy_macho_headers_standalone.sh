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
fi
