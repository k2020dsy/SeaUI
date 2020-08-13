/* SeaUITextBox.h  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don’t know about,
Therefore, please use it with caution. */

#include "dll.h"
class _SEAUIWINDOW;


class _SeaUITextBox:public _SEAUIWINDOW{
public: 
	_SeaUITextBox(){}
	
	virtual void SetTextSize(int,int);
	virtual void SetTextColor(COLORREF color);
	virtual bool Create(HWND,char*,int,int,int,int,HMENU);
	virtual void OnPaint();
};

	

void _SeaUITextBox::OnPaint(){
	HDC hdc=GetDC(hwnd);
	OnSeaUIWindowPaint(hdc);
	SetLayeredWindowAttributes(hwnd,RGB(255,0,0),TextBoxAlphaValue,LWA_ALPHA|LWA_COLORKEY);//窗口透明
	ReleaseDC(hwnd,hdc);
}

bool _SeaUITextBox::Create(HWND hWnd,char str[],int x,int y,int width,int height,HMENU hmenu){

	return CreateSeaUIWindow(hWnd,str,SUS_TEXTBOX,x,y,width,height,(HMENU)hmenu);
}

void _SeaUITextBox::SetTextSize(int _wordWidth,int _wordHeight){
	wordWidth=_wordWidth;
	wordHeight=_wordHeight;
	HFONT hFont = CreateFont(
                -wordHeight/*高度*/, -wordWidth/*宽度*/, 0/*不用管*/, 0/*不用管*/, 400 /*一般这个值设为400*/,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
                DEFAULT_QUALITY,  //默认输出质量
                FF_DONTCARE,  //不指定字体族*/
                "微软雅黑"  //字体名
            );
   ::SendMessage(TempTextbox,WM_SETFONT,(WPARAM)hFont,true);  //设置控件字体  
}

void _SeaUITextBox::SetTextColor(COLORREF color){
	TextColor=color;
}

void _SEAUIWINDOW::Password(bool IsPassword){
	switch(IsPassword){
	case true:{
		LONG nSty = ::GetWindowLong(hwnd, GWL_EXSTYLE);
		nSty = nSty | ES_PASSWORD;
		::SetWindowLong(hwnd, GWL_EXSTYLE, nSty);
		::SetWindowLong(hwnd, GWL_STYLE, nSty);
		break;
	}
	
	case false:{
		LONG nSty = ::GetWindowLong(hwnd, GWL_EXSTYLE);
		nSty =WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN;
		::SetWindowLong(hwnd, GWL_EXSTYLE, nSty);
		::SetWindowLong(hwnd, GWL_STYLE, nSty);
		break;
	}
	
	}

}



TCHAR*GetCtrText(HWND hwnd){
	 
	string str;

	TCHAR *Text = NULL;

	size_t bufferLength = (GetWindowTextLength(hwnd)+ 1) * sizeof(TCHAR);

	Text = (TCHAR *)malloc(bufferLength);

	if (Text){

		GetWindowText(hwnd,Text,bufferLength);

		str.clear();str+=Text;

		return Text;

		free(Text);

		Text = NULL;
	}else return (TCHAR*)"(null)";
	

}


TCHAR*_SEAUIWINDOW::GetText(){
	return GetCtrText(hwnd);
}







LRESULT CALLBACK SeaUIProcTextBox (HWND SeaUIWindow, UINT message, WPARAM wParam, LPARAM lParam)
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
		ShowWindow(tr->TempTextbox,SW_SHOW);
		//SetFocus(tr->TempTextbox);
		if(tr){
		tr->TextBoxAlphaValue=255;
		if(tr->_OnButtonDown)
				tr->_OnButtonDown();
		}
		if(tr->_HitAnimation)
		tr->ButtonDownAnimation();
		break;
		
	} 
	
	case WM_LBUTTONUP:
		{
			
			if(tr){
			
			if(tr->_OnButtonUp)
				tr->_OnButtonUp();
				}
			if(tr->_HitAnimation)
			tr->ButtonUpAnimationPlay();
			
			break;
		}	
		

	case WM_MOUSEHOVER:{	
		if(tr){

			if(tr->_OnMouseHover)
			tr->_OnMouseHover();}

			if(tr->_MouseHoverAnimation)
			{
				if(tr->ButtonStateConflictResolution!=1){
					tr->MouseHoverAnimationPlay();
				}
			break;
		}
	 }
					   
	case WM_MOUSELEAVE:{
		if(tr->_OnMouseLeave)
		tr->_OnMouseLeave();
		if(tr->_MouseHoverAnimation)
			tr->MouseLeaveAnimationPlay();
		break;
	}


	case WM_SETFOCUS:{

		break;				 
	}
	
	case WM_COMMAND:{
		if(lParam == (LPARAM)(tr->TempTextbox)){
			SendMessage(SeaUIWindow,WM_LBUTTONDOWN,0,0);
		}
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

