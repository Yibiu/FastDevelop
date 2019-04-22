#pragma once
#include <stdlib.h>
#include <time.h>
#include <string>
#ifdef WIN32
#include <Windows.h>
#endif


#ifdef WIN32
static std::wstring char_to_wchar(const char *ansy)
{
	std::wstring wansy_string = L"";
	int len = MultiByteToWideChar(CP_ACP, 0, ansy, strlen(ansy), NULL, 0);
	if (0 == len)
		return wansy_string;

	wchar_t *wansy = new wchar_t[len + 1];
	if (NULL != wansy) {
		MultiByteToWideChar(CP_ACP, 0, ansy, strlen(ansy), wansy, len);
		wansy[len] = L'\0';
		wansy_string = wansy;

		delete[] wansy;
	}

	return wansy_string;
}

static std::string wchar_to_char(const wchar_t *wansy)
{
	std::string ansy_string = "";
	int len = WideCharToMultiByte(CP_ACP, 0, wansy, wcslen(wansy), NULL, 0, NULL, NULL);
	if (0 == len)
		return ansy_string;

	char *ansy = new char[len + 1];
	if (NULL != ansy) {
		WideCharToMultiByte(CP_ACP, 0, wansy, wcslen(wansy), ansy, len, NULL, NULL);
		ansy[len] = '\0';

		ansy_string = ansy;
		delete[] ansy;
	}

	return ansy_string;
}


// Get elapsed time in us from Unix Epoch
inline uint64_t get_timestamp_us() {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	// Convert to Unix Epoch
	uint64_t current = (((uint64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
	current -= 116444736000000000LL;
	current /= 10;
	return current;
}

static uint64_t get_time_ms()
{
	SYSTEMTIME tm_sys;
	GetLocalTime(&tm_sys);
	tm tm_time;
	tm_time.tm_year = tm_sys.wYear - 1900;
	tm_time.tm_mon = tm_sys.wMonth - 1;
	tm_time.tm_mday = tm_sys.wDay;
	tm_time.tm_hour = tm_sys.wHour;
	tm_time.tm_min = tm_sys.wMinute;
	tm_time.tm_sec = tm_sys.wSecond;
	tm_time.tm_isdst = -1;
	time_t clock = mktime(&tm_time);
	return ((uint64_t)clock * 1000 + tm_sys.wMilliseconds);
}

#else

static uint64_t get_time_ms()
{
	timeval time_tv;
	gettimeofday(&time_tv, NULL);
	return ((uint64_t)time_tv.tv_sec * 1000 + time_tv.tv_usec / 1000);
}

#endif

