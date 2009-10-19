PROJECT(femtoxml C)

SET(SOURCE_FILES
        src/message.c
        src/output.c
        src/parse.c
        src/structure.c
        src/vfile.c
)

SET(CPACK_PACKAGE_NAME femtoxml)
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VENDOR "Fredrik Hultin <noname@nurd.se>")
SET(CPACK_PACKAGE_CONTACT "Fredrik Hultin <noname@nurd.se>")
SET(CPACK_PACKAGE_DESCRIPTION "FemtoXML is an easy to use XML-parser c-library.")

INCLUDE_DIRECTORIES(
        include
)

ADD_LIBRARY(femtoxml SHARED ${SOURCE_FILES})

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

INSTALL(TARGETS femtoxml LIBRARY DESTINATION lib)
INSTALL(FILES include/femtoxml.h DESTINATION include/femtoxml)
INSTALL(FILES pkg-config/femtoxml.pc DESTINATION lib/pkgconfig)
