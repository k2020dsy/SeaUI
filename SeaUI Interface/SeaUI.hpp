#pragma once

#pragma comment(lib,"SeaUI.lib")//链接库文件
#include <comdef.h>
#include <windows.h>
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
#include <tchar.h>

using namespace std; 
using namespace Gdiplus;

#pragma  comment(lib, "SeaUI.lib")
#pragma  comment(lib, "gdiplus.lib")
#pragma  comment(lib, "gdi32.lib")
#pragma comment(lib,"msimg32.lib")

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif



using namespace std;
#define SEA_UI_ON true
#define SEA_UI_OFF false


#define SUS_LAYER 1l
#define SUS_TEXTBOX 2l
#define SUS_NORMAL 3l
#define SUS_MASK 4l
#define SUS_PROGRESSBAR 5l

#define SUD_UP 1
#define SUD_DOWN 2
#define SUD_LEFT 3
#define SUD_RIGHT 4

typedef void (*_MessagePass)();

class DLLIMPORT _SEAUIWINDOW{
	
	public:

		void SetImage(char nBitmapName[],int cX,int cY);
		void ButtonHitAnimation(bool);
		void MouseHoverAnimation(bool);
		void MouseLeaveAnimation(bool);
		void ButtonAnimationInitialize(int BaiscValue,int Increment);

		void DrawFrame(HDC hdc);
		void DrawTxtOnPaint(HDC);
		HWND GetHwnd();
		void RemoveFrame();
		void RePaint();
		void Roundrect(int radius,BOOL Sswitch);
		void RemoveWindow();

		void SetAlphaValue(int);
		void SetCoordinates(int,int);
		void SetFrameInfo(int Thickness,COLORREF clr,BOOL autoclr,BOOL _3Dstyel);
		void SetThreadState(bool state);
		void SetAnimationStyle(LONG style);
		void SetCallback(UINT message,_MessagePass nMessagePass);
		void SetAlphaAnimation(int oldAlpha,int newAlpha,int Frame,int DelayEachFrame);

		void Password(bool IsPassword);
		void SetForegroundBitmap(char*,int,int);
		void SetBackgroundImage(char*,int,int,int x,int y);
		void Transform(COLORREF from,COLORREF to);
		void SetWindowLong(long style);
		void SetTextBoxLong(long style);
		void CoordinateLock();
		char*GetText();
		
		/******************ProgressBar function*************/
		void SetPercentage(float _percent);
		float GetClickPosition();
		float GetPercentage();
		void SetProgressBarDirection(int);
		/*************************************************/ 
		virtual void SetTextSize(int,int)=0;
		virtual void SetTextColor(COLORREF color)=0;
		virtual bool Create(HWND,char*,int,int,int,int,HMENU)=0;
		

};

void RemoveWindow(_SEAUIWINDOW*seauiw){
	seauiw->RemoveWindow();
	delete[]seauiw;
	seauiw=NULL;
}

DLLIMPORT _SEAUIWINDOW *Initialization(long Style);



HWND GetConsoleHWND(){
	
	typedef HWND (WINAPI *PROCGETCONSOLEWINDOW)();
	
	PROCGETCONSOLEWINDOW _GetConsoleWindow;

	HMODULE hKernel32 = GetModuleHandle("kernel32");
	
	_GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");
	
	return _GetConsoleWindow();
	
	FreeLibrary(hKernel32);
}

bool RegWindow(TCHAR szAppName[],WNDPROC WndProc,HWND hwnd,HINSTANCE hInstance)
{
   
    WNDCLASS     wndclass ;
    wndclass.style = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc = WndProc ;
	wndclass.cbClsExtra = 0 ;
	wndclass.cbWndExtra = 0 ;
	wndclass.hInstance = hInstance ;
	wndclass.hIcon = LoadIcon (NULL, IDI_ASTERISK ) ;
	wndclass.hCursor = LoadCursor (NULL, IDC_ICON) ;//IDC_WAIT 
	wndclass.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BTNHIGHLIGHT|COLOR_BTNSHADOW);// COLOR_BTNFACE
	wndclass.lpszMenuName = NULL ;
	wndclass.lpszClassName = szAppName ;

    if( !RegisterClass(&wndclass) )
    {
        MessageBox( NULL, TEXT("无法注册窗口类!"), szAppName, MB_OK | MB_ICONERROR ) ;
        return false;
    }


    hwnd = CreateWindow( szAppName, TEXT(szAppName),
        WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL ) ;
        
    UpdateWindow( hwnd ) ;
   return true;

}

void DrawPicture(HDC hdc,char BitmapName[],int width,int height){
	
	HDC hdcMem = CreateCompatibleDC(hdc);

	HDC memDCMAP = CreateCompatibleDC(hdc);
    //创建一个bmp内存空间
    HBITMAP hBmp = CreateCompatibleBitmap(hdc,width,height);
    //将bmp内存空间分配给内存DC
    HBITMAP hOldSel = (HBITMAP)::SelectObject(hdcMem,hBmp);
     
    //这是使用者需要绘制的画面,全部往内存DC绘制
     // 绘制代码.......................
	HBITMAP hBkbmp = (HBITMAP)LoadImage(NULL, BitmapName, IMAGE_BITMAP,  width, height, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	SelectObject(memDCMAP,hBkbmp);    

	BitBlt(hdcMem,0,0,width,height,memDCMAP,0,0,SRCCOPY);
     
    //将内存DC的内容复制到屏幕显示DC中,完成显示
    BitBlt(hdc,0,0,width,height,hdcMem,0,0,SRCCOPY);
   	 //清除资源
     
    DeleteDC(hdcMem);    DeleteDC(memDCMAP);   

	DeleteObject(hBmp);

	DeleteObject(hBkbmp);

	DeleteObject(hOldSel);
} 


