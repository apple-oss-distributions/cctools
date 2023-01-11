/*
 * Copyright (c) 2020 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 *  code_directory.c
 *  cctools libstuff
 *
 *  Created by Michael Trent on 8/6/20.
 */

#ifdef CODEDIRECTORY_SUPPORT

#include "stuff/code_directory.h"

#include "stuff/errors.h"
#include "stuff/rnd.h"

#include <libcodedirectory.h>
#include <mach-o/loader.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * #include "align.h"
 *
 * The "align.h" libstuff header includes stuff/bool.h, which is not compatible
 * with the libcodedirectory.h header. Instead of pulling in the entire header,
 * we'll just manually declare the function we want to call.
 */
__private_extern__
uint32_t
guess_align(uint64_t vmaddr, uint32_t min, uint32_t max);

/*
 * #include "bytesex.h"
 *
 * The "bytesex.h header includes stuff/bool.h, which is not compatible with
 * the libcodedirectory.h header. Instead of pulling in the entire header,
 * we'll just manually declare the functions we want to call.
 */
enum byte_sex {
    UNKNOWN_BYTE_SEX,
    BIG_ENDIAN_BYTE_SEX,
    LITTLE_ENDIAN_BYTE_SEX
};

__private_extern__ void swap_mach_header(
    struct mach_header *mh,
    enum byte_sex target_byte_sex);

__private_extern__ void swap_mach_header_64(
    struct mach_header_64 *mh,
    enum byte_sex target_byte_sex);

__private_extern__ void swap_load_command(
    struct load_command *lc,
    enum byte_sex target_byte_sex);

__private_extern__ void swap_segment_command(
    struct segment_command *sg,
    enum byte_sex target_byte_sex);

__private_extern__ void swap_segment_command_64(
    struct segment_command_64 *sg,
    enum byte_sex target_byte_sex);

__private_extern__ void swap_linkedit_data_command(
    struct linkedit_data_command *ld,
    enum byte_sex target_byte_sex);

__private_extern__ void swap_version_min_command(
    struct version_min_command *ver_cmd,
    enum byte_sex target_byte_sex);

__private_extern__ void swap_build_version_command(
    struct build_version_command *bv,
    enum byte_sex target_byte_sex);


/*
 * platform_name() returns a name for a platform field.
 */
static const char* platform_name(uint32_t platform)
{
    const char* names[] = {
        "unknown",
        "PLATFORM_MACOS",
        "PLATFORM_IOS",
        "PLATFORM_TVOS",
        "PLATFORM_WATCHOS",
        "PLATFORM_BRIDGEOS",
        "PLATFORM_MACCATALYST",
        "PLATFORM_IOSSIMULATOR",
        "PLATFORM_TVOSSIMULATOR",
        "PLATFORM_WATCHOSSIMULATOR",
        "PLATFORM_DRIVERKIT",
    };
    int nname = sizeof(names) / sizeof(*names);

    if (platform < nname)
        return names[platform];

    return NULL;
}

/*
 * Format a version number where components are encoded into a uint32_t in
 * nibbles: X.Y.Z => 0xXXXXYYZZ.
 */
static const char* format_version_xyz(uint32_t version)
{
    static char buf[128];

    if((version & 0xff) == 0)
        snprintf(buf, 128, "%u.%u", version >> 16, (version >> 8) & 0xff);
    else
        snprintf(buf, 128, "%u.%u.%u", version >> 16, (version >> 8) & 0xff,
                 version & 0xff);
    return buf;
}

/*
 * codedir_is_linker_signed() takes an LC_CODE_SIGNATURE data payload and
 * returns 1 if it contains a valid ad-hoc signature signed by ld(1), and 0 if
 * it contains a valid signature written by codesign(1) or an invalid signature.
 */
