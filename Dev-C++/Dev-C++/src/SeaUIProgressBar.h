/* SeaUIProgressBar.h  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don’t know about,
Therefore, please use it with caution. */

#include "dll.h"
class _SEAUIWINDOW;

class _SeaUIProgressBar:public _SEAUIWINDOW{
public:
	_SeaUIProgressBar(){}
	virtual void SetTextSize(int,int){return;}
	virtual void SetTextColor(COLORREF color){return;}
	virtual void OnPaint();
	virtual bool Create(HWND,char*,int,int,int,int,HMENU);
	void OnProgressBarPaint(HDC hdc);
};

void _SeaUIProgressBar::OnPaint(){
	HDC hdc=GetDC(hwnd);
	OnProgressBarPaint(hdc);
	OnSeaUIWindowPaint(hdc);
	ReleaseDC(hwnd,hdc);
}

bool _SeaUIProgressBar::Create(HWND hWnd,char str[],int x,int y,int width,int height,HMENU hmenu){

	return CreateSeaUIWindow(hWnd,str,SUS_PROGRESSBAR,x,y,width,height,(HMENU)hmenu);
}

void _SeaUIProgressBar::OnProgressBarPaint(HDC hdc){

	 HDC hdcMem = CreateCompatibleDC(hdc);

	 HDC memDCMAP = CreateCompatibleDC(hdc);
     //创建一个bmp内存空间
     HBITMAP hBmp = CreateCompatibleBitmap(hdc,width,height);
     //将bmp内存空间分配给内存DC
     HBITMAP hOldSel = (HBITMAP)::SelectObject(hdcMem,hBmp);
     
     //这是使用者需要绘制的画面,全部往内存DC绘制
      // 绘制代码.......................
	 HBITMAP hBkbmp = (HBITMAP)LoadImage(NULL, ParentBk, IMAGE_BITMAP,  width, height, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	 SelectObject(memDCMAP,hBkbmp);    

	 BitBlt(hdc,0,0,width,height,memDCMAP,0,0,SRCCOPY);
      
	 HBITMAP hBmpFg = (HBITMAP)LoadImage(NULL, fg, IMAGE_BITMAP,  width, height, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	 SelectObject(memDCMAP,hBmpFg);
	 
	  BitBlt(hdcMem,
	 (Pbdirection==SUD_LEFT)?width*(1-Percentage):0,
	 (Pbdirection==SUD_UP)?height*Percentage:0,
	 (Pbdirection==SUD_LEFT)?width*Percentage:(Pbdirection==SUD_RIGHT)?width*Percentage:width,
	 (Pbdirection==SUD_UP)?height*(1-Percentage):(Pbdirection==SUD_DOWN)?height*(1-Percentage):height,
	 memDCMAP,0,0,SRCCOPY);

     //将内存DC的内容复制到屏幕显示DC中,完成显示
     BitBlt(hdc,0,0,width,height,hdcMem,0,0,SRCCOPY);
     //清除资源
     
     DeleteDC(hdcMem);    DeleteDC(memDCMAP);   

	 DeleteObject(hBmp);

	 DeleteObject(hBkbmp);

	 DeleteObject(hBmpFg); DeleteObject(hOldSel);
}

float _SEAUIWINDOW::GetPercentage(){
	return Percentage;
}

void _SEAUIWINDOW::SetPercentage(float _percent){
	Percentage=_percent;
	OnPaint();
}

float _SEAUIWINDOW::GetClickPosition(){
	//cout<<pt.x;
	switch(Pbdirection){
		case SUD_UP:{
			Percentage=pt_y/height;
			break;
		}
		case SUD_DOWN:{
			Percentage=(height-pt_y)/height;
			break;
		}
		case SUD_LEFT:{
			Percentage=(width-pt_x)/width;
			break;
		}
		case SUD_RIGHT:{
			Percentage=pt_x/width;
			break;
		}
		default:break;
	}
	return Percentage;
}

void _SEAUIWINDOW::SetProgressBarDirection(int _pbdirection){
	Pbdirection=_pbdirection;
}


LRESULT CALLBACK SeaUIProgressBarProc (HWND SeaUIWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps ;
	_SEAUIWINDOW*tr=(_SEAUIWINDOW*)GetWindowLong(SeaUIWindow,GWL_USERDATA);
	TRACKMOUSEEVENT tme = {0};	
	HWND hp=GetParent(SeaUIWindow);

		
	
switch (message)
{

	
	case WM_MOUSEMOVE:
		{
			tme.cbSize = sizeof(tme);
        	tme.dwFlags = TME_HOVER | TME_LEAVE;
        	tme.hwndTrack = SeaUIWindow;
        	tme.dwHoverTime = 10; //check it each 10ms
        	TrackMouseEvent(&tme);
        	break;
		}

		
	case WM_LBUTTONDOWN:{
		
		
		
		if(tr){
		tr->pt_x=GET_X_LPARAM(lParam);
		tr->pt_y=GET_Y_LPARAM(lParam);
		if(tr->_HitAnimation)
		tr->ButtonDownAnimation();
		if(tr->_OnButtonDown)
				tr->_OnButtonDown();
		}
		
		break;
		
	} 
	
	case WM_LBUTTONUP:
		{
			
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
