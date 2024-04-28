#include <string>
#include <windows.h>
#include <iostream>
#include "AutoCloseHandle.h"
#include "WinErrorThrower.h"

HANDLE start_watchdog(std::wstring path)
{
	HANDLE changeNotification = FindFirstChangeNotification(path.c_str(), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);
	return changeNotification;
}

int main()
{
	// Choose directory to check
	std::wstring directory_path = L"C:\\temp\\ChangingDir";
	// Start watchdog
	AutoCloseHandle watchdog(start_watchdog(directory_path));

	// Get notification
	switch (WaitForSingleObject(watchdog.get(), INFINITE))
	{
	case WAIT_OBJECT_0:
		std::cout << "A change has occured!" << std::endl;
		break;
	case WAIT_TIMEOUT:
	case WAIT_ABANDONED:
	case WAIT_FAILED:
		break;
	}
	// Deal with notification
	return 0;
}