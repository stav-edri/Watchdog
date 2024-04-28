#include <string>
#include <windows.h>
#include <stdexcept>
#include "FilesWatchdog.h"
#include <iostream>


int main()
{
	// Choose directory to check
	std::wstring directory_path = L"C:\\temp\\ChangingDir";
	// Start watchdog
	FilesWatchdog watchdog(directory_path, FILE_NOTIFY_CHANGE_FILE_NAME);

	// Get notification
	try 
	{
		watchdog.start();
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}