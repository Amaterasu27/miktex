#pragma once

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define WIN32_LEAN_AND_MEAN

#include <miktex/Core/Core>
#include <miktex/Core/Paths>
#include <miktex/Core/win/DllProc>

#include <windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include <Shellapi.h>
#include <Shlwapi.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#define T_(x) MIKTEXTEXT(x)

#define UNEXPECTED_CONDITION(function)				\
  FATAL_MIKTEX_ERROR (function, T_("Unexpected condition."), 0)

#define FATAL_MIKTEX_ERROR(miktexFunction, traceMessage, lpszInfo)	\
  Session::FatalMiKTeXError (miktexFunction,				\
			     traceMessage,				\
			     lpszInfo,					\
			     __FILE__,					\
			     __LINE__)
