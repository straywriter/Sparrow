/*
 * Copyright 2010-2021 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */
#define CATCH_CONFIG_RUNNER

#include "test.h"

int runAllTests(int _argc, const char* _argv[]);


int runAllTests(int _argc, const char* _argv[])
{
	DBG("Compiler: " BX_COMPILER_NAME
		", CPU: " BX_CPU_NAME
		", Architecture: " BX_ARCH_NAME
		", OS: " BX_PLATFORM_NAME
		", CRT: " BX_CRT_NAME
		", Date: " __DATE__
		", Time: " __TIME__
		", C++: " BX_CPP_NAME
		);
	return Catch::Session().run(_argc, _argv);
}


#if BX_PLATFORM_ANDROID
#	include <android/native_activity.h>

static const char* s_argv[] = { "bx.test" };

void ANativeActivity_onCreate(ANativeActivity*, void*, size_t)
{
	exit(runAllTests(BX_COUNTOF(s_argv), s_argv) );
}
#else


int main(int _argc, const char* _argv[])
{
	return runAllTests(_argc, _argv);
}
#endif // BX_PLATFORM
