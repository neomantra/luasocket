# Copyright (C) 2007-2012 LuaDist.
# Created by Peter Drahoš
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

project ( luasocket C )
cmake_minimum_required ( VERSION 2.8 )
include ( cmake/dist.cmake )
include ( lua )

# The standard luasocket build has this enabled in release mode.
# It is required for running the test suite.
add_definitions ( -DLUASOCKET_DEBUG )

if ( WIN32 )
  # Shared library exporting.
  add_definitions("-DMIME_API=__declspec(dllexport)" "-DLUASOCKET_API=__declspec(dllexport)")
  set ( SOCKETC src/wsocket.c )
  set ( LIB_SOCKET ws2_32 )
else ()
  set ( SOCKETC src/usocket.c )
endif ()

set ( SRC_SOCKET src/luasocket.c src/timeout.c src/buffer.c src/io.c src/auxiliar.c 
  src/options.c src/inet.c src/tcp.c src/udp.c src/except.c src/select.c ${SOCKETC} )

# Binary
install_lua_module ( socket.core ${SRC_SOCKET} LINK ${LIB_SOCKET} )
install_lua_module ( mime.core src/mime.c )
# Lua 
install_lua_module ( ltn12 src/ltn12.lua )
install_lua_module ( mime src/mime.lua )
install_lua_module ( socket src/socket.lua )
install_lua_module ( socket.ftp src/ftp.lua )
install_lua_module ( socket.http src/http.lua )
install_lua_module ( socket.smtp src/smtp.lua )
install_lua_module ( socket.tp src/tp.lua )
install_lua_module ( socket.url src/url.lua )

install_foo ( etc/ )
install_test ( test/ )
install_example ( samples/ )
install_doc ( doc/ )
install_data ( README NEW LICENSE )

#2DO: how to spawn testsvr.lua to run in parallel with testclnt.lua?
#~ add_lua_test ( test/testclnt.lua )
file ( WRITE t/t.lua " require 'socket' " ) # very basic test
add_lua_test ( t/t.lua )
