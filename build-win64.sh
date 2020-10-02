#!/bin/bash
export TOOLCHAIN="x86_64-w64-mingw32.shared"
export PREFIX="/usr/src/mxe/usr/${TOOLCHAIN}"
./prepare/mingw-w64.sh
# shellcheck disable=SC2046
cmake -S. -Bbuild-win64 && cmake --build build-win64 -- -j $(nproc)
MINGW_BUNDLEDLLS_SEARCH_PATH="${PREFIX}/bin" mingw-bundledlls --copy bin/gonstruct.exe
MINGW_BUNDLEDLLS_SEARCH_PATH="${PREFIX}/bin" mingw-bundledlls --copy bin/libcore.dll