
#pragma   once  
#ifndef MYWINEASY_H
#define MYWINEASY_H
#include <windows.h>
#include <iostream>

using namespace std;


class MyWinEasy
{
	public:
		bool MyRegWindowClass(TCHAR szAppName[],WNDPROC WndProc);
		void LockWindowsSystem(); 
		
	private:
	protected:
}*MyWinC,MyWinCS;


void MyWinEasy::LockWindowsSystem()
{
	//ShellExecute(NULL,"open","rundll32.exe","USER32,LockWorkStation","",SW_SHOW);
}

bool MyWinEasy::MyRegWindowClass(TCHAR szAppName[],WNDPROC WndProc)
{
	WNDCLASS wndclass ;//wndclass类设置窗口样式
	wndclass.style = CS_HREDRAW | CS_VREDRAW ;// 窗口样式
	wndclass.lpfnWndProc = WndProc ;//窗口回调
	wndclass.cbClsExtra = 0 ;
	wndclass.cbWndExtra = 0 ;
	wndclass.hInstance = GetModuleHandle (NULL); 
	wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION) ;//图标
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW) ;//鼠标
	wndclass.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BTNHIGHLIGHT|COLOR_BTNSHADOW);// 背景颜色样式 
	wndclass.lpszMenuName = NULL ;//菜单
	wndclass.lpszClassName = szAppName;//窗口类名
	if (!RegisterClass (&wndclass)){
		MessageBox(0,"Fail to register Windows Class","ERROR",MB_ICONERROR);
		return false;
	}else return true;
}

#endif
