/*
	InAppFilteringCM
	© ultrashot 2011-2013
*/
#include "stdafx.h"
#include "regext.h"
#include "..\..\common\aygshell7_sessions.h"
#include "KeyAction.h"
#include "debug.h"

static HMODULE hLib = NULL;

typedef int (*GETSCHEMAFACTORY)(VOID *, VOID *,VOID *, VOID *);
typedef void (*SCHEMAUNLOADED)(VOID *, VOID *,VOID *, VOID *);
typedef int (*FOURPARAM)(void *arg1, void *arg2, void *arg3, void *arg4);

static GETSCHEMAFACTORY getSchemaFactory = NULL;
static SCHEMAUNLOADED schemaUnloaded = NULL;

static FOURPARAM hwKeyboardDeployed = NULL;
static FOURPARAM initialize = NULL;
static FOURPARAM processterminated = NULL;
static FOURPARAM registr = NULL;
static FOURPARAM setfilterstring = NULL;
static FOURPARAM setfocus = NULL;
static FOURPARAM setresultcount = NULL;
static FOURPARAM show = NULL;
static FOURPARAM uninitialize = NULL;
static FOURPARAM searchInvoked = NULL;

static int ticks = 0;

static void Load()
{
	if (hLib == NULL)
	{
		hLib = LoadLibrary(L"InAppFilteringCMStock.dll");
		RETAILMSG(ENABLELOG, (L"[INAPPFILTERINGCM] hLib = %X\n", hLib));
		if (hLib)
		{
			getSchemaFactory = (GETSCHEMAFACTORY)GetProcAddress(hLib, L"UIXGetSchemaFactory");
			schemaUnloaded = (SCHEMAUNLOADED)GetProcAddress(hLib, L"UIXSchemaUnloaded");
			hwKeyboardDeployed = (FOURPARAM)GetProcAddress(hLib, L"?HardwareKeyboardDeployed@IAF@@YAHH@Z");
			initialize = (FOURPARAM)GetProcAddress(hLib, L"?Initialize@IAF@@YAJXZ");
			processterminated = (FOURPARAM)GetProcAddress(hLib, L"?ProcessTerminated@IAF@@YAXK@Z");
			registr = (FOURPARAM)GetProcAddress(hLib, L"?Register@PSL@IAF@@YAJK@Z");
			setfilterstring = (FOURPARAM)GetProcAddress(hLib, L"?SetFilterString@PSL@IAF@@YAXPBGK@Z");
			setfocus = (FOURPARAM)GetProcAddress(hLib, L"?SetFocus@PSL@IAF@@YAXHK@Z");
			setresultcount = (FOURPARAM)GetProcAddress(hLib, L"?SetResultCount@PSL@IAF@@YAXIK@Z");
			show = (FOURPARAM)GetProcAddress(hLib, L"?Show@PSL@IAF@@YAXHK@Z");
			uninitialize = (FOURPARAM)GetProcAddress(hLib, L"?Uninitialize@IAF@@YAJXZ");
			searchInvoked = (FOURPARAM)GetProcAddress(hLib, L"?SearchInvoked@IAF@@YAHXZ");
		}
	}
}

namespace IAF
{
	__declspec(dllexport) int SearchInvoked(void)
	{
		int val = 0;
		__try
		{
			if (ProcessAction() == false)
			{
				Load();
				val = searchInvoked(NULL,NULL,NULL,NULL);
				return val;
			}
			else
			{
				return 1;
			}
		}
		__except (1)
		{
		}
		return val;
	}

	__declspec(dllexport) int HardwareKeyboardDeployed(int arg1)
	{
		int val = 0;
		__try 
		{
			Load();
			val = hwKeyboardDeployed((LPVOID)arg1, NULL, NULL, NULL);
		}
		__except (1)
		{
		}
		return val;
	}

	__declspec(dllexport) long Initialize(void)
	{
		long val = 0;
		__try 
		{
			Load();
			val = initialize(0,0,0,0);
		}
		__except (1)
		{
		}
		return val;
	}

	__declspec(dllexport) void ProcessTerminated(unsigned long arg1)
	{
		__try 
		{
			Load();
			processterminated((LPVOID)arg1, NULL, NULL, NULL);
		}
		__except (1)
		{
		}
	}

	namespace PSL
	{
		__declspec(dllexport) long Register(unsigned long arg1)
		{
			long val = 0;
			__try 
			{
				Load();
				val = registr((LPVOID)arg1, NULL, NULL, NULL);
			}
			__except (1)
			{
			}
			return val;
		}

		__declspec(dllexport) void SetFilterString(unsigned short const *arg1, unsigned long arg2)
		{
			__try 
			{	
				Load();
				setfilterstring((LPVOID)arg1, (LPVOID)arg2, NULL, NULL);
			}
			__except (1)
			{
			}
		}

		__declspec(dllexport) void SetFocus(int arg1, unsigned long arg2)
		{
			__try
			{
				Load();
				setfocus((LPVOID)arg1, (LPVOID)arg2, NULL, NULL);
			}
			__except (1)
			{
			}
		}

		__declspec(dllexport) void SetResultCount(unsigned int arg1, unsigned long arg2)
		{
			__try
			{
				Load();
				setresultcount((LPVOID)arg1, (LPVOID)arg2, NULL, NULL);
			}
			__except (1)
			{
			}
		}

		__declspec(dllexport) void Show(int arg1, unsigned long arg2)
		{
			__try 
			{
				Load();
				show((LPVOID)arg1, (LPVOID)arg2, NULL, NULL);
			}
			__except (1)
			{
			}
		}

	}

	__declspec(dllexport) long Uninitialize(void)
	{
		long val = 0;
		__try 
		{
			Load();
			val = uninitialize(NULL, NULL, NULL, NULL);
		}
		__except (1)
		{
		}
		return val;
	}
}

int UIXGetSchemaFactory(DWORD *dw, DWORD dw2, DWORD dw3, DWORD dw4)
{
	int val = 0;
	__try
	{
		Load();
		val = getSchemaFactory(dw, (LPVOID)dw2, (LPVOID)dw3, (LPVOID)dw4);
	}
	__except (1)
	{
	}
	return val;
}

void UIXSchemaUnloaded(DWORD *dw, DWORD dw2, DWORD dw3, DWORD dw4)
{
	__try 
	{
		Load();
		schemaUnloaded(dw, (LPVOID)dw2, (LPVOID)dw3, (LPVOID)dw4);
	}
	__except (1)
	{
	}
}


BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	return TRUE;
}