int codedir_is_linker_signed(const char* data, uint32_t size)
{
#if 1
    // HACK: libcodedirectory.h is in both the macOS SDK in /usr/local/include, and in the tool chain at /usr/include.
    // but there is no way to control clang's search path to look in the toolchain first.
    // So, declare newer API locally. Once this new header is in all SDKs we can remove this.
    // Can't use real function name because it returns an enum type and we cannot define the enum
    // here because it will cause a duplicate definition when the header is updated.
    extern int my_libcd_is_blob_a_linker_signature(const uint8_t *buf, size_t buf_len, int *is_linker_signature) __asm("_libcd_is_blob_a_linker_signature");
    int ret;
    int linker_signed = 0;

    ret = my_libcd_is_blob_a_linker_signature((const uint8_t*)data,
					   (size_t)size,
					   &linker_signed);
    if ( (ret == 0) && linker_signed )
        return 1;
#else
    enum libcd_signature_query_ret ret = LIBCD_SIGNATURE_QUERY_SUCCESS;
    int linker_signed = 0;

    ret = libcd_is_blob_a_linker_signature((const uint8_t*)data,
					   (size_t)size,
					   &linker_signed);
    switch (ret) {
	case LIBCD_SIGNATURE_QUERY_SUCCESS:
	    if (linker_signed) return 1;
	    /* FALLTHROUGH */
	case LIBCD_SIGNATURE_QUERY_INVALID_ARGUMENT:
	case LIBCD_SIGNATURE_QUERY_NOT_A_SIGNATURE:
	    ;
    }
#endif
    return 0;
}

/*
 * struct codedir is an opaque wrapper for libcd, along with some other data
 * useful in the re-measuring process.
 */
struct codedir {
    libcd* handle;
    uint32_t filesize;
    uint32_t old_datasize;
    uint32_t new_datasize;
};

/*
 * codedir_create() builds an object for measuring a Mach-O binary and writing
 * LC_CODE_SIGNATURE load command data. 'filesize' represents the size of a
 * non-relocatable Mach-O object file to be measured, including all load
 * commands and segments, but not including the LC_CODE_SIGNATURE data payload
 * in linkedit. (I.e., from the start of the mach_header to the end of the
 * string table plus any padding.) 'ptralign' is the pointer alignment, as a
 * power of two: e.g., 3 for 8-byte (64-bit) aligned pointers.
 * 'platform' and 'minos' match fields in 'struct build_version_command' used
 * by the LC_BUILD_VERSION load command.
 *
 * codedir_create() returns NULL on error.
 */
struct codedir* codedir_create(const char* output_path,
                               uint32_t filesize,
                               uint32_t ptralign,
                               uint32_t platform,
                               uint32_t minos)
{
    struct codedir* codedir = calloc(1, sizeof(*codedir));
    if (!codedir) {
        system_error("cannot allocate %lu bytes", sizeof(*codedir));
        return NULL;
    }

    codedir->filesize = filesize;
    codedir->handle = libcd_create((size_t)filesize);
    if (!codedir->handle) {
        // libcd_create will actually crash on failure. <_<
        error("internal: libcd_create() failed");
        free(codedir);
        return NULL;
    }

    if (libcd_set_hash_types_for_platform_version(codedir->handle,
                                                  (int)platform,
                                                  (int)minos)) {
        // libcd_set_hash_types_for_platform_version not expected to fail on
        // Darwin operating systems.
        const char* platformstr = platform_name(platform);
        const char* versionstr = format_version_xyz(minos);
        if (platform)
            error("libcd_set_hash_types_for_platform_version failed for "
                  "%s %s", platformstr, versionstr);
        else
            error("libcd_set_hash_types_for_platform_version failed for "
                  "platform #%d %s", platform, versionstr);
        libcd_free(codedir->handle);
        free(codedir);
        return NULL;
    }

    const char* slash = strrchr(output_path, '/');
    const char* name = slash ? slash + 1 : output_path;
    libcd_set_signing_id(codedir->handle, name);
    libcd_set_flags(codedir->handle, CS_ADHOC | CS_LINKER_SIGNED);

    size_t newcdsize = libcd_superblob_size(codedir->handle);
    size_t aligned_cdsize = rnd64(newcdsize, 1<<ptralign);
    codedir->new_datasize = (uint32_t)aligned_cdsize;
    return codedir;
}

