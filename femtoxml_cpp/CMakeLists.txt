INCLUDE(UsePkgConfig)

PROJECT(femtoxml_cpp CXX)

SET(SOURCE_FILES
        src/element.cpp
        src/exception.cpp
        src/settings.cpp
        src/document.cpp
)

PKGCONFIG(femtoxml FEMTOXML_INCDIR FEMTOXML_LIBDIR FEMTOXML_LDFLAGS FEMTOXML_CFLAGS)

INCLUDE_DIRECTORIES(
        include
	${FEMTOXML_INCDIR}
)

SET(CPACK_PACKAGE_NAME femtoxml-cpp)
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9")
SET(CPACK_PACKAGE_VERSION_PATCH "7")
SET(CPACK_PACKAGE_VENDOR "Fredrik Hultin <noname@nurd.se>")
SET(CPACK_PACKAGE_CONTACT "Fredrik Hultin <noname@nurd.se>")
SET(CPACK_PACKAGE_DESCRIPTION "FemtoXML_CPP is a C++ wrapper for the FemtoXML library.")

ADD_LIBRARY(femtoxml_cpp STATIC ${SOURCE_FILES})

SET(CMAKE_C_FLAGS "-Wall -pedantic -ansi -g ${FEMTOXML_CFLAGS}")

MESSAGE("${CMAKE_LD_FLAGS}")
SET(CMAKE_LINKER_FLAGS "${FEMTOXML_LDFLAGS}")
MESSAGE("${CMAKE_LD_FLAGS}")



# Check if cmake has the deb-file generator
IF(EXISTS "${CMAKE_ROOT}/Modules/CPackDeb.cmake")
	SET(CPACK_GENERATOR DEB)

	FIND_PROGRAM(DPKG "dpkg")
	IF(NOT DPKG STREQUAL "DPKG-NOTFOUND")
		EXEC_PROGRAM(dpkg ARGS --print-architecture OUTPUT_VARIABLE CPACK_SYSTEM_NAME)
	ELSE(NOT DPKG STREQUAL "DPKG-NOTFOUND")
		SET(CPACK_SYSTEM_NAME "i386")
	ENDIF(NOT DPKG STREQUAL "DPKG-NOTFOUND")

	INCLUDE(CPack)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPackDeb.cmake")

#INSTALL(TARGETS femtoxml_cpp LIBRARY DESTINATION lib)
INSTALL(TARGETS femtoxml_cpp ARCHIVE DESTINATION lib)
INSTALL(FILES include/document.h DESTINATION include/femtoxml_cpp)
INSTALL(FILES include/exception.h DESTINATION include/femtoxml_cpp)
INSTALL(FILES include/element.h DESTINATION include/femtoxml_cpp)
INSTALL(FILES include/settings.h DESTINATION include/femtoxml_cpp)
INSTALL(FILES pkg-config/femtoxml_cpp.pc DESTINATION lib/pkgconfig)
