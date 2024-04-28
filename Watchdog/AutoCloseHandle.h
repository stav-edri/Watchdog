#pragma once
#include <Windows.h>
#include <memory>

using AutoCloseHandle = std::unique_ptr<void, decltype(&CloseHandle)>;