/*
 * read_load_command() is a utility function for copying an individual load
 * command out of the load command buffer safely, with error checking and
 * byte swapping. It's not good enough for reading sub-command information,
 * such as sections or build tools.
 */
typedef void (*swapfn_t)(void*, enum byte_sex);

static
int read_load_command(char* buffer, uint32_t buflen,
                      uint32_t mh_size, uint32_t sizeofcmds,
                      int i, off_t lc_off, const struct load_command* lc,
                      const char* name, void* out_buf, uint32_t out_size,
                      bool swap, swapfn_t swapfn)
{
    if (!name)
        name = "unknown";
    if (buflen < lc_off + out_size) {
        error("load command #%d (%s) end %lld extends past file", i, name,
              lc_off + lc->cmdsize);
        return -1;
    }
    if (mh_size + sizeofcmds < lc_off + out_size) {
        error("load command #%d (%s) end %llu extends past sizeofcmds: %u",
              i, name, lc_off + sizeof(lc), sizeofcmds);
        return -1;
    }
    if (lc != NULL && buflen < lc_off + lc->cmdsize) {
        error("load command #%d (%s) end %lld extends past file", i, name,
              lc_off + lc->cmdsize);
        return -1;
    }
    if (lc != NULL && lc->cmdsize < out_size) {
        error("load command #%d (%s) too small: %d", i, name, lc->cmdsize);
        return -1;
    }
    memcpy(out_buf, buffer + lc_off, out_size);
    if (swap)
        swapfn(out_buf, UNKNOWN_BYTE_SEX);
    return 0;
}

/*
 * codedir_create_object() initializes a codedir struct from an existing
 * non-relocatable Mach-O file. The file needs to be fully laid out, with all
 * load commands present, including an LC_CODE_SIGNATURE load command. The
 * LC_CODE_SIGNATURE needs to either be pointing at existing data at the
 * end of linkedit or it needs to be initialized to 0. 'object_addr' should
 * point to the Mach-O file data, and 'object_size' should store the size of
 * that memory buffer. Only the file's header and load commands are processed
 * here, and the LC_CODE_SEGMENT and linkedit segment load commands will be
 * modified to reflect the final signed binary.
 *
 * codedir_create() returns 0 on success or -1 on error
 */
int codedir_create_object(const char* output_path,
                                       char* buffer,
                                       uint32_t buflen,
                                       struct codedir** out_codedir)
{
    // read magic
    uint32_t *magic = (uint32_t*)buffer;
    if (buflen < sizeof(magic)) {
        error("file too small: %u", buflen);
        return -1;
    }

    bool is64 = (*magic == MH_MAGIC_64 || *magic == MH_CIGAM_64);
    bool swap = (*magic == MH_CIGAM || *magic == MH_CIGAM_64);
    off_t offset = (is64 ?
                   sizeof(struct mach_header_64) :
                   sizeof(struct mach_header));

    if (buflen < offset) {
        error("file too small: %u", buflen);
        return -1;
    }

    // get some vital statistics from the mach header
    uint32_t sizeofhdr = 0;
    uint32_t ncmds = 0;
    uint32_t sizeofcmds = 0;
    uint32_t filetype = 0;
    if (is64) {
        struct mach_header_64 mh;
        memcpy(&mh, magic, sizeof(mh));
        if (swap)
            swap_mach_header_64(&mh, UNKNOWN_BYTE_SEX);
        ncmds = mh.ncmds;
        sizeofcmds = mh.sizeofcmds;
        filetype = mh.filetype;
        sizeofhdr = sizeof(mh);
    }
    else {
        struct mach_header mh;
        memcpy(&mh, magic, sizeof(mh));
        if (swap)
            swap_mach_header(&mh, UNKNOWN_BYTE_SEX);
        ncmds = mh.ncmds;
        sizeofcmds = mh.sizeofcmds;
        filetype = mh.filetype;
        sizeofhdr = sizeof(mh);
    }

