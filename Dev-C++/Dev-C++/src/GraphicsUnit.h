/* GraphicsUnit.h  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don¡¯t know about,
Therefore, please use it with caution. */

#pragma once

#include "stdafx.h"
#include <malloc.h>

using namespace std;
using namespace Gdiplus;
	

	class GraphicsUnit{

	public:

		GraphicsUnit(){i=0;}
		int i;

		wchar_t * CharToWCHAR(char _str[]){
				char str[MAX_PATH];
			strcpy(str,_str);
			int WideCharacterLength = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
			wchar_t *WcharWd;
			WcharWd = (wchar_t*)malloc(sizeof(WCHAR)*WideCharacterLength);
			memset(WcharWd, 0, sizeof(WcharWd));
			MultiByteToWideChar(CP_ACP, 0, str, -1, WcharWd, WideCharacterLength);//Parkage the mulit into wide
			return WcharWd;
			WcharWd=NULL;
		}
		/*This function will be used when you need to transform the string "fuck.jpg" into L"fuck.jpg"
		*/
		
		

		int DrawImage(HDC hdc,char s_FileName[MAX_PATH],int _X,int _Y,int _width,int _height){
				int nX=_X,nY=_Y,nwidth=_width,nheight=_height;
				GdiplusStartupInput gdiplusStartupInput;
				ULONG_PTR    gdiplusToken;
				GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
				Graphics graphics(hdc);
				Image* pImage = new Image(CharToWCHAR(s_FileName));
    			if ((pImage==NULL)||(pImage->GetLastStatus()!=Ok)) {
					cout<<"\nerr"<<endl;
					if (pImage){
					nX=0; nY=0; nwidth=40; nheight=40;
					}
			} 
		graphics.DrawImage(pImage, nX, nY, nwidth, nheight);
		delete[]pImage;
		pImage = NULL;
		return 0;
		}
		//int DrawImage(HDC hdc,Image *&  pImage,int X,int Y,int width,int height);
		/*the following code is can only completed in C++11
		*/
		
	};



	











