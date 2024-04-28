#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "AutoCloseHandle.h"

struct FileSearchData
{
	std::wstring path;
	DWORD		 filters = 0;
};

class FilesWatchdog
{
private:
	struct NotificationData {
		FileSearchData data;
		std::unique_ptr<AutoCloseHandle> handle;
	};

public:
	FilesWatchdog(std::wstring path, DWORD filters)
	{
		NotificationData notification;
		notification.data.path	 = path;
		notification.data.filters = filters;

		_notifications.push_back(std::move(notification));
	}
	FilesWatchdog(std::vector<FileSearchData> searches)
	{
		for (auto search : searches)
		{
			NotificationData notification;
			notification.data.path = search.path;
			notification.data.filters = search.filters;
			_notifications.push_back(std::move(notification));
		}
	}
	
	void start();
	void stop();

private:
	void _first_search(NotificationData &notification);
	void _next_search(NotificationData &notification);
	void _get_information(NotificationData &notification);
	void _listen_all_once();

private:
	std::vector<NotificationData> _notifications;
	// TODO: Thread safety
	bool _running = false;
};

