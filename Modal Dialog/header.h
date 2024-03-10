#pragma once

#include <windows.h>
#include <windowsX.h>
#include "resource.h"

#include <tchar.h>
#include <fstream>

#ifdef _UNICODE
typedef std::wfstream tfstream;
typedef std::wifstream tifstream;
#else
typedef std::fstream tfstream;
typedef std::ifstream tifstream;
#endif

typedef std::basic_string<TCHAR> tstring;
