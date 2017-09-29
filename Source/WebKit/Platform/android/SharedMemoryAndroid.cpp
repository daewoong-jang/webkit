/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Copyright (c) 2010 University of Szeged
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2013 NAVER Corp.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#if USE(UNIX_DOMAIN_SOCKETS)
#include "SharedMemory.h"

#include "Decoder.h"
#include "Encoder.h"
#include "WebCoreArgumentCoders.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wtf/Assertions.h>
#include <wtf/CurrentTime.h>
#include <wtf/RandomNumber.h>
#include <wtf/text/CString.h>
#include <wtf/text/WTFString.h>
#include <cutils/ashmem.h>

namespace WebKit {

SharedMemory::Handle::Handle()
{
}

SharedMemory::Handle::~Handle()
{
    clear();
}

void SharedMemory::Handle::clear()
{
    m_attachment = IPC::Attachment();
}

bool SharedMemory::Handle::isNull() const
{
    return m_attachment.fileDescriptor() == -1;
}

void SharedMemory::Handle::encode(IPC::Encoder& encoder) const
{
    encoder << releaseAttachment();
}

bool SharedMemory::Handle::decode(IPC::Decoder& decoder, Handle& handle)
{
    ASSERT_ARG(handle, handle.isNull());

    IPC::Attachment attachment;
    if (!decoder.decode(attachment))
        return false;

    handle.adoptAttachment(WTFMove(attachment));
    return true;
}

IPC::Attachment SharedMemory::Handle::releaseAttachment() const
{
    return WTFMove(m_attachment);
}

void SharedMemory::Handle::adoptAttachment(IPC::Attachment&& attachment)
{
    ASSERT(isNull());

    m_attachment = WTFMove(attachment);
}

RefPtr<SharedMemory> SharedMemory::allocate(size_t size)
{
    CString tempName;

    int fileDescriptor = -1;
    for (int tries = 0; fileDescriptor == -1 && tries < 10; ++tries) {
        String name = String("/WK2SharedMemory.") + String::number(static_cast<unsigned>(WTF::randomNumber() * (std::numeric_limits<unsigned>::max() + 1.0)));
        tempName = name.utf8();

        do {
            fileDescriptor = ashmem_create_region(tempName.data(), size);
        } while (fileDescriptor == -1 && errno == EINTR);
    }
    if (fileDescriptor == -1)
        return 0;

    int err = ashmem_set_prot_region(fileDescriptor, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (err < 0)
        return 0;

    void* data = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    if (data == MAP_FAILED) {
        while (close(fileDescriptor) == -1 && errno == EINTR) { }
        return 0;
    }

    RefPtr<SharedMemory> instance = adoptRef(new SharedMemory());
    instance->m_data = data;
    instance->m_fileDescriptor = fileDescriptor;
    instance->m_size = size;
    return instance;
}

static inline int accessModeMMap(SharedMemory::Protection protection)
{
    switch (protection) {
    case SharedMemory::Protection::ReadOnly:
        return PROT_READ;
    case SharedMemory::Protection::ReadWrite:
        return PROT_READ | PROT_WRITE;
    }

    ASSERT_NOT_REACHED();
    return PROT_READ | PROT_WRITE;
}

RefPtr<SharedMemory> SharedMemory::map(const Handle& handle, Protection protection)
{
    ASSERT(!handle.isNull());

    void* data = mmap(0, handle.m_attachment.size(), accessModeMMap(protection), MAP_SHARED, handle.m_attachment.fileDescriptor(), 0);
    if (data == MAP_FAILED)
        return nullptr;

    RefPtr<SharedMemory> instance = adoptRef(new SharedMemory());
    instance->m_data = data;
    instance->m_fileDescriptor = handle.m_attachment.releaseFileDescriptor();
    instance->m_size = handle.m_attachment.size();
    return instance;
}

SharedMemory::~SharedMemory()
{
    munmap(m_data, m_size);
    while (close(m_fileDescriptor.value()) == -1 && errno == EINTR) { }
}

static inline int accessModeFile(SharedMemory::Protection protection)
{
    switch (protection) {
    case SharedMemory::Protection::ReadOnly:
        return PROT_READ;
    case SharedMemory::Protection::ReadWrite:
        return PROT_READ | PROT_WRITE;
    }

    ASSERT_NOT_REACHED();
    return O_RDWR;
}

bool SharedMemory::createHandle(Handle& handle, Protection protection)
{
    ASSERT_ARG(handle, handle.isNull());

    int duplicatedHandle;
    while ((duplicatedHandle = dup(m_fileDescriptor.value())) == -1) {
        if (errno != EINTR) {
            ASSERT_NOT_REACHED();
            return false;
        }
    }

    int err = ashmem_set_prot_region(duplicatedHandle, accessModeFile(protection));
    if (err < 0) {
        ASSERT_NOT_REACHED();
        while (close(duplicatedHandle) == -1 && errno == EINTR) { }
        return false;
    }
    handle.m_attachment = IPC::Attachment(duplicatedHandle, m_size);
    return true;
}

unsigned SharedMemory::systemPageSize()
{
    static unsigned pageSize = 0;

    if (!pageSize)
        pageSize = getpagesize();

    return pageSize;
}

} // namespace WebKit

#endif
