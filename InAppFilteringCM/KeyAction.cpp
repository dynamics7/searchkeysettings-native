#include "stdafx.h"
#include "regext.h"
#include "KeyAction.h"
#include "..\..\common\aygshell7_sessions.h"
#include "..\..\common\pacmanclient.h"
#include "..\..\common\Process.hpp"
#include "..\..\common\Tasks.hpp"
#include "..\..\common\dynamics\SearchKey.hpp"

static void RunBing(wchar_t *args)
{
	Diagnostics::Process::TerminateIfRunning(L"websearch.exe");
	Diagnostics::Process::Start(L"websearch.exe", args);
}

static void RunSearch()
{
	// getting search provider 
	wchar_t searchProvider[500] = {0};
	RegistryGetString(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\SearchProviders", L"DefaultProvider", searchProvider, 500);
	if (wcslen(searchProvider) > 0)
	{
		wchar_t fullKeyName[500];
		wcscpy_s(fullKeyName, 500, L"Software\\Microsoft\\Internet Explorer\\SearchProviders\\");
		wcscat_s(fullKeyName, 500, searchProvider);
		wchar_t searchUrl[500] = {0};

		RegistryGetString(HKEY_CURRENT_USER, fullKeyName, L"Url", searchUrl, 500);

		if (wcslen(searchUrl) > 0)
		{
			wchar_t *pSearchUrl = searchUrl;
			if (wcsrchr(pSearchUrl, L'{'))
				*wcsrchr(pSearchUrl, L'{') = L'\0';
			wchar_t appUri[500];

			wcscpy_s(appUri, 500, L"app://5B04B775-356B-4AA0-AAF8-6491FFEA5660/_default?StartURL=");
			wcscat_s(appUri, 500, pSearchUrl);
			Shell::Tasks::StartSession(appUri);
		}
	}
	else
	{
		// running bing
		wchar_t appUri[500];
		wcscpy_s(appUri, 500, L"app://5B04B775-356B-4AA0-AAF8-6491FFEA5660/_default?StartURL=");
		wcscat_s(appUri, 500, L"http://m.bing.com");
		Shell::Tasks::StartSession(appUri);
	}
}

static void RunBrowserUrl()
{
	// getting url
	wchar_t url[500] = {0};
	RegistryGetString(HKEY_LOCAL_MACHINE, L"Software\\OEM\\WebSearchOverride", L"Url", url, 500);
	if (wcslen(url) > 0)
	{
		wchar_t appUri[500];
		wcscpy_s(appUri, 500, L"app://5B04B775-356B-4AA0-AAF8-6491FFEA5660/_default?StartURL=");
		wcscat_s(appUri, 500, url);
		Shell::Tasks::StartSession(appUri);
	}
}

bool ProcessAction()
{
	DWORD dwAction = Dynamics::SearchKeyAction::ACTION_BING;
	RegistryGetDWORD(HKEY_LOCAL_MACHINE, L"Software\\OEM\\WebSearchOverride", L"ActionType", &dwAction);

	bool processed = true;
	if (Dynamics::SearchKey::IsValidHash() == FALSE || Dynamics::SearchKey::IsSearchKeySettingsXapPresent() == false)
		return false;
	switch (dwAction)
	{
	case Dynamics::SearchKeyAction::ACTION_NO:
		{
			break;
		}
	case Dynamics::SearchKeyAction::ACTION_BING:
		{
			RunBing(NULL);
			break;
		}
	case Dynamics::SearchKeyAction::ACTION_SEARCH:
		{
			RunSearch();
			break;
		}
	case Dynamics::SearchKeyAction::ACTION_RUNAPPLICATION:
		{
			wchar_t appUri[500] = {0};
			RegistryGetString(HKEY_LOCAL_MACHINE, L"Software\\OEM\\WebSearchOverride", L"Uri", appUri, 500);
			Shell::Tasks::StartSession(appUri);
			break;
		}
	case Dynamics::SearchKeyAction::ACTION_EXE:
		{
			wchar_t exePath[500] = {0};
			RegistryGetString(HKEY_LOCAL_MACHINE, L"Software\\OEM\\WebSearchOverride", L"ExePath", exePath, 500);

			wchar_t exeArgs[500] = {0};
			RegistryGetString(HKEY_LOCAL_MACHINE, L"Software\\OEM\\WebSearchOverride", L"ExeArgs", exeArgs, 500);

			Diagnostics::Process::Start(exePath, exeArgs, NULL, true);
			break;
		}
	case Dynamics::SearchKeyAction::ACTION_URL:
		{
			RunBrowserUrl();
			break;
		}
	case Dynamics::SearchKeyAction::ACTION_KEYEVENT:
		{
			DWORD dwKeyCode = 0;
			RegistryGetDWORD(HKEY_LOCAL_MACHINE, L"Software\\OEM\\WebSearchOverride", L"KeyCode", &dwKeyCode);
			if (dwKeyCode != 0)
			{
				keybd_event(dwKeyCode, 0, KEYEVENTF_SILENT, 0);
				Sleep(100);
				keybd_event(dwKeyCode, 0, KEYEVENTF_SILENT | KEYEVENTF_KEYUP, 0);
			}
			break;
		}
	default:
		processed = false;
		break;
	}
	return processed;
}
