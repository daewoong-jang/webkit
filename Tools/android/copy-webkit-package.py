#!/usr/bin/env python2

# Copyright (C) 2016 NAVER Corp. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import os, sys
import mmap
import re
import shutil

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        sys.exit(1)

    filepath = sys.argv[1]
    if not os.path.isfile(filepath):
        sys.exit(-1)

    size = os.stat(filepath).st_size

    fp = open(filepath)
    filedata = mmap.mmap(fp.fileno(), size, access=mmap.ACCESS_READ)

    match = re.search('package (.+);', filedata)
    package = match.group(1)
    package = package.replace('.', '/')

    def copyFile(srcfile, destdir):
        if not os.path.isfile(srcfile):
            return

        mtime = os.stat(srcfile).st_mtime
        filename = os.path.basename(srcfile)

        dest = os.path.normpath(destdir)
        if not os.path.exists(dest):
            os.makedirs(dest)

        dest = os.path.normpath(os.path.join(dest, filename))
        if not os.path.exists(dest) or os.stat(dest).st_mtime < mtime:
            shutil.copyfile(srcfile, dest)

    copyFile(filepath, os.path.join(sys.argv[2], package))

    fileroot = os.path.join(sys.argv[3], package)
    destpath = os.path.join(sys.argv[4], package)

    filepath = filepath.replace('.java', '.h')
    copyFile(filepath, destpath)

    filename = os.path.basename(filepath)
    filepath = os.path.join(fileroot, 'Managed', filename)
    copyFile(filepath, os.path.join(destpath, 'Managed'))

    filepath = os.path.join(fileroot, 'Natives', filename)
    copyFile(filepath, os.path.join(destpath, 'Natives'))
