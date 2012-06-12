
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#include <malloc.h>

HMODULE s_hmod;

HMODULE Win32DllGetModuleHandle()
{
	return s_hmod;
}

/*----------------------------------------------------------------------------------------------
	DllMain. This is the main DLL entry point for a non-MFC DLL. For an MFC DLL, DllMain is
		in DllModul.cpp. Both DllMains call ModuleEntry::DllMain.
----------------------------------------------------------------------------------------------*/
extern "C" BOOL WINAPI DllMain(HMODULE hmod, DWORD dwReason, PVOID /*pvReserved*/)
{
	bool fRet = true;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		// REVIEW DarrellZ: This is temporary until MSFT fixes the realloc bug.
		// See the comment at the beginning of ModuleEntry::WinMain.
#if ! defined(MIKTEX)
		_set_sbh_threshold(0);
#endif

		s_hmod = hmod;
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_PROCESS_DETACH:
		// Check for memory leaks.
//		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
		break;

	case DLL_THREAD_DETACH:
		break;

	default:
		fRet = false;
		break;
	}

	return fRet;
}
