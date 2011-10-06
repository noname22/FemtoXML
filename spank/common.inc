name          FemtoXML
version       0.97
cflags        std=c99 O3
cflags        Iinclude
targettype    lib-static
target        femtoxml

inst_copy   include/femtoxml.h        $(inst_prefix)include
inst_copy   lib$(target).a            $(inst_prefix)lib

