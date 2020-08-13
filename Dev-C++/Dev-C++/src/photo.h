
#pragma   once  
#include "stdafx.h"


#ifndef PHOTO_H
#define PHOTO_H


#if(WINVER >= 0x0500)
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

//#define LWA_COLORKEY 0x00000001
//#define LWA_ALPHA 0x00000002

typedef BOOL (FAR WINAPI *LAYERFUNC)(HWND,COLORREF,BYTE,DWORD);

BOOL _SetLayeredWindowAttributes(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags)

{

LAYERFUNC SetLayer;

HMODULE hmod = LoadLibrary("user32.dll");

SetLayer=(LAYERFUNC)GetProcAddress(hmod,"SetLayeredWindowAttributes");

BOOL bReturn = SetLayer(hwnd,crKey,bAlpha,dwFlags);

FreeLibrary(hmod);

return bReturn;

}
bool _TransparentBltU(
     HDC dcDest,         // handle to Dest DC
     int nXOriginDest,   // x-coord of destination upper-left corner
     int nYOriginDest,   // y-coord of destination upper-left corner
     int nWidthDest,     // width of destination rectangle
     int nHeightDest,    // height of destination rectangle
     HDC dcSrc,          // handle to source DC
     int nXOriginSrc,    // x-coord of source upper-left corner
     int nYOriginSrc,    // y-coord of source upper-left corner
     int nWidthSrc,      // width of source rectangle
     int nHeightSrc,     // height of source rectangle
     UINT crTransparent  // color to make transparent
  );


/**Function TransparentBltU is made by Christian Graus in WinDEV.
I made some modifications to fit this program*/

bool _TransparentBltU(
     HDC dcDest,         // handle to Dest DC
     int nXOriginDest,   // x-coord of destination upper-left corner
     int nYOriginDest,   // y-coord of destination upper-left corner
     int nWidthDest,     // width of destination rectangle
     int nHeightDest,    // height of destination rectangle
     HDC dcSrc,          // handle to source DC
     int nXOriginSrc,    // x-coord of source upper-left corner
     int nYOriginSrc,    // y-coord of source upper-left corner
     int nWidthSrc,      // width of source rectangle
     int nHeightSrc,     // height of source rectangle
     UINT crTransparent,  // color to make transparent
     int AlphaValue
  )
{
if (nWidthDest < 1) return false;
if (nWidthSrc < 1) return false;
if (nHeightDest < 1) return false;
if (nHeightSrc < 1) return false;

     HDC dc = CreateCompatibleDC(NULL);
     HBITMAP bitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, GetDeviceCaps(dc,
                                                              BITSPIXEL), NULL);

if (bitmap == NULL)
     {
         DeleteDC(dc);    
return false;
     }

     HBITMAP oldBitmap = (HBITMAP)SelectObject(dc, bitmap);

if (!BitBlt(dc, 0, 0, nWidthSrc, nHeightSrc, dcSrc, nXOriginSrc,
                                                         nYOriginSrc, SRCCOPY))
     {
         SelectObject(dc, oldBitmap); 
         DeleteObject(bitmap);        
         DeleteDC(dc);                
return false;
     }

     HDC maskDC = CreateCompatibleDC(NULL);
     HBITMAP maskBitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, 1, NULL);

if (maskBitmap == NULL)
     {
         SelectObject(dc, oldBitmap); 
         DeleteObject(bitmap);        
         DeleteDC(dc);                
         DeleteDC(maskDC);            
return false;
     }

     HBITMAP oldMask =  (HBITMAP)SelectObject(maskDC, maskBitmap);

     SetBkColor(maskDC, RGB(0,0,0));
     SetTextColor(maskDC, RGB(255,255,255));
if (!BitBlt(maskDC, 0,0,nWidthSrc,nHeightSrc,NULL,0,0,BLACKNESS))
     {
         SelectObject(maskDC, oldMask); 
         DeleteObject(maskBitmap);      
         DeleteDC(maskDC);              
         SelectObject(dc, oldBitmap);   
         DeleteObject(bitmap);          
         DeleteDC(dc);                  
return false;
     }

     SetBkColor(dc, crTransparent);
     BitBlt(maskDC, 0,0,nWidthSrc,nHeightSrc,dc,0,0,SRCINVERT);

     SetBkColor(dc, RGB(0,0,0));
     SetTextColor(dc, RGB(255,255,255));
     BitBlt(dc, 0,0,nWidthSrc,nHeightSrc,maskDC,0,0,SRCAND);

     HDC newMaskDC = CreateCompatibleDC(NULL);
     HBITMAP newMask;
     newMask = CreateBitmap(nWidthDest, nHeightDest, 1,
                                    GetDeviceCaps(newMaskDC, BITSPIXEL), NULL);