    // walk the load commands
    struct linkedit_data_command cs;
    struct segment_command le32;
    struct segment_command_64 le64;
    uint32_t le_offset = 0;
    uint32_t cs_offset = 0;
    uint32_t platform = 0;
    uint32_t min_vers = 0;
    uint64_t execbase = 0;
    uint64_t execsize = 0;
    const uint32_t maxalign = 15;
    const uint32_t ptralign = is64 ? 3 : 2;
    uint32_t vmalign = UINT32_MAX;
    const char* lcname = NULL;

    for (int i = 0; i < ncmds; ++i) {
        struct load_command lc;

        // get the load command as an "abstract" struct load_command.
        if (read_load_command(buffer, buflen, sizeofhdr, sizeofcmds, i, offset,
                              NULL, NULL, &lc, sizeof(lc),
                              swap, (swapfn_t)swap_load_command))
            return -1;

        // convert that "abstract" load command into a "concrete" load command.
        switch (lc.cmd) {
            case LC_SEGMENT: {
                struct segment_command sg;
                if (read_load_command(buffer, buflen, sizeofhdr, sizeofcmds, i,
                                      offset, &lc, "LC_SEGMENT", &sg,sizeof(sg),
                                      swap, (swapfn_t)swap_segment_command))
                    return -1;

                uint32_t guess = guess_align(sg.vmaddr, ptralign, maxalign);
                if (guess < vmalign)
                    vmalign = guess;

                if (0 == strcmp(SEG_LINKEDIT, sg.segname)) {
                    memcpy(&le32, &sg, sizeof(sg));
                    le_offset = (uint32_t)offset;
                }

                if (sg.maxprot & VM_PROT_EXECUTE) {
                    if (execsize) {
                        // binary does not have a single contiguous executable
                        // segment
                        execbase = UINT64_MAX;
                        execsize = 0;
                    }
                    else {
                        execbase = sg.fileoff;
                        execsize = sg.filesize;
                    }
                }
            } break;
            case LC_SEGMENT_64: {
                struct segment_command_64 sg;
                if (read_load_command(buffer, buflen, sizeofhdr, sizeofcmds, i,
                                      offset, &lc, "LC_SEGMENT_64",
                                      &sg, sizeof(sg), swap,
                                      (swapfn_t)swap_segment_command_64))
                    return -1;

                uint32_t guess = guess_align(sg.vmaddr, ptralign, maxalign);
                if (guess < vmalign)
                    vmalign = guess;

                if (0 == strcmp(SEG_LINKEDIT, sg.segname)) {
                    memcpy(&le64, &sg, sizeof(sg));
                    le_offset = (uint32_t)offset;
                }

                if (sg.maxprot & VM_PROT_EXECUTE) {
                    if (execsize) {
                        // binary does not have a single contiguous executable
                        // segment
                        execbase = UINT64_MAX;
                        execsize = 0;
                    }
                    else {
                        execbase = sg.fileoff;
                        execsize = sg.filesize;
                    }
                }
            } break;
            case LC_CODE_SIGNATURE: {
                if (read_load_command(buffer, buflen, sizeofhdr, sizeofcmds, i,
                                      offset, &lc, "LC_CODE_SIGNATURE", &cs,
                                      sizeof(cs), swap,
                                      (swapfn_t)swap_linkedit_data_command))
                    return -1;
                cs_offset = (uint32_t)offset;
            } break;
            case LC_VERSION_MIN_MACOSX:
                if (!lcname)
                    lcname = "LC_VERSION_MIN_MACOSX";
                /* FALLTHROUGH */
            case LC_VERSION_MIN_IPHONEOS:
                if (!lcname)
                    lcname = "LC_VERSION_MIN_IPHONEOS";
                /* FALLTHROUGH */
            case LC_VERSION_MIN_WATCHOS:
                if (!lcname)
                    lcname = "LC_VERSION_MIN_WATCHOS";
                /* FALLTHROUGH */
            case LC_VERSION_MIN_TVOS:
            {
                struct version_min_command vm;
                if (!lcname)
                    lcname = "LC_VERSION_MIN_TVOS";
                if (read_load_command(buffer, buflen, sizeofhdr, sizeofcmds, i,
                                      offset, &lc, lcname, &vm, sizeof(vm),
                                      swap, (swapfn_t)swap_version_min_command))
                    return -1;
                lcname = NULL;

                if (!platform) {
                    switch (lc.cmd) { // mouichido
                        case LC_VERSION_MIN_MACOSX:
                            platform = PLATFORM_MACOS;
                            break;
                        case LC_VERSION_MIN_IPHONEOS:
                            platform = PLATFORM_IOS;
                            break;
                        case LC_VERSION_MIN_WATCHOS:
                            platform = PLATFORM_WATCHOS;
                            break;
                        case LC_VERSION_MIN_TVOS:
                            platform = PLATFORM_TVOS;
                            break;
                        default:
                            ;
                    }
                    min_vers = vm.version;
                }
            } break;
            case LC_BUILD_VERSION: {
                struct build_version_command bv;
                if (read_load_command(buffer, buflen, sizeofhdr, sizeofcmds,
                                      i, offset, &lc, "LC_BUILD_VERSION",
                                      &bv, sizeof(bv), swap,
                                      (swapfn_t)swap_build_version_command))
                    return -1;

                if (!platform) {
                    platform = bv.platform;
                    min_vers = bv.minos;
                }
            } break;
            default:
                ;
        }

        if (lc.cmdsize == 0) {
            error("load command #%d has 0 size", i);
            return -1;
        }
        offset += lc.cmdsize;
    }

