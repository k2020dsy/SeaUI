/* dllmain.cpp  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don’t know about,
Therefore, please use it with caution. */

#include "stdafx.h"
#include "dll.h"
#include "GraphicsUnit.h"
#include "SeaUITextBox.h"
#include "SeaUILayeredWindow.h"
#include "SeaUIMaskWindow.h"
#include "SeaUINormalWindow.h"
#include "SeaUIProgressBar.h"



DLLIMPORT _SEAUIWINDOW *Initialization(long Style){
	switch(Style){
	case SUS_LAYER:{
		return new  _SeaUIlayeredWindow;
	}
	case SUS_TEXTBOX:{
		return new _SeaUITextBox;
	}
	case SUS_NORMAL:{
		return new _SeaUINormalWindow;
	}

	case SUS_MASK:{
		 return new _SeaUIMaskWindow;
	}
	
	case SUS_PROGRESSBAR:{
		return new _SeaUIProgressBar;
		break;
	}
	
	default: return NULL;
	}
}

LRESULT CALLBACK SeaUILayeredWindowProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK SeaUIProcTextBox (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK SeaUIMaskWindowProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK SeaUINormalWindowProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK SeaUIProgressBarProc (HWND, UINT, WPARAM, LPARAM) ;



BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}
	
	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}





void EraseWindow(HWND hwnd,int ParentAddress){
	int err=0;int j=0;
	for(vector<ParentRecorder>::size_type i=0;i<CRegistered[ParentAddress].Parent.vchild.size();i++){
		if(err>2) break;
		HWND hc=CRegistered[ParentAddress].Parent.vchild[i];j++;
		if(hc==hwnd){
			CRegistered[ParentAddress].Parent.vchild.erase(CRegistered[ParentAddress].Parent.vchild.begin()+i);
			break;
		}else{
			err++;
		}
	}
	
}