if (newMask == NULL)
     {
         SelectObject(dc, oldBitmap);
         DeleteDC(dc);
         SelectObject(maskDC, oldMask);
         DeleteDC(maskDC);
          DeleteDC(newMaskDC);
         DeleteObject(bitmap);     
         DeleteObject(maskBitmap); 
return false;
     }

     SetStretchBltMode(newMaskDC, COLORONCOLOR);
     HBITMAP oldNewMask = (HBITMAP) SelectObject(newMaskDC, newMask);
     StretchBlt(newMaskDC, 0, 0, nWidthDest, nHeightDest, maskDC, 0, 0,
                                               nWidthSrc, nHeightSrc, SRCCOPY);

     SelectObject(maskDC, oldMask);
     DeleteDC(maskDC);
     DeleteObject(maskBitmap); 

     HDC newImageDC = CreateCompatibleDC(NULL);
     HBITMAP newImage = CreateBitmap(nWidthDest, nHeightDest, 1,
                                    GetDeviceCaps(newMaskDC, BITSPIXEL), NULL);

if (newImage == NULL)
     {
         SelectObject(dc, oldBitmap);
         DeleteDC(dc);
         DeleteDC(newMaskDC);
         DeleteObject(bitmap);     
return false;
     }

     HBITMAP oldNewImage = (HBITMAP)SelectObject(newImageDC, newImage);
     StretchBlt(newImageDC, 0, 0, nWidthDest, nHeightDest, dc, 0, 0, nWidthSrc,
                                                          nHeightSrc, SRCCOPY);

     SelectObject(dc, oldBitmap);
     DeleteDC(dc);
     DeleteObject(bitmap);     

     BitBlt( dcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
                                                      newMaskDC, 0, 0, SRCAND);

     BitBlt( dcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
                                                   newImageDC, 0, 0, SRCPAINT);
     SelectObject(newImageDC, oldNewImage);
     DeleteDC(newImageDC);
     SelectObject(newMaskDC, oldNewMask);
     DeleteDC(newMaskDC);
     DeleteObject(newImage);   
     DeleteObject(newMask);    

return true;
}








#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)
 
//-------------------------------------------------------------------------------
// ReplaceColor
//
// Author    : Dimitri Rochette drochette@coldcat.fr
// Specials Thanks to Joe Woodbury for his comments and code corrections
//
// Includes  : Only <windows.h>
 
//
// hBmp         : Source Bitmap
// cOldColor : Color to replace in hBmp
// cNewColor : Color used for replacement
// hBmpDC    : DC of hBmp ( default NULL ) could be NULL if hBmp is not selected
//
// Retcode   : HBITMAP of the modified bitmap or NULL for errors
//
//-------------------------------------------------------------------------------
HBITMAP ReplaceColor(HBITMAP hBmp,COLORREF cOldColor,COLORREF cNewColor,HDC hBmpDC)
{
    HBITMAP RetBmp=NULL;
    if (hBmp)
    {
        HDC BufferDC=CreateCompatibleDC(NULL);    // DC for Source Bitmap
        if (BufferDC)
        {
            HBITMAP hTmpBitmap = (HBITMAP) NULL;
            if (hBmpDC)
                if (hBmp == (HBITMAP)GetCurrentObject(hBmpDC, OBJ_BITMAP))
            {
                hTmpBitmap = CreateBitmap(1, 1, 1, 1, NULL);
                SelectObject(hBmpDC, hTmpBitmap);
            }
 
            HGDIOBJ PreviousBufferObject=SelectObject(BufferDC,hBmp);
            // here BufferDC contains the bitmap
             
            HDC DirectDC=CreateCompatibleDC(NULL); // DC for working
            if (DirectDC)
            {
                // Get bitmap size
                BITMAP bm;
                GetObject(hBmp, sizeof(bm), &bm);
 
                // create a BITMAPINFO with minimal initilisation 
                // for the CreateDIBSection
                BITMAPINFO RGB32BitsBITMAPINFO; 
                ZeroMemory(&RGB32BitsBITMAPINFO,sizeof(BITMAPINFO));
                RGB32BitsBITMAPINFO.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
                RGB32BitsBITMAPINFO.bmiHeader.biWidth=bm.bmWidth;
                RGB32BitsBITMAPINFO.bmiHeader.biHeight=bm.bmHeight;
                RGB32BitsBITMAPINFO.bmiHeader.biPlanes=1;
                RGB32BitsBITMAPINFO.bmiHeader.biBitCount=32;
 
                // pointer used for direct Bitmap pixels access
                UINT * ptPixels;    
 
                HBITMAP DirectBitmap = CreateDIBSection(DirectDC, 
                                       (BITMAPINFO *)&RGB32BitsBITMAPINFO, 
                                       DIB_RGB_COLORS,
                                       (void **)&ptPixels, 
                                       NULL, 0);
                if (DirectBitmap)
                {
                    // here DirectBitmap!=NULL so ptPixels!=NULL no need to test
                    HGDIOBJ PreviousObject=SelectObject(DirectDC, DirectBitmap);
                    BitBlt(DirectDC,0,0,
                                   bm.bmWidth,bm.bmHeight,
                                   BufferDC,0,0,SRCCOPY);
 
                       // here the DirectDC contains the bitmap
 
                    // Convert COLORREF to RGB (Invert RED and BLUE)
                    cOldColor=COLORREF2RGB(cOldColor);
                    cNewColor=COLORREF2RGB(cNewColor);
 
                    // After all the inits we can do the job : Replace Color
                    for (int i=((bm.bmWidth*bm.bmHeight)-1);i>=0;i--)
                    {
                        if (ptPixels[i]==cOldColor) ptPixels[i]=cNewColor;
                    }
                    // little clean up
                    // Don't delete the result of SelectObject because it's 
                    // our modified bitmap (DirectBitmap)
                       SelectObject(DirectDC,PreviousObject);
 
                    // finish
                    RetBmp=DirectBitmap;
                }
                // clean up
                DeleteDC(DirectDC);
            }            
            if (hTmpBitmap)
            {
                SelectObject(hBmpDC, hBmp);
                DeleteObject(hTmpBitmap);
            }
            SelectObject(BufferDC,PreviousBufferObject);
            // BufferDC is now useless
            DeleteDC(BufferDC);
        }
    }
    return RetBmp;
}






	void photo_L (HWND hwnd,char str[],int Long , int high,int xy,int zy,COLORREF old,COLORREF new_)
{
	
     HDC hdc ;
    
    hdc = GetDC (hwnd) ;
     
    HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP hBmp = (HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, Long, high, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	HBITMAP hNewBmp=ReplaceColor(hBmp,old,new_,0);

	SelectObject(memDC, hNewBmp);
	//HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBmp);
	BitBlt(hdc, xy, zy, Long , high, memDC, 0, 0, SRCCOPY);

	//HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBmp);

	DeleteDC(memDC);

    DeleteObject(hBmp);

	DeleteObject(hNewBmp);
	
	ReleaseDC (hwnd, hdc) ;

}






HDC printf_32(HWND hwnd,const char str[],int X,int Y,int FontHigh,int FontWidth,COLORREF CLRGB)
{
	
	
	HDC hdc=GetDC(hwnd);
	
		HFONT hFont = CreateFont(
                -19/*高度*/, -10/*宽度*/, 0/*不用管*/, 0/*不用管*/, 400 /*一般这个值设为400*/,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
                DEFAULT_QUALITY,  //默认输出质量
                FF_DONTCARE,  //不指定字体族*/
                "方正静蕾体"  //字体名
                );
				
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, CLRGB);
            
	TextOut(hdc, X, Y, str, strlen(str));
	DeleteObject(hFont);
	DeleteObject(hOldFont);
	return hdc;
	ReleaseDC(hwnd,hdc);
}



