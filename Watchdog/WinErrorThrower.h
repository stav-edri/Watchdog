#pragma once
#include <windows.h>

class WinErrorThrower
{
public:
	static void throw_last_error();
};

