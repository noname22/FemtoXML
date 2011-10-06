name            FemtoXML_CPP
version         0.97

template        c++
compiler        i586-mingw32msvc-g++
cflags          Wall O3 Iinclude
targettype      lib-static
target          femtoxml_cpp
sourcedir       src

inst_copy       include/document.h  $(inst_prefix)/include/femtoxml_cpp
inst_copy       include/exception.h $(inst_prefix)/include/femtoxml_cpp
inst_copy       include/settings.h  $(inst_prefix)/include/femtoxml_cpp
inst_copy       include/element.h   $(inst_prefix)/include/femtoxml_cpp

inst_copy       lib$(target).a      $(inst_prefix)/lib

inst_copy       pkg-config/femtoxml_cpp.pc $(inst_prefix)/lib