void _Printf(HWND hwnd,const char str[],int X,int Y,int FontHigh,int FontWidth,COLORREF CLRGB,BOOL UnderLine,BOOL DeleteLine){


	HDC hdc=GetDC(hwnd);
		HFONT hFont = CreateFont(
                -19/*高度*/, -10/*宽度*/, 0/*不用管*/, 0/*不用管*/, 400 /*一般这个值设为400*/,
                FALSE/*不带斜体*/, UnderLine/*不带下划线*/, DeleteLine/*不带删除线*/,
                DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
                DEFAULT_QUALITY,  //默认输出质量
                FF_DONTCARE,  //不指定字体族*/
                "方正静蕾体"  //字体名
                );
				
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
		

	int i=-1,CrdX=X,CrdY=Y,Partition=0;
	string Bufstr,Datastr;
	int DataID=0,SelectId=0,nOldMode=0;
	bool IsSelectArea=false;

	Bufstr.clear();

	do{
		i++;
		switch(SelectId){
		case 0:{
			nOldMode=SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, CLRGB);
			break;
		}
		default:
			SetBkMode(hdc, OPAQUE);
			SetBkColor(hdc, RGB(0,0,222));
			SetTextColor(hdc, RGB(255,255,255));
			break;
		
		}
			switch(str[i]){
			case '\n':{
			
				
				CrdX+=Datastr.length()*10;
				if(DataID==1){
					CrdX=X;
				}
				
				Partition++;
				TextOut(hdc, CrdX, CrdY, Bufstr.data(),Bufstr.length());
				CrdY+=19;
				Datastr.clear();
				Datastr=Bufstr;
				Bufstr.clear();
				DataID=1;
				break;
			}
			
			
			default:
				if(str[i+1]!='\0')
					Bufstr+=str[i];
				else {
					Bufstr+=str[i],Partition++;
					CrdX+=Datastr.length()*10;
					if(DataID==1){
						CrdX=X;
					}
					TextOut(hdc, CrdX, CrdY, Bufstr.data(),Bufstr.length());
					Datastr=Bufstr;
					Bufstr.clear();
					DataID=2;
				}
				break;
		}
	}while(str[i]!='\0'); 

	DeleteObject(hFont);
	DeleteObject(hOldFont);
	ReleaseDC(hwnd,hdc);
}


#define MAX_BUFF 1024

#endif
