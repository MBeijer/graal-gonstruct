#!/bin/bash
export GOBACK=$PWD
PREFIX=/usr/src/mxe/usr/x86_64-w64-mingw32.shared
cd $PREFIX

wget http://ftp.gnome.org/pub/gnome/binaries/win32/gtksourceview/2.10/gtksourceview-dev-2.10.0.zip -O gtksourceview.zip
7z x -y gtksourceview.zip
wget http://ftp.gnome.org/pub/gnome/binaries/win32/gtkmm/2.22/gtkmm-win32-devel-2.22.0-2.exe -O gtkmm.7z
7z x -y gtkmm.7z

wget http://ftp.gnome.org/pub/gnome/binaries/win32/glib/2.28/glib-dev_2.28.8-1_win32.zip -O glib.zip
7z x -y glib.zip

wget http://ftp.gnome.org/pub/gnome/sources/gtkglarea/2.1/gtkglarea-2.1.0.tar.xz -O gtkglarea.txz
tar xvf gtkglarea.txz
cd gtkglarea-2.1.0/
CC=x86_64-w64-mingw32.shared-gcc CXX=x86_64-w64-mingw32.shared-g++ CPP=x86_64-w64-mingw32.shared-cpp ./configure --prefix=$PREFIX --host=x86_64-w64-mingw32.shared --target=x86_64-w64-mingw32.shared --with-lib-opengl32
make -j`nproc`
make install
`