#include "FilesWatchdog.h"
#include <stdexcept>
#include "WinErrorThrower.h"

void FilesWatchdog::start()
{
	_running = true;
	for (auto& notification : _notifications)
	{
		_first_search(notification);
	}

	// TODO: Make asynchronous
	_listen_all_once();
	while (_running)
	{
		for (auto& notification : _notifications)
		{
			_next_search(notification);
		}
		
		_listen_all_once();
	}
}

void FilesWatchdog::_first_search(NotificationData &notification)
{
	HANDLE changeNotification = FindFirstChangeNotification(
		notification.data.path.c_str(), 
		TRUE, 
		FILE_NOTIFY_CHANGE_FILE_NAME);

	if (INVALID_HANDLE_VALUE == changeNotification)
	{
		WinErrorThrower::throw_last_error(); // Runtime error
	}

	notification.handle.reset(&AutoCloseHandle(changeNotification, FindCloseChangeNotification));
}

void FilesWatchdog::_next_search(NotificationData& notification)
{
	if (!FindNextChangeNotification(notification.handle.get()))
	{
		WinErrorThrower::throw_last_error();
	}
}

void FilesWatchdog::_listen_all_once()
{
	// vector of handles to send to WinAPI
	std::vector<HANDLE> handles;
	for (const auto& notification : _notifications)
	{
		handles.push_back(notification.handle.get());
	}

	DWORD waitResult = WaitForMultipleObjectsEx(
		static_cast<DWORD>(_notifications.size()), 
		handles.data(), 
		FALSE, 
		5 * 1000, 
		TRUE);

	// Wait result is non indexed
	switch (waitResult)
	{
	case WAIT_IO_COMPLETION:
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
		break;
	}

	// Wait result is abandoned
	if (waitResult - WAIT_ABANDONED_0 > 0 && waitResult - WAIT_ABANDONED_0 < _notifications.size())
	{}

	// For now do nothing. 
	// TODO: In the future we will need to bring change information.
	int object_index = waitResult - WAIT_OBJECT_0;
}
