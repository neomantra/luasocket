/**
 Lua C module used only for test suite.
 This allows the client and server to run under different threads.

 Copyright (C) 2007-2012 LuaDist.
 Created by David Manura.
 Redistribution and use of this file is allowed according to the terms of the MIT license.
 For details see the COPYRIGHT file distributed with LuaDist.
 Please note that the package source code is licensed under its own license.
*/

#include <lua.h>
#include <lauxlib.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

static char path[1000];

#ifdef _WIN32
DWORD WINAPI thread_func(void * p)
#else
void * thread_func(void * p)
#endif
{
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);
	lua_pushboolean(L, 1);
	lua_setglobal(L, "RUN_THREAD");
	lua_newtable(L);
	lua_setglobal(L, "arg");
	if (luaL_loadfile(L, path) != 0)
		lua_error(L);
	lua_call(L, 0,0);
#ifdef _WIN32
	return 0;
#else
	return NULL;
#endif
}

int lrun(lua_State * L)
{
	#ifdef _WIN32
	HANDLE hthread;
	#else
	int iret;
	#endif
	
	strcpy(path, lua_tostring(L, 1));
	
	#ifdef _WIN32
	hthread = CreateThread(NULL, 0, thread_func, NULL, 0, NULL);
	if (hthread == NULL) exit(1);
	#else
	pthread_t thread1;
	iret = pthread_create( &thread1, NULL, thread_func, (void*)NULL);
	if (iret != 0) exit(1);
	#endif
	
	return 0;
}

int luaopen_testing_thread(lua_State * L)
{
	lua_pushcfunction(L, lrun);
	return 1;
}