    // create a codedir object to measure this object up to the
    // LC_CODE_SIGNATURE data payload.
    uint64_t le_fileoff = is64 ? le64.fileoff : le32.fileoff;
    uint64_t le_filesize = is64 ? le64.filesize : le32.filesize;
    uint32_t filesize = (cs.dataoff ? cs.dataoff :
                         (uint32_t)(le_fileoff + le_filesize));

    struct codedir* codedir = codedir_create(output_path, filesize, ptralign,
                                             platform, min_vers);
    if (!codedir) {
        return -1;
    }

    // if we found a single executable segment, pass that into libcd.
    if (!(execbase == 0 && execsize == 0) && execbase != UINT64_MAX) {
        uint32_t execflags = filetype==MH_EXECUTE ? CS_EXECSEG_MAIN_BINARY : 0;
        libcd_set_exec_seg(codedir->handle, execbase, execsize, execflags);
    }

    // rewrite the linkedit segment load command with the updated
    // LC_CODE_SIGNATURE data payload.
    codedir->old_datasize = cs.datasize;
    le_filesize = le_filesize - codedir->old_datasize + codedir->new_datasize;
    uint64_t le_vmsize = rnd64(le_filesize, 1<<vmalign);
    if (is64) {
        le64.vmsize = le_vmsize;
        le64.filesize = le_filesize;
        if (swap)
            swap_segment_command_64(&le64, UNKNOWN_BYTE_SEX);
        memcpy(buffer + le_offset, &le64, sizeof(le64));
    }
    else {
        le32.vmsize = (uint32_t)le_vmsize;
        le32.filesize = (uint32_t)le_filesize;
        if (swap)
            swap_segment_command(&le32, UNKNOWN_BYTE_SEX);
        memcpy(buffer + le_offset, &le32, sizeof(le32));
    }

    // write out the new LC_CODE_SIGNATURE load command
    cs.dataoff = filesize;
    cs.datasize = codedir->new_datasize;
    if (swap)
        swap_linkedit_data_command(&cs, UNKNOWN_BYTE_SEX);
    memcpy(buffer + cs_offset, &cs, sizeof(cs));

    if (out_codedir)
        *out_codedir = codedir;

    return 0;
}

/*
 * codedir_free() releases memory held by struct codedir.
 */
void codedir_free(struct codedir* codedir)
{
    libcd_free(codedir->handle);
    free(codedir);
}

