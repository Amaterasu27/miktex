#include "StdAfx.h"
#include "resource.h"

using namespace std;
using namespace MiKTeX::Core;

#define SHOW_DIALOG 0

#define TRAYICONID 1

#define SWM_TRAYMSG		WM_APP

#if SHOW_DIALOG
#define SWM_SHOW		WM_APP + 1
#define SWM_HIDE		WM_APP + 2
#endif

#define SWM_SETTINGS		WM_APP + 3
#define SWM_UPDATE		WM_APP + 4
#define SWM_BROWSE_PACKAGES	WM_APP + 5
#define SWM_PREVIEWER		WM_APP + 6
#define SWM_COMMAND_PROMPT	WM_APP + 7
#define SWM_TEXWORKS		WM_APP + 8
#define SWM_EXIT		WM_APP + 10

static HINSTANCE hInst;
static NOTIFYICONDATAW niData;

/* __________________________________________________________________________

   ShowContextMenu
   __________________________________________________________________________*/

void
ShowContextMenu (/*[in]*/ HWND hWnd)
{
  POINT pt;
  GetCursorPos (&pt);
  HMENU hMenu = CreatePopupMenu();
  if (hMenu == 0)
    {
      UNEXPECTED_CONDITION ("ShowContextMenu");
    }
#if SHOW_DIALOG
  if (IsWindowVisible(hWnd))
    {
      InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_HIDE, L"Hide");
    }
  else
    {
      InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_SHOW, L"Show");
    }
#endif
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_TEXWORKS, L"TeXworks");
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_SETTINGS, L"Settings");
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_UPDATE, L"Update");
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_BROWSE_PACKAGES, L"Browse Packages");
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_PREVIEWER, L"Previewer");
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_COMMAND_PROMPT, L"Command Prompt");
  InsertMenuW (hMenu, -1, MF_SEPARATOR, 0, 0);
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_EXIT, L"Exit");
  SetForegroundWindow (hWnd);
  TrackPopupMenu (hMenu,
		  TPM_BOTTOMALIGN,
		  pt.x,
		  pt.y,
		  0,
		  hWnd,
		  0);
  DestroyMenu (hMenu);
}

/* __________________________________________________________________________

   About
   __________________________________________________________________________*/

static
INT_PTR
CALLBACK
About (/*[in]*/ HWND	hDlg,
       /*[in]*/ UINT	message,
       /*[in]*/ WPARAM	wParam,
       /*[in]*/ LPARAM	lParam)
{
  switch (message)
    {
    case WM_INITDIALOG:
      {
	return (TRUE);
      }
    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
	{
	  EndDialog (hDlg, LOWORD(wParam));
	  return (TRUE);
	}
      break;
    }
  return (FALSE);
}

/* __________________________________________________________________________

   OnInitDialog
   __________________________________________________________________________*/

