/* dll.h  (C) CopyRight SIyuan Deng 2020 -forever,
SeaUI is a free software that can be edited as needed.
The program is based on Win32, so it can only run in a window system.
There must be some errors in my program that I don¡¯t know about,
Therefore, please use it with caution. */


#pragma   once 
#include "stdafx.h"
#include "photo.h"
#include "MyWineasy.h"
#include "GraphicsUnit.h"

#ifndef SEAUI_H

#ifndef SEAUI_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif



#define SEA_UI_ON TRUE
#define SEA_UI_OFF FALSE


using namespace std;






class DLLIMPORT _CFont{
			public: _CFont(){
					DeleteLine=false,UnderLine=false;
					fwidth=0,fheight=0;
					X=0;
					Y=0;			
				}

				~_CFont(){
					DeleteLine=false,UnderLine=false;
					fwidth=0,fheight=0;
					X=0,Y=0;
				}
				int X,Y,fwidth,fheight;
				bool UnderLine,DeleteLine;
				COLORREF fcolor;
				
		};



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
		
		
		
		/******************Basic variables***************/
		BLENDFUNCTION bl;
		HWND hwnd,Parenthwnd;
		int AnimationStyle;
		char SeaUIName[100],SeaUIText[50];
		int StaticWidth,StaticHeight,width,height;
		int ParentAddress,ChildAddress;
		COLORREF BackgroundRGB,TextColor;
		int X,Y,AlphaValue;
		HMENU hmenu;
		long WindowsStyle,Style;
		int wordWidth,wordHeight;
		int MouseHoverState;
		bool threadstate,BtnUpAniState,BtnDownAniState;
		bool _HitAnimation,_MouseHoverAnimation;
		bool ButtonStateConflictResolution;
		bool EDIT_MOD,IsGraphics,_IsBmp;
		_MessagePass _OnButtonUp,_OnButtonDown,_OnPaint,_OnMouseHover,_OnMouseLeave;
		int _IDtimer;
		/******************Textbox variables***************/
		//string ContentStr;
		HWND TempTextbox;int TextBoxAlphaValue;
		virtual void SetTextSize(int,int)=0;
		virtual void SetTextColor(COLORREF color)=0;
		void Password(bool IsPassword);
		virtual bool Create(HWND,char*,int,int,int,int,HMENU)=0;
		virtual void OnPaint()=0;
		char*GetText();
		/******************Mask Window variables*************/
		char ParentBk[100],fg[100];int pbk_Width,pbk_Height;
		void SetBackgroundImage(char*,int,int,int,int);
		void SetForegroundBitmap(char*,int,int);
		HBITMAP hBmpMask;
		int pbk_x,pbk_y;
		/******************Normal Window variables*************/
		COLORREF from,to;bool IsHover;
		void Transform(COLORREF from,COLORREF to);
		/******************ProgressBar variables*************/
		float Percentage,pt_x,pt_y;int Pbdirection; 
		float GetClickPosition();
		void SetPercentage(float _percent);
		float GetPercentage();
		void SetProgressBarDirection(int);
		/******************Fuction***************/
		void ButtonDownAnimation();
		void ButtonUpAnimationPlay();
		void MouseHoverAnimationPlay();
		void MouseLeaveAnimationPlay();

		void ButtonHitAnimation(bool);
		void MouseHoverAnimation(bool);
		void MouseLeaveAnimation(bool);
		void ButtonAnimationInitialize(int BaiscValue,int Increment);
		void SetAnimationStyle(LONG style);
		void DrawFrame(HDC hdc);
		void LockPosition();
		void OnSeaUIWindowPaint(HDC hdc);
		bool CreateSeaUIWindow(HWND nParenthwnd,char nSeaUIText[],long nStyle,int nX,int nY,int nwidth,int nhigh,HMENU nhmenu);
		void DrawTxtOnPaint(HDC);
		void SetImage(char nBitmapName[],int cX,int cY);
		void SetAlphaValue(int);
		void RePaint();
		void Output
		(int tX,int tY,int wordWidth,int wordHigh,char _text[],BOOL nUnderLine,BOOL nDeleteLine,COLORREF color);
		BOOL RemoveDrawText(int nX,int nY,char _text[],double nkey);
		void Roundrect(int radius,BOOL Sswitch);
		void RemoveWord(int i);
		void RemoveWord(int start,int end);
		void Output(TCHAR str[],_CFont _Cfont);
		void Output(int nX,int nY,int wordWidth,int wordHeight,string str,BOOL nUnderLine,BOOL nDeleteLine,COLORREF color);
		void SetFrameInfo(int Thickness,COLORREF clr,BOOL autoclr,BOOL _3Dstyel);
		_CFont CreateFontStyle(int nX,int nY,int wordWidth,int wordHigh,TCHAR _text,BOOL nUnderLine,BOOL nDeleteLine,COLORREF color);
		void CoordinateLock();
		void SetThreadState(bool state);
		bool GetThreadState();
		void RemoveFrame();
		void RemoveWindow();
		HWND GetHwnd();
		void TurnBkColor(HWND,COLORREF,COLORREF,COLORREF,int,char str[],int,int,int,int,int);
		void DrawBoxOutline ( HDC hdc,int X,int dataX,int Y,int dataY);
		void SetCallback(UINT message,_MessagePass nMessagePass);
		void SetAlphaAnimation(int Increment,int Frame,int DeltaTime);
		void _photo(HWND hwnd,char str[],int Long , int high,int xy,int zy);
		void AllowPainting(bool Isallow);
		void Printf(int,int,char str[],...);
		void SetWindowLong(long style);
		void SetTextBoxLong(long style);
	protected:
		class DLLIMPORT _CBmpInfo{
			public:
				int bmpX,bmpY;
				int bmpwidth,bmphigh;
				char BitmapName[MAX_PATH];	
			public:
			_CBmpInfo(){
				bmpX=0,bmpY=0;
				bmpwidth=0,bmphigh=0;
			}

			~_CBmpInfo(){
				bmpX=0,bmpY=0;
				bmpwidth=0,bmphigh=0;
			}
		};
		
		class DLLIMPORT _CFrameInfo{
			public:
				int Thickness;
				COLORREF CFclr;
				BOOL _3Dstyel,autoclr,state;
			public:
				_CFrameInfo(){
					Thickness=2;
					CFclr=RGB(0,0,0);
					_3Dstyel=false,autoclr=true,state=false;
				}

				~_CFrameInfo(){
					Thickness=2;
					CFclr=RGB(0,0,0);
					_3Dstyel=false,autoclr=true,state=false;
				}
		};
		class DLLIMPORT _CRoundrectInfo{
			public:
				BOOL state;
				int radius;
			public:
				_CRoundrectInfo(){
					 state=0,radius=0;
				}

				~_CRoundrectInfo(){
					 state=0,radius=0;
				}
		};
		class DLLIMPORT _CBurshInfo{
			public:
				BOOL state;
				HBRUSH hbursh;
			public:	
				_CBurshInfo(){
					 state=0,hbursh=NULL;
				}

				~_CBurshInfo(){
					state=0;
					DeleteObject(hbursh);
				}
		};

		class DLLIMPORT _SeaUIBkinfo{
		public:
			int width,heigth,X,Y;
		public:
			_SeaUIBkinfo(){
				width=0,heigth=0,X=0,Y=0;
			}

			~_SeaUIBkinfo(){
				width=0,heigth=0,X=0,Y=0;
			}
		
		};


		public:
		COLORREF FontRGB;_SEAUIWINDOW();~_SEAUIWINDOW();
	private:
		
		public:int CacheAlpha,TempAlpha;
		public:int AlphaAniIncrement;
		_CRoundrectInfo CRoundrect;
		_CFrameInfo CFrameInfo;
		_CBmpInfo CBmpInfo;
		_CBurshInfo CBurshInfo;
		_SeaUIBkinfo SeaUIBkinfo;

};

