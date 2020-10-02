#!/bin/bash
export GOBACK=$PWD
PREFIX=/usr/src/mxe/usr/i686-w64-mingw32.shared
cd $PREFIX

wget http://ftp.gnome.org/pub/gnome/binaries/win32/gtksourceview/2.10/gtksourceview-dev-2.10.0.zip -O gtksourceview.zip
7z x -y gtksourceview.zip

wget http://ftp.gnome.org/pub/gnome/binaries/win32/gtkmm/2.22/gtkmm-win32-devel-2.22.0-2.exe -O gtkmm.7z
7z x -y gtkmm.7z

wget http://ftp.gnome.org/pub/gnome/binaries/win32/glib/2.28/glib-dev_2.28.8-1_win32.zip -O glib.zip
7z x -y glib.zip

sed -i 's|/devel/target/9af0ed5b9823f125da25c47653b9c863|'$PREFIX'|g' /usr/src/mxe/usr/i686-w64-mingw32.shared/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/0b150273e7a8329c3935ca90f169d52d|'$PREFIX'|g' /usr/src/mxe/usr/i686-w64-mingw32.shared/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/94d7ecd65464faa6be24937a758e580f|'$PREFIX'|g' /usr/src/mxe/usr/i686-w64-mingw32.shared/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/18e9007417f775a5bc83c60917e98632|'$PREFIX'|g' /usr/src/mxe/usr/i686-w64-mingw32.shared/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/442a1689e374b79d5db4da9d6526dcca|'$PREFIX'|g' /usr/src/mxe/usr/i686-w64-mingw32.shared/lib/pkgconfig/*.pc
sed -i 's|c:/devel/target/8694ad67a8496a826c55cee759a59a36|'$PREFIX'|g' /usr/src/mxe/usr/i686-w64-mingw32.shared/lib/pkgconfig/*.pc

wget http://ftp.gnome.org/pub/gnome/sources/gtkglarea/2.1/gtkglarea-2.1.0.tar.xz -O gtkglarea.txz
tar xvf gtkglarea.txz
cd gtkglarea-2.1.0/
CC=i686-w64-mingw32.shared-gcc CXX=i686-w64-mingw32.shared-g++ \
	CPP=i686-w64-mingw32.shared-cpp CFLAGS="`/usr/src/mxe/usr/bin/i686-w64-mingw32.shared-pkg-config --cflags glib-2.0 gdk-2.0`" \
	CXXFLAGS="`/usr/src/mxe/usr/bin/i686-w64-mingw32.shared-pkg-config --cflags glib-2.0 gdk-2.0`" \
	./configure --prefix=$PREFIX --host=i686-w64-mingw32.shared --target=i686-w64-mingw32.shared --with-lib-opengl32
make -j`nproc`
make install