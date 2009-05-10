#include "StdAfx.h"
#include "resource.h"

#define SHOW_DIALOG 0

#define TRAYICONID 1

#define SWM_TRAYMSG		WM_APP

#if SHOW_DIALOG
#define SWM_SHOW		WM_APP + 1
#define SWM_HIDE		WM_APP + 2
#endif

#define SWM_SETTINGS		WM_APP + 3
#define SWM_UPDATE		WM_APP + 4
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
      return;
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
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_SETTINGS, L"Settings");
  InsertMenuW (hMenu, -1, MF_BYPOSITION, SWM_UPDATE, L"Update");
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

/* __________________________________________________________________________

   InitInstance
   __________________________________________________________________________*/

static
ULONGLONG
GetDllVersion (/*[in]*/ const wchar_t * lpszDllName)
{
  ULONGLONG ullVersion = 0;
  HINSTANCE hinstDll;
  hinstDll = LoadLibraryW(lpszDllName);
  if (hinstDll != 0)
    {
      DLLGETVERSIONPROC pDllGetVersion;
      pDllGetVersion =
	reinterpret_cast<DLLGETVERSIONPROC>(GetProcAddress(hinstDll,
							   "DllGetVersion"));
      if (pDllGetVersion != 0)
        {
	  DLLVERSIONINFO dvi;
	  HRESULT hr;
	  ZeroMemory (&dvi, sizeof(dvi));
	  dvi.cbSize = sizeof(dvi);
	  hr = (*pDllGetVersion)(&dvi);
	  if(SUCCEEDED(hr))
	    {
	      ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion,
					 dvi.dwMinorVersion,
					 0,
					 0);
	    }
	}
      FreeLibrary (hinstDll);
    }
  return (ullVersion);
}

/* __________________________________________________________________________

   InitInstance
   __________________________________________________________________________*/

BOOL
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
      return (FALSE);
    }

  ZeroMemory (&niData, sizeof(NOTIFYICONDATA));

  ULONGLONG ullVersion = GetDllVersion(L"Shell32.dll");
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

  wcscpy_s (niData.szTip, sizeof(niData.szTip), L"MiKTeX");

  Shell_NotifyIconW (NIM_ADD, &niData);

  if (niData.hIcon != 0 && DestroyIcon(niData.hIcon))
    {
      niData.hIcon = 0;
    }

  return (TRUE);
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
  if (! InitInstance(hInstance, nCmdShow))
    {
      return (FALSE);
    }

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