_SEAUIWINDOW::_SEAUIWINDOW(){
		
	 WindowsStyle=0;
	 width=0,height=0;
	 BackgroundRGB=0;
	 X=0,Y=0;
	 threadstate=false,IsHover=false,IsGraphics=true,_IsBmp=false;
	 TextBoxAlphaValue=255;AlphaValue=255;
	 _OnButtonUp=NULL;
	 MouseHoverState=0,Pbdirection=4;
	 wordWidth=16,wordHeight=18;TextColor=RGB(0,0,0);
};



_SEAUIWINDOW::~_SEAUIWINDOW(){
	
};


int DLLIMPORT ParentMsgTracker(UINT msg,HWND HParent);


void RemoveWindow(_SEAUIWINDOW*window){
	window->RemoveWindow();
	delete window;
	window=NULL;
}


class _Parent{
	public:
	//HWND Parent;
		_Parent(){
			_Children=0;
		}
	 vector<HWND>vchild;
	 
	int _Children;
};


class ParentRecorder{
	public:
		ParentRecorder(){
		  HParent=NULL;
		  IsActivated=false;
		  rc1.bottom=0,rc2.bottom=0,DeltaTime=0,Counter=0;
		}

		~ParentRecorder(){
		  HParent=NULL;
		  IsActivated=false;
		}
		_Parent Parent;
		HWND HParent,Boss;
		bool IsActivated;
		RECT rc1,rc2;
		int DeltaTime,Counter;
		void RC(RECT _rect){
			if(!rc1.bottom)  rc1=_rect;
			else if(!rc2.bottom) rc2=_rect;
			else {rc1=rc2,rc2=_rect;}
		}

		bool IsMoving(){
			DeltaTime++;
			if(rc1.bottom!=rc2.bottom||rc1.left!=rc2.left||rc1.right!=rc2.right||rc1.top!=rc2.top){
				Counter++;
			}
			else{
				if(DeltaTime>=5)
					Counter=0,DeltaTime=0;
			}
			if(Counter>0){ return true; } else { return false; }
		}
};


HWND GetBoss(HWND child){
	HWND h=child,res=child;
	do{
		h=res;
		res=GetParent(h);
		if(res) continue; else break;
		
	}while(1);
	
 return h;
}

static vector<ParentRecorder>CRegistered;


bool IsParentMoving(int ParentID){
	RECT _rect;
	GetWindowRect(CRegistered[ParentID].HParent,&_rect);
	CRegistered[ParentID].RC(_rect);
	return CRegistered[ParentID].IsMoving();
}





#endif