bool _SEAUIWINDOW::CreateSeaUIWindow
(HWND nParenthwnd,char nSeaUIText[],long nStyle,int nX,int nY,int nwidth,int nhigh,HMENU nhmenu)
{
	sprintf(SeaUIName,"%d",this); strcpy(SeaUIText,nSeaUIText);

	long _Winstyle=(nStyle==SUS_LAYER||nParenthwnd==NULL)?WS_POPUP|WS_EX_LAYERED|WS_VISIBLE:WS_CHILD|WS_VISIBLE;
	
	MyWinC->MyRegWindowClass(TEXT(SeaUIName),(nStyle==SUS_TEXTBOX)?SeaUIProcTextBox:(nStyle==SUS_LAYER)?SeaUILayeredWindowProc:
	(nStyle==SUS_MASK)?SeaUIMaskWindowProc:(nStyle==SUS_PROGRESSBAR)?SeaUIProgressBarProc:SeaUINormalWindowProc);
	
    hwnd=CreateWindow(SeaUIName, TEXT (nSeaUIText),_Winstyle,nX,nY,nwidth,nhigh, nParenthwnd,nhmenu,GetModuleHandle(NULL), NULL) ; 

 	::SetWindowLong(hwnd,GWL_USERDATA,(LONG)this);

	Parenthwnd = nParenthwnd,hmenu=nhmenu;		
										
 	width=nwidth,height=nhigh;

	StaticWidth=nwidth,StaticHeight=nhigh,WindowsStyle=nStyle;;

	X=nX,Y=nY;

	Style=nStyle;
 	
 	#ifdef SU_DEBUG
 	AllocConsole();
 	freopen("CONOUT$", "w", stdout);
	#endif
	
	if(nParenthwnd!=NULL){
	vector<ParentRecorder>::iterator Wd;  

 	HWND Cbutd = Parenthwnd;  

 	int i=0;
 	
	for(vector<ParentRecorder>::size_type i=0;i<CRegistered.size();i++){
		ParentRecorder tr=CRegistered[i];
		if (tr.HParent == Cbutd){
				ParentAddress=i,CRegistered[ParentAddress].Parent._Children++;
				ChildAddress = CRegistered[ParentAddress].Parent._Children;
				CRegistered[ParentAddress].Parent.vchild.push_back(hwnd),CRegistered[ParentAddress].Boss=GetBoss(nParenthwnd);
				break;
			}

			
	}
              
	if(i==CRegistered.size()){
		ParentRecorder tr;
		tr.IsActivated=true,tr.Boss=GetBoss(nParenthwnd);
		tr.HParent=nParenthwnd,tr.Parent.vchild.push_back(hwnd);
		CRegistered.push_back(tr); 
		ChildAddress=0,ParentAddress=i;
	}
	
	}


	switch(nStyle){
		case SUS_LAYER:{
				_IDtimer=SetTimer(hwnd,WM_TIMER,300,NULL) ;
				LONG nRet = ::GetWindowLong(hwnd, GWL_EXSTYLE);
				nRet = nRet | WS_EX_LAYERED;
				::SetWindowLong(hwnd, GWL_EXSTYLE, nRet);
				CoordinateLock();
			break;
		}
		
		case SUS_TEXTBOX:{
			TempTextbox = CreateWindow ("EDIT","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN,0,0,width ,height,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);//创建Textbox 
			break;
		}
		default:break;
	}


	UpdateWindow(hwnd);
	return true;
}



void _SEAUIWINDOW::_photo(HWND hwnd,char str[],int _width , int _height,int _x,int _y){
	GraphicsUnit gh;
	gh.DrawImage(GetDC(hwnd),str, _x, _y, _width , _height);
}

void _SEAUIWINDOW::SetWindowLong(long style){
	::SetWindowLongA(hwnd,0,style);
}


void _SEAUIWINDOW::SetTextBoxLong(long style){
	::SetWindowLongA(TempTextbox,0,style);
}


void _SEAUIWINDOW::SetCallback(UINT message,_MessagePass nMessagePass){
	switch(message){
	case WM_LBUTTONDOWN:{
		_OnButtonDown=nMessagePass;
		break;
	}
	case WM_LBUTTONUP:{
		_OnButtonUp=nMessagePass;
		break;
	}
	case WM_MOUSEHOVER:{
		_OnMouseHover=nMessagePass;
		break;
	}
	case WM_MOUSELEAVE:{
		_OnMouseLeave=nMessagePass;
		break;
	}

	case WM_PAINT:{
		_OnPaint=nMessagePass;
		break;
	}

	}
	
}



void _SEAUIWINDOW::TurnBkColor
(
HWND hwnd,COLORREF RePlaceColor,COLORREF _From,COLORREF _To,int Times,char _PictureName[],int width , int height,int _X,int _Y,int _pause
)
//抠颜色 
{
					
	if(IsHover==true){ photo_L(hwnd,_PictureName,width ,height,_X,_Y,RePlaceColor,_To); return;}
	if(Style==SUS_NORMAL){
	int Vr_plus_R=0,Vr_plus_G=0,Vr_plus_B=0;
	BYTE old_r = GetRValue(_From),old_g = GetGValue(_From),old_b = GetBValue(_From);
	BYTE new_r = GetRValue(_To),new_g = GetGValue(_To),new_b = GetBValue(_To);
	Vr_plus_R=old_r-new_r,Vr_plus_G=old_g-new_g,Vr_plus_B=old_b-new_b;
	int R_v= Vr_plus_R/Times, G_v= Vr_plus_G/Times,B_v= Vr_plus_B/Times;
	for(int i=0;i<=Times;i++)
	{
		photo_L(hwnd,_PictureName,width ,height,_X,_Y,RePlaceColor,RGB(old_r+=R_v,old_g+=G_v,old_b+=B_v));
		Sleep(_pause);	
	}
	photo_L(hwnd,_PictureName,width ,height,_X,_Y,RePlaceColor,_To);
	}
}


void _SEAUIWINDOW::CoordinateLock(){
	LockPosition();
	ShowWindow(hwnd,SW_SHOW);
}

void _SEAUIWINDOW::SetImage(char nBitmapName[],int cX,int cY){	
	CBmpInfo.bmpX=cX;
	CBmpInfo.bmpY=cY;
	strcpy(CBmpInfo.BitmapName,nBitmapName);
	OnPaint();
}



void _SEAUIWINDOW::SetAlphaValue(int _AlphaValue){
	AlphaValue=(_AlphaValue<255)?_AlphaValue:255;
	OnPaint();
}


void _SEAUIWINDOW::Roundrect(int radius,BOOL Sswitch){
	CRoundrect.state=Sswitch;
	CRoundrect.radius=radius;
	OnPaint();
}


void _SEAUIWINDOW::SetAnimationStyle(LONG WindowsStyle){
	if(WindowsStyle&SUS_NORMAL)
	AnimationStyle =SUS_NORMAL;
}




void _SEAUIWINDOW::Output(int nX,int nY,int wordWidth,int wordHeight,char str[],BOOL nUnderLine,BOOL nDeleteLine,COLORREF color){
	
	_Printf(hwnd,str,nX,nY,wordHeight,wordWidth,color,nUnderLine,nDeleteLine);
}


void _SEAUIWINDOW::Output(int nX,int nY,int wordWidth,int wordHeight,string str,BOOL nUnderLine,BOOL nDeleteLine,COLORREF color){
	_Printf(hwnd,str.data(),nX,nY,wordHeight,wordWidth,color,nUnderLine,nDeleteLine);
}


void _SEAUIWINDOW::Printf(int x,int y,char str[],...){
    TCHAR szBuff[1024];
    va_list args;
    va_start( args, str );
    _vsnprintf( szBuff, sizeof( szBuff ) / sizeof( TCHAR ), str, args );
    va_end( args );
    return Output(x,y, wordWidth,wordHeight, szBuff, false, false,TextColor );
}



void SeaUI_DrawLine(HDC hdc,int fromX,int fromY,int toX,int toY){
	
	MoveToEx(hdc,fromX,fromY,NULL);                       
    
    LineTo(hdc,toX,toY);
}

void SeaUI_Rectangle(HDC hdc,int fromX,int fromY,int toX,int toY,int Thickness,COLORREF rgb){
	
	HPEN hPen = CreatePen(PS_SOLID,Thickness,rgb);   
	
	HPEN hOldPen = (HPEN)SelectObject(hdc,hPen);
	
    SeaUI_DrawLine(hdc,fromX,fromY,toX,fromY);
    
    SeaUI_DrawLine(hdc,fromX,toY,toX,toY);
    
	SeaUI_DrawLine(hdc,fromX,fromY,fromX,toY);
	
	SeaUI_DrawLine(hdc,toX,fromY,toX,toY);                             
    
    SelectObject(hdc, hOldPen);                    
	   
    DeleteObject(hPen);

	DeleteObject(hOldPen);
}

COLORREF SetDeepenRGB(COLORREF rgb){
	BYTE r = GetRValue(rgb),g = GetGValue(rgb),b = GetBValue(rgb);	
	BYTE ansR=r-10,ansG=g-10,ansB=b-10;
    return RGB((ansR>=0)?ansR:0,(ansG>=0)?ansG:0,(ansB>=0)?ansB:0);
}


extern "C++" void _SEAUIWINDOW::DrawFrame(HDC hdc){
	
	COLORREF clr;
	if(width<2&&height<2){
		return;
	}
	if(CFrameInfo.autoclr==TRUE){
		clr=(width>=1&&height>=1)?GetPixel(hdc,1,1):RGB(0,0,0);
		
	}else{
		clr=CFrameInfo.CFclr;
	}
	
	if(CFrameInfo._3Dstyel){
		SeaUI_Rectangle(hdc,0,0,width,height,2,SetDeepenRGB(clr));
		SeaUI_Rectangle(hdc,2,2,width-2,height-2,CFrameInfo.Thickness,clr);         
	}else{
		SeaUI_Rectangle(hdc,0,0,width,height,CFrameInfo.Thickness,clr);
	}	
}

void _SEAUIWINDOW::SetFrameInfo(int Thickness,COLORREF clr,BOOL autoclr,BOOL _3Dstyel){
	CFrameInfo.Thickness=Thickness;
	CFrameInfo.CFclr=clr;
	CFrameInfo._3Dstyel=_3Dstyel;
	CFrameInfo.autoclr=autoclr;
	CFrameInfo.state=true;
}

void _SEAUIWINDOW::RemoveFrame(){
	CFrameInfo.state=false;
}

void _SEAUIWINDOW::LockPosition(){
	RECT rect,rect1;

	::GetWindowRect(Parenthwnd,&rect),::GetWindowRect(hwnd,&rect1);

	LONG STY=::GetWindowLong(Parenthwnd, GWL_STYLE);

	int BorderWidth=(STY & WS_POPUP||STY & WS_CHILD)?0:GetSystemMetrics(SM_CYSIZEFRAME);
	int BorderHeight=(STY & WS_POPUP||STY & WS_CHILD)?0:GetSystemMetrics(SM_CXSIZEFRAME);

	int Width=rect.right-rect1.left-BorderWidth,Height=rect.bottom-rect1.top-BorderHeight;

	if(Width<0) Width=0;if(Height<0) Height=0;
	
	if(Width<StaticWidth||Height<StaticHeight){
	
		if(Width>StaticWidth&&Height<StaticHeight){Width=StaticWidth;}else if(Width<StaticWidth&&Height>StaticHeight){Height=StaticHeight;}
		 
		::MoveWindow(hwnd,rect.left+X+BorderHeight,rect.top+Y+BorderWidth,Width,Height,0);

		width=Width,height=Height;

	}else if(rect1.right<rect.right||rect1.bottom<rect.bottom){

		width=StaticWidth,height=StaticHeight;

		::MoveWindow(hwnd,rect.left+X+BorderHeight,rect.top+Y+BorderWidth,StaticWidth,StaticHeight,0);
	}

	
}



extern "C++" void _SEAUIWINDOW::OnSeaUIWindowPaint(HDC hdc){

	if(!IsGraphics) return;
	else{

	HWND TempHwnd=hwnd;
	
	if(MouseHoverState==1&&BtnUpAniState!=1&&WindowsStyle== SUS_NORMAL){		
	MouseHoverState=2;
	}else if(MouseHoverState!=2||BtnUpAniState==1){
		if(WindowsStyle==SUS_TEXTBOX||WindowsStyle == SUS_MASK||WindowsStyle == SUS_PROGRESSBAR){
			//_photo(TempHwnd,CBmpInfo.BitmapName, width, height, CBmpInfo.bmpX, CBmpInfo.bmpY);
		}else{
		
				HDC hdcMem = CreateCompatibleDC(hdc);

	 			HDC memDCMAP = CreateCompatibleDC(hdc);
     			//创建一个bmp内存空间
    			 HBITMAP hBmp = CreateCompatibleBitmap(hdc,width,height);
     			//将bmp内存空间分配给内存DC
     			HBITMAP hOldSel = (HBITMAP)::SelectObject(hdcMem,hBmp);
     	
     			//这是使用者需要绘制的画面,全部往内存DC绘制
     			 // 绘制代码.......................
	 			HBITMAP hBkbmp = (HBITMAP)LoadImage(NULL, CBmpInfo.BitmapName, IMAGE_BITMAP,  width, height, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		
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
	}

	if(CRoundrect.state==SEA_UI_ON){
		
		HRGN hRgn = CreateRoundRectRgn(0, 0,width,height, CRoundrect.radius, CRoundrect.radius);
		
		SetWindowRgn(TempHwnd, hRgn, TRUE);

		DeleteObject(hRgn);
	}
	
	if(CFrameInfo.state)
	DrawFrame(hdc);
	
    
	if(_OnPaint)_OnPaint();//交给用户绘制 

	UpdateWindow(TempTextbox);
	}
}

#define SEA_WM_PAINT WM_USER+300

void _SEAUIWINDOW::SetThreadState(bool state){
	threadstate=state;
}

bool _SEAUIWINDOW::GetThreadState(){
	return threadstate;
}


void _SEAUIWINDOW::SetAlphaAnimation(int Increment,int Frame,int DeltaTime)
{
	int value=Increment/Frame,_tempAlpha=AlphaValue;
	int i=0;
	for(i;i<Frame;i++)
	{
		_tempAlpha+=value;
		if(value<=TempAlpha+Increment-_tempAlpha&&value>0||value>_tempAlpha-TempAlpha+Increment&&value<0){
			SetAlphaValue(_tempAlpha);}
		else
			break;
		
		Sleep(DeltaTime);
		
	}
	SetAlphaValue(TempAlpha+Increment);
}


extern "C++" DLLIMPORT HWND _SEAUIWINDOW::GetHwnd(){
	return (Style==SUS_TEXTBOX)?TempTextbox:hwnd;
}

extern "C++" DLLIMPORT void _SEAUIWINDOW::AllowPainting(bool Isallow){
	IsGraphics=Isallow;
}

extern "C++" DLLIMPORT void _SEAUIWINDOW::ButtonAnimationInitialize(int _CacheAlpha,int Increment){
	TempAlpha=_CacheAlpha;
	CacheAlpha=_CacheAlpha;
	AlphaAniIncrement=Increment;
}


extern "C++" DLLIMPORT void _SEAUIWINDOW::ButtonHitAnimation(bool _IsAllow){
	_HitAnimation=_IsAllow;
}

extern "C++" DLLIMPORT void _SEAUIWINDOW::MouseHoverAnimation(bool _IsAllow){
	_MouseHoverAnimation=_IsAllow;
}

extern "C++"  void _SEAUIWINDOW::ButtonDownAnimation(){
	BtnUpAniState=false,BtnDownAniState=true;
	SetAlphaAnimation(AlphaAniIncrement,2,20);
	TempAlpha+=AlphaAniIncrement;
	
}

extern "C++"  void _SEAUIWINDOW::ButtonUpAnimationPlay(){
	BtnUpAniState=1;
	BtnDownAniState=0;
   	SetAlphaAnimation(-AlphaAniIncrement,3,25);
	TempAlpha-=AlphaAniIncrement;
}

extern "C++"  void _SEAUIWINDOW::MouseHoverAnimationPlay(){

	TempAlpha=CacheAlpha;
	SetAlphaAnimation(AlphaAniIncrement,1,20);
	TempAlpha+=AlphaAniIncrement;
	ButtonStateConflictResolution=1;
}

extern "C++"  void _SEAUIWINDOW::MouseLeaveAnimationPlay(){

	SetAlphaValue(CacheAlpha+AlphaAniIncrement);
	SetAlphaAnimation(-AlphaAniIncrement,2,40);
	TempAlpha=CacheAlpha;
	ButtonStateConflictResolution=0;
}

extern "C++"  void _SEAUIWINDOW::RemoveWindow(){
	EraseWindow(hwnd,ParentAddress);
	DestroyWindow(hwnd);
}

void DrawBoxOutline (HWND hwnd,int x,int dx,int y,int dy)
{
	 HDC hdc=GetDC(hwnd);
     SetROP2 (hdc, R2_WHITE) ;
	 HBRUSH hBrush;
     hBrush = CreateSolidBrush (RGB(0,0,0));
     SelectObject (hdc,hBrush) ;
     Rectangle (hdc, x, y,dx, dy) ;
     DeleteObject (hBrush);
	 ReleaseDC(hwnd,hdc);
}









/*".../Dev-Cpp/MinGW32/lib/libgdi32.a"
".../Dev-Cpp/MinGW32/lib/libgdiplus.a"
".../Dev-Cpp/MinGW32/lib/libmsimg32.a"*/




