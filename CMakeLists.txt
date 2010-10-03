# Copyright (C) 2007-2009 LuaDist.
# Created by Peter Drahoš
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

PROJECT(luasocket C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
INCLUDE(dist.cmake)

# Shared library exporting.
IF (WIN32)
	ADD_DEFINITIONS("-DMIME_API=__declspec(dllexport)"
	                "-DLUASOCKET_API=__declspec(dllexport)")
ENDIF(WIN32)

IF (WIN32 AND NOT CYGWIN)
	SET(SOCKETC src/wsocket.c)
ELSE ()
	SET(SOCKETC src/usocket.c)
ENDIF (WIN32 AND NOT CYGWIN)

SET(SRC_SOCKET src/luasocket.c src/timeout.c src/buffer.c src/io.c
               src/auxiliar.c src/options.c src/inet.c src/tcp.c
               src/udp.c src/except.c src/select.c ${SOCKETC} )

IF (WIN32 AND NOT CYGWIN)
	SET(LIB_SOCKET ws2_32)
ENDIF (WIN32 AND NOT CYGWIN)

# Build modules.
ADD_LUA_MODULE(socket ${SRC_SOCKET})
TARGET_LINK_LIBRARIES(socket ${LIB_SOCKET})
ADD_LUA_MODULE(mime MODULE src/mime.c)

# Targets were renamed to avoid name clashes. NOTE: Module authors AVOID "core" modules, PLEASE!
SET_TARGET_PROPERTIES(socket PROPERTIES LIBRARY_OUTPUT_DIRECTORY socket OUTPUT_NAME core)
SET_TARGET_PROPERTIES(mime   PROPERTIES LIBRARY_OUTPUT_DIRECTORY mime   OUTPUT_NAME core)

# Install all files and documentation
INSTALL (TARGETS socket DESTINATION ${INSTALL_CMOD}/socket)
INSTALL (TARGETS mime   DESTINATION ${INSTALL_CMOD}/mime)
INSTALL (FILES src/ltn12.lua src/mime.lua src/socket.lua DESTINATION ${INSTALL_LMOD})
INSTALL (FILES src/ftp.lua src/http.lua src/smtp.lua src/tp.lua src/url.lua DESTINATION ${INSTALL_LMOD}/socket)
INSTALL (DIRECTORY etc/ DESTINATION ${INSTALL_FOO} PATTERN ".git" EXCLUDE)
INSTALL (DIRECTORY test/ DESTINATION ${INSTALL_TEST} PATTERN ".git" EXCLUDE)
INSTALL (DIRECTORY samples/ DESTINATION ${INSTALL_EXAMPLE} PATTERN ".git" EXCLUDE)
INSTALL (DIRECTORY doc/ DESTINATION ${INSTALL_DOC} PATTERN ".git" EXCLUDE)
INSTALL (FILES README NEW LICENSE DESTINATION ${INSTALL_DATA})
