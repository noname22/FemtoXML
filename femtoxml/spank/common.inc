name          femtoxml
version       0.97
cflags        std=c99 O3
cflags        Iinclude
targettype    lib-static

inst_mkdir  $(inst_prefix)include/femtoxml
inst_copy   include/femtoxml.h        $(inst_prefix)include/femtoxml
inst_copy   $(target)            $(inst_prefix)lib
inst_mkdir  $(inst_prefix)lib/pkgconfig
inst_copy   pkg-config/femtoxml.pc    $(inst_prefix)lib/pkgconfig