/*
 * codedir_filesize() returns the size of the Mach-O being measured, not
 * including the size of the LC_CODE_SIGNATURE payload. In other words, this
 * value is also the location of the LC_CODE_SIGNATURE payload.
 */
uint32_t codedir_filesize(const struct codedir* codedir)
{
    return codedir->filesize;
}

/*
 * codedir_datasize() returns the size required to hold the LC_CODE_SIGNATURE
 * payload.
 */
uint32_t codedir_datasize(const struct codedir* codedir)
{
    return codedir->new_datasize;
}

/*
 * codedir_datasize_delta() returns the difference in size between the old
 * and new LC_CODE_SIGNATURE payloads if this struct codedir was created
 * via codedir_create_object(). Otherwise, will return codedir_datasize().
 */
int32_t codedir_datasize_delta(const struct codedir* codedir)
{
    return (int32_t)codedir->new_datasize - (int32_t)codedir->old_datasize;
}

/*
 * codedir_write() writes a new LC_CODE_SIGNATURE payload. 'input' must point
 * at the start of a non-relocatable Mach-O file of 'filesize' bytes in length
 * as passed to codedir_create(). The input and output buffers may point into
 * the same underlying memory.
 *
 * Once the LC_CODE_SIGNATURE payload is computed, the 'input' bytes cannot
 * change within the range of [0, 'filesize') without invalidating that payload.
 * This means all changes to the file, including load command sizes and offsets,
 * need to be finalized before the output data is written.
 *
 * codedir_write returns 0 on success, -1 on error.
 */
int codedir_write(const struct codedir* codedir,
                    const char* input,
                    char* output,
                    uint32_t output_size)
{
    libcd_set_input_mem(codedir->handle, (const uint8_t*)input);
    libcd_set_output_mem(codedir->handle, (uint8_t*)output,(size_t)output_size);
    enum libcd_serialize_ret sret = libcd_serialize(codedir->handle);
    if (sret != LIBCD_SERIALIZE_SUCCESS) {
        char* s = "UNKNOWN ERROR";
        switch (sret) {
            case LIBCD_SERIALIZE_SUCCESS:	  s = "SUCCESS";	 break;
	    case LIBCD_SERIALIZE_WRITE_ERROR:	  s = "WRITE_ERROR";	 break;
	    case LIBCD_SERIALIZE_READ_PAGE_ERROR: s = "READ_PAGE_ERROR"; break;
            case LIBCD_SERIALIZE_INVALID_THREAD_COUNT:
					     s = "INVALID_THREAD_COUNT"; break;
	    case LIBCD_SERIALIZE_SIGNATURE_FAILURE:
						s = "SIGNATURE_FAILURE"; break;
	    case LIBCD_SERIALIZE_EMPTY:		  s = "EMPTY";		 break;
	    case LIBCD_SERIALIZE_NO_MEM:	  s = "NO_MEM";		 break;
        }
        error("can't serialize code directory: %s", s);
        return -1;
    }
    return 0;
}

/*
 * codedir_write_object() computes the LC_CODE_SIGNATURE payload for a whole
 * Mach-O binary. 'object_addr' points to a fully laid out and finalized
 * non-relocatable Mach-O file, with an LC_CODE_SIGNATURE load command pointing
 * to the correct location within the file's __LINKEDIT segment. The
 * 'object_addr' memory must be large enough to hold the new LC_CODE_SIGNATURE
 * payload. 'object_size' is the size of the 'object_addr' memory buffer.
 *
 * codedir_write_object returns 0 on success, -1 on error.
 */
int codedir_write_object(const struct codedir* codedir,
                           char* object_addr,
                           uint32_t object_size)
{
    uint32_t filesize = codedir_filesize(codedir);
    uint32_t datasize = codedir_datasize(codedir);
    char* dataaddr = object_addr + filesize;

    if (object_size < (filesize + datasize)) {
        error("can't serialize code directory: buffer size %u cannot hold "
              "%u bytes", object_size, filesize + datasize);
        return -1;
    }

    return codedir_write(codedir, object_addr, dataaddr, datasize);
}

#endif
