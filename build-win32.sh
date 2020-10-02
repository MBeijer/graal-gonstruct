#!/bin/bash
export TOOLCHAIN="i686-w64-mingw32.shared"
export PREFIX="/usr/src/mxe/usr/${TOOLCHAIN}"
./prepare/mingw-w32.sh
# shellcheck disable=SC2046
cmake -S. -Bbuild-win32 && cmake --build build-win32 -- -j $(nproc)
MINGW_BUNDLEDLLS_SEARCH_PATH="${PREFIX}/bin" mingw-bundledlls --copy bin/gonstruct.exe