BOOL
OnInitDialog (/*[in]*/ HWND hWnd)
{
  HMENU hMenu = GetSystemMenu(hWnd, FALSE);
  if (hMenu != 0)
    {
      AppendMenuW (hMenu, MF_SEPARATOR, 0, 0);
      AppendMenuW (hMenu, MF_STRING, IDM_ABOUT, L"About");
    }
  HICON hIcon =
    static_cast<HICON>(LoadImageW(hInst,
				  MAKEINTRESOURCEW(IDI_MIKTEX),
				  IMAGE_ICON,
				  0,
				  0,
				  LR_SHARED|LR_DEFAULTSIZE));
  SendMessage (hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
  SendMessage (hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
  return (TRUE);
}

/* __________________________________________________________________________

   DlgProc
   __________________________________________________________________________*/

static
INT_PTR
CALLBACK
DlgProc (/*[in]*/ HWND		hWnd,
	 /*[in]*/ UINT		message,
	 /*[in]*/ WPARAM	wParam,
	 /*[in]*/ LPARAM	lParam)
{
  try
    {
      SessionWrapper pSession (true);

      int wmId, wmEvent;
      
      switch (message) 
	{
	case SWM_TRAYMSG:
	  switch(lParam)
	    {
#if SHOW_DIALOG
	    case WM_LBUTTONDBLCLK:
	      ShowWindow (hWnd, SW_RESTORE);
	      break;
#endif
	    case WM_RBUTTONDOWN:
	    case WM_CONTEXTMENU:
	      ShowContextMenu (hWnd);
	    }
	  break;
	case WM_SYSCOMMAND:
	  if((wParam & 0xFFF0) == SC_MINIMIZE)
	    {
	      ShowWindow (hWnd, SW_HIDE);
	      return (1);
	    }
	  else if (wParam == IDM_ABOUT)
	    {
	      DialogBoxW (hInst,
			  MAKEINTRESOURCEW(IDD_ABOUT),
			  hWnd,
			  About);
	    }
	  break;
	case WM_COMMAND:
	  wmId    = LOWORD(wParam);
	  wmEvent = HIWORD(wParam); 
	  switch (wmId)
	    {
#if SHOW_DIALOG
	    case SWM_SHOW:
	      ShowWindow (hWnd, SW_RESTORE);
	      break;
	    case SWM_HIDE:
	    case IDOK:
	      ShowWindow (hWnd, SW_HIDE);
	      break;
#endif
	    case SWM_TEXWORKS:
	      {
		PathName exePath;
		if (pSession->FindFile(MIKTEX_TEXWORKS_EXE, FileType::EXE, exePath))
		  {
		    pSession->UnloadFilenameDatabase ();
		    Process::Start (exePath);
		  }
		break;
	      }
	    case SWM_SETTINGS:
	      {
		PathName exePath;
		if (pSession->FindFile(MIKTEX_MO_EXE, FileType::EXE, exePath))
		  {
		    pSession->UnloadFilenameDatabase ();
		    Process::Start (exePath);
		  }
		break;
	      }
	    case SWM_UPDATE:
	      {
		PathName exePath;
		if (pSession->FindFile(MIKTEX_UPDATE_EXE,
				       FileType::EXE,
				       exePath))
		  {
		    pSession->UnloadFilenameDatabase ();
		    Process::Start (exePath);
		  }
		break;
	      }
	    case SWM_BROWSE_PACKAGES:
	      {
		PathName exePath;
		if (pSession->FindFile(MIKTEX_MPM_MFC_EXE, FileType::EXE, exePath))
		  {
		    pSession->UnloadFilenameDatabase ();
		    Process::Start (exePath);
		  }
		break;
	      }
	    case SWM_PREVIEWER:
	      {
		PathName exePath;
		if (pSession->FindFile(MIKTEX_YAP_EXE, FileType::EXE, exePath))
		  {
		    pSession->UnloadFilenameDatabase ();
		    Process::Start (exePath);
		  }
		break;
	      }
	    case SWM_COMMAND_PROMPT:
	      {
		string oldPath;
		bool haveOldPath = Utils::GetEnvironmentString("PATH", oldPath);
		string newPath = pSession->GetSpecialPath(SpecialPath::BinDirectory).Get();
		if (haveOldPath)
		{
		  newPath += ';';
		  newPath += oldPath;
		}
		Utils::SetEnvironmentString ("PATH", newPath.c_str());
		string cmd;
		if (! Utils::GetEnvironmentString("COMSPEC", cmd))
		{
		  cmd = "cmd.exe";
		}
		pSession->UnloadFilenameDatabase ();
		Process::Start (cmd);
		if (haveOldPath)
		{
		  Utils::SetEnvironmentString ("PATH", oldPath.c_str());
		}
		break;
	      }
	    case SWM_EXIT:
	      DestroyWindow (hWnd);
	      break;
	    case IDM_ABOUT:
	      DialogBoxW (hInst,
			  MAKEINTRESOURCEW(IDD_ABOUT),
			  hWnd,
			  About);
	      break;
	    }
	  return (1);
	case WM_INITDIALOG:
	  return (OnInitDialog(hWnd));
	case WM_CLOSE:
	  DestroyWindow (hWnd);
	  break;
	case WM_DESTROY:
	  niData.uFlags = 0;
	  Shell_NotifyIconW (NIM_DELETE, &niData);
	  PostQuitMessage (0);
	  break;
	}
      return (0);
    }
  catch (const exception &)
    {
      return (0);
    }
}

/* __________________________________________________________________________

   GetDllVersion
   __________________________________________________________________________*/

static
ULONGLONG
GetDllVersion (/*[in]*/ const char * lpszDllName)
{
  ULONGLONG ullVersion = 0;
  DllProc1<HRESULT, DLLVERSIONINFO*> dllGetVersion (lpszDllName, "DllGetVersion");
  DLLVERSIONINFO dvi;
  ZeroMemory (&dvi, sizeof(dvi));
  dvi.cbSize = sizeof(dvi);
  HRESULT hr = dllGetVersion(&dvi);
  if(SUCCEEDED(hr))
  {
    ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion,
      dvi.dwMinorVersion,
      0,
      0);
  }
  return (ullVersion);
}

/* __________________________________________________________________________

   InitInstance
   __________________________________________________________________________*/

void
InitInstance (/*[in]*/ HINSTANCE	hInstance,
	      /*[in]*/ int		nCmdShow)
{
  InitCommonControls ();
  
  hInst = hInstance;
  
  HWND hWnd =
    CreateDialogW(hInstance,
		  MAKEINTRESOURCEW(IDD_STATUS),
		  0,
		  DlgProc);

  if (hWnd == 0)
    {
      UNEXPECTED_CONDITION ("InitInstance");
    }

  ZeroMemory (&niData, sizeof(NOTIFYICONDATA));

  ULONGLONG ullVersion = GetDllVersion("Shell32.dll");
  if(ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
    {
      niData.cbSize = sizeof(NOTIFYICONDATA);
    }
  else
    {
      niData.cbSize = NOTIFYICONDATA_V2_SIZE;
    }

  niData.uID = TRAYICONID;

  niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

  niData.hIcon =
    reinterpret_cast<HICON>(LoadImageW(hInstance,
				       MAKEINTRESOURCEW(IDI_MIKTEX),
				       IMAGE_ICON,
				       GetSystemMetrics(SM_CXSMICON),
				       GetSystemMetrics(SM_CYSMICON),
				       LR_DEFAULTCOLOR));

  niData.hWnd = hWnd;

  niData.uCallbackMessage = SWM_TRAYMSG;

  if (SessionWrapper(true)->IsMiKTeXPortable())
    {
      wcscpy_s (niData.szTip, sizeof(niData.szTip), L"MiKTeX Portable");
    }
  else
    {
      wcscpy_s (niData.szTip, sizeof(niData.szTip), L"MiKTeX");
    }

  Shell_NotifyIconW (NIM_ADD, &niData);

  if (niData.hIcon != 0 && DestroyIcon(niData.hIcon))
    {
      niData.hIcon = 0;
    }
}

/* __________________________________________________________________________

   WinMain
   __________________________________________________________________________*/

int
WINAPI
WinMain (/*[in]*/ HINSTANCE	hInstance,
	 /*[in]*/ HINSTANCE	hPrevInstance,
	 /*[in]*/ char *	lpCmdLine,
	 /*[in]*/ int		nCmdShow)
{
  try
    {
      SessionWrapper pSession (Session::InitInfo("miktex-taskbar-icon"));

      InitInstance (hInstance, nCmdShow);
      
      HACCEL hAccelTable =
	LoadAcceleratorsW(hInstance,
			  MAKEINTRESOURCEW(IDC_STATUSDIALOG));
      
      MSG msg;
      
      while (GetMessage(&msg, 0, 0, 0))
	{
	  if (! TranslateAccelerator(msg.hwnd, hAccelTable, &msg)
	      || ! IsDialogMessage(msg.hwnd, &msg))
	    {
	      TranslateMessage (&msg);
	      DispatchMessage (&msg);
	    }
	}
      
      return (static_cast<int>(msg.wParam));
    }
  catch (const exception &)
    {
      return (0);
    }
}
