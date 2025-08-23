#!/bin/bash -eu
#
# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-License-Identifier: LGPL-2.0-or-later
#
# This script must be run after kio-extras/thumbnail/autotests/ossfuzz/build_fuzzers.sh

export PATH="$WORK/bin:$WORK/libexec:$PATH"
export PKG_CONFIG="$(which pkg-config) --static"
export PKG_CONFIG_PATH="$WORK/lib/pkgconfig:$WORK/share/pkgconfig:$WORK/lib/x86_64-linux-gnu/pkgconfig"

# For PoCreator
cd $SRC
tar -xJf gettext-*.tar.xz && rm -rf gettext-*.tar.xz
cd gettext-*/gettext-tools
./configure --disable-shared --enable-static --prefix $WORK
make -C libgettextpo install -j$(nproc)

cd $SRC/kdesdk-thumbnailers
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH=$WORK \
    -DCMAKE_INSTALL_PREFIX=$WORK \
    -DBUILD_FUZZERS=ON \
    -DFUZZERS_USE_QT_MINIMAL_INTEGRATION_PLUGIN=ON \
    -DBUILD_SHARED_LIBS=OFF
ninja -C build -j$(nproc)

EXTENSIONS="pothumbnail_fuzzer po"

echo "$EXTENSIONS" | while read fuzzer_name extensions; do
    # copy the fuzzer binary
    cp build/bin/fuzzers/$fuzzer_name $OUT

    # create seed corpus
    for extension in $extensions; do
        find . -name "*.$extension" -exec zip -q "$OUT/${fuzzer_name}_seed_corpus.zip" {} +
    done
done
