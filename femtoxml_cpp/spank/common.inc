name            femtoxml_cpp
version         0.97

template        c++
cflags          Wall O3 Iinclude
targettype      lib-static
sourcedir       src

inst_mkdir      $(inst_prefix)/include/femtoxml_cpp
inst_copy       include/document.h  $(inst_prefix)/include/femtoxml_cpp
inst_copy       include/exception.h $(inst_prefix)/include/femtoxml_cpp
inst_copy       include/settings.h  $(inst_prefix)/include/femtoxml_cpp
inst_copy       include/element.h   $(inst_prefix)/include/femtoxml_cpp

inst_copy       $(target)           $(inst_prefix)/lib

inst_mkdir      $(inst_prefix)lib/pkgconfig
inst_copy       pkg-config/femtoxml_cpp.pc $(inst_prefix)/lib/pkgconfig
