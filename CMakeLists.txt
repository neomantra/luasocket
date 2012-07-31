# Copyright (C) 2007-2012 LuaDist.
# Created by Peter Drahoš, David Manura
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

# Shared library exporting.
if ( WIN32 )
  add_definitions ( "-DMIME_API=__declspec(dllexport)" "-DLUASOCKET_API=__declspec(dllexport)" )
endif ( )

if ( WIN32 AND NOT CYGWIN )
  set ( SOCKETC src/wsocket.c )
else ( )
  set ( SOCKETC src/usocket.c )
endif ( )

set ( SRC_SOCKET src/luasocket.c src/timeout.c src/buffer.c src/io.c src/auxiliar.c 
  src/options.c src/inet.c src/tcp.c src/udp.c src/except.c src/select.c ${SOCKETC} )

if ( WIN32 AND NOT CYGWIN )
  set ( LIB_SOCKET ws2_32 )
endif ( )

# Binary
install_lua_module ( socket.core ${SRC_SOCKET} LINK ${LIB_SOCKET} )
install_lua_module ( mime.core src/mime.c )
# Lua 
install_lua_module ( ltn12 src/ltn12.lua )
install_lua_module ( mime src/mime.lua )
install_lua_module ( socket src/socket.lua )
install_lua_module ( socket.ftp src/ftp.lua )
install_lua_module ( socket.headers src/headers.lua )
install_lua_module ( socket.http src/http.lua )
install_lua_module ( socket.smtp src/smtp.lua )
install_lua_module ( socket.tp src/tp.lua )
install_lua_module ( socket.url src/url.lua )

install_foo ( etc/ )
install_test ( test/ )
install_example ( samples/ )
install_doc ( doc/ )
install_data ( README NEW LICENSE )

# very basic test
file ( WRITE ${CMAKE_CURRENT_BINARY_DIR}/t/t.lua " require 'socket' " )
add_lua_test ( ${CMAKE_CURRENT_BINARY_DIR}/t/t.lua )

# test with client-server.
# This is more complicated because the test suite client and server must be
# run in different threads (or different processes).
find_package ( Threads )
if ( Threads_FOUND )
  add_lua_module ( testing_thread test/testing_thread.c LINK ${CMAKE_THREAD_LIBS_INIT} )
  file ( WRITE ${CMAKE_CURRENT_BINARY_DIR}/t/clientserver.lua "
    local socket = require 'socket'
    function sleep(sec) socket.select(nil, nil, sec) end
    print('RUN_THREAD=', RUN_THREAD)
    if RUN_THREAD then
      dofile '${CMAKE_CURRENT_SOURCE_DIR}/test/testsrvr.lua'
    else
      require 'testing_thread' '${CMAKE_CURRENT_BINARY_DIR}/clientserver.lua'
      sleep(2) -- allow some time for server to start
      dofile '${CMAKE_CURRENT_SOURCE_DIR}/test/testclnt.lua'
    end
  " )
  add_lua_test ( ${CMAKE_CURRENT_BINARY_DIR}/t/clientserver.lua )
else ()
  message ( WARNING "skipping tests (no threading)" )
endif ()
