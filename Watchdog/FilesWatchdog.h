#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "AutoCloseHandle.h"
#include <mutex>

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
		AutoCloseHandle handle;
	};

public:
	FilesWatchdog(const std::wstring &path, const DWORD filters)
	{
		FileSearchData search_data;
		search_data.path	 = path;
		search_data.filters = filters;

		_inactive_notifications.push_back(std::move(search_data));
	}
	FilesWatchdog(const std::vector<FileSearchData> &searches)
	{
		for (const auto &search : searches)
		{
			FileSearchData search_data;
			search_data.path = search.path;
			search_data.filters = search.filters;

			_inactive_notifications.push_back(std::move(search_data));
		}
	}
	
	void start();
	void stop();

private:
	HANDLE _first_search(FileSearchData & notification);
	void _next_search(NotificationData &notification);
	void _get_information(NotificationData &notification);
	void _listen_all_once();

private:
	std::vector<FileSearchData> _inactive_notifications;
	std::vector<NotificationData> _notifications;
	// TODO: Thread safety
	std::atomic<bool> _running = false;
	std::mutex _inactive_notifications_mutex;
	std::mutex _notifications_mutex;
};

