#include "WinErrorThrower.h"
#include <string>
#include <memory>
#include <stdexcept>

void WinErrorThrower::throw_last_error() 
{
    DWORD error_code = GetLastError();
    if (error_code == 0) 
        return;

    LPWSTR message_buffer = nullptr;
    size_t size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, 
        error_code, 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        (LPWSTR)&message_buffer, 
        0, 
        NULL
    );

    // Use unique_ptr with custom deleter to ensure memory gets released
    std::unique_ptr<wchar_t, decltype(&LocalFree)> message_guard(message_buffer, LocalFree);

    std::wstring message(message_guard.get(), size);
    throw std::runtime_error(std::string(message.begin(), message.end()));
}
