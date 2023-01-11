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
 *  code_directory.h
 *  cctools libstuff
 *
 *  Created by Michael Trent on 8/6/20.
 */

#ifndef code_directory_h
#define code_directory_h
#ifdef CODEDIRECTORY_SUPPORT

#include <stdint.h>

struct codedir;

/*
 * codedir_is_linker_signed() takes an LC_CODE_SIGNATURE data payload and
 * returns 1 if it contains a valid ad-hoc signature signed by ld(1), and 0 if
 * it contains a valid signature written by codesign(1) or an invalid signature.
 */
int codedir_is_linker_signed(const char* data, uint32_t size);

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
__private_extern__
struct codedir* codedir_create(const char* output_path,
                               uint32_t filesize,
                               uint32_t ptralign,
                               uint32_t platform,
                               uint32_t minos);

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
__private_extern__
int codedir_create_object(const char* output_path,
                                       char* object_addr,
                                       uint32_t object_size,
                                       struct codedir** out_codedir);
/*
 * codedir_free() releases memory held by struct codedir.
 */
__private_extern__
void codedir_free(struct codedir* codedir);

/*
 * codedir_filesize() returns the size of the Mach-O being measured, not
 * including the size of the LC_CODE_SIGNATURE payload. In other words, this
 * value is also the location of the LC_CODE_SIGNATURE payload.
 */
__private_extern__
uint32_t codedir_filesize(const struct codedir* codedir);

/*
 * codedir_datasize() returns the size required to hold the LC_CODE_SIGNATURE
 * payload.
 */
__private_extern__
uint32_t codedir_datasize(const struct codedir* codedir);

/*
 * codedir_datasize_delta() returns the difference in size between the old
 * and new LC_CODE_SIGNATURE payloads if this struct codedir was created
 * via codedir_create_object(). Otherwise, will return codedir_datasize().
 */
__private_extern__
int32_t codedir_datasize_delta(const struct codedir* codedir);

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
 * codedir_write() returns 0 on success, -1 on error.
 */
__private_extern__
int codedir_write(const struct codedir* codedir,
                  const char* input,
                  char* output,
                  uint32_t output_size);

/*
 * codedir_write_object() computes the LC_CODE_SIGNATURE payload for a whole
 * Mach-O binary. 'object_addr' points to a fully laid out and finalized
 * non-relocatable Mach-O file, with an LC_CODE_SIGNATURE load command pointing
 * to the correct location within the file's __LINKEDIT segment. The
 * 'object_addr' memory must be large enough to hold the new LC_CODE_SIGNATURE
 * payload. 'object_size' is the size of the 'object_addr' memory buffer.
 *
 * codedir_write_object() returns 0 on success, -1 on error.
 */
__private_extern__
int codedir_write_object(const struct codedir* codedir,
                         char* object_addr,
                         uint32_t object_size);

#endif /* CODEDIRECTORY_SUPPORT */
#endif /* code_directory_h */
