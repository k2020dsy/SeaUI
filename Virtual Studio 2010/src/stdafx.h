// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:

#include <comdef.h>
#include <windows.h>
#include <windowsx.h>
#include <ShellAPI.h>
#include <iostream>
#include <process.h> 
#include <gdiplus.h>
#include <wingdi.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>  
#include <windows.h>
#include <math.h>
#include <string>
#include <locale>
#include <codecvt>
#include <mbctype.h>
#include <tchar.h>
#include <string>

#pragma  comment(lib, "gdiplus.lib")
#pragma  comment(lib, "gdi32.lib")
#pragma comment(lib,"msimg32.lib")
using namespace std; 
using namespace Gdiplus;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

#ifndef SEAUI_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif



