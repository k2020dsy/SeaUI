/* SeaUIMaskWindow.h  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don’t know about,
Therefore, please use it with caution. */


#include "dll.h"
class _SEAUIWINDOW;

class _SeaUIMaskWindow:public _SEAUIWINDOW{
public:
	_SeaUIMaskWindow(){
	bl.BlendOp = 0;
    bl.BlendFlags = 0;
    bl.SourceConstantAlpha = 255;  // 透明度指定 
    bl.AlphaFormat = 0;	
	pbk_x=0,pbk_y=0;
	}
	
	virtual void SetTextSize(int,int){return;}
	virtual void SetTextColor(COLORREF color){return;}
	//function
	virtual void OnPaint();
	virtual bool Create(HWND,char*,int,int,int,int,HMENU);
	void PaintMask(HDC hdc,BYTE AlphaValue);
	GraphicsUnit gh;
};


void _SeaUIMaskWindow::OnPaint(){
	
	HDC hdc=GetDC(hwnd);
	PaintMask(hdc,AlphaValue);
	OnSeaUIWindowPaint(hdc);
	ReleaseDC(hwnd,hdc);
}

bool _SeaUIMaskWindow::Create(HWND hWnd,char str[],int x,int y,int width,int height,HMENU hmenu){
	return CreateSeaUIWindow(hWnd,str,SUS_MASK,x,y,width,height,(HMENU)hmenu);
}


extern "C++" void _SEAUIWINDOW::SetBackgroundImage(char* BitmapName,int width,int height,int x,int y){
	strcpy(ParentBk,BitmapName);
	cout<<ParentBk;
	pbk_Width=width,pbk_Height=height;
	pbk_x=x,pbk_y=y;
	return;
}

extern "C++" void _SEAUIWINDOW::SetForegroundBitmap(char* BitmapName,int width,int height){
	strcpy(fg,BitmapName);
	return;
}

char*RecodeStr(char*dest){
	int i=0;
	while(dest[i]!='\0'){
	if (dest[i] >= 'a' && dest[i] <= 'z')
	{
		dest[i] -= 32;
	}
	else if (dest[i] >= 'A' && dest[i] <= 'Z')
	{
		dest[i] += 32;
		
	}}
	return dest;

}

void _SeaUIMaskWindow::PaintMask(HDC hdc,BYTE AlphaValue){

     bl.SourceConstantAlpha = AlphaValue;

	 HDC hdcMem = CreateCompatibleDC(hdc);

	 HDC memDCMAP = CreateCompatibleDC(hdc);
     //创建一个bmp内存空间
     HBITMAP hBmp = CreateCompatibleBitmap(hdc,width,height);
     //将bmp内存空间分配给内存DC
     HBITMAP hOldSel = (HBITMAP)::SelectObject(hdcMem,hBmp);
     
     //全部往内存DC绘制
      // 绘制代码.......................
	 HBITMAP hBkbmp = (HBITMAP)LoadImage(NULL, ParentBk, IMAGE_BITMAP,  width, height, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	 SelectObject(memDCMAP,hBkbmp);    

	 BitBlt(hdcMem,0,0,width,height,memDCMAP,0,0,SRCCOPY);
      //Rectangle(hdcMem,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
      
	 HBITMAP hBmpFg = (HBITMAP)LoadImage(NULL, fg, IMAGE_BITMAP,  width, height, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	 SelectObject(memDCMAP,hBmpFg);

	 AlphaBlend(hdcMem,0, 0, width, height, memDCMAP, 0, 0, width, height, bl); 
     
     //将内存DC的内容复制到屏幕显示DC中,完成显示
     BitBlt(hdc,0,0,width,height,hdcMem,0,0,SRCCOPY);
     //清除资源
     
     DeleteDC(hdcMem);    DeleteDC(memDCMAP);   

	 DeleteObject(hBmp);

	 DeleteObject(hBkbmp);

	 DeleteObject(hBmpFg); DeleteObject(hOldSel);

}




LRESULT CALLBACK SeaUIMaskWindowProc (HWND SeaUIWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps ;
	_SEAUIWINDOW*tr=(_SEAUIWINDOW*)GetWindowLong(SeaUIWindow,GWL_USERDATA);
	TRACKMOUSEEVENT tme = {0};	
	HWND hp=GetParent(SeaUIWindow);

	
switch (message)
{




case WM_CREATE:{

	break;
}	
	
	case WM_MOUSEMOVE:
		{
			tme.cbSize = sizeof(tme);
        	tme.dwFlags = TME_HOVER | TME_LEAVE;
        	tme.hwndTrack = SeaUIWindow;
        	tme.dwHoverTime = 10; 
        	TrackMouseEvent(&tme);
        	break;
		}

		
	case WM_LBUTTONDOWN:{
		
		if(tr){
		
		if(tr->_HitAnimation)
		tr->ButtonDownAnimation();
		if(tr->_OnButtonDown)
				tr->_OnButtonDown();
		}
		
		break;
		
	} 
	
	case WM_LBUTTONUP:{
			if(tr){
				if(tr->_HitAnimation)
					tr->ButtonUpAnimationPlay();
				if(tr->_OnButtonUp)
					tr->_OnButtonUp();
			}
			break;
		}	
		
	case WM_MOUSEHOVER:{	
		if(tr){
			if(tr->_MouseHoverAnimation)
			{
				if(tr->ButtonStateConflictResolution!=1){
					tr->MouseHoverAnimationPlay();
				}
				
			}
			if(tr->_OnMouseHover)
			tr->_OnMouseHover();
		}
			
			break;
		}	
					   
	case WM_MOUSELEAVE:{
		if(tr->_MouseHoverAnimation)
			tr->MouseLeaveAnimationPlay();
		if(tr->_OnMouseLeave)
		tr->_OnMouseLeave();
		break;
	}


case WM_PAINT:{
	if(tr->GetThreadState()==false){
	HDC hdc = BeginPaint (SeaUIWindow, &ps) ;
	tr->SetThreadState(true);
	tr->OnPaint();
	tr->SetThreadState(false);
	EndPaint (SeaUIWindow, &ps) ;
	ReleaseDC(SeaUIWindow,hdc);

	}
	
	return 0;
	
}

case WM_DESTROY:{
	break;
}

}
tr=NULL;
return DefWindowProc (SeaUIWindow, message, wParam, lParam) ;


}

