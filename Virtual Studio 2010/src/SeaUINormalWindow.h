/* SeaUINormalWindow.h  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don¡¯t know about,
Therefore, please use it with caution. */

#include "dll.h"


class _SeaUINormalWindow:public _SEAUIWINDOW{
public: 
	_SeaUINormalWindow(){}
	virtual void SetTextSize(int,int){return;}
	virtual void SetTextColor(COLORREF color){return;}
	virtual bool Create(HWND,char*,int,int,int,int,HMENU);
	virtual void OnPaint();
};

void _SeaUINormalWindow::OnPaint(){
	HDC hdc=GetDC(hwnd);
	OnSeaUIWindowPaint(hdc);
	ReleaseDC(hwnd,hdc);
}

bool _SeaUINormalWindow::Create(HWND hWnd,char str[],int x,int y,int width,int height,HMENU hmenu){
	return CreateSeaUIWindow(hWnd,str,SUS_NORMAL,x,y,width,height,(HMENU)hmenu);
}

void _SEAUIWINDOW::Transform(COLORREF from,COLORREF to){
	this->from=from;this->to=to;
}


LRESULT CALLBACK SeaUINormalWindowProc (HWND SeaUIWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps ;
	_SEAUIWINDOW*tr=(_SEAUIWINDOW*)GetWindowLong(SeaUIWindow,GWL_USERDATA);
	TRACKMOUSEEVENT tme = {0};	
	HWND hp=GetParent(SeaUIWindow);

	
switch (message)
{
	
	case WM_MOUSEMOVE:{
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
			tr->TurnBkColor(SeaUIWindow,tr->from,tr->from,tr->to,3,tr->CBmpInfo.BitmapName,tr->width, tr->height,tr->CBmpInfo.bmpX, tr->CBmpInfo.bmpY,50);
		if(tr->_OnButtonDown)
				tr->_OnButtonDown();
		}
		
		break;
		
	} 
	
	case WM_LBUTTONUP:
		{
			
			if(tr){
			if(tr->_HitAnimation)
				tr->TurnBkColor(SeaUIWindow,tr->to,tr->to,tr->from,3,tr->CBmpInfo.BitmapName,tr->width, tr->height,tr->CBmpInfo.bmpX, tr->CBmpInfo.bmpY,50);
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
					tr->TurnBkColor(SeaUIWindow,tr->from,tr->from,tr->to,3,tr->CBmpInfo.BitmapName,tr->width, tr->height,tr->CBmpInfo.bmpX, tr->CBmpInfo.bmpY,50);
				}
				
			}
			if(tr->_OnMouseHover)
			tr->_OnMouseHover();
		}tr->IsHover=true;
			
			break;
		}	
					   
	case WM_MOUSELEAVE:{
		tr->IsHover=false;
		if(tr->_MouseHoverAnimation)
			tr->TurnBkColor(SeaUIWindow,tr->to,tr->to,tr->from,3,tr->CBmpInfo.BitmapName,tr->width, tr->height,tr->CBmpInfo.bmpX, tr->CBmpInfo.bmpY,50);
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


