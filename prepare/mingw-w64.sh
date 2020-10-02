#!/bin/bash
export GOBACK="${PWD}"
export TOOLCHAIN="x86_64-w64-mingw32.shared"
export PREFIX="/usr/src/mxe/usr/${TOOLCHAIN}"

cd ${PREFIX}/

wget http://ftp.acc.umu.se/pub/gnome/binaries/win64/gtkmm/2.22/gtkmm-win64-devel-2.22.0-2.exe -O gtkmm.7z
7z x -y gtkmm.7z

wget http://ftp.acc.umu.se/pub/gnome/binaries/win64/glib/2.26/glib_2.26.1-1_win64.zip -O glib.zip
7z x -y glib.zip

wget http://ftp.acc.umu.se/pub/gnome/binaries/win64/glib/2.26/glib-dev_2.26.1-1_win64.zip -O glib.zip
7z x -y glib.zip

sed -i 's|/devel/target/9af0ed5b9823f125da25c47653b9c863|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/0b150273e7a8329c3935ca90f169d52d|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/94d7ecd65464faa6be24937a758e580f|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/18e9007417f775a5bc83c60917e98632|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/442a1689e374b79d5db4da9d6526dcca|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/8694ad67a8496a826c55cee759a59a36|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/c01378aafb50980950f7e58bcd36fc40|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|/win32/gtkmm_debug|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/51c6c4453aea27281e19d627c866a973|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|/usr/local|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/aaa7e17cc06d35f4b9deab283149826a|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/ab605b0e70119f93a0fa52114f734660|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/dbd53f05b5573494af068d8fd4463c54|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/bafe2c95df134ccce0f7fb71da2332ed|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/48d6e6a341fea05855999619c49d66f6|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/4baf4f94927a6ca0abcde5c3112e7501|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/d4846d031bd12a9a513e7500f80db6a9|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/1187c76b5bdd041823eecfa0050878e8|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/4d0add2a6f7c8ee39d34a3af8b451d08|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/895975facc6ebc4ed857e97878b6d3a5|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|/home/nacho/temp/win64/mingww64|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc
sed -i 's|/win64/gtkmm_debug|'${PREFIX}'|g' ${PREFIX}/lib/pkgconfig/*.pc

ln -sf /usr/src/mxe/usr/bin/${TOOLCHAIN}-windres /usr/src/mxe/usr/bin/windres

wget http://ftp.gnome.org/pub/gnome/sources/gtkglarea/2.1/gtkglarea-2.1.0.tar.xz -O gtkglarea.txz
tar xf gtkglarea.txz
cd ${PREFIX}/gtkglarea-2.1.0/
CFLAGS=$(/usr/src/mxe/usr/bin/$TOOLCHAIN-pkg-config --cflags glib-2.0 gdk-2.0) CXXFLAGS=$(/usr/src/mxe/usr/bin/$TOOLCHAIN-pkg-config --cflags glib-2.0 gdk-2.0) ./configure --prefix=${PREFIX} --host=${TOOLCHAIN} --target=${TOOLCHAIN} --with-lib-opengl32
make -j$(nproc)
make install

cd ${PREFIX}/

wget http://ftp.acc.umu.se/pub/gnome/sources/gtksourceview/2.10/gtksourceview-2.10.5.tar.bz2 -O gtksourceview.tbz2
tar xvf gtksourceview.tbz2
cd ${PREFIX}/gtksourceview-2.10.5/
CFLAGS=$(/usr/src/mxe/usr/bin/$TOOLCHAIN-pkg-config --cflags glib-2.0 gdk-2.0) CXXFLAGS=$(/usr/src/mxe/usr/bin/$TOOLCHAIN-pkg-config --cflags glib-2.0 gdk-2.0) ./configure --prefix=${PREFIX} --host=${TOOLCHAIN} --target=${TOOLCHAIN} --with-lib-opengl32
make -j$(nproc)
make install

wget https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.bz2 -O boost.tbz2
tar xf boost.tbz2
cd ${PREFIX}/boost_1_74_0/
echo "using gcc : : ${TOOLCHAIN}-g++ ;" > user-config.jam
CC=gcc CXX=g++ ./bootstrap.sh
./b2 --prefix=${PREFIX} --user-config=user-config.jam toolset=gcc target-os=windows address-model=64 architecture=x86 define=_GLIBCXX_USE_CXX11_ABI=0 release install

cd ${PREFIX}/
rm -rf ${PREFIX}/mingw-bundledlls
wget https://raw.githubusercontent.com/mpreisler/mingw-bundledlls/master/mingw-bundledlls -O ${PREFIX}/mingw-bundledlls
ln -sf ${PREFIX}/mingw-bundledlls /usr/src/mxe/usr/bin/mingw-bundledlls
chmod a+x ${PREFIX}/mingw-bundledlls
sed -i 's|"rpcrt4.dll"|"rpcrt4.dll", "libcore.dll"|g' /usr/src/mxe/usr/bin/mingw-bundledlls