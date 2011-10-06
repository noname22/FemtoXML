========
FemtoXML
========

About
=====

FemtoXML is a small XML DOM parser written i C to suite the author's personal needs for XML parsing. There is also a wrapper for C++ kept up to date with the C library, called FemtoXML_CPP. The library is sufficiently fast for most applications. It loads a fairly advanced XML file, ~25k lines 1.6MB, in just below 0.2 seconds on a 1.67 GHz Intel core. This when compiled with GCC 4.1.3 using O3 optimizations. The same file then consumes about 5 MB RAM as a DOM structure, with an additional 1.6 MB during loading. FemtoXML and FemtoXML_CPP are released under the GNU GPLv3 license.

Authors
=======

Right now only me, Fredrik Hultin. I can be contacted by e-mail: noname @ nurd dot se.

Status
======

FemtoXML hasn't been extensively tested but I haven't had any problems with it. I did do some testing using the excellent fuzzer zzuf by Sam Hocevar which exposed at least one bug in the parser, but femtoxml never segfaulted. Please report any bugs you encounter.
Documentation

Installation
============

Using cmake
-----------

cd femtoxml (or femtoxml_cpp)
mkdir build && cd build
cmake ..
make && make install

Using spank
-----------
cd femtoxml (or femtoxml_cpp) 
spank install
